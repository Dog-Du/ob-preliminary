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

#include "sql/operator/physical_operator.h"
#include "sql/parser/value.h"
#include "sql/stmt/update_stmt.h"
#include "storage/field/field_meta.h"
#include <memory>

class Trx;
class UpdateStmt;

struct UpdatePhysicalNode
{
  Value                             value;
  std::unique_ptr<PhysicalOperator> child;
  bool                              nullable;

  UpdatePhysicalNode(const Value &v, bool n) : value(v), child(nullptr), nullable(n) {}
  UpdatePhysicalNode(std::unique_ptr<PhysicalOperator> c, bool n) : child(std::move(c)), nullable(n)
  {}
};

/**
 * @brief 物理算子，udpate
 * @ingroup PhysicalOperator
 */
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(
      Table *table, std::vector<UpdatePhysicalNode> &values, std::vector<int> &indexs)
      : table_(table)
  {
    update_nodes_.swap(values);
    indexs_.swap(indexs);
  }

  virtual ~UpdatePhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::UPDATE; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

private:
  Table                          *table_ = nullptr;
  Trx                            *trx_   = nullptr;
  std::vector<UpdatePhysicalNode> update_nodes_;
  std::vector<Value>              values_;
  std::vector<int>                indexs_;
};
