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
// Created by Wangyunlai 2023/6/27
//

#pragma once

#include <string>
#include "common/defs.h"
#include "common/lang/comparator.h"
/**
 * @brief 属性的类型
 *
 */
enum AttrType
{
  UNDEFINED = 0,
  CHARS,   ///< 字符串类型
  INTS,    ///< 整数类型(4字节)
  FLOATS,  ///< 浮点数类型(4字节)
  DATES,  ///< 日期类型，可以用一个int表示。为了初始化的时候与int区分，应该是用unsigned_int，即
          ///< uint32_t
  NULLS,
  BOOLEANS,  ///< boolean类型，当前不是由parser解析出来的，是程序内部使用的
};

using date_t = uint32_t;
const char *attr_type_to_string(AttrType type);
AttrType    attr_type_from_string(const char *s);
bool        check_date(date_t v);
date_t      str_to_date(const char *ch);
/**
 * @brief 属性的值
 *
 */

constexpr int VALUE_SIZE_NOT_CHARS =
    1 + std::max(sizeof(int), std::max(sizeof(float), std::max(sizeof(bool), sizeof(date_t))));

class Value
{
public:
  Value() = default;

  Value(AttrType attr_type, char *data, int length = VALUE_SIZE_NOT_CHARS) : attr_type_(attr_type)
  {
    this->set_data(data, length);
  }

  explicit Value(date_t val);
  explicit Value(int val);
  explicit Value(float val);
  explicit Value(bool val);
  explicit Value(const char *s, int len = 0);

  Value(const Value &other)            = default;
  Value &operator=(const Value &other) = default;

  void set_type(AttrType type) { this->attr_type_ = type; }
  void set_data(char *data, int length);
  void set_data(const char *data, int length) { this->set_data(const_cast<char *>(data), length); }
  void set_int(int val);
  void set_float(float val);
  void set_boolean(bool val);
  void set_null();
  void set_string(const char *s, int len = 0);
  void set_value(const Value &value);
  void set_date(date_t val);
  void add(const Value &val);
  void min(const Value &val);
  void max(const Value &val);

  std::string to_string() const;  // 似乎这样直接输出就行了。

  CompareResult compare(const Value &other) const;

  const char *data() const;
  int length() const { return (attr_type_ == CHARS ? str_value_.size() : VALUE_SIZE_NOT_CHARS); }

  AttrType attr_type() const { return attr_type_; }
  // const char *is_null_ptr() const { return &is_null_; }

public:
  /**
   * 获取对应的值
   * 如果当前的类型与期望获取的类型不符，就会执行转换操作
   */
  int         get_int() const;
  float       get_float() const;
  std::string get_string() const;
  bool        get_boolean() const;
  date_t      get_date() const;

private:
  AttrType attr_type_ = UNDEFINED;

  void be_null();
  void be_not_null();
  void be_all_null();
  // char is_null_{'n'};  //
  // 唔，或许我应该更改一下方式，不在纠结怎么一次存储了，直接分两次存储得了。
  // 结果到头来还是需要考虑怎么一次存储。 'n'表示否，'y'表示是。

  union
  {
    int    int_value_{0};
    float  float_value_;
    bool   bool_value_;
    date_t date_value_;
    char   is_null_[VALUE_SIZE_NOT_CHARS];
    // char is_null_[8];  // 之所以是 8 个，是因为，char is_null_[5] 会被编译器对齐为 8 个字节。
  } num_value_;

  // 又想复杂了，虽然有字节对齐，但是我们只在磁盘里面存5byte就行了。

  // char is_null_{'n'};  // 放在后端。
  // 只能放在后端的原因：字节对齐。
  // 因为字节对齐的原因，导致虽然is_null_看起来只有1byte，但是却占了4byte内存空间，剩下3byte用来字节对齐。
  // 所以如果放在前端，就会导致虽然看似只需要5byte，但实际上还是需要8byte。因此放在后端合理
  // 放在后端，相当于位于高位，这个时候处理方法和第二种方法一致。

  // bool is_null_{false};  // 一定一定把is_null放在紧挨着null_value_的地方。
  // 对于字符串的话，在value中没办法提前知道表中规定的长度，所以bool值不能放在尾端，只能放在首端。

  std::string str_value_;  // 让它的第一位始终是 is_null_。
};