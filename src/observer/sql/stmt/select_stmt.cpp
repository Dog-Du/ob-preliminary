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
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/groupby_stmt.h"
#include "sql/stmt/orderby_stmt.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include "storage/table/table.h"
#include "sql/parser/expression_binder.h"
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;
using namespace common;

SelectStmt::~SelectStmt() {}

RC check_alias_and_joins(Db *db, std::unordered_map<std::string, std::string> &alias_map,
    std::unordered_map<std::string, Table *> &all_tables, std::vector<Table *> &tables,
    std::vector<JoinSqlNode> &from_list, std::vector<JoinNodes> &join_tabes)
{
  std::unordered_set<std::string> alias_set;  // 别名，表名是否重复

  auto push_and_check_table = [&](RelAttrSqlNode &table_name) {
    if (table_name.relation_name.empty()) {
      LOG_WARN("error, table_name empty");
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    Table *table = db->find_table(table_name.relation_name.c_str());

    if (nullptr == table) {
      LOG_WARN("table not exist. table_name=%s.", table_name.relation_name.c_str());
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    if (!table_name.alias.empty()) {
      if (alias_set.count(table_name.alias) > 0) {
        LOG_WARN("alias count > 0");
        return RC::INVALID_ARGUMENT;
      }

      alias_set.emplace(table_name.alias);
      alias_map.emplace(std::make_pair(table_name.relation_name, table_name.alias));
      all_tables.emplace(std::make_pair(table_name.alias, table));
    }

    tables.push_back(table);
    all_tables.emplace(std::make_pair(table_name.relation_name, table));
    return RC::SUCCESS;
  };

  auto check_table = [&](RelAttrSqlNode &table, std::shared_ptr<Expression> conditions, JoinNodes &join) {
    RC rc = RC::SUCCESS;
    rc    = push_and_check_table(table);

    if (rc != RC::SUCCESS) {
      return rc;
    }

    FilterStmt *filter = nullptr;

    if (nullptr != conditions) {
      rc = FilterStmt::create(db, all_tables[table.relation_name], &all_tables, conditions, filter);
      if (rc != RC::SUCCESS) {
        LOG_WARN("create filter stmt failed.");
        return rc;
      }
    }

    join.tables_.emplace_back(all_tables[table.relation_name]);
    join.conditions_.emplace_back(filter);
    return RC::SUCCESS;
  };

  for (auto &from_node : from_list) {
    JoinNodes join;
    RC        rc = check_table(from_node.first_rel, nullptr, join);

    if (rc != RC::SUCCESS) {
      return rc;
    }

    for (size_t i = 0; i < from_node.conditions.size(); ++i) {
      rc = check_table(from_node.join_rel_list[i], from_node.conditions[i], join);

      if (rc != RC::SUCCESS) {
        return rc;
      }
    }

    join_tabes.emplace_back(join);
  }
  return RC::SUCCESS;
}

// all_tables 是所有的表，包括当前表和复杂子查询中的表
RC SelectStmt::create(
    Db *db, SelectSqlNode &select_sql, Stmt *&stmt, const std::unordered_map<std::string, Table *> &all_tables)
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  // 阿哲，在mysql里，即使没有group by也可以having，就是吧所有结果当作一个分组。但是这里应该可以不实现。
  if (select_sql.group_by.empty() && select_sql.having != nullptr) {
    LOG_DEBUG("having while no group_by.");
    return RC::SQL_SYNTAX;
  }

  std::vector<Table *> tables;

  // 记录 name -> alias
  std::unordered_map<std::string, std::string> alias_map;
  std::unordered_map<std::string, Table *>     table_map = all_tables;
  std::vector<JoinNodes>                       join_tables;  // 连接表。

  // 获取别名和连接表。
  RC rc = check_alias_and_joins(db, alias_map, table_map, tables, select_sql.relations, join_tables);

  if (rc != RC::SUCCESS) {
    LOG_WARN("check_alias_and_joins failed in select stmt.");
    return rc;
  }

  Table                                      *default_table = nullptr;
  std::vector<std::shared_ptr<Expression>>    projects;
  std::vector<std::shared_ptr<AggregateExpr>> agg_exprs;
  std::vector<FieldExpr *>                    field_exprs;

  if (tables.size() == 1) {
    default_table = tables[0];
  }

  auto fulls_tables = [&](const Table *table, const std::string &alias) {
    const TableMeta &table_meta = table->table_meta();
    int              field_num  = table_meta.field_num();

    for (int i = table_meta.sys_field_num(); i < field_num; i++) {
      auto field = table_meta.field(i);
      if (field->visible()) {
        FieldExpr *field_expr = new FieldExpr(table, field);
        if (tables.size() == 1) {
          field_expr->set_name(field_expr->field_name());  // should same as origin
        } else if (alias.empty()) {
          field_expr->set_name(std::string(field_expr->table_name()) + "." + field_expr->field_name());
        } else {
          field_expr->set_name(alias + "." + field_expr->field_name());
        }
        projects.push_back(std::shared_ptr<Expression>(field_expr));
      }
    }
  };

  bool need_continue_check = true;
  bool have_agg            = false;
  bool have_not_agg        = false;
  bool now_is_agg          = false;

  std::function<void(Expression *)> check_projections = [&](Expression *expression) {
    if (!need_continue_check) {
      return;
    }

    switch (expression->type()) {
      case ExprType::FIELD: {
        if (!now_is_agg) {
          have_not_agg = true;
        }
        rc = static_cast<FieldExpr *>(expression)->check_field(table_map, default_table, tables, alias_map);
        field_exprs.push_back(static_cast<FieldExpr *>(expression));
        if (rc != RC::SUCCESS) {
          need_continue_check = false;
        }
      } break;
      case ExprType::COMPARISON: {
        ComparisonExpr *expr = static_cast<ComparisonExpr *>(expression);
        if (expr->left() != nullptr) {
          check_projections(expr->left().get());
        }
        if (expr->right() != nullptr) {
          check_projections(expr->right().get());
        }
      } break;
      case ExprType::ARITHMETIC: {
        auto *expr = static_cast<ArithmeticExpr *>(expression);

        if (expr->left() != nullptr) {
          check_projections(expr->left().get());
        }

        if (expr->right() != nullptr) {
          check_projections(expr->right().get());
        }
      } break;
      case ExprType::CONJUNCTION: {
        auto *expr = static_cast<ConjunctionExpr *>(expression);
        for (auto &child : expr->children()) {
          check_projections(child.get());
        }
      } break;
      case ExprType::SUBQUERY_OR_VALUELIST: {
        need_continue_check = false;
        rc                  = RC::SQL_SYNTAX;
        LOG_WARN("sub_query should not in query_expression");
      } break;
      case ExprType::AGGREGATION: {
        have_agg   = true;
        auto *expr = static_cast<AggregateExpr *>(expression);
        if (expr->child() != nullptr) {
          check_projections(expr->child().get());
        }
      } break;
      default: {
      } break;
    }
  };

  for (auto &expression : select_sql.expressions) {
    now_is_agg = false;
    if (expression->type() == ExprType::FIELD) {
      have_not_agg                 = true;
      FieldExpr  *field_expression = static_cast<FieldExpr *>(expression.get());
      const char *table_name       = field_expression->table_name();
      const char *field_name       = field_expression->field_name();

      field_exprs.push_back(field_expression);
      if ((0 == strcmp(table_name, "*")) && (0 == strcmp(field_name, "*"))) {  // * or *.*
        if (tables.empty() || (strlen(field_expression->alias()) > 0 && strcmp(field_expression->alias(), "*") != 0)) {
          return RC::INVALID_ARGUMENT;  // not allow: select *; select * as xxx;
        }
        for (const Table *table : tables) {
          if (alias_map.count(table->name())) {
            fulls_tables(table, alias_map[table->name()]);
          } else {
            fulls_tables(table, std::string());
          }
        }
      } else if (0 == strcmp(field_name, "*")) {
        auto iter = table_map.find(table_name);
        if (iter == table_map.end()) {
          LOG_WARN("no such table in from list: %s", table_name);
          return RC::SCHEMA_FIELD_MISSING;
        }
        Table *table = iter->second;
        if (alias_map.count(table->name())) {
          fulls_tables(table, alias_map[table->name()]);
        } else {
          fulls_tables(table, std::string());
        }
      } else {
        rc = field_expression->check_field(table_map, default_table, tables, alias_map);
        if (rc != RC::SUCCESS) {
          return rc;
        }
        projects.push_back(expression);
      }
    } else {
      if (expression->type() == ExprType::AGGREGATION) {
        agg_exprs.push_back(std::static_pointer_cast<AggregateExpr>(expression));
        auto *expr = static_cast<AggregateExpr *>(expression.get());
        now_is_agg = true;
        have_agg   = true;
        if (expr->child() != nullptr) {
          expr->child()->check_or_get(check_projections);
        }
      } else {
        expression->check_or_get(check_projections);
      }

      if (rc != RC::SUCCESS) {
        LOG_WARN("check_projections failed.");
        return rc;
      }
      projects.push_back(expression);
    }
  }

  if (have_not_agg && select_sql.group_by.empty() && have_agg) {
    LOG_WARN("no group by.");
    return RC::SQL_SYNTAX;
  }
  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;

  if (select_sql.conditions != nullptr) {
    rc = FilterStmt::create(db, default_table, &table_map, select_sql.conditions, filter_stmt);
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  std::vector<std::shared_ptr<FieldExpr>> group_by;
  for (auto &x : select_sql.group_by) {
    std::shared_ptr<FieldExpr> field_expr(new FieldExpr(x.relation_name, x.attribute_name, x.alias));
    rc = field_expr->check_field(table_map, default_table, tables, alias_map);
    if (rc != RC::SUCCESS) {
      LOG_WARN("check_field failed.");
      return rc;
    }
    group_by.push_back(field_expr);
  }

  std::vector<std::shared_ptr<FieldExpr>> order_by;
  std::vector<OrderByType>                order_by_type_;
  for (auto &x : select_sql.order_by) {
    std::shared_ptr<FieldExpr> field_expr(new FieldExpr(x.relation_name, x.attribute_name, x.alias));
    rc = field_expr->check_field(table_map, default_table, tables, alias_map);
    if (rc != RC::SUCCESS) {
      LOG_WARN("check_field failed.");
      return rc;
    }
    order_by.push_back(field_expr);
    order_by_type_.push_back(x.order_by_type);
  }

  FilterStmt *having_filter = nullptr;

  // 让having走一遍FilterStmt进行一定的检查和FieldExpr的初始化，但是只需要其中的expression。
  if (select_sql.having != nullptr) {
    rc = FilterStmt::create(db, default_table, &table_map, select_sql.having, having_filter);
    if (rc != RC::SUCCESS) {
      LOG_WARN("having filter create failed.");
      return rc;
    }
  }

  // everything alright
  SelectStmt  *select_stmt  = new SelectStmt();
  GroupByStmt *groupby_stmt = new GroupByStmt();
  OrderByStmt *orderby_stmt = new OrderByStmt();

  groupby_stmt->group_by_.swap(group_by);
  groupby_stmt->agg_exprs_.swap(agg_exprs);
  groupby_stmt->field_exprs_.swap(field_exprs);
  groupby_stmt->have_aggregate = have_agg;
  if (having_filter != nullptr) {
    groupby_stmt->having_filter_ = having_filter->get_conditions();
    delete having_filter;
  }

  orderby_stmt->order_by_.swap(order_by);
  orderby_stmt->order_by_type_.swap(order_by_type_);

  select_stmt->tables_.swap(join_tables);
  select_stmt->query_expressions_.swap(projects);
  select_stmt->filter_stmt_.reset(filter_stmt);
  select_stmt->group_by_.reset(groupby_stmt);
  select_stmt->order_by_.reset(orderby_stmt);

  // select_stmt->group_by_.swap(group_by_expressions);
  stmt = select_stmt;
  return RC::SUCCESS;
}
