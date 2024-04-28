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
// Created by WangYunlai on 2022/07/01.
//

#include "sql/operator/aggregation_physical_operator.h"
#include "common/log/log.h"
#include "sql/expr/tuple.h"
#include "sql/operator/aggregation_logical_operator.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "storage/field/field.h"
#include "storage/record/record.h"
#include "storage/table/table.h"

void AggregationPhysicalOperator::set_cell_at(int index, const Value &value)
{
  if (value.attr_type() == AttrType::NULLS) {
    if (tuple_.agg_type_at(index) == AggregationType::COUNT_STAR_TYPE) {
      tuple_.cell_at(index).add(Value(int(1)));
    }
    return;
  }

  tuple_.count_at(index)++;

  switch (tuple_.agg_type_at(index)) {
    case AggregationType::AVG_TYPE: {
      tuple_.cell_at(index).add(value);
    } break;
    case AggregationType::COUNT_STAR_TYPE: {
      tuple_.cell_at(index).add(Value(int(1)));
    } break;
    case AggregationType::COUNT_TYPE: {
      tuple_.cell_at(index).add(Value(int(1)));
    } break;
    case AggregationType::SUM_TYPE: {
      tuple_.cell_at(index).add(value);
    } break;
    case AggregationType::MIN_TYPE: {
      tuple_.cell_at(index).min(value);
    } break;
    case AggregationType::MAX_TYPE: {
      tuple_.cell_at(index).max(value);
    }
    default: break;
  }
}

void AggregationPhysicalOperator::init_tuple(Tuple *tuple)
{
  int cell_num = tuple->cell_num();

  for (int i = 0; i < cell_num; ++i) {
    Value val;
    tuple->cell_at(i, val);
    switch (tuple_.agg_type_at(i)) {
      case AggregationType::COUNT_STAR_TYPE: {
        tuple_.cell_at(i).set_int(1);
      } break;
      case AggregationType::COUNT_TYPE: {
        tuple_.cell_at(i).set_int( (val.attr_type() == NULLS ? 0 : 1));
      } break;
      default: {
        tuple_.cell_at(i) = val;
      } break;
    }

    tuple_.count_at(i) = (val.attr_type() == NULLS ? 0 : 1);
  }
}

RC AggregationPhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  PhysicalOperator *child = children_[0].get();
  RC                rc    = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  return RC::SUCCESS;
}

RC AggregationPhysicalOperator::next()
{
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }

  Tuple *tuple;
  auto  &child = children_[0];

  if (used_ || RC::SUCCESS != child->next()) {
    used_ = false;
    return RC::RECORD_EOF;
  }

  tuple = child->current_tuple();

  if (tuple == nullptr) {
    return RC::RECORD_EOF;
  }

  init_tuple(tuple);
  int   cell_num = tuple_.cell_num();
  Value val;

  // std::cout << "tuple : " << tuple->to_string() << std::endl;
  // std::cout << tuple_.to_string() << std::endl;
  while (RC::SUCCESS == child->next()) {
    tuple = child->current_tuple();
    for (int i = 0; i < cell_num; ++i) {
      tuple->cell_at(i, val);
      set_cell_at(i, val);
    }
    // std::cout << "tuple : " << tuple->to_string() << std::endl;
    // std::cout << tuple_.to_string() << std::endl;
  }

  for (int i = 0; i < cell_num; ++i) {

    if (tuple_.count_at(i) == 0 && (tuple_.agg_type_at(i) == AggregationType::AVG_TYPE ||
                                       tuple_.agg_type_at(i) == AggregationType::SUM_TYPE)) {
      tuple_.cell_at(i).set_type(NULLS);
      continue;
    }

    if (tuple_.agg_type_at(i) == AggregationType::AVG_TYPE) {
      if (tuple_.cell_at(i).attr_type() == AttrType::INTS) {
        int sum = tuple_.cell_at(i).get_int();
        tuple_.cell_at(i).set_float(1.0 * sum / tuple_.count_at(i));
      } else {
        tuple_.cell_at(i).set_float(tuple_.cell_at(i).get_float() / tuple_.count_at(i));
      }
    }
  }

  if (!used_) {
    used_ = true;
    return RC::SUCCESS;
  }
  used_ = true;
  return RC::RECORD_EOF;
}

RC AggregationPhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}

Tuple *AggregationPhysicalOperator::current_tuple() { return &tuple_; }

void AggregationPhysicalOperator::set_aggregation(const std::vector<Field> &fields)
{
  tuple_.set_fields(fields);
  int cell_num = tuple_.cell_num();

  for (int i = 0; i < cell_num; ++i) {
    switch (tuple_.agg_type_at(i)) {
      case AggregationType::COUNT_STAR_TYPE:
      case AggregationType::COUNT_TYPE: {
        tuple_.cell_at(i).set_int(0);
      } break;
      default: break;
    }
  }
}
