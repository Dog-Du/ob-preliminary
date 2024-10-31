/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/6/5.
//

#pragma once

#include <memory>
#include <vector>

#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include "storage/field/field.h"

class SelectStmt;

/**
 * @brief 表示select语句
 * @ingroup Statement
 */
class OrderByStmt : public Stmt
{
public:
  OrderByStmt() = default;
  ~OrderByStmt() override{};

  StmtType type() const override { return StmtType::ORDER_BY; }

public:
  std::vector<std::shared_ptr<FieldExpr>> &order_by() { return order_by_; }
  std::vector<OrderByType>                &order_by_type() { return order_by_type_; }

private:
  friend SelectStmt;
  std::vector<std::shared_ptr<FieldExpr>> order_by_;
  std::vector<OrderByType>                order_by_type_;
};
