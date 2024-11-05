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
// Created by Wangyunlai on 2022/07/05.
//

#include "sql/expr/expression.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "common/type/attr_type.h"
#include "common/value.h"
#include "sql/expr/aggregator.h"
#include "sql/expr/tuple.h"
#include "sql/expr/arithmetic_operator.hpp"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/select_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/optimizer/logical_plan_generator.h"
#include "sql/optimizer/physical_plan_generator.h"
#include <string>
#include <unordered_map>

using namespace std;

bool match_str(const std::string &pattern, const std::string &text)
{
  int                  m = pattern.size();
  int                  n = text.size();
  vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));

  dp[0][0] = true;  // Empty pattern matches empty text

  // Initialize the first row (pattern is empty)
  for (int j = 1; j <= n; ++j) {
    dp[0][j] = false;
  }

  // Initialize the first column (text is empty)
  for (int i = 1; i <= m; ++i) {
    if (pattern[i - 1] == '%') {
      dp[i][0] = dp[i - 1][0];
    }
  }

  // Fill in the rest of the dp table
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      if (pattern[i - 1] == text[j - 1] || pattern[i - 1] == '_') {
        dp[i][j] = dp[i - 1][j - 1];
      } else if (pattern[i - 1] == '%') {
        dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
      }
    }
  }

  return dp[m][n];
}

RC FieldExpr::get_value(const Tuple &tuple, Value &value) const
{
  return tuple.find_cell(TupleCellSpec(table_name(), field_name()), value);
}

bool FieldExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::FIELD) {
    return false;
  }
  const auto &other_field_expr = static_cast<const FieldExpr &>(other);
  return table_name() == other_field_expr.table_name() && field_name() == other_field_expr.field_name();
}

RC FieldExpr::check_field(const std::unordered_map<std::string, Table *> &all_tables, Table *default_table,
    const std::vector<Table *> &tables, const std::unordered_map<std::string, std::string> &alias_map)
{
  auto   table_name = std::string(Expression::table_name());
  auto   field_name = std::string(Expression::field_name());
  Table *table      = nullptr;

  if (table_name == "*") {
    return RC::SUCCESS;
  }

  if (!common::is_blank(table_name.c_str())) {
    auto iter = all_tables.find(table_name);
    if (iter == all_tables.end()) {
      LOG_WARN("all tables have not table_name : %s in check_field", table_name.c_str());
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    table = iter->second;
  } else {
    if (tables.size() != 1 && default_table == nullptr) {
      return RC::SCHEMA_FIELD_MISSING;
    }

    table = default_table == nullptr ? tables.front() : default_table;
  }

  ASSERT(table != nullptr, "something wrong");
  // 得到了表的名字
  table_name = table->name();

  const FieldMeta *field_meta = table->table_meta().field(field_name.c_str());

  if (nullptr == field_meta) {
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  if (strlen(Expression::alias()) > 0) {
    set_name(Expression::alias());
  } else {
    if (tables.size() > 1) {
      set_name(std::string(table_name) + "." + field_name);
    } else {
      set_name(field_name);
    }
  }

  set_table_name(table_name);
  set_field_name(field_name);

  field_ = Field(table, field_meta);

  return RC::SUCCESS;
}

// TODO: 在进行表达式计算时，`chunk` 包含了所有列，因此可以通过 `field_id`
// 获取到对应列。 后续可以优化成在 `FieldExpr` 中存储 `chunk` 中某列的位置信息。
RC FieldExpr::get_column(Chunk &chunk, Column &column)
{
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    column.reference(chunk.column(field().meta()->field_id()));
  }
  return RC::SUCCESS;
}

bool ValueExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::VALUE) {
    return false;
  }
  const auto &other_value_expr = static_cast<const ValueExpr &>(other);
  return value_.compare(other_value_expr.get_value()) == 0;
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value) const
{
  value = value_;
  return RC::SUCCESS;
}

