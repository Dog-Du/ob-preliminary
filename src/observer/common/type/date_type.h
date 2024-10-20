#pragma once

#include "common/rc.h"
#include "common/type/data_type.h"
#include "common/value.h"
#include <string>

class DateType : public DataType
{
public:
  DateType() : DataType(AttrType::DATES) {}
  virtual ~DateType() = default;

  int compare(const Value &left, const Value &right) const override;

  RC to_string(const Value &val, string &result) const override;

  static bool check_data(int y, int m, int d);
};