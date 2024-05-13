/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
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
#include "sql/expr/tuple.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

RC UpdatePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  RC rc = RC::SUCCESS;
  {
    for (auto &it : update_nodes_) {
      if (it.child != nullptr) {  // 如果是sub-query
        if ((rc = it.child->open(trx)) != RC::SUCCESS) {
          return rc;
        }

        rc = it.child->next();

        Value v;

        // 得到空表，相当于null
        if (rc == RC::RECORD_EOF) {
          rc = RC::SUCCESS;
          v.set_null();

          if (!it.nullable) {  // 不可为空，返回错误
            it.child->close();
            return RC::SUCCESS;
          }

          // 可以为空，继续。
          values_.emplace_back(v);
          it.child->close();
          continue;
        }

        if (rc != RC::SUCCESS) {
          it.child->close();
          return rc;
        }

        Tuple *tuple;
        tuple = it.child->current_tuple();

        // tuple空，说明出了某种错误。
        if (tuple == nullptr) {
          it.child->close();
          return RC::SQL_SYNTAX;
        }

        // 不是1，错
        if (tuple->cell_num() != 1) {
          it.child->close();
          return RC::SQL_SYNTAX;
        }

        tuple->cell_at(0, v);

        rc = it.child->next();

        // 还有tuple，错
        if (rc == RC::SUCCESS) {
          it.child->close();
          return RC::SQL_SYNTAX;
        }

        // if (rc == RC::RECORD_EOF) {
        //   rc = RC::SUCCESS;
        // }

        // if (rc != RC::SUCCESS) {
        //   it.child->close();
        //   return RC::SQL_SYNTAX;
        // }

        values_.emplace_back(v);
        it.child->close();
      } else {
        values_.emplace_back(it.value);
      }
    }
  }

  std::unique_ptr<PhysicalOperator> &child = children_[0];
  rc                                       = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  // 直接删除试试。
  std::vector<Record>             delete_records;
  std::vector<std::vector<Value>> values;

  while (RC::SUCCESS == (rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);

    int cell_num = row_tuple->cell_num();
    values.emplace_back(std::vector<Value>(cell_num));
    Value *new_tuple = values.back().data();

    for (int i = 0; i < cell_num; ++i) {
      row_tuple->cell_at(i, new_tuple[i]);
    }

    for (int i = 0, n = indexs_.size(); i < n; ++i) {
      new_tuple[indexs_[i]].set_data(values_[i].data(), values_[i].length());
    }

    Record &record = row_tuple->record();
    delete_records.emplace_back(record);
  }

  for (auto &record : delete_records) {
    rc = trx_->delete_record(table_, record);

    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record: %s", strrc(rc));
    }
  }

  for (auto &tuple : values) {
    Record record;
    rc = table_->make_record(tuple.size(), tuple.data(), record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to make record. rc=%s", strrc(rc));
      return rc;
    }

    rc = trx_->insert_record(table_, record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
      return rc;
    }
  }

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}
