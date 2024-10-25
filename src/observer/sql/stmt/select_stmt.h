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
// Created by Wangyunlai on 2022/6/5.
//

#pragma once

#include <memory>
#include <vector>

#include "common/rc.h"
#include "sql/stmt/stmt.h"
#include "storage/field/field.h"

class FieldMeta;
class FilterStmt;
class Db;
class Table;
class Table;

struct JoinNodes
{
  std::vector<Table *>                     tables_;
  std::vector<std::shared_ptr<FilterStmt>> conditions_;  // on的条件
};

/**
 * @brief 表示select语句
 * @ingroup Statement
 */
class SelectStmt : public Stmt
{
public:
  SelectStmt() = default;
  ~SelectStmt() override;

  StmtType type() const override { return StmtType::SELECT; }

public:
  static RC create(
      Db *db, SelectSqlNode &select_sql, Stmt *&stmt, const std::unordered_map<std::string, Table *> &all_tables = {});

public:
  std::vector<JoinNodes> &join_nodes() { return tables_; }
  FilterStmt             *filter_stmt() const { return filter_stmt_.get(); }

  std::vector<std::shared_ptr<Expression>> &query_expressions() { return query_expressions_; }
  std::vector<std::shared_ptr<Expression>> &group_by() { return group_by_; }

private:
  std::vector<std::shared_ptr<Expression>> query_expressions_;
  std::vector<JoinNodes>                   tables_;
  std::shared_ptr<FilterStmt>              filter_stmt_ = nullptr;
  std::vector<std::shared_ptr<Expression>> group_by_;
};
