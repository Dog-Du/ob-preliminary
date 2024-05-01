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

#include "common/rc.h"
#include "sql/parser/value.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/stmt.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include <memory>

class Table;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt() = default;
  UpdateStmt(
      Table *table, std::vector<Value> &values, std::vector<int> &indexs, FilterStmt *filter_stmt)
      : table_(table), filter_stmt_(filter_stmt)
  {
    values_.swap(values);
    indexs_.swap(indexs);
  }

  ~UpdateStmt();

public:
  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);

public:
  Table              *table() const { return table_; }
  StmtType            type() const { return StmtType::UPDATE; }
  FilterStmt         *filter_stmt() const { return filter_stmt_; }
  std::vector<Value> &values() { return values_; }
  std::vector<int>   &indexs() { return indexs_; }

private:
  Table             *table_ = nullptr;
  FilterStmt        *filter_stmt_;
  std::vector<Value> values_;
  std::vector<int>   indexs_;
};
