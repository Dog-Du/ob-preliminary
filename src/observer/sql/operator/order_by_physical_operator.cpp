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

#include "common/log/log.h"
#include "sql/expr/tuple.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "storage/field/field.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "sql/operator/order_by_physical_operator.h"

RC OrderByPhysicalOperator::open(Trx *trx)
{
  if (!children_.empty() && i_ == -2) {
    auto &child = children_[0];
    child->open(trx);

    ValueListTuple value_tuple;

    while (child->next() == RC::SUCCESS) {
      Tuple *tuple = child->current_tuple();

      int n = tuple->cell_num();

      std::vector<Value> values;
      values.reserve(n);

      for (int i = 0; i < n; ++i) {
        Value v;
        tuple->cell_at(i, v);
        values.emplace_back(v);
      }

      value_tuple.set_cells(values);

      tuples_.emplace_back(value_tuple);
    }

    child->close();  // 需要直接关闭.

    auto cmp = [this](const ValueListTuple &lhs, const ValueListTuple &rhs) -> bool {
      for (auto &it : orders_) {
        Value left;
        Value right;

        it->left()->try_get_value(left);
        it->right()->try_get_value(right);

        lhs.cell_at(left.get_int(), left);
        rhs.cell_at(right.get_int(), right);

        if (left.attr_type() == NULLS && right.attr_type() == NULLS) {
          continue;
        } else if (left.attr_type() == NULLS) {
          return (it->comp() == CompOp::LESS_THAN ? true : false);
        } else if (right.attr_type() == NULLS) {
          return (it->comp() == CompOp::GREAT_THAN ? true : false);
        }

        bool l, r;
        it->compare_value(left, right, l);
        it->compare_value(right, left, r);

        if (l) {
          return true;
        }

        if (r) {
          return false;
        }
      }

      // 如果完全相同，返回false
      return false;
    };

    std::sort(tuples_.begin(), tuples_.end(), cmp);
  }

  i_ = -1;
  return RC::SUCCESS;
}

RC OrderByPhysicalOperator::next()
{
  return (++i_ >= static_cast<int>(tuples_.size())) ? RC::RECORD_EOF : RC::SUCCESS;
}

RC OrderByPhysicalOperator::close() { return RC::SUCCESS; }
