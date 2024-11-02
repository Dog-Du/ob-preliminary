#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "common/type/attr_type.h"
#include "common/type/data_type.h"
#include "common/type/date_type.h"
#include "common/value.h"
#include "common/lang/limits.h"
#include "common/value.h"
#include <cstdint>
#include <iomanip>
#include <sstream>

int DateType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::DATES, "invalid type");

  if (left.is_null(left)) {
    return -1;
  }

  if (right.is_null(right)) {
    return 1;
  }

  if (right.attr_type() == AttrType::DATES) {
    return common::compare_int((void *)&left.value_.date_value_, (void *)&right.value_.date_value_);
  } else {
    Value cast_value;
    right.cast_to(right, AttrType::DATES, cast_value);
    return common::compare_int((void *)&left.value_.date_value_, (void *)&cast_value.value_.date_value_);
  }
  return INT32_MAX;
}

bool DateType::is_null(const Value &val) const { return val.value_.date_value_ == DATE_NULL; }

RC DateType::to_string(const Value &val, string &result) const
{
  if (is_null(val)) {
    result = NULL_STR;
    return RC::SUCCESS;
  }

  stringstream ss;
  ss << std::setw(4) << std::setfill('0') << val.value_.int_value_ / 10000 << '-' << std::setw(2) << std::setfill('0')
     << (val.value_.int_value_ % 10000) / 100 << '-' << std::setw(2) << std::setfill('0')
     << val.value_.int_value_ % 100;
  result = ss.str();
  return RC::SUCCESS;
}

bool DateType::check_data(int y, int m, int d)
{
  static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  bool       leap  = (y % 400 == 0 || (y % 100 && y % 4 == 0));
  return y > 0 && (m > 0) && (m <= 12) && (d > 0) && (d <= ((m == 2 && leap) ? 1 : 0) + mon[m]);
}

bool DateType::check_data(int32_t date)
{
  int d = date % 100;
  date /= 100;
  int m = date % 100;
  date /= 100;
  int y = date;
  return check_data(y, m, d);
}