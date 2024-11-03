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
// Created by Wangyunlai on 2023/6/13.
//

#include "common/log/log.h"
#include "common/types.h"
#include "sql/stmt/create_table_stmt.h"
#include "event/sql_debug.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/select_stmt.h"
#include <cstddef>

RC create_table_with_select(Db *db, const CreateTableSqlNode &create_table, Stmt *&stmt, SelectSqlNode &select_sql,
    StorageFormat storage_format)
{
  Stmt *select_stmt = nullptr;
  RC    rc          = SelectStmt::create(db, select_sql, select_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create select_stmt.");
    return rc;
  }

  // 新的 attr_infos
  std::vector<AttrInfoSqlNode> attr_infos;
  for (auto &attr_expr : static_cast<SelectStmt *>(select_stmt)->query_expressions()) {
    AttrInfoSqlNode attr_info;
    std::string     attr_name = attr_expr->alias();

    if (attr_name.empty()) {
      attr_info.name = attr_expr->name();
    } else if (attr_name.find('.') != std::string::npos) {
      size_t p       = attr_name.find('.');
      attr_info.name = attr_name.substr(p + 1);
    } else {
      attr_info.name = attr_expr->alias();
    }

    if (attr_expr->type() == ExprType::FIELD) {
      auto field_expr    = static_cast<FieldExpr *>(attr_expr.get());
      attr_info.nullable = field_expr->get_field_meta()->nullable();
      attr_info.length   = field_expr->get_field_meta()->len();
      attr_info.type     = field_expr->get_field_meta()->type();
    } else if (attr_expr->type() == ExprType::VALUE) {
      auto value_expr    = static_cast<ValueExpr *>(attr_expr.get());
      attr_info.type     = value_expr->value_type();
      attr_info.length   = value_expr->value_length();
      attr_info.nullable = true;
    } else {
      attr_info.type     = attr_expr->value_type();
      attr_info.length   = attr_expr->value_length();
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

      attr_expr->check_or_get(check_nullable);
    }

    attr_infos.push_back(attr_info);
  }

  if (!create_table.attr_infos.empty()) {
    if (attr_infos.size() != create_table.attr_infos.size()) {
      LOG_WARN("row num error.");
      return RC::INVALID_ARGUMENT;
    }

    for (size_t i = 0; i < attr_infos.size(); ++i) {
      if (attr_infos[i].type != create_table.attr_infos[i].type) {
        LOG_WARN("type error.");
        return RC::INVALID_ARGUMENT;
      }
    }

    stmt = new CreateTableStmt(create_table.relation_name,
        create_table.attr_infos,
        storage_format,
        db,
        static_cast<SelectStmt *>(select_stmt));
  } else {
    stmt = new CreateTableStmt(
        create_table.relation_name, attr_infos, storage_format, db, static_cast<SelectStmt *>(select_stmt));
  }
  sql_debug("create table statement: table name %s", create_table.relation_name.c_str());
  return RC::SUCCESS;
}

RC CreateTableStmt::create(Db *db, const CreateTableSqlNode &create_table, Stmt *&stmt, SelectSqlNode &select_sql)
{
  StorageFormat storage_format = StorageFormat::UNKNOWN_FORMAT;
  if (create_table.storage_format.length() == 0) {
    storage_format = StorageFormat::ROW_FORMAT;
  } else {
    storage_format = get_storage_format(create_table.storage_format.c_str());
  }
  if (storage_format == StorageFormat::UNKNOWN_FORMAT) {
    return RC::INVALID_ARGUMENT;
  }

  if (!select_sql.expressions.empty()) {
    return create_table_with_select(db, create_table, stmt, select_sql, storage_format);
  }

  stmt = new CreateTableStmt(create_table.relation_name, create_table.attr_infos, storage_format, db, nullptr);
  sql_debug("create table statement: table name %s", create_table.relation_name.c_str());
  return RC::SUCCESS;
}

StorageFormat CreateTableStmt::get_storage_format(const char *format_str)
{
  StorageFormat format = StorageFormat::UNKNOWN_FORMAT;
  if (0 == strcasecmp(format_str, "ROW")) {
    format = StorageFormat::ROW_FORMAT;
  } else if (0 == strcasecmp(format_str, "PAX")) {
    format = StorageFormat::PAX_FORMAT;
  } else {
    format = StorageFormat::UNKNOWN_FORMAT;
  }
  return format;
}
