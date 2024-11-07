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

#include "sql/operator/delete_physical_operator.h"
#include "common/log/log.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

RC DeletePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  std::shared_ptr<PhysicalOperator> &child = children_[0];

  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record   &record    = row_tuple->record();
    records_.emplace_back(std::move(record));
  }

  child->close();

  if (!table_->is_view()) {
    // 先收集记录再删除
    // 记录的有效性由事务来保证，如果事务不保证删除的有效性，那说明此事务类型不支持并发控制，比如VacuousTrx
    for (Record &record : records_) {
      rc = trx_->delete_record(table_, record);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to delete record: %s", strrc(rc));
        return rc;
      }
    }

    return RC::SUCCESS;
  }

  View *view = static_cast<View *>(table_);
  for (Record &view_record : records_) {
    Record record;
    rc = view->table()->get_record(view_record.rid(), record);  // 拿到原来的record。
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get record.");
      return rc;
    }

    rc = trx_->delete_record(view->table(), record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record");
      return rc;
    }
  }

  return RC::SUCCESS;
}

RC DeletePhysicalOperator::next() { return RC::RECORD_EOF; }

RC DeletePhysicalOperator::close() { return RC::SUCCESS; }
