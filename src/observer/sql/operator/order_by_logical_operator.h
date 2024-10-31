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
// Created by NiuXiangNan on 2023/4/25.
//

#pragma once

#include <vector>

#include "sql/expr/expression.h"
#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"
#include "storage/field/field.h"
#include "sql/stmt/groupby_stmt.h"
/**
 * @brief 逻辑算子
 * @ingroup LogicalOperator
 */
class OrderByLogicalOperator : public LogicalOperator
{
public:
  OrderByLogicalOperator(
      std::vector<std::shared_ptr<FieldExpr>> &orderby_fields, std::vector<OrderByType> &order_by_type);
  virtual ~OrderByLogicalOperator() = default;

  LogicalOperatorType                      type() const override { return LogicalOperatorType::ORDER_BY; }
  std::vector<std::shared_ptr<FieldExpr>> &order_by() { return order_by_; }
  std::vector<OrderByType>                &order_by_type() { return order_by_type_; }
  // std::vector<std::shared_ptr<FieldExpr>>     &field_exprs() { return field_exprs_; }

private:
  std::vector<std::shared_ptr<FieldExpr>> order_by_;
  std::vector<OrderByType>                order_by_type_;
  // std::vector<std::shared_ptr<AggregateExpr>> agg_exprs_;
  // std::vector<std::shared_ptr<FieldExpr>>     field_exprs_;
};