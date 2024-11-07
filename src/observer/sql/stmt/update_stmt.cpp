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
#include "common/log/log.h"
#include "common/value.h"
#include "sql/expr/expression.h"
#include "sql/stmt/select_stmt.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include "storage/table/table.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/table/view.h"

UpdateStmt::UpdateStmt(Table *table, std::vector<const FieldMeta *> &fields,
    std::vector<std::shared_ptr<Expression>> &value, FilterStmt *filter_stmt, SelectStmt *select_stmt, View *view)
    : view_(view),
      table_(table),
      fields_meta_(fields),
      expressions_(value),
      filter_stmt_(filter_stmt),
      view_select_stmt_(select_stmt)
{}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt)
{
  const char *table_name = update_sql.rel_name.c_str();

  if (nullptr == db || nullptr == table_name) {
    return RC::INVALID_ARGUMENT;
  }

  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  if (!table->can_write()) {
    LOG_WARN("this view can not write.");
    return RC::INVALID_ARGUMENT;
  }

  std::vector<const FieldMeta *>           fields_meta;
  std::vector<std::shared_ptr<Expression>> expressions;
  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(std::string(table_name), table));

  bool                              need_continue_check = true;
  RC                                rc                  = RC::SUCCESS;
  std::function<void(Expression *)> check_field         = [&](Expression *expression) {
    if (!need_continue_check) {
      return;
    }
    switch (expression->type()) {
      case ExprType::SUBQUERY_OR_VALUELIST: {
        auto expr = static_cast<SubQuery_ValueList_Expression *>(expression);
        rc        = expr->create_stmt(db, {});
        if (rc != RC::SUCCESS) {
          LOG_WARN("create_stmt failed.");
          need_continue_check = false;
        }
      } break;
      default: {
      } break;
    }
  };

  for (auto &update_node : update_sql.update_list) {
    if (!update_node.rel_attr.relation_name.empty() && update_node.rel_attr.relation_name != table_name) {
      LOG_WARN("update rel_attr != table_name.");
      return RC::VARIABLE_NOT_VALID;
    }
    auto field = table->table_meta().field(update_node.rel_attr.attribute_name.c_str());
    if (field == nullptr) {
      LOG_WARN("update attr not exists.");
      return RC::SCHEMA_FIELD_NOT_EXIST;
    }

    for (auto f : fields_meta) {
      if (f == field) {
        LOG_WARN("update_attr have two same.");
        return RC::SQL_SYNTAX;
      }
    }

    fields_meta.push_back(field);

    update_node.expression->check_or_get(check_field);

    if (rc != RC::SUCCESS) {
      LOG_WARN("check_or_get failed.");
      return rc;
    }
    expressions.push_back(update_node.expression);
  }

  rc                      = RC::SUCCESS;
  FilterStmt *filter_stmt = nullptr;

  if (update_sql.conditions != nullptr) {
    rc = FilterStmt::create(db, table, &table_map, {table}, update_sql.conditions, filter_stmt);
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  Stmt *select_stmt = nullptr;
  View *view        = nullptr;
  if (table->is_view()) {
    view = static_cast<View *>(table);
    rc   = SelectStmt::create(db, view->select_sql(), select_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("view create failed.");
      return rc;
    }
  }

  stmt = new UpdateStmt(table, fields_meta, expressions, filter_stmt, static_cast<SelectStmt *>(select_stmt), view);
  return RC::SUCCESS;
}
