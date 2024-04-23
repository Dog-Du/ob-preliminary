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
#include <memory>

/**
 * @brief 逻辑算子，用于执行delete语句
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, std::shared_ptr<Value> &ptr, int attr_index);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType     type() const override { return LogicalOperatorType::UPDATE; }
  Table                  *table() const { return table_; }
  std::shared_ptr<Value> &value_ptr() { return value_; }
  int                     attr_index() const { return attr_index_; }

private:
  Table                 *table_ = nullptr;
  std::shared_ptr<Value> value_;
  int                    attr_index_;
};
