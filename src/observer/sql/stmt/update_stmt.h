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

#pragma once

#include "sql/expr/expression.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"

class View;
class Table;
class FieldMeta;
class Value;
class FilterStmt;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt() = default;
  UpdateStmt(Table *table, std::vector<const FieldMeta *> &fields, std::vector<std::shared_ptr<Expression>> &value,
      FilterStmt *filter_stmt, SelectStmt *select_stmt, View *view = nullptr);

  StmtType type() const override { return StmtType::UPDATE; }

public:
  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);

public:
  auto  table() const { return table_; }
  auto  filter_stmt() { return filter_stmt_; }
  auto &expressions() { return expressions_; }
  auto &fields_meta() { return fields_meta_; }
  auto  view() { return view_; }
  auto  view_select_stmt() { return view_select_stmt_; }

private:
  View                                    *view_  = nullptr;
  Table                                   *table_ = nullptr;
  std::vector<const FieldMeta *>           fields_meta_;
  std::vector<std::shared_ptr<Expression>> expressions_;
  FilterStmt                              *filter_stmt_;
  std::shared_ptr<SelectStmt>              view_select_stmt_;
};
