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
#include <cstdint>

RC SumAggregator::accumulate(const Value &value)
{
  // 初始化。
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = Value::NULL_VALUE();
  }

  if (value.is_null(value)) {
    return RC::SUCCESS;
  }

  if (value_.is_null(value_)) {
    value_ = value;
    return RC::SUCCESS;
  }

  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s",
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));

  Value::add(value, value_, value_);
  return RC::SUCCESS;
}

RC SumAggregator::evaluate(Value &result)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = Value::NULL_VALUE();
  }
  result = value_;
  return RC::SUCCESS;
}

RC MinAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = Value::NULL_VALUE();
  }

  if (value.is_null(value)) {
    return RC::SUCCESS;
  }

  if (value_.is_null(value_)) {
    value_ = value;
    return RC::SUCCESS;
  }

  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s",
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));

  if (value.compare(value_) < 0) {
    value_ = value;
  }
  return RC::SUCCESS;
}

RC MinAggregator::evaluate(Value &result)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = Value::NULL_VALUE();
  }
  result = value_;
  return RC::SUCCESS;
}

RC MaxAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = Value::NULL_VALUE();
  }

  if (value.is_null(value)) {
    return RC::SUCCESS;
  }

  if (value_.is_null(value_)) {
    value_ = value;
    return RC::SUCCESS;
  }

  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s",
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));

  if (value.compare(value_) > 0) {
    value_ = value;
  }
  return RC::SUCCESS;
}

RC MaxAggregator::evaluate(Value &result)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = Value::NULL_VALUE();
  }
  result = value_;
  return RC::SUCCESS;
}

RC CountAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_.set_type(AttrType::INTS);
    int32_t x = 0;
    value_.set_data((const char *)&x, sizeof(x));
  }

  if (value.is_null(value)) {  // NULL 不计。
    return RC::SUCCESS;
  }

  if (value_.is_null(value_)) {
    value_.set_type(AttrType::INTS);
    int32_t x = 0;
    value_.set_data((const char *)&x, sizeof(x));
  }

  Value tmp;
  tmp.set_type(AttrType::INTS);
  int32_t x = 1;
  tmp.set_data((const char *)&x, sizeof(x));
  Value::add(value_, tmp, value_);
  return RC::SUCCESS;
}

RC CountAggregator::evaluate(Value &result)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_.set_type(AttrType::INTS);
    int32_t x = 0;
    value_.set_data((const char *)&x, sizeof(x));
  }
  result = value_;
  return RC::SUCCESS;
}

RC CountStarAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_.set_type(AttrType::INTS);
    int32_t x = 1;
    value_.set_data((const char *)&x, sizeof(x));
    return RC::SUCCESS;
  }

  if (value_.is_null(value_)) {
    value_.set_type(AttrType::INTS);
    int32_t x = 0;
    value_.set_data((const char *)&x, sizeof(x));
  }

  Value tmp;
  tmp.set_type(AttrType::INTS);
  int32_t x = 1;
  tmp.set_data((const char *)&x, sizeof(x));
  Value::add(value_, tmp, value_);
  return RC::SUCCESS;
}

RC CountStarAggregator::evaluate(Value &result)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_.set_type(AttrType::INTS);
    int32_t x = 0;
    value_.set_data((const char *)&x, sizeof(x));
  }
  result = value_;
  return RC::SUCCESS;
}

RC AvgAggregator::accumulate(const Value &value)
{
  // 初始化。
  if (value_.attr_type() == AttrType::UNDEFINED) {
    count_ = 0;
    value_ = Value::NULL_VALUE();
  }

  if (value.is_null(value)) {
    return RC::SUCCESS;
  }

  ++count_;
  if (value_.is_null(value_)) {
    value_ = value;
    return RC::SUCCESS;
  }

  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s",
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));

  Value::add(value, value_, value_);
  return RC::SUCCESS;
}

RC AvgAggregator::evaluate(Value &result)
{
  Value tmp;
  result.set_type(AttrType::FLOATS);
  tmp.set_type(AttrType::INTS);
  tmp.set_data((const char *)&count_, sizeof(count_));
  return Value::divide(value_, tmp, result);
}