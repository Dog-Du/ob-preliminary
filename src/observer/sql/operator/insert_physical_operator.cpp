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
// Created by WangYunlai on 2021/6/9.
//

#include "sql/operator/insert_physical_operator.h"
#include "sql/stmt/insert_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

using namespace std;

InsertPhysicalOperator::InsertPhysicalOperator(
    Table *table, const std::vector<std::vector<Value>> &values)
    : table_(table), values_(values)
{}

RC InsertPhysicalOperator::open(Trx *trx)
{
  Record              record;
  std::vector<Record> records;
  RC                  rc = RC::SUCCESS;
  for (int i = 0; i < values_.size(); ++i) {
    auto &value = values_[i];
    rc          = table_->make_record(
        static_cast<int>(value.size()), value.data(), record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to make record. rc=%s", strrc(rc));
      return rc;
    }

    rc = trx->insert_record(table_, record);
    records.emplace_back(record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
      // 回滚。
      RC rc2 = RC::SUCCESS;
      for (int j = i - 1; j >= 0; --j) {
        rc2 = trx->delete_record(table_, records[j]);
        if (rc2 != RC::SUCCESS) {
          LOG_WARN("rollback failed.");
          break;
        }
      }
      break;
    }
  }

  return rc;
}

RC InsertPhysicalOperator::next() { return RC::RECORD_EOF; }

RC InsertPhysicalOperator::close() { return RC::SUCCESS; }
