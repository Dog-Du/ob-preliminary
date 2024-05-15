/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
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
#include <memory>

#include "sql/expr/expression.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/pipeline_break_physical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/aggregation_logical_operator.h"

#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"
#include "sql/stmt/update_stmt.h"
#include "storage/field/field_meta.h"

using namespace std;

RC LogicalPlanGenerator::create(Stmt *stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  RC rc = RC::SUCCESS;
  switch (stmt->type()) {
    case StmtType::CALC: {
      CalcStmt *calc_stmt = static_cast<CalcStmt *>(stmt);

      rc = create_plan(calc_stmt, logical_operator);
    } break;

    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);

      rc = create_plan(select_stmt, logical_operator);
    } break;

    case StmtType::INSERT: {
      InsertStmt *insert_stmt = static_cast<InsertStmt *>(stmt);

      rc = create_plan(insert_stmt, logical_operator);
    } break;

    case StmtType::UPDATE: {
      UpdateStmt *update_stmt = static_cast<UpdateStmt *>(stmt);

      rc = create_plan(update_stmt, logical_operator);
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
      rc = RC::UNIMPLENMENT;
    }
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(
    CalcStmt *calc_stmt, std::unique_ptr<LogicalOperator> &logical_operator)
{
  logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  unique_ptr<LogicalOperator> table_oper(nullptr);

  const std::vector<Table *> &tables     = select_stmt->tables();
  const std::vector<Field>   &all_fields = select_stmt->query_fields();
  // 丑陋的答辩

  for (const Field &field : all_fields) {
    if (field.agg_type() != AggregationType::INVALID_TYPE) {
      switch (field.agg_type()) {
        case AggregationType::AVG_TYPE:
        case AggregationType::SUM_TYPE: {
          switch (field.attr_type()) {
            case AttrType::BOOLEANS:
            case AttrType::CHARS:
            case AttrType::DATES:
            case AttrType::NULLS:
            case AttrType::UNDEFINED: {
              return RC::VARIABLE_NOT_VALID;
            } break;
            default: break;
          }
        }
        default: break;
      }
    }
  }

  for (Table *table : tables) {
    std::vector<Field> fields;
    for (const Field &field : all_fields) {
      if (0 == strcmp(field.table_name(), table->name())) {
        fields.push_back(field);
      }
    }

    unique_ptr<LogicalOperator> table_get_oper(
        new TableGetLogicalOperator(table, fields, true /*readonly*/));

    if (table_oper == nullptr) {
      table_oper = std::move(table_get_oper);
    } else {
      auto                 iter      = select_stmt->joined_tables().find(table);
      JoinLogicalOperator *join_oper = nullptr;

      if (iter == select_stmt->joined_tables().end()) {
        join_oper = new JoinLogicalOperator;
      } else {
        std::vector<std::unique_ptr<ComparisonExpr>> comp_exprs;
        for (auto &it : iter->second) {
          Expression *left_expr  = nullptr;
          Expression *right_expr = nullptr;

          if (it.left_is_attr == 0) {
            left_expr = new ValueExpr(it.left_value);
          } else {
            auto left_iter = find_if(tables.begin(), tables.end(), [tables, &it](Table *t) {
              return t->name() == it.left_attr.relation_name;
            });

            if (left_iter == tables.end()) {
              return RC::VARIABLE_NOT_EXISTS;
            }

            Table *left_table = *left_iter;
            auto   left_field = left_table->table_meta().field(it.left_attr.attribute_name.c_str());

            if (left_field == nullptr) {
              return RC::VARIABLE_NOT_EXISTS;
            }

            left_expr = new FieldExpr(left_table, left_field);
          }

          if (it.right_is_attr == 0) {
            right_expr = new ValueExpr(it.right_value);
          } else {
            auto right_iter = find_if(tables.begin(), tables.end(), [tables, &it](Table *t) {
              return t->name() == it.right_attr.relation_name;
            });

            if (right_iter == tables.end()) {
              return RC::VARIABLE_NOT_EXISTS;
            }

            Table *right_table = *right_iter;
            auto   right_field =
                right_table->table_meta().field(it.right_attr.attribute_name.c_str());

            if (right_field == nullptr) {
              return RC::VARIABLE_NOT_EXISTS;
            }

            right_expr = new FieldExpr(right_table, right_field);
          }

          comp_exprs.emplace_back(std::unique_ptr<ComparisonExpr>(new ComparisonExpr(it.comp,
              std::unique_ptr<Expression>(left_expr),
              std::unique_ptr<Expression>(right_expr))));
        }

        join_oper = new JoinLogicalOperator;
        join_oper->comp_expr().swap(comp_exprs);
      }

      join_oper->add_child(std::move(table_oper));
      join_oper->add_child(std::move(table_get_oper));
      table_oper = unique_ptr<LogicalOperator>(join_oper);
    }
  }

  unique_ptr<LogicalOperator> predicate_oper;

  // 创建filter
  RC rc = create_plan(select_stmt->filter_stmt(), predicate_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
    return rc;
  }

  unique_ptr<LogicalOperator> project_oper(new ProjectLogicalOperator(all_fields));
  unique_ptr<LogicalOperator> aggregation_oper;

  if (predicate_oper) {
    if (table_oper) {
      predicate_oper->add_child(std::move(table_oper));
    }
    project_oper->add_child(std::move(predicate_oper));

    if (select_stmt->is_agg()) {
      aggregation_oper.reset(new AggregationLogicalOperator(all_fields));
      aggregation_oper->add_child(
          std::move(project_oper));  // 打错了，之前写错谓词了，找了半天问题。
    }
  } else {
    if (table_oper) {
      project_oper->add_child(std::move(table_oper));
      if (select_stmt->is_agg()) {
        aggregation_oper.reset(new AggregationLogicalOperator(all_fields));
        aggregation_oper->add_child(std::move(project_oper));
      }
    }
  }

  if (select_stmt->is_agg()) {
    logical_operator.swap(aggregation_oper);
  } else {
    logical_operator.swap(project_oper);
  }
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    FilterStmt *filter_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  std::vector<unique_ptr<Expression>> cmp_exprs;
  const std::vector<FilterUnit *>    &filter_units = filter_stmt->filter_units();
  for (const FilterUnit *filter_unit : filter_units) {
    const FilterObj &filter_obj_left  = filter_unit->left();
    const FilterObj &filter_obj_right = filter_unit->right();

    // 用一个滤嘴算子，把value_list和sub-query统一处理。

    unique_ptr<Expression> left(nullptr);

    switch (filter_obj_left.is_attr) {
      case 0: {
        left.reset(static_cast<Expression *>(new ValueExpr(filter_obj_left.value)));
        break;
      }
      case 1: {
        left.reset(static_cast<Expression *>(new FieldExpr(filter_obj_left.field)));
        break;
      }
      case 3: {
        std::unique_ptr<LogicalOperator> sel_operator;
        if (RC::SUCCESS != create_plan(filter_obj_left.select_stmt.get(), sel_operator)) {
          return RC::SQL_SYNTAX;
        }

        left.reset(static_cast<Expression *>(new SubLogicalExpression(sel_operator)));

      } break;
      case 2: {
        left.reset(static_cast<Expression *>(new SubLogicalExpression(
            const_cast<std::vector<Value> &>(filter_obj_left.value_list))));

      } break;
      default: {
        return RC::SQL_SYNTAX;
      }
    }

    unique_ptr<Expression> right(nullptr);

    switch (filter_obj_right.is_attr) {
      case 0: {
        right.reset(static_cast<Expression *>(new ValueExpr(filter_obj_right.value)));
        break;
      }
      case 1: {
        right.reset(static_cast<Expression *>(new FieldExpr(filter_obj_right.field)));
        break;
      }
      case 3: {
        std::unique_ptr<LogicalOperator> sel_operator;
        if (RC::SUCCESS != create_plan(filter_obj_right.select_stmt.get(), sel_operator)) {
          return RC::SQL_SYNTAX;
        }

        right.reset(static_cast<Expression *>(new SubLogicalExpression(sel_operator)));
      } break;
      case 2: {
        right.reset(static_cast<Expression *>(new SubLogicalExpression(
            const_cast<std::vector<Value> &>(filter_obj_right.value_list))));
      } break;
      default: {
        return RC::SQL_SYNTAX;
      }
    }

    ComparisonExpr *cmp_expr =
        new ComparisonExpr(filter_unit->comp(), std::move(left), std::move(right));
    cmp_exprs.emplace_back(cmp_expr);
  }

  unique_ptr<PredicateLogicalOperator> predicate_oper;
  if (!cmp_exprs.empty()) {
    unique_ptr<ConjunctionExpr> conjunction_expr(
        new ConjunctionExpr(ConjunctionExpr::Type::AND, cmp_exprs));
    predicate_oper = unique_ptr<PredicateLogicalOperator>(
        new PredicateLogicalOperator(std::move(conjunction_expr)));
  }

  logical_operator = std::move(predicate_oper);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    InsertStmt *insert_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table        *table = insert_stmt->table();
  vector<Value> values(insert_stmt->values(), insert_stmt->values() + insert_stmt->value_amount());

  InsertLogicalOperator *insert_operator = new InsertLogicalOperator(table, values);
  logical_operator.reset(insert_operator);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    UpdateStmt *update_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table             *table       = update_stmt->table();
  FilterStmt        *filter_stmt = update_stmt->filter_stmt();
  std::vector<Field> fields;

  for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(i);
    fields.push_back(Field(table, field_meta));
  }
  unique_ptr<LogicalOperator> table_get_oper(
      new TableGetLogicalOperator(table, fields, false /*readonly*/));

  unique_ptr<LogicalOperator> predicate_oper;
  RC                          rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  std::vector<UpdateLogicalNode> values;

  for (auto &it : update_stmt->values()) {
    if (!it.sub_query) {
      values.emplace_back(UpdateLogicalNode(it.value, it.nullable, it.attr_type));
    } else {
      std::unique_ptr<LogicalOperator> log;
      if (create_plan(it.sub_query.get(), log) != RC::SUCCESS) {
        return RC::SQL_SYNTAX;
      }

      values.emplace_back(UpdateLogicalNode(std::move(log), it.nullable, it.attr_type));
    }
  }

  unique_ptr<LogicalOperator> update_oper(
      new UpdateLogicalOperator(table, values, update_stmt->indexs()));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    update_oper->add_child(std::move(predicate_oper));
  } else {
    update_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(update_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(
    DeleteStmt *delete_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table             *table       = delete_stmt->table();
  FilterStmt        *filter_stmt = delete_stmt->filter_stmt();
  std::vector<Field> fields;
  for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(i);
    fields.push_back(Field(table, field_meta));
  }
  unique_ptr<LogicalOperator> table_get_oper(
      new TableGetLogicalOperator(table, fields, false /*readonly*/));

  unique_ptr<LogicalOperator> predicate_oper;
  RC                          rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalOperator> delete_oper(new DeleteLogicalOperator(table));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    delete_oper->add_child(std::move(predicate_oper));
  } else {
    delete_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(delete_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(
    ExplainStmt *explain_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  unique_ptr<LogicalOperator> child_oper;

  Stmt *child_stmt = explain_stmt->child();

  RC rc = create(child_stmt, child_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child operator. rc=%s", strrc(rc));
    return rc;
  }

  logical_operator = unique_ptr<LogicalOperator>(new ExplainLogicalOperator);
  logical_operator->add_child(std::move(child_oper));
  return rc;
}
