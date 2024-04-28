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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "common/rc.h"
#include "common/log/log.h"
#include "sql/parser/value.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include <cstddef>
#include <string>
#include <unordered_map>

UpdateStmt::UpdateStmt(Table *table, const Value &value, FilterStmt *filter_stmt, int index_attr)
    : attr_index_(index_attr), table_(table), value_(new Value(value)), filter_stmt_(filter_stmt)
{}

UpdateStmt::~UpdateStmt()
{
  if (filter_stmt_ != nullptr) {
    delete filter_stmt_;
  }
}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  // TOD
  Table *table = db->find_table(update.relation_name.c_str());

  if (table == nullptr) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), update.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  std::unordered_map<std::string, Table *> table_map;
  table_map.emplace(table->name(), table);
  FilterStmt *filter_stmt = nullptr;

  RC rc = FilterStmt::create(db,
      table,
      &table_map,
      update.conditions.data(),
      static_cast<int>(update.conditions.size()),
      filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
    return rc;
  }

  {
    const FieldMeta *field_meta = table->table_meta().field(update.attribute_name.c_str());

    if (field_meta == nullptr) {
      LOG_WARN("invalid value. rc=%d:%s", rc, strrc(rc));
      return RC::VARIABLE_NOT_VALID;
    }

    if (update.value.attr_type() == CHARS &&
        (field_meta = table->table_meta().field(update.attribute_name.c_str())) != nullptr &&
        field_meta->type() == DATES) {
      date_t v = str_to_date(update.value.data());
      if (check_date(v)) {
        Value *value = const_cast<Value *>(&update.value);
        value->set_date(v);
      } else {
        LOG_WARN("can't convert to date in update.");
        return RC::VARIABLE_NOT_VALID;
      }
    }

    // 排除一种情况，可为null，且为null
    if (!(field_meta->nullable() && update.value.attr_type() == NULLS) &&
        field_meta->type() != update.value.attr_type()) {
      LOG_WARN("invalid value. rc=%d:%s", rc, strrc(rc));
      return RC::VARIABLE_NOT_VALID;
    }
  }
  const TableMeta table_meta  = table->table_meta();
  int             field_num   = table_meta.field_num();
  auto           &field_metas = *table_meta.field_metas();

  for (int i = 0; i < field_num; ++i) {
    if (field_metas[i].name() == update.attribute_name) {
      stmt = new UpdateStmt(table, update.value, filter_stmt, i);
      return rc;
    }
  }

  return RC::VARIABLE_NOT_VALID;
}
