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
#include "common/value.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "storage/field/field_meta.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include <cstddef>

RC UpdatePhysicalOperator::convert_expression_to_values()
{
  RC         rc = RC::SUCCESS;
  ShellTuple tuple;  // 空壳，不会被触摸，如果被触摸是不应该的.
  for (size_t i = 0; i < expressions_.size(); ++i) {
    auto &e     = expressions_[i];
    auto  field = fields_meta_[i];

    rc = ComparisonExpr::check_comparison_with_subquery(e.get());
    if (rc != RC::SUCCESS) {
      LOG_WARN("check_comparison failed.");
      return rc;
    }

    if (e->type() == ExprType::SUBQUERY_OR_VALUELIST) {
      auto expr = static_cast<SubQuery_ValueList_Expression *>(e.get());
      if (expr->value_num() > 1) {
        LOG_WARN("update failed.");
        return rc;
      }
    }

    Value tmp;
    e->get_value(tuple, tmp);
    Value value;
    tmp.cast_to(tmp, field->type(), value);
    value.resize(field->len());

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

  auto &child = children_[0];

  RC rc = convert_expression_to_values();

  if (rc != RC::SUCCESS) {
    LOG_WARN("convert failed.");
    return rc;
  }

  rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  std::vector<Record> new_records;
  std::vector<Record> old_records;
  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple  = static_cast<RowTuple *>(tuple);
    Record    new_record = row_tuple->record();
    Record    old_record(new_record);

    for (int i = 0; i < fields_meta_.size(); ++i) {
      memcpy(new_record.data() + fields_meta_[i]->offset(), values_[i].data(), fields_meta_[i]->len());
    }
    old_records.emplace_back(old_record);
    new_records.emplace_back(new_record);
  }
  child->close();

  for (int i = 0; i < old_records.size(); ++i) {
    rc = table_->update_record(old_records[i].rid(), old_records[i], new_records[i]);
    if (rc != RC::SUCCESS) {
      LOG_WARN("update_record failed. maybe duplicate key.");
      // 回滚。
      RC rc2 = RC::SUCCESS;
      for (int j = i - 1; j >= 0; --j) {
        rc2 = table_->update_record(new_records[j].rid(), new_records[j], old_records[j]);
        if (rc2 != RC::SUCCESS) {
          LOG_WARN("rollback failed while update_record.");
          break;
        }
      }
      break;
    }
  }
  return rc;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }
