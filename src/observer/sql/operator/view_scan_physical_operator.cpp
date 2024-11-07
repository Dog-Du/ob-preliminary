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
// Created by WangYunlai on 2021/6/9.
//

#include "sql/operator/view_scan_physical_operator.h"
#include "event/sql_debug.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "storage/table/table.h"
#include "storage/table/view.h"
#include <memory>

using namespace std;

RC ViewScanPhysicalOperator::open(Trx *trx)
{
  auto child = children_[0].get();
  trx_       = trx;
  RC rc      = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("view child failed open.");
    return rc;
  }

  tuple_.set_schema(view_);
  tuple_.set_record(&record_);
  return RC::SUCCESS;
}

// 从查询中得到结果，但是尽可能地保留RID的信息，因为可能会需要进行写操作。
RC ViewScanPhysicalOperator::next()
{
  RC   rc    = RC::SUCCESS;
  auto child = children_[0].get();
  tuple_.record().new_record(view_->table_meta().record_size());

  while ((rc = child->next()) == RC::SUCCESS) {
    Tuple *tuple            = child->current_tuple();
    auto  &row_tuple_record = tuple_.record();
    for (int i = 0; i < tuple->cell_num(); ++i) {
      auto  field = view_->table_meta().field(i);
      Value val;
      tuple->cell_at(i, val);
      val.resize(field->len());
      row_tuple_record.set_field(field->offset(), field->len(), val.data());
    }

    row_tuple_record.set_rid(tuple->rid());

    bool filter_result;
    rc = filter(tuple_, filter_result);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (filter_result) {
      sql_debug("get a tuple: %s", tuple_.to_string().c_str());
      break;
    } else {
      sql_debug("a tuple is filtered: %s", tuple_.to_string().c_str());
      rc = RC::RECORD_EOF;
    }
  }
  return rc;
}

RC ViewScanPhysicalOperator::close() { return children_[0]->close(); }

Tuple *ViewScanPhysicalOperator::current_tuple() { return &tuple_; }

string ViewScanPhysicalOperator::param() const { return view_->name(); }

void ViewScanPhysicalOperator::set_predicates(vector<shared_ptr<Expression>> &&exprs)
{
  predicates_ = std::move(exprs);
}

RC ViewScanPhysicalOperator::filter(RowTuple &tuple, bool &result)
{
  RC    rc = RC::SUCCESS;
  Value value;
  for (shared_ptr<Expression> &expr : predicates_) {
    rc = expr->get_value(tuple, value);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    bool tmp_result = value.get_boolean();
    if (!tmp_result) {
      result = false;
      return rc;
    }
  }

  result = true;
  return rc;
}
