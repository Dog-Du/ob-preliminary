#include "common/type/vector_type.h"
#include "common/lang/string.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include "common/log/log.h"
#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/float_type.h"
#include "common/type/attr_type.h"
#include "common/lang/limits.h"
#include <cmath>
#include <cstdlib>
#include <sstream>

bool is_float_char(char s) { return (s >= '0' && s <= '9') || s == '.'; }
bool is_valid_char(char s) { return is_float_char(s) || s == ',' || s == '[' || s == ']'; }

RC VectorType::set_from_str(const char *s, int len, Value &result)
{
  if (len <= 2) {
    LOG_WARN("str to vector failed.");
    return RC::INVALID_ARGUMENT;
  }

  for (int i = 0; i < len; ++i) {
    if (!is_valid_char(s[i])) {
      LOG_WARN("str to vector failed.");
      return RC::INVALID_ARGUMENT;
    }
  }

  std::vector<float> values;
  for (int i = 1; i < len - 1;) {
    int j = i;

    while (j < len - 1 && is_float_char(s[j])) {
      ++j;
    }

    if (s[j] != ',' && s[j] != ']') {
      return RC::INVALID_ARGUMENT;
    }

    float f = std::strtof(s + i, nullptr);
    values.push_back(f);
    i = j + 1;
  }
  result.set_vector(values.data(), values.size() * 4);
  return RC::SUCCESS;
}

int VectorType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::VECTORS && right.attr_type() == AttrType::VECTORS,"");
  ASSERT(left.length_%4==0 && right.length_%4==0,"");
  int left_size  = left.length_ / 4;
  int right_size = right.length_ / 4;

  int i;
  for (i = 0; i < left_size && i < right_size; ++i) {
    int result =
        common::compare_float((void *)&left.value_.vector_pointer_[i], (void *)&right.value_.vector_pointer_[i]);
    if (result != 0) {
      return result;
    }
  }

  return left_size == right_size ? 0 : (left_size > right_size ? 1 : -1);
}

RC VectorType::add(const Value &left, const Value &right, Value &result) const
{
  ASSERT(left.attr_type() == AttrType::VECTORS && right.attr_type() == AttrType::VECTORS,"");
  ASSERT(left.length_%4==0 && right.length_%4==0,"");
  result.attr_type_ = AttrType::VECTORS;

  if (left.length_ != right.length_) {
    return RC::INVALID_ARGUMENT;
  }

  int size = left.length_ / 4;

  std::vector<float> values(size);

  for (int i = 0; i < size; ++i) {
    values[i] = left.value_.vector_pointer_[i] + right.value_.vector_pointer_[i];
  }

  result.set_vector(values.data(), size * 4);
  return RC::SUCCESS;
}

RC VectorType::subtract(const Value &left, const Value &right, Value &result) const
{
  ASSERT(left.attr_type() == AttrType::VECTORS && right.attr_type() == AttrType::VECTORS,"");
  ASSERT(left.length_%4==0 && right.length_%4==0,"");
  result.attr_type_ = AttrType::VECTORS;

  if (left.length_ != right.length_) {
    return RC::INVALID_ARGUMENT;
  }

  int size = left.length_ / 4;

  std::vector<float> values(size);

  for (int i = 0; i < size; ++i) {
    values[i] = left.value_.vector_pointer_[i] - right.value_.vector_pointer_[i];
  }

  result.set_vector(values.data(), size * 4);
  return RC::SUCCESS;
}

RC VectorType::to_string(const Value &val, string &result) const
{
  ASSERT(val.attr_type() == AttrType::VECTORS && val.length_ %4==0,"");
  int size = val.length_ / 4;

  std::stringstream ss;
  ss << "[";
  for (int i = 0; i < size - 1; ++i) {
    ss << common::double_to_str(val.value_.vector_pointer_[i]) << ",";
  }
  ss << common::double_to_str(val.value_.vector_pointer_[size - 1]) << "]";
  result = ss.str();
  return RC::SUCCESS;
}

RC VectorType::multiply(const Value &left, const Value &right, Value &result) const { return RC::INTERNAL; }