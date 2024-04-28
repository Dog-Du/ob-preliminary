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
// Created by WangYunlai on 2023/06/28.
//

#include "sql/parser/value.h"
#include "common/defs.h"
#include "common/lang/comparator.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include <cstring>
#include <sstream>

// 这里的名字数组的下标应该是和value.h中的枚举类型的值是一一对应的，能不能标记清晰一点。
const char *ATTR_TYPE_NAME[] = {
    "undefined", "chars", "ints", "floats", "dates", "nulls", "booleans"};

const char *attr_type_to_string(AttrType type)
{
  // 妈的，这句话表示了，DATES应该放在,chars 和 dates 中间，不能放在booleans后面，不然会出问题。
  if (type >= UNDEFINED && type <= NULLS) {  // 修改成DATES。
    return ATTR_TYPE_NAME[type];
  }
  return "unknown";
}

AttrType attr_type_from_string(const char *s)
{
  for (unsigned int i = 0; i < sizeof(ATTR_TYPE_NAME) / sizeof(ATTR_TYPE_NAME[0]); i++) {
    if (0 == strcmp(ATTR_TYPE_NAME[i], s)) {
      return (AttrType)i;  // hhh，
    }
  }
  return UNDEFINED;
}

bool check_date(date_t v)
{
  int        y     = v / 10000;
  int        m     = v / 100 % 100;
  int        d     = v % 100;
  static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  bool       leap  = (y % 400 == 0 || (y % 100 && y % 4 == 0));
  return y > 0 && (m > 0) && (m <= 12) && (d > 0) && (d <= ((m == 2 && leap) ? 1 : 0) + mon[m]);
}

date_t str_to_date(const char *ch)
{
  int i      = -1;
  int j      = -1;
  int length = strlen(ch);
  {
    int cnt = 0;
    for (int k = 0; k < length; ++k) {
      if (ch[k] == '-') {
        ++cnt;
        if (i == -1) {
          i = k;
        } else {
          j = k;
        }
      }
    }

    if (cnt != 2) {
      return 0;  //
    }
  }

  int y = 0;
  int m = 0;
  int d = 0;

  for (int k = 0; k < i; ++k) {
    if (isdigit(ch[k])) {
      y = y * 10 + (ch[k] - '0');
    }
  }

  for (int k = i + 1; k < j; ++k) {
    if (isdigit(ch[k])) {
      m = m * 10 + (ch[k] - '0');
    }
  }

  for (int k = j + 1; k < length; ++k) {
    if (isdigit(ch[k])) {
      d = d * 10 + (ch[k] - '0');
    }
  }

  return y * 10000 + m * 100 + d;
}

void Value::max(const Value &val)
{
  if (compare(val) == CompareResult::LESS) {
    *this = val;
  }
}

void Value::min(const Value &val)
{
  if (compare(val) == CompareResult::MORE) {
    *this = val;
  }
}

void Value::add(const Value &val)
{
  switch (attr_type_) {
    case AttrType::INTS: {
      num_value_.int_value_ += val.num_value_.int_value_;
    } break;
    case AttrType::FLOATS: {
      num_value_.float_value_ += val.num_value_.float_value_;
    } break;
    default: break;
  }
}

Value::Value(int val) { set_int(val); }

Value::Value(float val) { set_float(val); }

Value::Value(bool val) { set_boolean(val); }

Value::Value(date_t val) { set_date(val); }

Value::Value(const char *s, int len /*= 0*/) { set_string(s, len); }

void Value::be_null()
{
  num_value_.is_null_[4] = num_value_.is_null_[5] = num_value_.is_null_[6] =
      num_value_.is_null_[7]                      = 'y';
}

void Value::be_not_null()
{
  num_value_.is_null_[4] = num_value_.is_null_[5] = num_value_.is_null_[6] =
      num_value_.is_null_[7]                      = 'n';
}

