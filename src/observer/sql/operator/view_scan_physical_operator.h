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
// Created by WangYunlai on 2022/6/7.
//
#pragma once

#include "sql/operator/physical_operator.h"
#include "storage/record/record_manager.h"
#include "common/rc.h"

class View;

/**
 * @brief 表扫描物理算子
 * @ingroup PhysicalOperator
 */
// 致力于向上提供与表扫描算子没有差异的服务。
class ViewScanPhysicalOperator : public PhysicalOperator
{
public:
  ViewScanPhysicalOperator(View *view, bool readonly) : view_(view), readonly_(readonly) {}

  virtual ~ViewScanPhysicalOperator() {}

  std::string param() const override;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::VIEW_SCAN; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

  void set_predicates(std::vector<std::shared_ptr<Expression>> &&exprs);

private:
  RC filter(RowTuple &tuple, bool &result);

private:
  View                                    *view_     = nullptr;
  Trx                                     *trx_      = nullptr;
  bool                                     readonly_ = false;

  RowTuple                                 tuple_;
  std::vector<std::shared_ptr<Expression>> predicates_;  // TODO chang predicate to table tuple filter
  Record                                   record_;
};