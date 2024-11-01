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
// Created by WangYunlai on 2024/05/29.
//

#pragma once

#include "common/value.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/expr/tuple_cell.h"
#include "sql/operator/physical_operator.h"
#include "sql/expr/composite_tuple.h"
#include "storage/trx/trx.h"

struct GroupByKey
{
  ShellTuple                               tuple;
  std::vector<std::shared_ptr<FieldExpr>> *field_exprs;
  GroupByKey()                        = default;
  GroupByKey(const GroupByKey &other) = default;
  GroupByKey(std::vector<Value> *values, std::vector<TupleCellSpec> *tuple_schema,
      std::vector<std::shared_ptr<FieldExpr>> *field_exprs)
  {
    this->tuple.set_cells(values);
    this->tuple.set_names(tuple_schema);
    this->field_exprs = field_exprs;
  }

  bool operator<(const GroupByKey &rhs) const
  {
    ASSERT(rhs.tuple.cell_num()==this->tuple.cell_num(), "");
    ASSERT(rhs.field_exprs->size()==this->field_exprs->size(),"");

    Value left, right;
    int   result = 0;
    for (auto &field : *field_exprs) {
      field->get_value(this->tuple, left);
      field->get_value(rhs.tuple, right);
      result = left.compare(right);

      if (result != 0) {
        return result < 0;
      }
    }
    return false;
  }
};

/**
 * @brief Group By 物理算子基类
 * @ingroup PhysicalOperator
 */
class GroupByPhysicalOperator : public PhysicalOperator
{
public:
  GroupByPhysicalOperator(std::vector<AggregateExpr *> &aggregation_expression,
      std::vector<std::shared_ptr<FieldExpr>> &groupby_expressions, std::vector<FieldExpr *> &field_exprs,
      std::shared_ptr<Expression> &having_filter);

  virtual ~GroupByPhysicalOperator() = default;

  virtual RC                   open(Trx *trx) override;
  virtual RC                   next() override;
  virtual RC                   close() override;
  virtual Tuple               *current_tuple() override;
  virtual PhysicalOperatorType type() const override { return PhysicalOperatorType::GROUP_BY; }

protected:
  using AggregatorList = std::vector<std::unique_ptr<Aggregator>>;
  /**
   * @brief 聚合出来的一组数据
   * @details
   * 第一个参数是聚合函数列表，比如需要计算 sum(a), avg(b), count(c)。
   * 第二个参数是聚合的最终结果，它也包含两个元素，第一个是缓存下来的元组，第二个是聚合函数计算的结果。
   * 第二个参数中，之所以要缓存下来一个元组，是要解决这个问题：
   * select a, b, sum(a) from t group by a;
   * 我们需要知道b的值是什么，虽然它不确定。
   */
  using GroupValueType = std::tuple<AggregatorList, CompositeTuple>;

protected:
  void create_aggregator_list(AggregatorList &aggregator_list) {}

  /// @brief 聚合一条记录
  /// @param aggregator_list 需要执行聚合运算的列表
  /// @param tuple 执行聚合运算的一条记录
  RC aggregate(AggregatorList &aggregator_list, const Tuple &tuple) { return RC::SUCCESS; }

  /// @brief 所有tuple聚合结束后，运算最终结果
  RC evaluate(GroupValueType &group_value) { return RC::SUCCESS; }

  // 进行分组。
  RC group_by(PhysicalOperator *child, Trx *trx);
  // 用having_filter进行分组筛选。
  RC group_filter();

protected:
  std::vector<AggregateExpr *>            aggregate_expressions_;  /// 聚合表达式
  std::vector<std::shared_ptr<FieldExpr>> groupby_field_expressions_;
  std::vector<FieldExpr *>                field_expressions_;  /// 非聚合的

  std::vector<TupleCellSpec>                              tuple_schema_;
  std::vector<std::vector<Value>>                         tuples_;
  std::map<GroupByKey, std::vector<ShellTuple>>           group_tuples_;
  std::map<GroupByKey, std::vector<ShellTuple>>::iterator group_iter_;
  std::shared_ptr<Expression>                             having_filter_;
  ValueListTuple                                          tuple_;
  bool                                                    first_;
  // std::vector<Expression *>                   value_expressions_;  /// 计算聚合时的表达式
};