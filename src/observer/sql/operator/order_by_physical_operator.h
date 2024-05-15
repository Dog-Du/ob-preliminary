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
// Created by WangYunlai on 2022/07/01.
//

#pragma once

#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include <memory>

class OrderByPhysicalOperator : public PhysicalOperator
{
public:
  OrderByPhysicalOperator() {}

  OrderByPhysicalOperator(std::vector<std::unique_ptr<ComparisonExpr>> &o) : orders_(std::move(o))
  {}

  virtual ~OrderByPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::ORDER_BY; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return &tuples_[i_]; }

private:
  std::vector<ValueListTuple>                  tuples_;
  std::vector<std::unique_ptr<ComparisonExpr>> orders_;
  int                                          i_{-2};  // i=-2表示初态
};