void Value::set_data(char *data, int length)
{
  memset(&num_value_, 0, sizeof(num_value_));

  switch (attr_type_) {
    case CHARS: {
      set_string(data + 1, length - 1);
      length_            = str_value_.size();
      str_value_.front() = *data;

      if (str_value_.front() == 'y') {
        attr_type_ = NULLS;
        str_value_ = "y";
      }
    } break;
    case INTS: {
      num_value_.int_value_ = *(int *)data;
      length_               = length;
    } break;
    case FLOATS: {
      num_value_.float_value_ = *(float *)data;
      length_                 = length;
    } break;
    case BOOLEANS: {
      num_value_.bool_value_ = (*(int *)data) != 0;
      length_                = length;
    } break;
    case DATES: {
      num_value_.date_value_ = *(date_t *)data;
      length_                = length;
    } break;
    case NULLS: {
      length_ = length;
      be_null();
      ;  // do nothing.
    } break;
    default: {
      LOG_WARN("unknown data type: %d", attr_type_);
    } break;
  }

  if (attr_type_ >= INTS && attr_type_ <= BOOLEANS) {
    num_value_.is_null_[7] = data[7];

    if (num_value_.is_null_[7] == 'y') {
      attr_type_ = NULLS;
      be_null();
    } else {
      be_not_null();
    }
  }
}

void Value::set_null()
{
  memset(&num_value_, 0, sizeof(num_value_));
  attr_type_ = NULLS;
  length_    = sizeof(num_value_);
  be_null();

  // 如果字段中为chars，而放在存储时，会变成null存储，也就是后面四个为y而前面四个为0
  // 这就导致出错，所以应该在把前4为也变成y。
  num_value_.is_null_[0] = num_value_.is_null_[1] = num_value_.is_null_[2] =
      num_value_.is_null_[3]                      = 'y';
  // num_value_.is_null_[7] = 'y';
  str_value_ = "y";
}

void Value::set_int(int val)
{
  memset(&num_value_, 0, sizeof(num_value_));
  attr_type_            = INTS;
  num_value_.int_value_ = val;
  length_               = sizeof(num_value_);
  be_not_null();
  str_value_ = "n";
}

void Value::set_float(float val)
{
  memset(&num_value_, 0, sizeof(num_value_));
  attr_type_              = FLOATS;
  num_value_.float_value_ = val;
  length_                 = sizeof(num_value_);
  be_not_null();
  str_value_ = "n";
}

void Value::set_boolean(bool val)
{
  memset(&num_value_, 0, sizeof(num_value_));
  attr_type_             = BOOLEANS;
  num_value_.bool_value_ = val;
  length_                = sizeof(num_value_);
  be_not_null();
  str_value_ = "n";
}

void Value::set_date(date_t val)
{
  memset(&num_value_, 0, sizeof(num_value_));

  attr_type_             = DATES;
  num_value_.date_value_ = val;
  length_                = sizeof(num_value_);
  be_not_null();
  str_value_ = "n";
}

void Value::set_string(const char *s, int len /*= 0*/)
{
  memset(&num_value_, 0, sizeof(num_value_));
  attr_type_ = CHARS;
  if (len > 0) {
    len = strnlen(s, len);
    str_value_.assign(s, len);
  } else {
    str_value_.assign(s);
  }
  str_value_.insert(str_value_.begin(), 'n');
  length_ = str_value_.length();
  be_not_null();
}

void Value::set_value(const Value &value)
{
  switch (value.attr_type_) {  // null 待修改。
    case INTS: {
      set_int(value.get_int());
    } break;
    case FLOATS: {
      set_float(value.get_float());
    } break;
    case CHARS: {
      set_string(value.get_string().c_str());
    } break;
    case BOOLEANS: {
      set_boolean(value.get_boolean());
    } break;
    case UNDEFINED: {
      ASSERT(false, "got an invalid value type");
    } break;
    case DATES: {
      set_date(value.get_date());
    } break;
    case NULLS: {
      length_ = sizeof(num_value_);
      be_null();
      break;
    }
  }
}

const char *Value::data() const
{
  switch (attr_type_) {
    case CHARS: {
      return const_cast<char *>(str_value_.c_str());
    } break;
    default: {
      return (const char *)&num_value_;
    } break;
  }
}

std::string Value::to_string() const
{
  std::stringstream os;
  switch (attr_type_) {
    case INTS: {
      os << num_value_.int_value_;
    } break;
    case FLOATS: {
      os << common::double_to_str(num_value_.float_value_);
    } break;
    case BOOLEANS: {
      os << num_value_.bool_value_;
    } break;
    case CHARS: {
      os << (str_value_.data() + 1);
    } break;
    case DATES: {
      os << common::date_to_str(num_value_.date_value_);  // 在这里写一个函数。
    } break;
    case NULLS: {
      os << "NULL";
    } break;
    default: {
      LOG_WARN("unsupported attr type: %d", attr_type_);
    } break;
  }
  return os.str();
}

