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
// Created by WangYunlai on 2024/05/30.
//

#include <memory>

#include "common/log/log.h"
#include "sql/operator/group_by_logical_operator.h"
#include "sql/expr/expression.h"

using namespace std;

GroupByLogicalOperator::GroupByLogicalOperator(std::vector<std::shared_ptr<FieldExpr>> &group_by_exprs,
    std::vector<std::shared_ptr<AggregateExpr>> &expressions, std::vector<FieldExpr *> &field_exprs,
    std::shared_ptr<Expression> &having_filter)
{
  group_by_expressions_  = group_by_exprs;
  aggregate_expressions_ = expressions;
  field_exprs_           = field_exprs;
  having_filter_         = having_filter;
}