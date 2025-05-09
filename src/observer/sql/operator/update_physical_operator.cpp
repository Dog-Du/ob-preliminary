/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/6/27.
//

#include "sql/operator/update_physical_operator.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "storage/field/field_meta.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/table/view.h"
#include "storage/trx/trx.h"
#include <cstddef>

// 把view得到的record转化为table的record，
// 先用table->get_record得到原来的record，然后覆盖对应值。
RC convert_view_record_to_table_record(Table *t, Record &view_record, Record &table_record)
{
  if (!t->is_view()) {
    table_record = view_record;
    return RC::SUCCESS;
  }

  View  *view  = static_cast<View *>(t);
  Table *table = view->table();

  RC rc = table->get_record(view_record.rid(), table_record);
  if (rc != RC::SUCCESS) {
    LOG_WARN("get_record failed.");
    return rc;
  }

  auto &view_fields = view->fields();
  auto &view_meta   = *view->table_meta().field_metas();
  for (int i = 0; i < view_fields.size(); ++i) {
    table_record.set_field(
        view_fields[i].meta()->offset(), view_fields[i].meta()->len(), view_record.data() + view_meta[i].offset());
  }
  table_record.set_rid(view_record.rid());
  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::convert_expression_to_values()
{
  RC         rc = RC::SUCCESS;
  ShellTuple tuple;  // 空壳，不会被触摸，如果被触摸是不应该的.
  for (size_t i = 0; i < expressions_.size(); ++i) {
    auto &e     = expressions_[i];
    auto  field = fields_meta_[i];
    Value tmp;

    rc = ComparisonExpr::check_comparison_with_subquery(e.get(), false);
    if (rc != RC::SUCCESS) {
      LOG_WARN("check_comparison failed.");
      return rc;
    }

    if (e->type() == ExprType::SUBQUERY_OR_VALUELIST) {
      auto expr = static_cast<SubQuery_ValueList_Expression *>(e.get());
      rc        = expr->open(trx_);
      if (rc != RC::SUCCESS) {
        LOG_WARN("open failed.");
        expr->close();
        return rc;
      }

      rc = expr->next(tuple, tmp);

      if (rc == RC::RECORD_EOF) {
        tmp = Value::NULL_VALUE();
        expr->close();
      } else if (rc != RC::SUCCESS) {
        LOG_WARN("error");
        expr->close();
        return rc;
      } else {
        Value val;
        rc = expr->next(tuple, val);
        expr->close();
        if (rc != RC::RECORD_EOF) {
          LOG_WARN("something wrong");
          return rc == RC::SUCCESS ? RC::INVALID_ARGUMENT : rc;
        }

        rc = RC::SUCCESS;
      }
    } else {
      e->get_value(tuple, tmp);
    }

    Value value;

    rc = tmp.cast_to(tmp, field->type(), value);

    if (rc != RC::SUCCESS) {
      LOG_WARN("cast_to failed.");
      return rc;
    }

    rc = value.resize(field->len());

    if (rc != RC::SUCCESS) {
      LOG_WARN("resize failed.");
      return rc;
    }

    if (!field->nullable() && value.is_null(value)) {
      LOG_WARN("should not be null.");
      return RC::VARIABLE_NOT_VALID;
    }
    values_.emplace_back(std::move(value));
  }
  return rc;
}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  RC    rc    = RC::SUCCESS;
  auto &child = children_[0];

  rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  rc = convert_expression_to_values();

  if (rc != RC::SUCCESS) {
    RC rc2 = child->next();
    if (rc2 == RC::RECORD_EOF) {
      child->close();
      LOG_WARN("convert failed but empty.");
      return RC::SUCCESS;
    }
    child->close();
    LOG_WARN("convert failed.");
    return rc;
  }

  std::vector<Record> new_records;
  std::vector<Record> old_records;
  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record    new_record;
    new_record.copy_data(row_tuple->record().data(), row_tuple->record().len());
    Record old_record(new_record);
    old_record.set_rid(row_tuple->record().rid());
    new_record.set_rid(row_tuple->record().rid());

    for (size_t i = 0; i < fields_meta_.size(); ++i) {
      memcpy(new_record.data() + fields_meta_[i]->offset(), values_[i].data(), fields_meta_[i]->len());
    }

    Record old_table_record;
    Record new_table_record;
    rc = convert_view_record_to_table_record(table_, old_record, old_table_record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("convert view record failed.");
      return rc;
    }
    rc = convert_view_record_to_table_record(table_, new_record, new_table_record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("convert view record failed.");
      return rc;
    }
    old_records.emplace_back(old_table_record);
    new_records.emplace_back(new_table_record);
  }

  if (rc != RC::SUCCESS) {
    if (rc != RC::RECORD_EOF) {
      LOG_WARN("next failed.");
      child->close();
      return rc;
    }
    rc = RC::SUCCESS;
  }

  rc = child->close();
  if (rc != RC::SUCCESS) {
    LOG_WARN("close failed.");
    return rc;
  }

  for (size_t i = 0; i < old_records.size(); ++i) {
    rc = table_->table()->delete_record(old_records[i]);
    // rc = table_->update_record(old_records[i].rid(), old_records[i], new_records[i]);
    if (rc != RC::SUCCESS) {
      LOG_WARN("delete_record failed. something wrong maybe duplicate key.");
      // 回滚。
      RC rc2 = RC::SUCCESS;
      for (int j = i - 1; j >= 0; --j) {
        rc2 = table_->table()->insert_record(old_records[j]);
        // rc2 = table_->update_record(new_records[j].rid(), new_records[j], old_records[j]);
        if (rc2 != RC::SUCCESS) {
          LOG_WARN("rollback failed while update_record.");
          break;
        }
      }
      break;
    }
  }

  if (rc != RC::SUCCESS) {
    LOG_WARN("update abort before insert.");
    return rc;
  }

  for (size_t i = 0; i < new_records.size(); ++i) {
    rc = table_->table()->insert_record(new_records[i]);
    if (rc != RC::SUCCESS) {
      LOG_WARN("insert_record failed. may be duplicate.");
      RC rc2 = RC::SUCCESS;
      for (int j = i - 1; j >= 0; --j) {
        rc2 = table_->table()->delete_record(new_records[j]);
        // rc2 = table_->update_record(new_records[j].rid(), new_records[j], old_records[j]);
        if (rc2 != RC::SUCCESS) {
          LOG_WARN("rollback failed while update_record.");
          break;
        }
      }

      if (rc2 != RC::SUCCESS) {
        return rc2;
      }

      for (size_t j = 0; j < old_records.size(); ++j) {
        rc2 = table_->table()->insert_record(old_records[j]);
        if (rc2 != RC::SUCCESS) {
          LOG_WARN("rollback failed while update_record.");
          break;
        }
      }

      if (rc2 != RC::SUCCESS) {
        return rc2;
      }
      break;
    }
  }
  return rc;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }
