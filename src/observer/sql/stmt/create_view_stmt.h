/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/4/25.
//

#pragma once

#include <string>

#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include "storage/field/field.h"

struct CreateIndexSqlNode;
class Table;
class FieldMeta;
class Field;

/**
 * @brief 创建索引的语句
 * @ingroup Statement
 */
class CreateViewStmt : public Stmt
{
public:
  CreateViewStmt(const std::string &view_name, const std::vector<AttrInfoSqlNode> &attr_infos,
      const std::vector<Field> &fields, const SelectSqlNode &selection, bool can_write)
      : view_name_(view_name), attr_infos_(attr_infos), fields_(fields), selection_(selection), can_write_(can_write)
  {}

  virtual ~CreateViewStmt() = default;

  StmtType type() const override { return StmtType::CREATE_VIEW; }
  auto    &can_write() const { return can_write_; }
  auto    &attr_infos() { return attr_infos_; }
  auto    &fields() { return fields_; }
  auto    &view_name() { return view_name_; }
  auto    &selection() { return selection_; }

public:
  static RC create(Db *db, const CreateViewSqlNode &create_view, Stmt *&stmt, SelectSqlNode &selection);

private:
  std::string                  view_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
  std::vector<Field>           fields_;  // 转化成原来的列
  SelectSqlNode                selection_;
  bool                         can_write_ = false;
};
