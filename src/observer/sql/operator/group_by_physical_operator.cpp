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
// Created by WangYunlai on 2024/06/11.
//
#include <algorithm>

#include "common/log/log.h"
#include "sql/operator/group_by_physical_operator.h"
#include "sql/expr/expression.h"
#include "sql/expr/expression_tuple.h"
#include "sql/expr/composite_tuple.h"
#include "sql/expr/tuple.h"
#include "sql/expr/tuple_cell.h"
#include "sql/operator/physical_operator.h"
#include "storage/trx/trx.h"

using namespace std;
using namespace common;

GroupByPhysicalOperator::GroupByPhysicalOperator(std::vector<std::shared_ptr<AggregateExpr>> &aggregation_expression,
    std::vector<std::shared_ptr<FieldExpr>> &groupby_expressions, std::vector<FieldExpr *> &field_exprs,
    std::shared_ptr<Expression> &having_filter)
{
  aggregate_expressions_     = aggregation_expression;
  groupby_field_expressions_ = groupby_expressions;
  field_expressions_         = field_exprs;
  having_filter_             = having_filter;

  std::vector<TupleCellSpec> tuple_schema;
  for (auto field : field_expressions_) {
    TupleCellSpec tuple_cell(field->table_name(), field->field_name());
    tuple_schema.push_back(tuple_cell);
  }

  for (auto &agg : aggregate_expressions_) {
    TupleCellSpec tuple_cell(agg->name());
    tuple_schema.push_back(tuple_cell);
  }

  tuple_.set_names(tuple_schema);
  tuple_.resize(tuple_schema.size());
}

RC GroupByPhysicalOperator::group_by(PhysicalOperator *child, Trx *trx)
{
  RC rc = child->open(trx);

  if (rc != RC::SUCCESS) {
    LOG_WARN("open failed.");
    return rc;
  }

  rc = child->next();

  if (rc != RC::SUCCESS) {
    if (rc != RC::RECORD_EOF) {
      LOG_WARN("next failed.");
      return rc;
    }
    rc = RC::SUCCESS;
    return rc;
  }

  Tuple *current_tuple = child->current_tuple();

  std::vector<Value> values;
  values.resize(current_tuple->cell_num());
  for (int i = 0; i < current_tuple->cell_num(); ++i) {
    TupleCellSpec tuple_cell;
    current_tuple->cell_at(i, values[i]);
    current_tuple->spec_at(i, tuple_cell);
    tuple_schema_.push_back(tuple_cell);
  }
  tuples_.emplace_back(std::move(values));

  // 收集所有tuple。
  while ((rc = child->next()) == RC::SUCCESS) {
    current_tuple = child->current_tuple();
    values.resize(current_tuple->cell_num());
    for (int i = 0; i < current_tuple->cell_num(); ++i) {
      current_tuple->cell_at(i, values[i]);
    }

    tuples_.emplace_back(std::move(values));
  }

  if (rc != RC::SUCCESS) {
    if (rc != RC::RECORD_EOF) {
      LOG_WARN("next failed");
      return rc;
    }
    rc = RC::SUCCESS;
  }

  // 把所有tuple分组。
  for (auto &tuple : tuples_) {
    GroupByKey key(&tuple, &tuple_schema_, &groupby_field_expressions_);
    ShellTuple value;
    value.set_cells(&tuple);
    value.set_names(&tuple_schema_);
    group_tuples_[key].push_back(value);
  }

  // 现在已经分好组了。可以开始聚合。
  return rc;
}

RC GroupByPhysicalOperator::group_filter()
{
  if (having_filter_ == nullptr) {
    return RC::SUCCESS;
  }

  Tuple                            *tuple      = nullptr;
  std::function<void(Expression *)> expr_reset = [&](Expression *expression) {
    switch (expression->type()) {
      case ExprType::AGGREGATION: {
        auto expr = static_cast<AggregateExpr *>(expression);
        expr->reset();
        if (expr->child() != nullptr) {
          expr_reset(expr->child().get());
        }
      } break;
      case ExprType::ARITHMETIC: {
        auto expr = static_cast<ArithmeticExpr *>(expression);
        if (expr->left() != nullptr) {
          expr_reset(expr->left().get());
        }
        if (expr->right() != nullptr) {
          expr_reset(expr->right().get());
        }
      } break;
      case ExprType::COMPARISON: {
        auto expr = static_cast<ComparisonExpr *>(expression);
        if (expr->left() != nullptr) {
          expr_reset(expr->left().get());
        }
        if (expr->right() != nullptr) {
          expr_reset(expr->right().get());
        }
      } break;
      case ExprType::CONJUNCTION: {
        auto expr = static_cast<ConjunctionExpr *>(expression);
        for (auto &c : expr->children()) {
          expr_reset(c.get());
        }
      } break;
      default: {
      } break;
    }
  };

  std::function<void(Expression *)> expr_accumulate = [&](Expression *expression) {
    switch (expression->type()) {
      case ExprType::AGGREGATION: {
        auto expr = static_cast<AggregateExpr *>(expression);
        if (expr->child() != nullptr) {
          expr_accumulate(expr->child().get());
        }
        expr->accumulate(*tuple);
      } break;
      case ExprType::ARITHMETIC: {
        auto expr = static_cast<ArithmeticExpr *>(expression);
        if (expr->left() != nullptr) {
          expr_accumulate(expr->left().get());
        }
        if (expr->right() != nullptr) {
          expr_accumulate(expr->right().get());
        }
      } break;
      case ExprType::COMPARISON: {
        auto expr = static_cast<ComparisonExpr *>(expression);
        if (expr->left() != nullptr) {
          expr_accumulate(expr->left().get());
        }
        if (expr->right() != nullptr) {
          expr_accumulate(expr->right().get());
        }
      } break;
      case ExprType::CONJUNCTION: {
        auto expr = static_cast<ConjunctionExpr *>(expression);
        for (auto &c : expr->children()) {
          expr_accumulate(c.get());
        }
      } break;
      default: {
      } break;
    }
  };

  for (auto group_iter = group_tuples_.begin(); group_iter != group_tuples_.end();) {
    having_filter_->check_or_get(expr_reset);

    ASSERT(!group_iter->second.empty(),"group be empty.");
    for (auto &t : group_iter->second) {
      tuple = &t;
      having_filter_->check_or_get(expr_accumulate);
    }

    /// TODO: 如果分组为空怎么办？
    // 哦哦哦，分组不可能为空，因为key是按照value分的组，
    Value value;
    having_filter_->get_value(*tuple, value);

    if (value.get_boolean() == false) {
      LOG_WARN("a group be filtered.");
      group_iter = group_tuples_.erase(group_iter);
    } else {
      ++group_iter;
    }
  }

  return RC::SUCCESS;
}

