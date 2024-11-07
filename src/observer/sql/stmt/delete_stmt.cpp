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

#include "sql/stmt/delete_stmt.h"
#include "common/log/log.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "storage/table/view.h"

DeleteStmt::DeleteStmt(Table *table, FilterStmt *filter_stmt, SelectStmt *view_select_stmt, View *view)
    : table_(table), filter_stmt_(filter_stmt), view_select_stmt_(view_select_stmt), view_(view)
{}

DeleteStmt::~DeleteStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC DeleteStmt::create(Db *db, const DeleteSqlNode &delete_sql, Stmt *&stmt)
{
  const char *table_name = delete_sql.relation_name.c_str();
  if (nullptr == db || nullptr == table_name) {
    LOG_WARN("invalid argument. db=%p, table_name=%p", db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table_or_view = db->find_table(table_name);
  if (nullptr == table_or_view) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  if (!table_or_view->can_write()) {
    LOG_WARN("this view can not write.");
    return RC::INVALID_ARGUMENT;
  }

  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(std::string(table_name), table_or_view));

  RC          rc          = RC::SUCCESS;
  FilterStmt *filter_stmt = nullptr;

  if (delete_sql.conditions != nullptr) {
    rc = FilterStmt::create(db, table_or_view, &table_map, delete_sql.conditions, filter_stmt);
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  View *view             = nullptr;
  Stmt *view_select_stmt = nullptr;

  if (table_or_view->is_view()) {
    view = static_cast<View *>(table_or_view);
    rc   = SelectStmt::create(db, view->select_sql(), view_select_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("view create failed.");
      return rc;
    }
  }

  stmt = new DeleteStmt(table_or_view, filter_stmt, static_cast<SelectStmt *>(view_select_stmt), view);
  return rc;
}
