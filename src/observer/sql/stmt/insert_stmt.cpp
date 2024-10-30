/* Copyright (c) 2021OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/insert_stmt.h"
#include "common/log/log.h"
#include "sql/parser/parse_defs.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "storage/table/table_meta.h"

InsertStmt::InsertStmt(
    Table *table, const std::vector<std::vector<Value>> &values)
    : table_(table), values_(values)
{}

RC check_and_convert_rows(Table           *table,
    const std::vector<std::vector<Value>> &values,
    std::vector<std::vector<Value>>       &result)
{
  result.clear();
  RC               rc         = RC::SUCCESS;
  const TableMeta &table_meta = table->table_meta();
  const int field_num     = table_meta.field_num() - table_meta.sys_field_num();
  const int sys_field_num = table_meta.sys_field_num();

  for (auto &value : values) {
    const int value_num = static_cast<int>(value.size());

    if (field_num != value_num) {
      LOG_WARN("field_num != value_num");
      return RC::VARIABLE_NOT_VALID;
    }

    std::vector<Value> tmp_value;

    for (int i = 0; i < field_num; ++i) {
      auto field_meta = table_meta.field(i + sys_field_num);
      if (!field_meta->nullable() && value[i].is_null(value[i])) {
        LOG_WARN("insert_row should not be null.");
        return RC::VARIABLE_NOT_VALID;
      }

      Value tmp = value[i];

      if (field_meta->type() != value[i].attr_type()) {

        rc = value[i].cast_to(value[i], field_meta->type(), tmp);

        if (rc != RC::SUCCESS) {
          LOG_WARN("convert failed.");
          return rc;
        }
      }

      tmp.resize(field_meta->len());
      tmp_value.push_back(tmp);
    }

    result.push_back(tmp_value);
  }

  return rc;
}

RC InsertStmt::create(Db *db, const InsertSqlNode &inserts, Stmt *&stmt)
{
  const char *table_name = inserts.relation_name.c_str();
  if (nullptr == db || nullptr == table_name || inserts.values.empty()) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, value_num=%d",
        db, table_name, static_cast<int>(inserts.values.size()));
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  std::vector<std::vector<Value>> result;
  RC rc = check_and_convert_rows(table, inserts.values, result);

  if (rc != RC::SUCCESS) {
    LOG_WARN("check_and_convert_rows failed.");
    return rc;
  }

  stmt = new InsertStmt(table, result);
  return RC::SUCCESS;
}
