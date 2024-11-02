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
// Created by WangYunlai on 2022/6/9.
//

#pragma once

#include "common/value.h"
#include "sql/expr/expression.h"
#include "sql/operator/physical_operator.h"

class Trx;
class DeleteStmt;
class FieldMeta;

/**
 * @brief 物理算子，删除
 * @ingroup PhysicalOperator
 */
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(
      Table *table, std::vector<const FieldMeta *> &fields, std::vector<std::shared_ptr<Expression>> &values)
      : table_(table), fields_meta_(fields), expressions_(values)
  {}

  virtual ~UpdatePhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::UPDATE; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

private:
  RC convert_expression_to_values();

  Table                                   *table_ = nullptr;
  std::vector<const FieldMeta *>           fields_meta_;
  std::vector<std::shared_ptr<Expression>> expressions_;
  std::vector<Value>                       values_;  // 通过expression转换得到的values
  Trx                                     *trx_;
};