RC ValueExpr::get_column(Chunk &chunk, Column &column)
{
  column.init(value_);
  return RC::SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
CastExpr::CastExpr(shared_ptr<Expression> child, AttrType cast_type) : child_(std::move(child)), cast_type_(cast_type)
{}

CastExpr::~CastExpr() {}

RC CastExpr::cast(const Value &value, Value &cast_value) const
{
  RC rc = RC::SUCCESS;
  if (this->value_type() == value.attr_type()) {
    cast_value = value;
    return rc;
  }
  rc = Value::cast_to(value, cast_type_, cast_value);
  return rc;
}

RC CastExpr::get_value(const Tuple &tuple, Value &result) const
{
  Value value;
  RC    rc = child_->get_value(tuple, value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

RC CastExpr::try_get_value(Value &result) const
{
  Value value;
  RC    rc = child_->try_get_value(value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

////////////////////////////////////////////////////////////////////////////////

ComparisonExpr::ComparisonExpr(CompOp comp, shared_ptr<Expression> left, shared_ptr<Expression> right)
    : comp_(comp), left_(std::move(left)), right_(std::move(right))
{}

ComparisonExpr::~ComparisonExpr() {}

RC ComparisonExpr::check_comparison_with_subquery(Expression *expression, bool need_check_complex_subquery)
{
  return RC::SUCCESS;  // 暂时决定什么都不做

  // bool need_continue_check = true;
  // RC   rc                  = RC::SUCCESS;

  // std::function<void(Expression *)> open_subquery = [&](Expression *expression) {
  //   if (!need_continue_check) {
  //     return;
  //   }

  //   switch (expression->type()) {
  //     case ExprType::SUBQUERY_OR_VALUELIST: {
  //       auto expr = static_cast<SubQuery_ValueList_Expression *>(expression);
  //       rc        = expr->open(nullptr);
  //       if (rc != RC::SUCCESS) {
  //         LOG_WARN("subquery open failed.");
  //         need_continue_check = false;
  //         return;
  //       }
  //       rc = expr->close();
  //       if (rc != RC::SUCCESS) {
  //         LOG_WARN("subquery close failed.");
  //       }
  //     } break;
  //     case ExprType::ARITHMETIC: {
  //       auto expr = static_cast<ArithmeticExpr *>(expression);
  //       if (expr->left() != nullptr) {
  //         open_subquery(expr->left().get());
  //       }
  //       if (expr->right() != nullptr) {
  //         open_subquery(expr->right().get());
  //       }
  //     } break;
  //     case ExprType::AGGREGATION: {
  //       auto expr = static_cast<AggregateExpr *>(expression);
  //       if (expr->child() != nullptr) {
  //         open_subquery(expr->child().get());
  //       }
  //     } break;
  //     case ExprType::COMPARISON: {
  //       auto expr = static_cast<ComparisonExpr *>(expression);
  //       if (expr->left() != nullptr) {
  //         open_subquery(expr->left().get());
  //       }
  //       if (expr->right() != nullptr) {
  //         open_subquery(expr->right().get());
  //       }
  //     } break;
  //     case ExprType::CONJUNCTION: {
  //       auto expr = static_cast<ConjunctionExpr *>(expression);
  //       for (auto &e : expr->children()) {
  //         open_subquery(e.get());
  //       }
  //     } break;
  //     default: {
  //     } break;
  //   }
  // };

  // std::function<void(Expression *)> check_comparison = [&](Expression *expression) {
  //   if (!need_continue_check) {
  //     return;
  //   }

  //   switch (expression->type()) {
  //     case ExprType::COMPARISON: {
  //       auto expr = static_cast<ComparisonExpr *>(expression);
  //       auto comp = expr->comp();
  //       /// TODO:考虑如果子查询为空怎么办 -> 转到子查询的地方去处理。

  //       if (comp != CompOp::IN && comp != CompOp::NOT_IN && comp != CompOp::EXISTS && comp != CompOp::NOT_EXISTS) {
  //         if (expr->left() != nullptr && expr->left()->type() == ExprType::SUBQUERY_OR_VALUELIST) {
  //           auto    left_expr = static_cast<SubQuery_ValueList_Expression *>(expr->left().get());
  //           int32_t num;
  //           rc = left_expr->value_num(num);
  //           if (rc != RC::SUCCESS) {
  //             LOG_WARN("value_num failed.");
  //             need_continue_check = false;
  //             return;
  //           }

  //           if (num > 1) {
  //             LOG_WARN("value_num > 1 failed.");
  //             rc                  = RC::VARIABLE_NOT_VALID;
  //             need_continue_check = false;
  //           }
  //         }

  //         if (expr->right() != nullptr && expr->right()->type() == ExprType::SUBQUERY_OR_VALUELIST) {
  //           auto    right_expr = static_cast<SubQuery_ValueList_Expression *>(expr->right().get());
  //           int32_t num;
  //           rc = right_expr->value_num(num);

  //           if (rc != RC::SUCCESS) {
  //             LOG_WARN("value_num failed.");
  //             need_continue_check = false;
  //             return;
  //           }

  //           if (num > 1) {
  //             LOG_WARN("value_num > 1 failed.");
  //             rc                  = RC::VARIABLE_NOT_VALID;
  //             need_continue_check = false;
  //           }
  //         }
  //       }
  //     } break;
  //     case ExprType::ARITHMETIC: {
  //       auto expr = static_cast<ArithmeticExpr *>(expression);
  //       if (expr->left() != nullptr) {
  //         check_comparison(expr->left().get());
  //       }
  //       if (expr->right() != nullptr) {
  //         check_comparison(expr->right().get());
  //       }
  //     } break;
  //     case ExprType::AGGREGATION: {
  //       auto expr = static_cast<AggregateExpr *>(expression);
  //       if (expr->child() != nullptr) {
  //         check_comparison(expr->child().get());
  //       }
  //     } break;
  //     case ExprType::CONJUNCTION: {
  //       auto expr = static_cast<ConjunctionExpr *>(expression);
  //       for (auto &e : expr->children()) {
  //         check_comparison(e.get());
  //       }
  //     } break;
  //     default: {
  //     } break;
  //   }
  // };

  // expression->check_or_get(open_subquery);
  // if (rc != RC::SUCCESS) {
  //   LOG_WARN("open subquery failed.");
  //   return rc;
  // }

  // rc                  = RC::SUCCESS;
  // need_continue_check = true;
  // expression->check_or_get(check_comparison);
  // if (rc != RC::SUCCESS) {
  //   LOG_WARN("check_comparison failed.");
  //   return rc;
  // }
  // return rc;
}

RC ComparisonExpr::compare_value(const Value &left, const Value &right, bool &result) const
{
  RC  rc         = RC::SUCCESS;
  int cmp_result = left.compare(right);
  result         = false;
  bool has_null  = left.is_null(left) || right.is_null(right);

  switch (comp_) {
    case EQUAL_TO: {
      result = (0 == cmp_result) && !has_null;
    } break;
    case LESS_EQUAL: {
      result = (cmp_result <= 0) && !has_null;
    } break;
    case NOT_EQUAL: {
      result = (cmp_result != 0) && !has_null;
    } break;
    case LESS_THAN: {
      result = (cmp_result < 0) && !has_null;
    } break;
    case GREAT_EQUAL: {
      result = (cmp_result >= 0) && !has_null;
    } break;
    case GREAT_THAN: {
      result = (cmp_result > 0) && !has_null;
    } break;
    case IS: {
      result = (left.is_null(left) && right.is_null(right));
    } break;
    case NOT_IS: {
      result = !(left.is_null(left) && right.is_null(right));
    } break;
    case LIKE: {
      result = match_str(right.get_string(), left.get_string()) && !has_null;
    } break;
    case NOT_LIKE: {
      result = !match_str(right.get_string(), left.get_string()) && !has_null;
    } break;
    default: {
      LOG_WARN("unsupported comparison. %d", comp_);
      rc = RC::INTERNAL;
    } break;
  }

  return rc;
}

RC ComparisonExpr::try_get_value(Value &cell) const
{
  if (left_->type() == ExprType::VALUE && right_->type() == ExprType::VALUE) {
    ValueExpr   *left_value_expr  = static_cast<ValueExpr *>(left_.get());
    ValueExpr   *right_value_expr = static_cast<ValueExpr *>(right_.get());
    const Value &left_cell        = left_value_expr->get_value();
    const Value &right_cell       = right_value_expr->get_value();

    bool value = false;
    RC   rc    = compare_value(left_cell, right_cell, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to compare tuple cells. rc=%s", strrc(rc));
    } else {
      cell.set_boolean(value);
    }
    return rc;
  }

  return RC::INVALID_ARGUMENT;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value) const
{
  Value left_value;
  Value right_value;

  if (right_ == nullptr) {
    ASSERT(comp_ == EXISTS || comp_ == NOT_EXISTS, "must be exists or not exists");
  }

  if (comp_ == EXISTS || comp_ == NOT_EXISTS) {
    ASSERT(right_ == nullptr, "right must be nullptr.");
  }

  RC rc = RC::SUCCESS;

  if (comp_ == EXISTS || comp_ == NOT_EXISTS) {
    auto expr = static_cast<SubQuery_ValueList_Expression *>(left_.get());
    expr->set_prev_tuple(&tuple);
    rc = expr->open(nullptr);
    if (rc != RC::SUCCESS) {
      LOG_WARN("open failed");
      expr->close();
      return rc;
    }

    rc = expr->get_value(tuple, value);
    if (rc != RC::SUCCESS) {
      if (rc != RC::RECORD_EOF) {
        LOG_WARN("something wrong.");
        expr->close();
        return rc;
      }
    }

    bool exists = rc == RC::RECORD_EOF ? false : true;
    rc          = expr->close();

    if (rc != RC::SUCCESS) {
      LOG_WARN("close failed.");
      return rc;
    }

    value.set_boolean(comp_ == EXISTS ? exists : !exists);
    return rc;
  }

  if (comp_ == IN || comp_ == NOT_IN) {
    ASSERT(right_->type() == ExprType::SUBQUERY_OR_VALUELIST, "");
    ASSERT(left_->type() != ExprType::SUBQUERY_OR_VALUELIST, "");
    rc = left_->get_value(tuple, left_value);  // 这里是这样想的 ： 只会有 value/field in ( select_stmt ) 的情况
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
      return rc;
    }
    auto expr = static_cast<SubQuery_ValueList_Expression *>(right_.get());
    expr->set_prev_tuple(&tuple);
    rc = expr->open(nullptr);

    if (rc != RC::SUCCESS) {
      LOG_WARN("open failed.");
      expr->close();
      return rc;
    }

    bool in = false;
    while ((rc = expr->next(tuple, right_value)) == RC::SUCCESS) {
      if (left_value.compare(right_value) == 0) {
        in = true;
        break;
      }
    }

    if (rc != RC::SUCCESS) {
      if (rc != RC::RECORD_EOF) {
        LOG_WARN("next failed.");
        expr->close();
        return rc;
      }
      rc = RC::SUCCESS;
    }
    rc = expr->close();

    if (rc != RC::SUCCESS) {
      LOG_WARN("close failed");
      return rc;
    }
    value.set_boolean(comp_ == IN ? in : !in);
    return rc;
  }

  auto get_value_from_subquery = [&rc, &tuple](Expression *expression, Value &value) {
    if (expression->type() == ExprType::SUBQUERY_OR_VALUELIST) {
      auto expr = static_cast<SubQuery_ValueList_Expression *>(expression);
      expr->set_prev_tuple(&tuple);
      rc = expr->open(nullptr);
      if (rc != RC::SUCCESS) {
        LOG_WARN("open failed.");
        expr->close();
        return;
      }

      rc = expr->next(tuple, value);

      if (rc == RC::RECORD_EOF) {  // 空表
        value.set_type(AttrType::INTS);
        value.set_data((const char *)&INT_NULL, sizeof(INT_NULL));
        expr->close();
      } else if (rc != RC::SUCCESS) {  // 出错
        expr->close();
        LOG_WARN("next failed.");
      } else {  // 检查是否有多个tuple
        Value val;
        rc = expr->next(tuple, val);
        expr->close();
        if (rc != RC::RECORD_EOF) {
          LOG_WARN("more than one tuple.");
          rc = RC::INVALID_ARGUMENT;
          return;
        }
        rc = RC::SUCCESS;
      }
    } else {
      rc = expression->get_value(tuple, value);
    }
  };

  get_value_from_subquery(left_.get(), left_value);

  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  get_value_from_subquery(right_.get(), right_value);

  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }

  bool bool_value = false;

  rc = compare_value(left_value, right_value, bool_value);
  if (rc == RC::SUCCESS) {
    value.set_boolean(bool_value);
  }
  return rc;
}

RC ComparisonExpr::eval(Chunk &chunk, std::vector<uint8_t> &select)
{
  RC     rc = RC::SUCCESS;
  Column left_column;
  Column right_column;

  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }
  if (left_column.attr_type() != right_column.attr_type()) {
    LOG_WARN("cannot compare columns with different types");
    return RC::INTERNAL;
  }
  if (left_column.attr_type() == AttrType::INTS) {
    rc = compare_column<int>(left_column, right_column, select);
  } else if (left_column.attr_type() == AttrType::FLOATS) {
    rc = compare_column<float>(left_column, right_column, select);
  } else {
    // TODO: support string compare
    LOG_WARN("unsupported data type %d", left_column.attr_type());
    return RC::INTERNAL;
  }
  return rc;
}

template <typename T>
RC ComparisonExpr::compare_column(const Column &left, const Column &right, std::vector<uint8_t> &result) const
{
  RC rc = RC::SUCCESS;

  bool left_const  = left.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    compare_result<T, true, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else if (left_const && !right_const) {
    compare_result<T, true, false>((T *)left.data(), (T *)right.data(), right.count(), result, comp_);
  } else if (!left_const && right_const) {
    compare_result<T, false, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else {
    compare_result<T, false, false>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  }
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
ConjunctionExpr::ConjunctionExpr(Type type, vector<shared_ptr<Expression>> &children)
    : conjunction_type_(type), children_(std::move(children))
{}

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    value.set_boolean(true);
    return rc;
  }

  Value tmp_value;
  for (const shared_ptr<Expression> &expr : children_) {
    rc = expr->get_value(tuple, tmp_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value by child expression. rc=%s", strrc(rc));
      return rc;
    }
    bool bool_value = tmp_value.get_boolean();
    if ((conjunction_type_ == Type::AND && !bool_value) || (conjunction_type_ == Type::OR && bool_value)) {
      value.set_boolean(bool_value);
      return rc;
    }
  }

  bool default_value = (conjunction_type_ == Type::AND);
  value.set_boolean(default_value);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////

ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, Expression *left, Expression *right)
    : arithmetic_type_(type), left_(left), right_(right)
{}
ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, shared_ptr<Expression> left, shared_ptr<Expression> right)
    : arithmetic_type_(type), left_(std::move(left)), right_(std::move(right))
{}

bool ArithmeticExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (type() != other.type()) {
    return false;
  }
  auto &other_arith_expr = static_cast<const ArithmeticExpr &>(other);
  return arithmetic_type_ == other_arith_expr.arithmetic_type() && left_->equal(*other_arith_expr.left_) &&
         right_->equal(*other_arith_expr.right_);
}