RC GroupByPhysicalOperator::open(Trx *trx)
{
  RC rc = group_by(children_[0].get(), trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("init failed.");
    return rc;
  }

  rc = group_filter();
  if (rc != RC::SUCCESS) {
    LOG_WARN("group_filter failed.");
    return rc;
  }

  group_iter_ = group_tuples_.begin();
  first_      = true;
  return rc;
}

RC GroupByPhysicalOperator::close() { return children_[0]->close(); }

RC GroupByPhysicalOperator::next()
{
  if (first_) {
    first_ = false;
  } else {
    ++group_iter_;
  }

  if (group_iter_ == group_tuples_.end()) {
    return RC::RECORD_EOF;
  }

  for (auto &agg : aggregate_expressions_) {
    agg->reset();
  }

  for (auto &tuple : group_iter_->second) {
    for (auto &agg : aggregate_expressions_) {
      agg->accumulate(tuple);
    }
  }

  auto &field_tuple = group_iter_->first.tuple;

  for (int i = 0; i < field_expressions_.size(); ++i) {
    Value tmp;
    field_expressions_[i]->get_value(field_tuple, tmp);
    tuple_.set_cell(i, tmp);
  }

  for (int i = field_expressions_.size(); i < field_expressions_.size() + aggregate_expressions_.size(); ++i) {
    Value tmp;
    aggregate_expressions_[i - field_expressions_.size()]->evaluate(tmp);
    tuple_.set_cell(i, tmp);
  }

  return RC::SUCCESS;
}

Tuple *GroupByPhysicalOperator::current_tuple() { return &tuple_; }

// RC     GroupByPhysicalOperator::evaluate(GroupValueType &group_value)
// {
//   RC rc = RC::SUCCESS;

//   vector<TupleCellSpec> aggregator_names;
//   for (auto &expr : aggregate_expressions_) {
//     aggregator_names.emplace_back(expr->name());
//   }

//   AggregatorList &aggregators           = get<0>(group_value);
//   CompositeTuple &composite_value_tuple = get<1>(group_value);

//   ValueListTuple evaluated_tuple;
//   vector<Value>  values;
//   for (unique_ptr<Aggregator> &aggregator : aggregators) {
//     Value value;
//     rc = aggregator->evaluate(value);
//     if (OB_FAIL(rc)) {
//       LOG_WARN("failed to evaluate aggregator. rc=%s", strrc(rc));
//       return rc;
//     }
//     values.emplace_back(value);
//   }

//   evaluated_tuple.set_cells(values);
//   evaluated_tuple.set_names(aggregator_names);

//   composite_value_tuple.add_tuple(make_unique<ValueListTuple>(std::move(evaluated_tuple)));

//   return rc;
// }

// RC GroupByPhysicalOperator::aggregate(AggregatorList &aggregator_list, const Tuple &tuple)
// {
//   ASSERT(static_cast<int>(aggregator_list.size()) == tuple.cell_num(),
//          "aggregator list size must be equal to tuple size. aggregator num: %d, tuple num: %d",
//          aggregator_list.size(), tuple.cell_num());

//   RC        rc = RC::SUCCESS;
//   Value     value;
//   const int size = static_cast<int>(aggregator_list.size());
//   for (int i = 0; i < size; i++) {
//     Aggregator *aggregator = aggregator_list[i].get();

//     rc = tuple.cell_at(i, value);
//     if (OB_FAIL(rc)) {
//       LOG_WARN("failed to get value from expression. rc=%s", strrc(rc));
//       return rc;
//     }

//     rc = aggregator->accumulate(value);
//     if (OB_FAIL(rc)) {
//       LOG_WARN("failed to accumulate value. rc=%s", strrc(rc));
//       return rc;
//     }
//   }

//   return rc;
// }