#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "common/type/data_type.h"
#include "common/type/date_type.h"
#include "common/value.h"
#include "common/lang/limits.h"
#include "common/value.h"
#include <iomanip>
#include <sstream>

int DateType::compare(const Value &left, const Value &right) const
{
  return common::compare_int((void *)&left.value_.date_value_, (void *)&right.value_.date_value_);
}

bool DateType::is_null(const Value &val) const { return val.value_.date_value_ == DATE_NULL; }

RC DateType::to_string(const Value &val, string &result) const
{
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