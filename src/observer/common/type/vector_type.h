/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include "common/type/data_type.h"

/**
 * @brief 向量类型
 * @ingroup DataType
 */
class VectorType : public DataType
{
public:
  VectorType() : DataType(AttrType::VECTORS) {}
  virtual ~VectorType() {}

  // 要求输入的是形如 ： "[****]" 的字符串。
  static RC set_from_str(const char *s, int len, Value &result);

  int compare(const Value &left, const Value &right) const override;
  RC  add(const Value &left, const Value &right, Value &result) const override;
  RC  subtract(const Value &left, const Value &right, Value &result) const override;
  RC  multiply(const Value &left, const Value &right, Value &result) const override;
  RC  to_string(const Value &val, string &result) const override;
};