AttrType ArithmeticExpr::value_type() const
{
  if (!right_) {
    return left_->value_type();
  }

  if (left_->value_type() == AttrType::INTS && right_->value_type() == AttrType::INTS &&
      arithmetic_type_ != Type::DIV) {
    return AttrType::INTS;
  }

  if (left_->value_type() == AttrType::VECTORS && right_->value_type() == AttrType::VECTORS &&
      arithmetic_type_ != Type::DIV) {
    return AttrType::VECTORS;
  }

  return AttrType::FLOATS;
}

RC ArithmeticExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const
{
  RC rc = RC::SUCCESS;

  const AttrType target_type = value_type();
  value.set_type(target_type);

  switch (arithmetic_type_) {
    case Type::ADD: {
      Value::add(left_value, right_value, value);
    } break;

    case Type::SUB: {
      Value::subtract(left_value, right_value, value);
    } break;

    case Type::MUL: {
      Value::multiply(left_value, right_value, value);
    } break;

    case Type::DIV: {
      Value::divide(left_value, right_value, value);
    } break;

    case Type::NEGATIVE: {
      Value::negative(left_value, value);
    } break;

    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported arithmetic type. %d", arithmetic_type_);
    } break;
  }
  return rc;
}

template <bool LEFT_CONSTANT, bool RIGHT_CONSTANT>
RC ArithmeticExpr::execute_calc(
    const Column &left, const Column &right, Column &result, Type type, AttrType attr_type) const
{
  RC rc = RC::SUCCESS;
  switch (type) {
    case Type::ADD: {
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, AddOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, AddOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
    } break;
    case Type::SUB:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, SubtractOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, SubtractOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::MUL:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, MultiplyOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, MultiplyOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::DIV:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, DivideOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, DivideOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::NEGATIVE:
      if (attr_type == AttrType::INTS) {
        unary_operator<LEFT_CONSTANT, int, NegateOperator>((int *)left.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        unary_operator<LEFT_CONSTANT, float, NegateOperator>(
            (float *)left.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    default: rc = RC::UNIMPLEMENTED; break;
  }
  if (rc == RC::SUCCESS) {
    result.set_count(result.capacity());
  }
  return rc;
}

RC ArithmeticExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  ASSERT(right_!=nullptr || arithmetic_type_ == Type::NEGATIVE, "error operation");

  if (arithmetic_type_ == Type::NEGATIVE) {
    value.set_type(left_value.attr_type());
    rc = Value::negative(left_value, value);

    if (rc != RC::SUCCESS) {
      LOG_WARN("failed get value's negative. value : %s", left_value.to_string().c_str());
    }
    return rc;
  }

  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }
  return calc_value(left_value, right_value, value);
}

RC ArithmeticExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
    return rc;
  }
  Column left_column;
  Column right_column;

  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of right expression. rc=%s", strrc(rc));
    return rc;
  }
  return calc_column(left_column, right_column, column);
}

