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

#include "sql/operator/logical_operator.h"
#include "sql/parser/value.h"
#include "storage/field/field_meta.h"
#include <memory>

struct UpdateLogicalNode
{
  Value                            value;
  std::unique_ptr<LogicalOperator> sub_query;
  bool                             nullable;
  AttrType                         attr_type;
  UpdateLogicalNode(const Value &v, bool n, AttrType a)
      : value(v), sub_query(nullptr), nullable(n), attr_type(a)
  {}
  UpdateLogicalNode(std::unique_ptr<LogicalOperator> sub, bool n, AttrType a)
      : sub_query(std::move(sub)), nullable(n), attr_type(a)
  {}
};
/**
 * @brief 逻辑算子，用于执行delete语句
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(
      Table *table, std::vector<UpdateLogicalNode> &values, std::vector<int> &indexs);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType             type() const override { return LogicalOperatorType::UPDATE; }
  Table                          *table() const { return table_; }
  std::vector<UpdateLogicalNode> &values() { return values_; }
  std::vector<int>               &indexs() { return indexs_; }

private:
  Table                         *table_ = nullptr;
  std::vector<UpdateLogicalNode> values_;
  std::vector<int>               indexs_;
};
