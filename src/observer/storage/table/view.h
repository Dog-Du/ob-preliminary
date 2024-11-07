/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its
affiliates. All rights reserved. miniob is licensed under Mulan PSL v2. You can
use this software according to the terms and conditions of the Mulan PSL v2. You
may obtain a copy of Mulan PSL v2 at: http://license.coscl.org.cn/MulanPSL2 THIS
SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Meiyi & Wangyunlai on 2021/5/12.
//

#pragma once

#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "storage/field/field.h"
#include "storage/table/table_meta.h"
#include "common/types.h"
#include "common/lang/span.h"
#include "common/lang/functional.h"
#include "storage/table/table.h"
#include "common/log/log.h"

/**
 * @brief 表
 *
 */
class View : public Table
{
public:
  View()  = default;
  ~View() = default;

public:
  RC drop(const char *path) override { return RC::SUCCESS; }
  RC open(Db *db, const char *meta_file, const char *base_dir) override { return RC::SUCCESS; }

  RC create(Db *db, int32_t table_id, const char *path, const char *name, const char *base_dir,
      span<const AttrInfoSqlNode> attr_infos, const std::vector<Field> &fields, const SelectSqlNode &select_sql,
      StorageFormat storage_format, bool can_write);

  int32_t     table_id() const override { return table_id_; }
  const char *name() const override { return view_name_.c_str(); }

  Db *db() const override { return db_; }

  RC                  sync() override { return RC::SUCCESS; };
  bool                is_view() const override { return true; }
  bool                can_write() const override { return can_write_; }
  std::vector<Field> &fields() { return fields_; }
  SelectSqlNode      &select_sql() { return select_sql_; }
  Table              *table() override { return table_; }
  const TableMeta    &table_meta() const override { return table_meta_; }

private:
  Table             *table_ = nullptr;  // 如果can_write==true, table_ != nullptr.
  TableMeta          table_meta_;       // 存放的是本视图的meta，偏移从 0 开始
  std::string        view_name_;
  int32_t            table_id_;
  Db                *db_        = nullptr;
  bool               can_write_ = false;
  std::vector<Field> fields_;  // 放在的是到原表的映射，
  SelectSqlNode      select_sql_;
};