RC ArithmeticExpr::calc_column(const Column &left_column, const Column &right_column, Column &column) const
{
  RC rc = RC::SUCCESS;

  const AttrType target_type = value_type();
  column.init(target_type, left_column.attr_len(), std::max(left_column.count(), right_column.count()));
  bool left_const  = left_column.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right_column.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    column.set_column_type(Column::Type::CONSTANT_COLUMN);
    rc = execute_calc<true, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (left_const && !right_const) {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<true, false>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (!left_const && right_const) {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, false>(left_column, right_column, column, arithmetic_type_, target_type);
  }
  return rc;
}

RC ArithmeticExpr::try_get_value(Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->try_get_value(left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if (right_) {
    rc = right_->try_get_value(right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  return calc_value(left_value, right_value, value);
}

////////////////////////////////////////////////////////////////////////////////

UnboundAggregateExpr::UnboundAggregateExpr(const char *aggregate_name, Expression *child, const char *alias)
    : aggregate_name_(aggregate_name), alias_(alias), child_(child)
{}

////////////////////////////////////////////////////////////////////////////////
AggregateExpr::AggregateExpr(Type type, Expression *child) : aggregate_type_(type), child_(child) {}

AggregateExpr::AggregateExpr(Type type, shared_ptr<Expression> child) : aggregate_type_(type), child_(std::move(child))
{}

RC AggregateExpr::create_aggregator()
{
  if (child_->type() == ExprType::FIELD && strcmp(child_->alias(), "*") == 0) {
    if (aggregate_type_ != Type::COUNT) {
      return RC::SQL_SYNTAX;
    }

    aggregate_type_ = Type::COUNT_STAR;

    child_ = nullptr;
  }

  shared_ptr<Aggregator> aggregator;
  switch (aggregate_type_) {
    case Type::SUM: {
      aggregator = make_shared<SumAggregator>();
      break;
    }
    case Type::COUNT: {
      aggregator = make_shared<CountAggregator>();
    } break;
    case Type::COUNT_STAR: {
      aggregator = make_shared<CountStarAggregator>();
    } break;
    case Type::AVG: {
      aggregator = make_shared<AvgAggregator>();
    } break;
    case Type::MAX: {
      aggregator = make_shared<MaxAggregator>();
    } break;
    case Type::MIN: {
      aggregator = make_shared<MinAggregator>();
    } break;
    default: {
      ASSERT(false, "unsupported aggregate type");
      break;
    }
  }
  aggregator_ = aggregator;
  return RC::SUCCESS;
}

RC AggregateExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    rc = RC::INTERNAL;
  }
  return rc;
}

bool AggregateExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != type()) {
    return false;
  }
  const AggregateExpr &other_aggr_expr = static_cast<const AggregateExpr &>(other);
  return aggregate_type_ == other_aggr_expr.aggregate_type() && child_->equal(*other_aggr_expr.child());
}

