/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/integer_type.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include <gtest/gtest.h>

int IntegerType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::INTS, "left type is not integer");
  // ASSERT(right.attr_type() == AttrType::INTS || right.attr_type() == AttrType::FLOATS, "right type is not numeric");

  if (left.is_null(left)) {
    return -1;
  }

  if (right.is_null(right)) {
    return 1;
  }

  if (right.attr_type() == AttrType::INTS) {
    return common::compare_int((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);
  } else if (right.attr_type() == AttrType::FLOATS) {
    float left_val  = left.get_float();
    float right_val = right.get_float();
    return common::compare_float((void *)&left_val, (void *)&right_val);
  } else if (right.attr_type() == AttrType::CHARS) {
    int l_int = left.get_int();
    int r_int = right.get_int();
    return common::compare_int((void *)&l_int, (void *)&r_int);
  }
  return INT32_MAX;
}

bool IntegerType::is_null(const Value &val) const { return val.value_.int_value_ == INT_NULL; }

RC IntegerType::cast_to(const Value &val, AttrType attr_type, Value &result) const
{
  RC rc = RC::SUCCESS;
  switch (attr_type) {
    case AttrType::INTS: {
      result.attr_type_        = AttrType::INTS;
      result.value_.int_value_ = val.get_int();
    } break;
    case AttrType::FLOATS: {
      result.attr_type_          = AttrType::FLOATS;
      result.value_.float_value_ = static_cast<float>(val.value_.int_value_);
    } break;
    default: {
      rc = RC::VARIABLE_NOT_VALID;
    } break;
  }
  return rc;
};

RC IntegerType::add(const Value &left, const Value &right, Value &result) const
{
  result.set_int(left.get_int() + right.get_int());
  return RC::SUCCESS;
}

RC IntegerType::divide(const Value &left, const Value &right, Value &result) const
{
  if (right.get_float() == 0) {
    result.set_int(INT_NULL);
    return RC::SUCCESS;
  }

  result.set_float(left.get_float() / right.get_float());
  return RC::SUCCESS;
}

RC IntegerType::subtract(const Value &left, const Value &right, Value &result) const
{
  result.set_int(left.get_int() - right.get_int());
  return RC::SUCCESS;
}

RC IntegerType::multiply(const Value &left, const Value &right, Value &result) const
{
  result.set_int(left.get_int() * right.get_int());
  return RC::SUCCESS;
}

RC IntegerType::negative(const Value &val, Value &result) const
{
  result.set_int(-val.get_int());
  return RC::SUCCESS;
}

RC IntegerType::set_value_from_str(Value &val, const string &data) const
{
  RC           rc = RC::SUCCESS;
  stringstream deserialize_stream;
  deserialize_stream.clear();  // 清理stream的状态，防止多次解析出现异常
  deserialize_stream.str(data);
  int int_value;
  deserialize_stream >> int_value;
  if (!deserialize_stream || !deserialize_stream.eof()) {
    rc = RC::SCHEMA_FIELD_TYPE_MISMATCH;
  } else {
    val.set_int(int_value);
  }
  return rc;
}

RC IntegerType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.value_.int_value_;
  result = ss.str();
  return RC::SUCCESS;
}