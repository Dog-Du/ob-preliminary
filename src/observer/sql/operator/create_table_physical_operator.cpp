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

#include "sql/operator/create_table_physical_operator.h"
#include "common/rc.h"
#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include "storage/db/db.h"
#include "storage/record/record.h"
#include "storage/table/table.h"

RC create_table_with_subquery(Db *db, Table *table, PhysicalOperator *child)
{
  RC rc = RC::SUCCESS;

  std::vector<Value> values;
  while ((rc = child->next()) == RC::SUCCESS) {
    Tuple *tuple = child->current_tuple();
    ASSERT(tuple != nullptr, "");
    values.resize(tuple->cell_num());

    for (int i = 0; i < tuple->cell_num(); ++i) {
      tuple->cell_at(i, values[i]);
    }

    Record record;
    rc = table->make_record(values.size(), values.data(), record);

    if (rc != RC::SUCCESS) {
      LOG_WARN("make_record failed.");
      child->close();
      return rc;
    }

    rc = table->insert_record(record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("insert_record failed.");
      child->close();
      return rc;
    }
  }

  if (rc != RC::SUCCESS) {
    if (rc != RC::RECORD_EOF) {
      LOG_WARN("something wrong.");
      return rc;
    }
    rc = RC::SUCCESS;
  }
  return rc;
}

RC CreateTablePhysicalOperator::open(Trx *trx)
{
  RC rc = RC::SUCCESS;

  if (!children_.empty()) {
    rc = children_[0]->open(trx);
    if (rc != RC::SUCCESS) {
      LOG_WARN("open failed.");
      return rc;
    }
  }

  rc = db_->create_table(table_name_.c_str(), attr_infos_, storage_format_);
  if (rc != RC::SUCCESS) {
    LOG_WARN("create_table failed.");
    return rc;
  }

  if (!children_.empty()) {
    Table *table = db_->find_table(table_name_.c_str());
    if (table == nullptr) {
      LOG_WARN("find_table failed.");
      return RC::SCHEMA_DB_NOT_EXIST;
    }

    rc = create_table_with_subquery(db_, table, children_[0].get());
    if (rc != RC::SUCCESS) {
      LOG_WARN("create_table_with_subquery failed.");
      db_->drop_table(table_name_.c_str());
      return rc;
    }
    return rc;
  }
  return rc;
}

RC CreateTablePhysicalOperator::next() { return RC::RECORD_EOF; }

RC     CreateTablePhysicalOperator::close() { return RC::SUCCESS; }
Tuple *CreateTablePhysicalOperator::current_tuple() { return nullptr; }