RC AggregateExpr::accumulate(const Tuple &tuple)
{
  RC    rc = RC::SUCCESS;
  Value tmp;
  if (child_ == nullptr) {
    tuple.cell_at(0, tmp);  // 肯定是count_star.
    rc = aggregator_->accumulate(tmp);
    if (rc != RC::SUCCESS) {
      LOG_WARN("aggregator_->accumulate failed.");
    }
    return rc;
  }

  rc = child_->get_value(tuple, tmp);
  if (rc != RC::SUCCESS) {
    LOG_WARN("get_value failed.");
    return rc;
  }

  rc = aggregator_->accumulate(tmp);
  if (rc != RC::SUCCESS) {
    LOG_WARN("aggregator_->accumulate failed.");
  }
  return rc;
}

RC AggregateExpr::evaluate(Value &value) { return aggregator_->evaluate(value); }

RC AggregateExpr::get_value(const Tuple &tuple, Value &value) const
{
  return aggregator_->evaluate(value);
  // return tuple.find_cell(TupleCellSpec(name()), value);
}

RC AggregateExpr::type_from_string(const char *type_str, AggregateExpr::Type &type)
{
  RC rc = RC::SUCCESS;
  if (0 == strcasecmp(type_str, "count")) {
    type = Type::COUNT;
  } else if (0 == strcasecmp(type_str, "sum")) {
    type = Type::SUM;
  } else if (0 == strcasecmp(type_str, "avg")) {
    type = Type::AVG;
  } else if (0 == strcasecmp(type_str, "max")) {
    type = Type::MAX;
  } else if (0 == strcasecmp(type_str, "min")) {
    type = Type::MIN;
  } else {
    rc = RC::INVALID_ARGUMENT;
  }
  return rc;
}

