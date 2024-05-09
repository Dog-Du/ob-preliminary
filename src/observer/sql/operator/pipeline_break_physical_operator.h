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

#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include <memory>

class PipeLineBreakPhysicalOperator : public PhysicalOperator
{
public:
  PipeLineBreakPhysicalOperator() {}
  PipeLineBreakPhysicalOperator(std::vector<Value> &values)
  {
    tuples_.reserve(values.size());
    ValueListTuple tuple;

    for (auto &it : values) {
      tuple.set_cells({it});
      tuples_.emplace_back(tuple);
    }
  }

  PipeLineBreakPhysicalOperator(std::unique_ptr<PhysicalOperator> child)
  {
    add_child(std::move(child));
  }

  virtual ~PipeLineBreakPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::PipeLineBreak; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return &tuples_[i_]; }

private:
  std::vector<ValueListTuple> tuples_;
  int                         i_{-2};  // i=-2表示初态
};
