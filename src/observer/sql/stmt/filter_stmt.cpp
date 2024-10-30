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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/filter_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "common/type/attr_type.h"
#include "common/type/date_type.h"
#include "common/value.h"
#include "sql/expr/expression.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include "storage/table/table.h"

FilterStmt::~FilterStmt() {}

RC check_comparison_invalid(Expression *left_expr, Expression *right_expr)
{
  if (left_expr == nullptr || right_expr == nullptr) {
    return RC::SUCCESS;
  }

  if (left_expr->type() == ExprType::FIELD &&
      static_cast<FieldExpr *>(left_expr)->value_type() == AttrType::DATES &&
      right_expr->type() == ExprType::VALUE) {
    Value tmp = static_cast<ValueExpr *>(right_expr)->get_value();
    Value date_tmp;
    date_tmp.set_type(AttrType::DATES);
    if (tmp.attr_type() == AttrType::CHARS) {
      return tmp.cast_to(tmp, AttrType::DATES, date_tmp);
    }
  }
  return RC::SUCCESS;
}

RC FilterStmt::create(Db *db, Table *default_table,
    std::unordered_map<std::string, Table *> *tables,
    std::shared_ptr<Expression> conditions, FilterStmt *&stmt)
{
  RC rc = RC::SUCCESS;
  stmt  = nullptr;
  if (conditions == nullptr) {
    return rc;
  }

  bool                              need_continue_check = true;
  std::function<void(Expression *)> check_condition =
      [&rc, &need_continue_check, tables, default_table, &check_condition, db](
          Expression *expression) {
        if (!need_continue_check) {
          return;
        }

        switch (expression->type()) {
          case ExprType::FIELD: {
            auto expr = static_cast<FieldExpr *>(expression);
            rc        = expr->check_field(*tables, default_table, {}, {});

            if (rc != RC::SUCCESS) {
              LOG_WARN("check_field failed.");
              need_continue_check = false;
            }
          } break;
          case ExprType::COMPARISON: {
            ComparisonExpr *expr = static_cast<ComparisonExpr *>(expression);
            if (expr->left() != nullptr) {
              check_condition(expr->left().get());
            }

            if (expr->right() != nullptr) {
              check_condition(expr->right().get());
            }

            if (!need_continue_check) {
              LOG_WARN("something wrong before check_comparison_invalid.");
              return;
            }

            rc = check_comparison_invalid(
                expr->left().get(), expr->right().get());
            if (rc != RC::SUCCESS) {
              LOG_WARN("check_comparison_invalid failed.");
              need_continue_check = false;
              return;
            }
          } break;
          case ExprType::ARITHMETIC: {
            auto *expr = static_cast<ArithmeticExpr *>(expression);
            if (expr->left() != nullptr) {
              check_condition(expr->left().get());
            }

            if (expr->right() != nullptr) {
              check_condition(expr->right().get());
            }
          } break;
          case ExprType::CONJUNCTION: {
            auto *expr = static_cast<ConjunctionExpr *>(expression);
            for (auto &child : expr->children()) {
              check_condition(child.get());
            }
          } break;
          case ExprType::VALUE: {

          } break;
          case ExprType::SUBQUERY_OR_VALUELIST: {
            auto expr =
                static_cast<SubQuery_ValueList_Expression *>(expression);

            rc = expr->create_stmt(db, *tables);
            if (rc != RC::SUCCESS) {
              need_continue_check = false;
              LOG_WARN("sub_query in filter creat_stmt wrong.");
              return;
            }
          } break;
          default: {

          } break;
        }
      };

  conditions->check_or_get(check_condition);

  if (rc != RC::SUCCESS) {
    return rc;
  }

  stmt              = new FilterStmt();
  stmt->conditions_ = conditions;
  return rc;
}

RC get_table_and_field(Db *db, Table *default_table,
    std::unordered_map<std::string, Table *> *tables,
    const RelAttrSqlNode &attr, Table *&table, const FieldMeta *&field)
{
  if (common::is_blank(attr.relation_name.c_str())) {
    table = default_table;
  } else if (nullptr != tables) {
    auto iter = tables->find(attr.relation_name);
    if (iter != tables->end()) {
      table = iter->second;
    }
  } else {
    table = db->find_table(attr.relation_name.c_str());
  }
  if (nullptr == table) {
    LOG_WARN("No such table: attr.relation_name: %s", attr.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  field = table->table_meta().field(attr.attribute_name.c_str());
  if (nullptr == field) {
    LOG_WARN("no such field in table: table %s, field %s", table->name(), attr.attribute_name.c_str());
    table = nullptr;
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  return RC::SUCCESS;
}

// RC FilterStmt::create_filter_unit(Db *db, Table *default_table,
// std::unordered_map<std::string, Table *> *tables,
//     const ConditionSqlNode &condition, FilterUnit *&filter_unit)
// {
//   RC rc = RC::SUCCESS;

//   CompOp comp = condition.comp;
//   if (comp < EQUAL_TO || comp >= NO_OP) {
//     LOG_WARN("invalid compare operator : %d", comp);
//     return RC::INVALID_ARGUMENT;
//   }

//   filter_unit = new FilterUnit;

//   if (condition.left_is_attr) {
//     Table           *table = nullptr;
//     const FieldMeta *field = nullptr;
//     rc                     = get_table_and_field(db, default_table, tables,
//     condition.left_attr, table, field); if (rc != RC::SUCCESS) {
//       LOG_WARN("cannot find attr");
//       return rc;
//     }
//     FilterObj filter_obj;
//     filter_obj.init_attr(Field(table, field));
//     filter_unit->set_left(filter_obj);
//   } else {
//     FilterObj filter_obj;
//     filter_obj.init_value(condition.left_value);
//     filter_unit->set_left(filter_obj);
//   }

//   if (condition.right_is_attr) {
//     Table           *table = nullptr;
//     const FieldMeta *field = nullptr;
//     rc                     = get_table_and_field(db, default_table, tables,
//     condition.right_attr, table, field); if (rc != RC::SUCCESS) {
//       LOG_WARN("cannot find attr");
//       return rc;
//     }
//     FilterObj filter_obj;
//     filter_obj.init_attr(Field(table, field));
//     filter_unit->set_right(filter_obj);
//   } else {
//     FilterObj filter_obj;
//     filter_obj.init_value(condition.right_value);
//     filter_unit->set_right(filter_obj);
//   }

//   filter_unit->set_comp(comp);

//   // 检查两个类型是否能够比较
//   return rc;
// }