// RC SubQuery_ValueList_Expression::open(Expression *expression) { return RC::SUCCESS; }

RC SubQuery_ValueList_Expression::open(Trx *trx)
{
  if (is_sub_query) {
    RC rc = sub_physical_operator_->open(trx);
    if (rc != RC::SUCCESS) {
      LOG_WARN("subquery open failed.");
      sub_physical_operator_->close();
      return rc;
    }
    return rc;
  }

  if (is_value_list) {
    value_list_iterator_ = value_list_.begin();
    return RC::SUCCESS;
  }
  return RC::UNSUPPORTED;
}

RC SubQuery_ValueList_Expression::close()
{
  if (is_sub_query) {
    return sub_physical_operator_->close();
  }

  if (is_value_list) {
    value_list_iterator_ = value_list_.end();
    return RC::SUCCESS;
  }

  return RC::UNSUPPORTED;
}

RC SubQuery_ValueList_Expression::next(const Tuple &tuple, Value &value) const { return get_value(tuple, value); }

RC SubQuery_ValueList_Expression::get_value(const Tuple &tuple, Value &value) const
{
  if (is_sub_query) {
    sub_physical_operator_->set_prev_tuple(&tuple);
    RC rc = sub_physical_operator_->next();

    if (rc == RC::RECORD_EOF) {  // 子查询如果为EOF，直接为空。
      value.set_type(AttrType::INTS);
      value.set_data((const char *)&INT_NULL, sizeof(INT_NULL));
      return rc;
    }

    if (rc != RC::SUCCESS) {
      LOG_WARN("sub_query get_value failed.");
      return rc;
    }
    return sub_physical_operator_->current_tuple()->cell_at(0, value);
  }

  if (is_value_list) {
    if (value_list_iterator_ == value_list_.end()) {
      return RC::RECORD_EOF;
    }
    RC rc = (*value_list_iterator_)->get_value(tuple, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("sub_query get_value failed.");
      return rc;
    }
    ++value_list_iterator_;
    return RC::SUCCESS;
  }

  return RC::UNSUPPORTED;
}

