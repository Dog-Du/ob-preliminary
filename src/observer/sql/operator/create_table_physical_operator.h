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
// Created by WangYunlai on 2022/6/9.
//

#pragma once

#include "sql/operator/physical_operator.h"
#include "common/types.h"

class Trx;
class DeleteStmt;

class CreateTablePhysicalOperator : public PhysicalOperator
{
public:
  CreateTablePhysicalOperator(Db *db, const std::string &table_name, const std::vector<AttrInfoSqlNode> &attr_infos,
      StorageFormat storage_format)
      : db_(db), table_name_(table_name), attr_infos_(attr_infos), storage_format_(storage_format)
  {}

  virtual ~CreateTablePhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::CREATE_TABLE; }

  RC     open(Trx *trx) override;
  RC     next() override;
  RC     close() override;
  Tuple *current_tuple() override;

private:
  Db                          *db_ = nullptr;
  std::string                  table_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
  StorageFormat                storage_format_;
};