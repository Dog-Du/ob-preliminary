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
// Created by Wangyunlai on 2023/4/25.
//

#include "sql/parser/parse_defs.h"
#include "sql/stmt/create_index_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/type/attr_type.h"
#include "sql/stmt/select_stmt.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include "storage/table/table.h"
#include "sql/expr/expression.h"
#include <cstddef>
#include "sql/stmt/create_view_stmt.h"

using namespace std;
using namespace common;

RC CreateViewStmt::create(Db *db, const CreateViewSqlNode &create_view, Stmt *&stmt, SelectSqlNode &selection)
{
  RC rc = RC::SUCCESS;
  if (!create_view.attr_names.empty() && create_view.attr_names.size() != selection.expressions.size()) {
    LOG_WARN("failed.");
    return RC::INVALID_ARGUMENT;
  }

  Stmt *select_stmt = nullptr;
  rc                = SelectStmt::create(db, selection, select_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("select failed.");
    return rc;
  }

  std::vector<AttrInfoSqlNode> attr_infos;
  std::vector<Field>           fields;
  bool                         can_write;
  SelectStmt                  *select = static_cast<SelectStmt *>(select_stmt);

  for (auto &expr : select->query_expressions()) {
    Field           field;
    AttrInfoSqlNode attr_info;
    std::string     alias = expr->alias();
    if (alias.empty()) {
      attr_info.name = expr->name();
    } else {
      size_t pos = alias.find('.');
      if (pos == std::string::npos) {
        attr_info.name = alias;
      } else {
        attr_info.name = alias.substr(pos + 1);
      }
    }

    if (expr->type() == ExprType::FIELD) {
      can_write          = true;
      auto field_expr    = static_cast<FieldExpr *>(expr.get());
      attr_info.nullable = field_expr->get_field_meta()->nullable();
      attr_info.length   = field_expr->get_field_meta()->len();
      attr_info.type     = field_expr->get_field_meta()->type();
      field              = field_expr->field();
    } else if (expr->type() == ExprType::VALUE) {
      can_write          = false;
      auto value_expr    = static_cast<ValueExpr *>(expr.get());
      attr_info.type     = value_expr->value_type();
      attr_info.length   = value_expr->value_length();
      attr_info.nullable = true;
    } else {
      can_write          = false;
      attr_info.type     = expr->value_type();
      attr_info.length   = expr->value_length();
      attr_info.nullable = false;

      bool                              need_continue_check = true;
      std::function<void(Expression *)> check_nullable      = [&](Expression *expression) {
        if (!need_continue_check) {
          return;
        }

        switch (expression->type()) {
          case ExprType::AGGREGATION: {
            auto expr = static_cast<AggregateExpr *>(expression);
            if (expr->child() != nullptr) {
              check_nullable(expr->child().get());
            }
          } break;
          case ExprType::ARITHMETIC: {
            auto expr = static_cast<ArithmeticExpr *>(expression);
            if (expr->left() != nullptr) {
              check_nullable(expr->left().get());
            }

            if (expr->right() != nullptr) {
              check_nullable(expr->right().get());
            }
          } break;
          case ExprType::COMPARISON: {
            auto expr = static_cast<ComparisonExpr *>(expression);
            if (expr->left() != nullptr) {
              check_nullable(expr->left().get());
            }

            if (expr->right() != nullptr) {
              check_nullable(expr->right().get());
            }
          } break;
          case ExprType::CONJUNCTION: {
            auto expr = static_cast<ConjunctionExpr *>(expression);
            for (auto &c : expr->children()) {
              check_nullable(c.get());
            }
          } break;
          case ExprType::FIELD: {
            auto expr = static_cast<FieldExpr *>(expression);
            if (expr->get_field_meta()->nullable()) {
              attr_info.nullable  = true;
              need_continue_check = false;
            }
          } break;
          default: {
          } break;
        }
      };

      expr->check_or_get(check_nullable);
    }

    fields.push_back(field);
    attr_infos.push_back(attr_info);
  }

  if (!create_view.attr_names.empty()) {
    for (size_t i = 0; i < create_view.attr_names.size(); ++i) {
      attr_infos[i].name = create_view.attr_names[i];
    }
  }

  stmt = new CreateViewStmt(create_view.view_name, attr_infos, fields, selection, can_write);
  return RC::SUCCESS;
}