RC SubQuery_ValueList_Expression::try_get_value(Value &value) const
{
  LOG_WARN("subquery_valuelist_expression try get value failed.");
  return RC::UNIMPLEMENTED;
}

RC SubQuery_ValueList_Expression::create_stmt(Db *db, const std::unordered_map<std::string, Table *> &all_tables)
{
  if (is_value_list) {
    return RC::SUCCESS;
  }

  if (sub_stmt_ != nullptr) {
    return RC::SUCCESS;
  }

  Stmt *stmt = nullptr;
  RC    rc   = SelectStmt::create(db, *sub_sql_node_, stmt, all_tables);

  if (rc != RC::SUCCESS) {
    LOG_WARN("create select_stmt failed in subquery");
    return rc;
  }

  if (static_cast<SelectStmt *>(stmt)->query_expressions().size() != 1) {
    LOG_WARN("sub_query projection size != 1");
    return RC::VARIABLE_NOT_VALID;
  }

  sub_stmt_ = std::shared_ptr<SelectStmt>(static_cast<SelectStmt *>(stmt));
  return RC::SUCCESS;
}

RC SubQuery_ValueList_Expression::create_logical()
{
  if (is_value_list) {
    return RC::SUCCESS;
  }

  if (sub_logical_operator_ != nullptr) {
    return RC::SUCCESS;
  }

  LogicalPlanGenerator tmp_planner;
  RC                   rc = tmp_planner.create(sub_stmt_.get(), sub_logical_operator_);
  if (rc != RC::SUCCESS) {
    LOG_WARN("sub_query create failed");
    return rc;
  }

  return RC::SUCCESS;
}

