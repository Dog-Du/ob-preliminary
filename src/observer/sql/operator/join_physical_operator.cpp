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
// Created by WangYunlai on 2022/12/30.
//

#include "sql/operator/join_physical_operator.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple_cell.h"
#include "sql/parser/parse_defs.h"

RC NestedLoopJoinPhysicalOperator::open(Trx *trx)
{
  if (children_.size() != 2) {
    LOG_WARN("nlj operator should have 2 children");
    return RC::INTERNAL;
  }

  RC rc  = RC::SUCCESS;
  left_  = children_[0].get();
  right_ = children_[1].get();

  right_closed_ = false;
  round_done_   = false;

  rc = left_->open(trx);
  left_next();
  right_->open(trx);

  while (right_->next() == RC::SUCCESS) {
    Tuple *tuple = right_->current_tuple();
    right_tuples_.push_back(*tuple);
  }

  trx_ = trx;
  return rc;
}

bool NestedLoopJoinPhysicalOperator::filter()
{
  if (comp_expr_.comp() == INVALID_COMP) {
    return true;
  }

  FieldExpr *left_expr  = static_cast<FieldExpr *>(comp_expr_.left().get());
  FieldExpr *right_expr = static_cast<FieldExpr *>(comp_expr_.right().get());

  TupleCellSpec left_spec(left_expr->table_name(), left_expr->field_name());
  TupleCellSpec right_spec(right_expr->table_name(), right_expr->field_name());

  Value left_value;
  Value right_value;

  joined_tuple_.find_cell(left_spec, left_value);
  joined_tuple_.find_cell(right_spec, right_value);

  bool ret = false;

  comp_expr_.compare_value(left_value, right_value, ret);
  return ret;
}

RC NestedLoopJoinPhysicalOperator::next()
{
  RC rc = RC::SUCCESS;

  do {
    bool left_need_step = (left_tuple_ == nullptr);
    bool flag           = true;
    if (round_done_) {
      left_need_step = true;
    } else {
      rc = right_next();
      if (rc != RC::SUCCESS) {
        if (rc == RC::RECORD_EOF) {
          left_need_step = true;
        } else {
          flag = false;
        }
      } else {
        flag = false;
      }
    }

    if (left_need_step && flag) {
      rc = left_next();
      if (rc != RC::SUCCESS) {
        return rc;
      }
    }
  } while (!filter());

  return rc;
}

RC NestedLoopJoinPhysicalOperator::close()
{
  RC rc = left_->close();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to close left oper. rc=%s", strrc(rc));
  }

  if (!right_closed_) {
    rc = right_->close();
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to close right oper. rc=%s", strrc(rc));
    } else {
      right_closed_ = true;
    }
  }
  return rc;
}

Tuple *NestedLoopJoinPhysicalOperator::current_tuple() { return &joined_tuple_; }

RC NestedLoopJoinPhysicalOperator::left_next()
{
  RC rc = RC::SUCCESS;
  rc    = left_->next();
  if (rc != RC::SUCCESS) {
    return rc;
  }

  left_tuple_ = left_->current_tuple();
  joined_tuple_.set_left(left_tuple_);
  return rc;
}

RC NestedLoopJoinPhysicalOperator::right_next()
{
  RC rc = RC::SUCCESS;
  if (round_done_) {
    if (!right_closed_) {
      rc = right_->close();

      right_closed_ = true;
      if (rc != RC::SUCCESS) {
        return rc;
      }
    }

    rc = right_->open(trx_);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    right_closed_ = false;

    round_done_ = false;
  }

  rc = right_->next();
  if (rc != RC::SUCCESS) {
    if (rc == RC::RECORD_EOF) {
      round_done_ = true;
    }
    return rc;
  }

  right_tuple_ = right_->current_tuple();
  joined_tuple_.set_right(right_tuple_);
  return rc;
}
