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
#include "sql/stmt/select_stmt.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include <cstddef>
#include <string>
#include <unordered_map>

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

  std::vector<int>            indexs;
  std::vector<UpdateStmtNode> values;

  {  // 进行输入的检查、类型的检查与转化。
    int sys_num   = table->table_meta().sys_field_num();
    int field_num = table->table_meta().field_num();

    for (auto &it : update.update_values) {
      const FieldMeta *field_meta = nullptr;
      int              index      = 0;

      for (int i = sys_num; i < field_num; ++i, ++index) {
        auto table_field = table->table_meta().field(i);
        if (table_field->name() == it.attribute_name) {
          field_meta = table_field;
          break;
        }
      }

      if (field_meta == nullptr) {
        LOG_WARN("invalid value. rc=%d:%s", rc, strrc(rc));
        return RC::VARIABLE_NOT_VALID;
      }

      indexs.emplace_back(index);

      if (!it.is_sub) {
        if (it.value.attr_type() == CHARS &&
            (field_meta = table->table_meta().field(it.attribute_name.c_str())) != nullptr &&
            field_meta->type() == DATES) {
          date_t v = str_to_date(it.value.data());
          if (check_date(v)) {
            Value *value = const_cast<Value *>(&it.value);
            value->set_date(v);
          } else {
            LOG_WARN("can't convert to date in update.");
            return RC::VARIABLE_NOT_VALID;
          }
        }

        if (it.value.attr_type() != field_meta->type()) {
          const_cast<Value &>(it.value).convert_to_type(field_meta->type());
        }

        // 排除一种情况，可为null，且为null
        if (!(field_meta->nullable() && it.value.attr_type() == NULLS) &&
            field_meta->type() != it.value.attr_type()) {
          LOG_WARN("invalid value. rc=%d:%s", rc, strrc(rc));
          return RC::VARIABLE_NOT_VALID;
        }

        values.emplace_back(UpdateStmtNode(it.value, field_meta->nullable(), field_meta->type()));
      } else {
        Stmt *tmp = nullptr;
        if (SelectStmt::create(db, it.sub_query, tmp) != RC::SUCCESS || tmp == nullptr) {
          return RC::SQL_SYNTAX;
        }

        values.emplace_back(UpdateStmtNode(tmp, field_meta->nullable(), field_meta->type()));
      }
    }
  }

  UpdateStmt *update_stmt = new UpdateStmt(table, values, indexs, filter_stmt);
  stmt                    = update_stmt;

  return RC::SUCCESS;
}
