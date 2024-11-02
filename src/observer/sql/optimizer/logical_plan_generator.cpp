/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/08/16.
//

#include "sql/optimizer/logical_plan_generator.h"

#include <common/log/log.h>
#include <cstddef>

#include "common/types.h"
#include "sql/expr/expression.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/group_by_logical_operator.h"
#include "sql/operator/order_by_logical_operator.h"
#include "sql/operator/update_logical_operator.h"

#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/groupby_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/orderby_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"

#include "sql/expr/expression_iterator.h"
#include "sql/stmt/update_stmt.h"
#include "storage/field/field_meta.h"

using namespace std;
using namespace common;

RC LogicalPlanGenerator::create(Stmt *stmt, shared_ptr<LogicalOperator> &logical_operator)
{
  RC rc = RC::SUCCESS;
  switch (stmt->type()) {
    case StmtType::CALC: {
      CalcStmt *calc_stmt = static_cast<CalcStmt *>(stmt);

      rc = create_plan(calc_stmt, logical_operator);
    } break;

    case StmtType::UPDATE: {
      UpdateStmt *update_stmt = static_cast<UpdateStmt *>(stmt);

      rc = create_plan(update_stmt, logical_operator);
    } break;

    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);

      rc = create_plan(select_stmt, logical_operator);
    } break;

    case StmtType::INSERT: {
      InsertStmt *insert_stmt = static_cast<InsertStmt *>(stmt);

      rc = create_plan(insert_stmt, logical_operator);
    } break;

    case StmtType::DELETE: {
      DeleteStmt *delete_stmt = static_cast<DeleteStmt *>(stmt);

      rc = create_plan(delete_stmt, logical_operator);
    } break;

    case StmtType::EXPLAIN: {
      ExplainStmt *explain_stmt = static_cast<ExplainStmt *>(stmt);

      rc = create_plan(explain_stmt, logical_operator);
    } break;
    default: {
      rc = RC::UNIMPLEMENTED;
    }
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(CalcStmt *calc_stmt, std::shared_ptr<LogicalOperator> &logical_operator)
{
  logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::join_table_in_tree(
    std::shared_ptr<LogicalOperator> &oper, Table *table, FilterStmt *filter_stmt)
{
  RC                               rc = RC::SUCCESS;
  std::vector<Field>               fields;
  std::shared_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_ONLY));
  std::shared_ptr<LogicalOperator> this_table_predicate_oper;

  if (nullptr != filter_stmt) {
    rc = LogicalPlanGenerator::create_plan(filter_stmt, this_table_predicate_oper);
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  // oper == nullptr 说明还没有表被设置。
  if (oper == nullptr) {
    if (this_table_predicate_oper != nullptr) {
      auto oper = static_cast<TableGetLogicalOperator *>(table_get_oper.get());
      oper->set_predicates(std::move(this_table_predicate_oper->expressions()));
    }
    oper = table_get_oper;
  } else {
    std::shared_ptr<JoinLogicalOperator> join_oper(new JoinLogicalOperator());
    join_oper->add_child(oper);
    join_oper->add_child(table_get_oper);

    if (this_table_predicate_oper != nullptr) {
      this_table_predicate_oper->add_child(join_oper);
      oper = this_table_predicate_oper;
    } else {
      oper = join_oper;
    }
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(GroupByStmt *groupby_stmt, std::shared_ptr<LogicalOperator> &logical_operator)
{
  if (groupby_stmt->group_by().empty() && !groupby_stmt->have_agg()) {
    logical_operator = nullptr;
    return RC::SUCCESS;
  }
  std::shared_ptr<LogicalOperator> groupby_oper(new GroupByLogicalOperator(
      groupby_stmt->group_by(), groupby_stmt->agg_exprs(), groupby_stmt->field_exprs(), groupby_stmt->having()));
  logical_operator = groupby_oper;
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(OrderByStmt *orderby_stmt, std::shared_ptr<LogicalOperator> &logical_operator)
{
  if (orderby_stmt->order_by().empty()) {
    logical_operator = nullptr;
    return RC::SUCCESS;
  }

  std::shared_ptr<LogicalOperator> orderby_oper(
      new OrderByLogicalOperator(orderby_stmt->order_by(), orderby_stmt->order_by_type()));
  logical_operator = orderby_oper;
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(SelectStmt *select_stmt, shared_ptr<LogicalOperator> &logical_operator)
{
  RC                           rc        = RC::SUCCESS;
  shared_ptr<LogicalOperator> *last_oper = nullptr;

  shared_ptr<LogicalOperator> table_oper(nullptr);
  last_oper = &table_oper;

  auto &join_nodes = select_stmt->join_nodes();
  for (auto &join_node : join_nodes) {
    std::shared_ptr<LogicalOperator> oper;

    for (size_t j = 0; j < join_node.tables_.size(); ++j) {
      rc = join_table_in_tree(oper, join_node.tables_[j], join_node.conditions_[j].get());

      if (rc != RC::SUCCESS) {
        return rc;
      }
    }

    if (table_oper == nullptr) {
      table_oper = oper;
    } else {
      JoinLogicalOperator *join_oper = new JoinLogicalOperator;
      join_oper->add_child(table_oper);
      join_oper->add_child(oper);
      table_oper = shared_ptr<LogicalOperator>(join_oper);
    }
  }

  std::shared_ptr<LogicalOperator> predicate_oper;

  rc = create_plan(select_stmt->filter_stmt(), predicate_oper);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
    return rc;
  }

  if (predicate_oper != nullptr) {
    if (*last_oper) {
      predicate_oper->add_child(*last_oper);
    }

    last_oper = &predicate_oper;
  }

  std::shared_ptr<LogicalOperator> groupby_oper;
  rc = create_plan(select_stmt->group_by().get(), groupby_oper);

  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create groupby oper.");
    return rc;
  }

  if (groupby_oper != nullptr) {
    if (*last_oper) {
      groupby_oper->add_child(*last_oper);
    }

    last_oper = &groupby_oper;
  }

  // std::shared_ptr<LogicalOperator> having_oper;
  // rc = create_plan(select_stmt->having().get(), having_oper);
  // if (rc != RC::SUCCESS) {
  //   LOG_WARN("failed to create having oper.");
  //   return rc;
  // }
  // if (having_oper != nullptr) {
  //   if (*last_oper) {
  //     having_oper->add_child(*last_oper);
  //   }
  //   last_oper = &having_oper;
  // }

  std::shared_ptr<LogicalOperator> orderby_oper;

  rc = create_plan(select_stmt->order_by().get(), orderby_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create order_by oper.");
    return rc;
  }

  if (orderby_oper != nullptr) {
    if (*last_oper) {
      orderby_oper->add_child(*last_oper);
    }

    last_oper = &orderby_oper;
  }

  auto project_oper = make_shared<ProjectLogicalOperator>(select_stmt->query_expressions());
  if (*last_oper) {
    project_oper->add_child(*last_oper);
  }

  logical_operator = project_oper;
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(FilterStmt *filter_stmt, shared_ptr<LogicalOperator> &logical_operator)
{
  if (filter_stmt == nullptr || filter_stmt->get_conditions() == nullptr) {
    return RC::SUCCESS;
  }

  std::vector<std::shared_ptr<Expression>> comparison_expressions;
  comparison_expressions.push_back(filter_stmt->get_conditions());
  std::shared_ptr<ConjunctionExpr> conjunction_expr(
      new ConjunctionExpr(ConjunctionExpr::Type::AND, comparison_expressions));

  RC                                rc                  = RC::SUCCESS;
  bool                              need_continue_check = true;
  std::function<void(Expression *)> check_sub_query     = [&rc, &need_continue_check, &check_sub_query](
                                                          Expression *expression) {
    if (!need_continue_check) {
      return;
    }

    switch (expression->type()) {
      case ExprType::SUBQUERY_OR_VALUELIST: {
        rc = static_cast<SubQuery_ValueList_Expression *>(expression)->create_logical();
        if (rc != RC::SUCCESS) {
          LOG_WARN("create physical operator failed.");
          need_continue_check = false;
          return;
        }
      } break;
      case ExprType::ARITHMETIC: {
        auto expr = static_cast<ArithmeticExpr *>(expression);
        if (expr->left() != nullptr) {
          check_sub_query(expr->left().get());
        }

        if (expr->right() != nullptr) {
          check_sub_query(expr->right().get());
        }
      } break;
      case ExprType::COMPARISON: {
        auto expr = static_cast<ComparisonExpr *>(expression);
        if (expr->left() != nullptr) {
          check_sub_query(expr->left().get());
        }

        if (expr->right() != nullptr) {
          check_sub_query(expr->right().get());
        }
      } break;
      case ExprType::CONJUNCTION: {
        auto expr = static_cast<ConjunctionExpr *>(expression);

        for (auto &child : expr->children()) {
          check_sub_query(child.get());
        }
      } break;
      default: {
      } break;
    }
  };

  filter_stmt->get_conditions()->check_or_get(check_sub_query);
  logical_operator.reset(new PredicateLogicalOperator(conjunction_expr));
  return RC::SUCCESS;
}

int LogicalPlanGenerator::implicit_cast_cost(AttrType from, AttrType to)
{
  if (from == to) {
    return 0;
  }
  return DataType::type_instance(from)->cast_cost(to);
}

RC LogicalPlanGenerator::create_plan(InsertStmt *insert_stmt, shared_ptr<LogicalOperator> &logical_operator)
{
  Table *table = insert_stmt->table();

  InsertLogicalOperator *insert_operator = new InsertLogicalOperator(table, insert_stmt->values());
  logical_operator.reset(insert_operator);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(UpdateStmt *update_stmt, std::shared_ptr<LogicalOperator> &logical_operator)
{
  Table                      *table       = update_stmt->table();
  FilterStmt                 *filter_stmt = update_stmt->filter_stmt();
  shared_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_WRITE));

  shared_ptr<LogicalOperator> predicate_oper;

  RC rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  bool                              need_continue_check = true;
  std::function<void(Expression *)> check_subquery      = [&](Expression *expression) {
    if (!need_continue_check) {
      return;
    }

    switch (expression->type()) {
      case ExprType::SUBQUERY_OR_VALUELIST: {
        auto expr = static_cast<SubQuery_ValueList_Expression *>(expression);
        rc        = expr->create_logical();
        if (rc != RC::SUCCESS) {
          LOG_WARN("create_logical failed.");
          need_continue_check = false;
        }
      } break;
      default: {
      } break;
    }
  };

  for (auto &e : update_stmt->expressions()) {
    e->check_or_get(check_subquery);
    if (rc != RC::SUCCESS) {
      LOG_WARN("check_or_get failed.");
      return rc;
    }
  }

  shared_ptr<LogicalOperator> update_oper(
      new UpdateLogicalOperator(table, update_stmt->fields_meta(), update_stmt->expressions()));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    update_oper->add_child(std::move(predicate_oper));
  } else {
    update_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(update_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(DeleteStmt *delete_stmt, shared_ptr<LogicalOperator> &logical_operator)
{
  Table                      *table       = delete_stmt->table();
  FilterStmt                 *filter_stmt = delete_stmt->filter_stmt();
  shared_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_WRITE));

  shared_ptr<LogicalOperator> predicate_oper;

  RC rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  shared_ptr<LogicalOperator> delete_oper(new DeleteLogicalOperator(table));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    delete_oper->add_child(std::move(predicate_oper));
  } else {
    delete_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(delete_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(ExplainStmt *explain_stmt, shared_ptr<LogicalOperator> &logical_operator)
{
  shared_ptr<LogicalOperator> child_oper;

  Stmt *child_stmt = explain_stmt->child();

  RC rc = create(child_stmt, child_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child operator. rc=%s", strrc(rc));
    return rc;
  }

  logical_operator = shared_ptr<LogicalOperator>(new ExplainLogicalOperator);
  logical_operator->add_child(std::move(child_oper));
  return rc;
}

// RC LogicalPlanGenerator::create_group_by_plan(SelectStmt *select_stmt, shared_ptr<LogicalOperator> &logical_operator)
// {
//   return RC::SUCCESS;
//   // vector<shared_ptr<Expression>>             &group_by_expressions =
//   // select_stmt->group_by(); vector<Expression *> aggregate_expressions;
//   // vector<shared_ptr<Expression>>             &query_expressions =
//   // select_stmt->query_expressions(); function<RC(std::shared_ptr<Expression>
//   // &)> collector         = [&](shared_ptr<Expression> &expr) -> RC {
//   //   RC rc = RC::SUCCESS;
//   //   if (expr->type() == ExprType::AGGREGATION) {
//   //     expr->set_pos(aggregate_expressions.size() +
//   //     group_by_expressions.size());
//   //     aggregate_expressions.push_back(expr.get());
//   //   }
//   //   rc = ExpressionIterator::iterate_child_expr(*expr, collector);
//   //   return rc;
//   // };

//   // function<RC(std::shared_ptr<Expression> &)> bind_group_by_expr =
//   // [&](shared_ptr<Expression> &expr) -> RC {
//   //   RC rc = RC::SUCCESS;
//   //   for (size_t i = 0; i < group_by_expressions.size(); i++) {
//   //     auto &group_by = group_by_expressions[i];
//   //     if (expr->type() == ExprType::AGGREGATION) {
//   //       break;
//   //     } else if (expr->equal(*group_by)) {
//   //       expr->set_pos(i);
//   //       continue;
//   //     } else {
//   //       rc = ExpressionIterator::iterate_child_expr(*expr,
//   //       bind_group_by_expr);
//   //     }
//   //   }
//   //   return rc;
//   // };

//   // bool                                        found_unbound_column = false;
//   // function<RC(std::shared_ptr<Expression> &)> find_unbound_column  =
//   // [&](shared_ptr<Expression> &expr) -> RC {
//   //   RC rc = RC::SUCCESS;
//   //   if (expr->type() == ExprType::AGGREGATION) {
//   //     // do nothing
//   //   } else if (expr->pos() != -1) {
//   //     // do nothing
//   //   } else if (expr->type() == ExprType::FIELD) {
//   //     found_unbound_column = true;
//   //   } else {
//   //     rc = ExpressionIterator::iterate_child_expr(*expr,
//   //     find_unbound_column);
//   //   }
//   //   return rc;
//   // };

//   // for (shared_ptr<Expression> &expression : query_expressions) {
//   //   bind_group_by_expr(expression);
//   // }

//   // for (shared_ptr<Expression> &expression : query_expressions) {
//   //   find_unbound_column(expression);
//   // }

//   // // collect all aggregate expressions
//   // for (shared_ptr<Expression> &expression : query_expressions) {
//   //   collector(expression);
//   // }

//   // if (group_by_expressions.empty() && aggregate_expressions.empty()) {
//   //   // 既没有group by也没有聚合函数，不需要group by
//   //   return RC::SUCCESS;
//   // }

//   // if (found_unbound_column) {
//   //   LOG_WARN("column must appear in the GROUP BY clause or must be part of an
//   //   aggregate function"); return RC::INVALID_ARGUMENT;
//   // }

//   // // 如果只需要聚合，但是没有group by 语句，需要生成一个空的group by 语句

//   // auto group_by_oper =
//   //     make_unique<GroupByLogicalOperator>(std::move(group_by_expressions),
//   //     std::move(aggregate_expressions));
//   // logical_operator = std::move(group_by_oper);
//   // return RC::SUCCESS;
// }