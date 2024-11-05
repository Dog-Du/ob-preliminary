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
// Created by WangYunlai on 2022/6/27.
//

#include "sql/operator/order_by_physical_operator.h"
#include "common/log/log.h"
#include "common/value.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/expr/tuple_cell.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/field/field.h"
#include "storage/record/record.h"
#include <climits>
#include <cstddef>

OrderByPhysicalOperator::OrderByPhysicalOperator(
    std::vector<std::shared_ptr<Expression>> &field_expression, std::vector<OrderByType> &order_by_type)
    : field_expressions_(field_expression), order_by_type_(order_by_type)
{}

RC OrderByPhysicalOperator::open(Trx *trx)
{
  if (children_.size() != 1) {
    LOG_WARN("order_by operator must has one child");
    return RC::INTERNAL;
  }

  auto &child = children_[0];
  RC    rc    = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("order_by open failed.");
    child->close();
    return rc;
  }

  std::vector<Value> values;
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

  values.resize(current_tuple->cell_num());
  tuple_cells_.resize(current_tuple->cell_num());

  for (int i = 0; i < current_tuple->cell_num(); ++i) {
    current_tuple->cell_at(i, values[i]);
    current_tuple->spec_at(i, tuple_cells_[i]);
  }

  tuples_.emplace_back(std::move(values));

  std::vector<int> field_index;  // 这些个field都在tuple的第几列，直接得到，减少查找开销。
  for (auto &field : field_expressions_) {
    TupleCellSpec tmp_cell(field->table_name(), field->field_name());
    int           i;
    for (i = 0; i < tuple_cells_.size(); ++i) {
      if (tuple_cells_[i].equals(tmp_cell)) {
        field_index.push_back(i);
        break;
      }
    }

    if (i == tuple_cells_.size()) {
      field_index.push_back(-1);  // 说明应该是函数。
      if (field->type() != ExprType::VECTOR_FUNCTION) {
        LOG_WARN("should be vector_function");
        child->close();
        return RC::INVALID_ARGUMENT;
      }
    }
  }

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
      LOG_WARN("next failed.");
      return rc;
    }
    rc = RC::SUCCESS;
  }

  tuples_index_.resize(tuples_.size());

  for (int i = 0; i < tuples_index_.size(); ++i) {
    tuples_index_[i] = i;
  }

  Value      left_value;
  Value      right_value;
  ShellTuple left_tuple;
  ShellTuple right_tuple;
  left_tuple.set_names(&tuple_cells_);
  right_tuple.set_names(&tuple_cells_);

  auto cmp = [this, &field_index, &left_value, &right_value, &left_tuple, &right_tuple](int lhs, int rhs) {
    int result = 0;
    for (size_t i = 0; i < field_index.size(); ++i) {
      Value *left;
      Value *right;

      if (field_index[i] == -1) {
        left_tuple.set_cells(&tuples_[lhs]);
        right_tuple.set_cells(&tuples_[rhs]);
        field_expressions_[i]->get_value(left_tuple, left_value);
        field_expressions_[i]->get_value(right_tuple, right_value);
        left  = &left_value;
        right = &right_value;
      } else {
        left  = &tuples_[lhs][field_index[i]];
        right = &tuples_[rhs][field_index[i]];
      }

      result = left->compare(*right);
      result = (left->is_null(*left) && right->is_null(*right)) ? 0 : result;
      if (result == 0) {
        continue;
      }

      return order_by_type_[i] == OrderByType::ASC ? result < 0 : result > 0;
    }
    return false;
  };

  std::sort(tuples_index_.begin(), tuples_index_.end(), cmp);
  i_ = -1;
  return RC::SUCCESS;
}

RC OrderByPhysicalOperator::next() { return ++i_ == tuples_index_.size() ? RC::RECORD_EOF : RC::SUCCESS; }

RC OrderByPhysicalOperator::close()
{
  children_[0]->close();
  tuples_.clear();
  tuple_cells_.clear();
  tuples_index_.clear();
  i_ = -1;
  return RC::SUCCESS;
}

Tuple *OrderByPhysicalOperator::current_tuple()
{
  tuple_.set_cells(&tuples_[tuples_index_[i_]]);
  tuple_.set_names(&tuple_cells_);
  return &tuple_;
}
