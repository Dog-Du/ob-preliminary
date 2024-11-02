/* Copyright (c) OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/12/26.
//

#pragma once

#include "common/value.h"
#include "sql/expr/expression.h"
#include "sql/operator/logical_operator.h"
#include "storage/field/field_meta.h"

/**
 * @brief 逻辑算子，用于执行delete语句
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(
      Table *table, std::vector<const FieldMeta *> &fields, std::vector<std::shared_ptr<Expression>> &values);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }
  Table              *table() const { return table_; }
  auto               &fields_meta() { return fields_meta_; }
  auto               &expressions() { return expressions_; }

private:
  Table                                   *table_ = nullptr;
  std::vector<const FieldMeta *>           fields_meta_;
  std::vector<std::shared_ptr<Expression>> expressions_;
};
