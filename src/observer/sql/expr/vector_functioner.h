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
// Created by Wangyunlai on 2024/05/29.
//

#pragma once

#include "common/type/attr_type.h"
#include "common/value.h"
#include "common/rc.h"

class FunctionWorker
{
public:
  virtual ~FunctionWorker() = default;

  virtual RC compute_result(const Value &left, const Value &right, Value &result) const { return RC::UNIMPLEMENTED; }
};

class L2DistanceWorker : public FunctionWorker
{
public:
  virtual ~L2DistanceWorker() = default;
  virtual RC compute_result(const Value &left, const Value &right, Value &result) const override;
};

class CosineDistanceWorker : public FunctionWorker
{
public:
  virtual ~CosineDistanceWorker() = default;
  virtual RC compute_result(const Value &left, const Value &right, Value &result) const override;
};

class InnerProduceWorker : public FunctionWorker
{
public:
  virtual ~InnerProduceWorker() = default;
  virtual RC compute_result(const Value &left, const Value &right, Value &result) const override;
};