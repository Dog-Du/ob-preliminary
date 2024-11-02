/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/float_type.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include "common/lang/limits.h"
#include "common/value.h"
#include <cmath>

int FloatType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::FLOATS, "left type is not integer");

  if (left.is_null(left)) {
    return -1;
  }

  if (right.is_null(right)) {
    return 1;
  }

  float left_val  = left.get_float();
  float right_val = right.get_float();
  return common::compare_float((void *)&left_val, (void *)&right_val);
}

bool FloatType::is_null(const Value &val) const { return val.value_.float_value_ == FLOAT_NULL; }

RC FloatType::cast_to(const Value &val, AttrType attr_type, Value &result) const
{
  ASSERT(val.attr_type()==AttrType::FLOATS,"");
  RC rc = RC::SUCCESS;
  switch (attr_type) {
    case AttrType::FLOATS: {
      result.attr_type_ = AttrType::FLOATS;

      if (!val.is_null(val)) {
        result.value_.float_value_ = val.get_float();
      } else {
        result.value_.float_value_ = FLOAT_NULL;
      }
    } break;
    case AttrType::INTS: {
      result.attr_type_ = AttrType::INTS;

      if (!val.is_null(val)) {
        result.value_.int_value_ = static_cast<int>(val.get_float() + 0.5);  // 需要四舍五入。
      } else {
        result.value_.int_value_ = INT_NULL;
      }

    } break;
    case AttrType::DATES: {
      if (!val.is_null(val)) {
        return RC::VARIABLE_NOT_VALID;
      }
      result.attr_type_         = AttrType::DATES;
      result.value_.date_value_ = DATE_NULL;
    } break;
    case AttrType::CHARS: {
      result.attr_type_ = AttrType::CHARS;
      if (val.is_null(val)) {
        result.set_string(nullptr);
      } else {
        auto tmp = std::to_string(val.value_.float_value_);
        result.set_string(tmp.c_str(), tmp.size());
      }
    } break;
    default: {
      rc = RC::VARIABLE_NOT_VALID;
    }
  }
  return rc;
}

RC FloatType::add(const Value &left, const Value &right, Value &result) const
{
  result.set_float(left.get_float() + right.get_float());
  return RC::SUCCESS;
}
RC FloatType::subtract(const Value &left, const Value &right, Value &result) const
{
  result.set_float(left.get_float() - right.get_float());
  return RC::SUCCESS;
}
RC FloatType::multiply(const Value &left, const Value &right, Value &result) const
{
  result.set_float(left.get_float() * right.get_float());
  return RC::SUCCESS;
}

RC FloatType::divide(const Value &left, const Value &right, Value &result) const
{
  if (right.get_float() > -EPSILON && right.get_float() < EPSILON) {
    // NOTE:
    // 设置为浮点数最大值是不正确的。通常的做法是设置为NULL，但是当前的miniob没有NULL概念，所以这里设置为浮点数最大值。
    result.set_float(FLOAT_NULL);
  } else {
    result.set_float(left.get_float() / right.get_float());
  }
  return RC::SUCCESS;
}

RC FloatType::negative(const Value &val, Value &result) const
{
  result.set_float(-val.get_float());
  return RC::SUCCESS;
}

RC FloatType::set_value_from_str(Value &val, const string &data) const
{
  RC           rc = RC::SUCCESS;
  stringstream deserialize_stream;
  deserialize_stream.clear();
  deserialize_stream.str(data);

  float float_value;
  deserialize_stream >> float_value;
  if (!deserialize_stream || !deserialize_stream.eof()) {
    rc = RC::SCHEMA_FIELD_TYPE_MISMATCH;
  } else {
    val.set_float(float_value);
  }
  return rc;
}

RC FloatType::to_string(const Value &val, string &result) const
{
  if (is_null(val)) {
    result = NULL_STR;
    return RC::SUCCESS;
  }
  stringstream ss;
  ss << common::double_to_str(val.value_.float_value_);
  result = ss.str();
  return RC::SUCCESS;
}