CompareResult Value::compare(const Value &other) const
{
  if (this->attr_type_ == other.attr_type_) {
    switch (this->attr_type_) {
      case INTS: {
        return common::compare_int(
            (void *)&this->num_value_.int_value_, (void *)&other.num_value_.int_value_);
      } break;
      case FLOATS: {
        return common::compare_float(
            (void *)&this->num_value_.float_value_, (void *)&other.num_value_.float_value_);
      } break;
      case CHARS: {
        return common::compare_string((void *)(this->str_value_.c_str() + 1),
            this->str_value_.length() - 1,
            (void *)(other.str_value_.c_str() + 1),
            other.str_value_.length() - 1);
      } break;
      case BOOLEANS: {
        return common::compare_int(
            (void *)&this->num_value_.bool_value_, (void *)&other.num_value_.bool_value_);
      }
      case DATES: {
        return common::compare_int(
            (void *)&this->num_value_.date_value_, (void *)&other.num_value_.date_value_);
      }
      default: {
        LOG_WARN("unsupported type: %d", this->attr_type_);
      }
    }
  } else if (this->attr_type_ == INTS && other.attr_type_ == FLOATS) {
    float this_data = this->num_value_.int_value_;
    return common::compare_float((void *)&this_data, (void *)&other.num_value_.float_value_);
  } else if (this->attr_type_ == FLOATS && other.attr_type_ == INTS) {
    float other_data = other.num_value_.int_value_;
    return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other_data);
  }

  LOG_WARN("not supported");
  return CompareResult::INVALID;  // TODO return rc?
}

int Value::get_int() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        return (int)(std::stol(str_value_.data() + 1));
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to number. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return 0;
      }
    }
    case INTS: {
      return num_value_.int_value_;
    }
    case FLOATS: {
      return (int)(num_value_.float_value_);
    }
    case BOOLEANS: {
      return (int)(num_value_.bool_value_);
    }
    case DATES: {
      return (int)num_value_.date_value_;
    }
    case NULLS: {
      return 0;
    } break;
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return 0;
    }
  }
  return 0;
}

float Value::get_float() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        return std::stof(str_value_.data() + 1);
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to float. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return 0.0;
      }
    } break;
    case INTS: {
      return float(num_value_.int_value_);
    } break;
    case FLOATS: {
      return num_value_.float_value_;
    } break;
    case BOOLEANS: {
      return float(num_value_.bool_value_);
    } break;
    case DATES: {
      return float(num_value_.date_value_);
    } break;
    case NULLS: {
      return 0;
    } break;
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return 0;
    }
  }
  return 0;
}

std::string Value::get_string() const { return this->to_string(); }

bool Value::get_boolean() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        float val = std::stof(str_value_.data() + 1);
        if (val >= EPSILON || val <= -EPSILON) {
          return true;
        }

        int int_val = std::stol(str_value_.data() + 1);
        if (int_val != 0) {
          return true;
        }

        return !str_value_.empty();
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to float or integer. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return str_value_.size() > 1;
      }
    } break;
    case INTS: {
      return num_value_.int_value_ != 0;
    } break;
    case FLOATS: {
      float val = num_value_.float_value_;
      return val >= EPSILON || val <= -EPSILON;
    } break;
    case BOOLEANS: {
      return num_value_.bool_value_;
    } break;
    case DATES: {
      return num_value_.date_value_ != 0;
    } break;
    case NULLS: {
      return 0;
    } break;
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return false;
    }
  }
  return false;
}

date_t Value::get_date() const
{
  switch (attr_type_) {
    case CHARS: {  // 其实这里应该进行判断，但是懒得管了。
      try {
        return (date_t)(std::stol(str_value_.data() + 1));
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to date. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return 0;
      }
    }
    case INTS: {
      return date_t(num_value_.int_value_);
    }
    case FLOATS: {
      return date_t(num_value_.float_value_);
    }
    case BOOLEANS: {
      return date_t(num_value_.bool_value_);
    }
    case DATES: {
      return num_value_.date_value_;
    }
    case NULLS: {
      return 0;
    } break;
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return 0;
    }
  }
  return 0;
}