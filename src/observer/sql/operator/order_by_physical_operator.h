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
// Created by WangYunlai on 2024/05/29.
//

#pragma once

#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/expr/tuple_cell.h"
#include "sql/operator/physical_operator.h"
#include "sql/expr/composite_tuple.h"
#include "storage/trx/trx.h"
#include <cstddef>

/**
 * @brief Group By 物理算子基类
 * @ingroup PhysicalOperator
 */
class OrderByPhysicalOperator : public PhysicalOperator
{
public:
  OrderByPhysicalOperator(
      std::vector<std::shared_ptr<FieldExpr>> &field_expression, std::vector<OrderByType> &order_by_type);

  virtual ~OrderByPhysicalOperator() = default;

  RC                   open(Trx *trx) override;
  RC                   next() override;
  RC                   close() override;
  Tuple               *current_tuple() override;
  PhysicalOperatorType type() const override { return PhysicalOperatorType::ORDER_BY; }

private:
  std::vector<std::shared_ptr<FieldExpr>> field_expressions_;
  std::vector<OrderByType>                order_by_type_;
  std::vector<std::vector<Value>>         tuples_;
  std::vector<TupleCellSpec>              tuple_cells_;
  std::vector<int>                        tuples_index_;
  int                                     i_ = -1;
  ShellTuple                              tuple_;
  // std::vector<Expression *>                   value_expressions_;  /// 计算聚合时的表达式
};