RC SubQuery_ValueList_Expression::create_physical()
{
  if (is_value_list) {
    return RC::SUCCESS;
  }

  if (sub_physical_operator_ != nullptr) {
    return RC::SUCCESS;
  }

  PhysicalPlanGenerator tmp_planner;
  RC                    rc = tmp_planner.create(*sub_logical_operator_, sub_physical_operator_);

  if (rc != RC::SUCCESS) {
    LOG_WARN("sub_query create filed.");
    return rc;
  }

  return RC::SUCCESS;
}

void SubQuery_ValueList_Expression::set_prev_tuple(const Tuple *tuple)
{
  if (sub_physical_operator_ == nullptr) {
    return;
  }
  sub_physical_operator_->set_prev_tuple(tuple);
}

// RC SubQuery_ValueList_Expression::value_num(int32_t &num) const
// {
//   if (is_sub_query) {
//     // 考虑到只需要 0，1，2这三个数字，所以简单处理即可。
//     num   = 0;
//     RC rc = RC::SUCCESS;
//     // rc =  sub_physical_operator_->open(nullptr); // 只考虑当前的位置
//     if (rc != RC::SUCCESS) {
//       LOG_WARN("sub_query open failed");
//       return rc;
//     }

//     sub_physical_operator_->open(nullptr);
//     while ((rc = sub_physical_operator_->next()) == RC::SUCCESS && ++num < 2) {
//       ;
//     }

//     if (rc != RC::SUCCESS) {
//       if (rc != RC::RECORD_EOF) {
//         LOG_WARN("close failed.");
//         sub_physical_operator_->close();
//         return rc;
//       }
//       rc = RC::SUCCESS;
//     }

//     rc = sub_physical_operator_->close();
//     if (rc != RC::SUCCESS) {
//       LOG_WARN("close failed.");
//     }
//     return rc;
//   }

//   if (is_value_list) {
//     num = value_list_.size();
//     return RC::SUCCESS;
//   }

//   return RC::INVALID_ARGUMENT;
// }