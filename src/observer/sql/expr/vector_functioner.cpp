/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2024/05/29.
//

#include "sql/expr/aggregator.h"
#include "common/log/log.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include "sql/expr/expression.h"
#include "sql/expr/vector_functioner.h"
#include <cmath>
#include <cstdint>

RC L2DistanceWorker::compute_result(const Value &left, const Value &right, Value &result) const
{
  ASSERT(left.attr_type() == AttrType::VECTORS && right.attr_type() == AttrType::VECTORS,"");
  if (left.length() != right.length()) {
    return RC::INVALID_ARGUMENT;
  }

  int size = left.length() / VECTOR_UNIT_SIZE;
  result.set_type(AttrType::FLOATS);
  float                f           = 0;
  const vector_unit_t *left_value  = (const vector_unit_t *)left.data();
  const vector_unit_t *right_value = (const vector_unit_t *)right.data();

  for (int i = 0; i < size; ++i) {
    f += (left_value[i] - right_value[i]) * (left_value[i] - right_value[i]);
  }
  f = std::sqrt(f);
  result.set_data((const char *)&f, sizeof(f));
  return RC::SUCCESS;
}

RC CosineDistanceWorker::compute_result(const Value &left, const Value &right, Value &result) const
{
  ASSERT(left.attr_type() == AttrType::VECTORS && right.attr_type() == AttrType::VECTORS,"");
  if (left.length() != right.length()) {
    return RC::INVALID_ARGUMENT;
  }

  int size = left.length() / VECTOR_UNIT_SIZE;
  result.set_type(AttrType::FLOATS);

  const vector_unit_t *left_value  = (const vector_unit_t *)left.data();
  const vector_unit_t *right_value = (const vector_unit_t *)right.data();

  float left_m  = 0;
  float right_m = 0;
  float mul     = 0;

  for (int i = 0; i < size; ++i) {
    mul += left_value[i] * right_value[i];
    left_m += left_value[i] * left_value[i];
    right_m += right_value[i] * right_value[i];
  }

  left_m  = std::sqrt(left_m);
  right_m = std::sqrt(right_m);

  float res = 1 - mul / (left_m * right_m);

  result.set_data((const char *)&res, sizeof(res));
  return RC::SUCCESS;
}

RC InnerProduceWorker::compute_result(const Value &left, const Value &right, Value &result) const
{
  ASSERT(left.attr_type() == AttrType::VECTORS && right.attr_type() == AttrType::VECTORS,"");
  if (left.length() != right.length()) {
    return RC::INVALID_ARGUMENT;
  }

  int size = left.length() / VECTOR_UNIT_SIZE;
  result.set_type(AttrType::FLOATS);

  const vector_unit_t *left_value  = (const vector_unit_t *)left.data();
  const vector_unit_t *right_value = (const vector_unit_t *)right.data();

  float f = 0;

  for (int i = 0; i < size; ++i) {
    f += left_value[i] * right_value[i];
  }

  result.set_data((const char *)&f, sizeof(f));
  return RC::SUCCESS;
}