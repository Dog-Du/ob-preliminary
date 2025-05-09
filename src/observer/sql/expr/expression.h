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

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "common/type/attr_type.h"
#include "common/value.h"

#include "storage/field/field.h"
#include "sql/expr/aggregator.h"
#include "storage/common/chunk.h"
#include "storage/field/field_meta.h"
#include "storage/table/table.h"
#include "sql/expr/vector_functioner.h"

class Tuple;
class LogicalOperator;
class PhysicalOperator;
class SelectStmt;

/**
 * @defgroup Expression
 * @brief 表达式
 */

/**
 * @brief 表达式类型
 * @ingroup Expression
 */
enum class ExprType
{
  NONE,
  STAR,                 ///< 星号，表示所有字段
  UNBOUND_FIELD,        ///< 未绑定的字段，需要在resolver阶段解析为FieldExpr
  UNBOUND_AGGREGATION,  ///< 未绑定的聚合函数，需要在resolver阶段解析为AggregateExpr

  FIELD,                  ///< 字段。在实际执行时，根据行数据内容提取对应字段的值
  VALUE,                  ///< 常量值
  CAST,                   ///< 需要做类型转换的表达式
  COMPARISON,             ///< 需要做比较的表达式
  CONJUNCTION,            ///< 多个表达式使用同一种关系(AND或OR)来联结
  ARITHMETIC,             ///< 算术运算
  SUBQUERY_OR_VALUELIST,  /// <  子查询 or 常量列表
  VECTOR_FUNCTION,
  AGGREGATION,  ///< 聚合运算
};

/**
 * @brief 表达式的抽象描述
 * @ingroup Expression
 * @details 在SQL的元素中，任何需要得出值的元素都可以使用表达式来描述
 * 比如获取某个字段的值、比较运算、类型转换
 * 当然还有一些当前没有实现的表达式，比如算术运算。
 *
 * 通常表达式的值，是在真实的算子运算过程中，拿到具体的tuple后
 * 才能计算出来真实的值。但是有些表达式可能就表示某一个固定的
 * 值，比如ValueExpr。
 *
 * TODO 区分unbound和bound的表达式
 */
class Expression
{
public:
  Expression() = default;
  Expression(const std::string &table_name, const std::string &field_name, const std::string &alias)
      : name_(), table_name_(table_name), field_name_(field_name), alias_(alias)
  {}
  virtual ~Expression() = default;

  /**
   * @brief 判断两个表达式是否相等
   */
  virtual bool equal(const Expression &other) const { return false; }
  /**
   * @brief
   * 根据具体的tuple，来计算当前表达式的值。tuple有可能是一个具体某个表的行数据
   */
  virtual RC get_value(const Tuple &tuple, Value &value) const = 0;

  /**
   * @brief
   * 在没有实际运行的情况下，也就是无法获取tuple的情况下，尝试获取表达式的值
   * @details 有些表达式的值是固定的，比如ValueExpr，这种情况下可以直接获取值
   */
  virtual RC try_get_value(Value &value) const { return RC::UNIMPLEMENTED; }

  /**
   * @brief 从 `chunk` 中获取表达式的计算结果 `column`
   */
  virtual RC get_column(Chunk &chunk, Column &column) { return RC::UNIMPLEMENTED; }

  // 使用匿名函数，传递进入作为参数，然后进行检查，或者获取某些信息的接口，提供遍历的操作。
  virtual void check_or_get(std::function<void(Expression *)> &worker_func) = 0;

  /**
   * @brief 表达式的类型
   * 可以根据表达式类型来转换为具体的子类
   */
  virtual ExprType type() const = 0;

  /**
   * @brief 表达式值的类型
   * @details 一个表达式运算出结果后，只有一个值
   */
  virtual AttrType value_type() const = 0;

  /**
   * @brief 表达式值的长度
   */
  virtual int value_length() const { return -1; }

  /**
   * @brief 表达式的名字，比如是字段名称，或者用户在执行SQL语句时输入的内容
   */
  virtual const char *name() const { return name_.c_str(); }
  virtual const char *alias() const { return alias_.c_str(); }
  virtual const char *table_name() const { return table_name_.c_str(); }
  virtual const char *field_name() const { return field_name_.c_str(); }

  virtual void set_name(std::string name) { name_ = name; }
  virtual void set_alias(std::string alias) { alias_ = alias; }
  virtual void set_field_name(std::string field_name) { field_name_ = field_name; }
  virtual void set_table_name(std::string table_name) { table_name_ = table_name; }

  /**
   * @brief 表达式在下层算子返回的 chunk 中的位置
   */
  virtual int  pos() const { return pos_; }
  virtual void set_pos(int pos) { pos_ = pos; }

  /**
   * @brief 用于 ComparisonExpr 获得比较结果 `select`。
   */
  virtual RC eval(Chunk &chunk, std::vector<uint8_t> &select) { return RC::UNIMPLEMENTED; }

protected:
  /**
   * @brief 表达式在下层算子返回的 chunk 中的位置
   * @details 当 pos_ = -1 时表示下层算子没有在返回的 chunk
   * 中计算出该表达式的计算结果， 当 pos_ >=
   * 0时表示在下层算子中已经计算出该表达式的值（比如聚合表达式），且该表达式对应的结果位于
   * chunk 中 下标为 pos_ 的列中。
   */
  int pos_ = -1;

private:
  std::string name_;
  std::string table_name_;
  std::string field_name_;
  std::string alias_; 
};

class StarExpr : public Expression
{
public:
  StarExpr() : table_name_() {}
  StarExpr(const char *table_name) : table_name_(table_name) {}
  virtual ~StarExpr() = default;

  ExprType type() const override { return ExprType::STAR; }
  AttrType value_type() const override { return AttrType::UNDEFINED; }

  RC get_value(const Tuple &tuple, Value &value) const override { return RC::UNIMPLEMENTED; }  // 不需要实现

  const char *table_name() const override { return table_name_.c_str(); }
  void        check_or_get(std::function<void(Expression *)> &worker_func) override { worker_func(this); }

private:
  std::string table_name_;
};

class UnboundFieldExpr : public Expression
{
public:
  UnboundFieldExpr(const std::string &table_name, const std::string &field_name, const std::string &alias)
      : table_name_(table_name), field_name_(field_name), alias_(alias)
  {}

  virtual ~UnboundFieldExpr() = default;

  ExprType type() const override { return ExprType::UNBOUND_FIELD; }
  AttrType value_type() const override { return AttrType::UNDEFINED; }

  RC get_value(const Tuple &tuple, Value &value) const override { return RC::INTERNAL; }

  const char *table_name() const override { return table_name_.c_str(); }
  const char *field_name() const override { return field_name_.c_str(); }
  void        check_or_get(std::function<void(Expression *)> &worker_func) override { worker_func(this); }

private:
  std::string table_name_;
  std::string field_name_;
  std::string alias_;
};

/**
 * @brief 字段表达式
 * @ingroup Expression
 */
class FieldExpr : public Expression
{
public:
  FieldExpr() = default;
  FieldExpr(std::string table_name, std::string field_name, std::string alias)
      : Expression(table_name, field_name, alias)
  {}

  FieldExpr(const Table *table, const FieldMeta *field) : field_(table, field)
  {
    set_field_name(field_.field_name());
    set_table_name(field_.table_name());
    // set_alias("");
  }

  FieldExpr(const Field &field) : field_(field) {}

  virtual ~FieldExpr() = default;

  bool equal(const Expression &other) const override;

  ExprType type() const override { return ExprType::FIELD; }
  AttrType value_type() const override { return field_.attr_type(); }
  int      value_length() const override { return field_.meta()->len(); }

  Field &field() { return field_; }

  const Field &field() const { return field_; }

  const char      *alias() const override { return Expression::alias(); }
  const char      *table_name() const override { return Expression::table_name(); }
  const char      *field_name() const override { return Expression::field_name(); }
  const FieldMeta *get_field_meta() const { return field_.meta(); }
  RC               get_column(Chunk &chunk, Column &column) override;

  RC check_field(const std::unordered_map<std::string, Table *> &all_tables, Table *default_table,
      const std::vector<Table *> &tables, const std::unordered_map<std::string, std::string> &alias_map);

  RC   get_value(const Tuple &tuple, Value &value) const override;
  void check_or_get(std::function<void(Expression *)> &worker_func) override { worker_func(this); }

private:
  Field field_;
};

/**
 * @brief 常量值表达式
 * @ingroup Expression
 */
class ValueExpr : public Expression
{
public:
  ValueExpr() = default;
  explicit ValueExpr(const Value &value) : value_(value) {}

  virtual ~ValueExpr() = default;

  bool equal(const Expression &other) const override;

  RC get_value(const Tuple &tuple, Value &value) const override;
  RC get_column(Chunk &chunk, Column &column) override;
  RC try_get_value(Value &value) const override
  {
    value = value_;
    return RC::SUCCESS;
  }

  ExprType type() const override { return ExprType::VALUE; }
  AttrType value_type() const override { return value_.attr_type(); }
  int      value_length() const override { return value_.length(); }

  void         get_value(Value &value) const { value = value_; }
  const Value &get_value() const { return value_; }
  void         check_or_get(std::function<void(Expression *)> &worker_func) override { worker_func(this); }

private:
  Value value_;
};

/**
 * @brief 类型转换表达式
 * @ingroup Expression
 */
class CastExpr : public Expression
{
public:
  CastExpr(std::shared_ptr<Expression> child, AttrType cast_type);
  virtual ~CastExpr();

  ExprType type() const override { return ExprType::CAST; }

  RC get_value(const Tuple &tuple, Value &value) const override;

  RC try_get_value(Value &value) const override;

  AttrType value_type() const override { return cast_type_; }

  std::shared_ptr<Expression> &child() { return child_; }

  void check_or_get(std::function<void(Expression *)> &worker_func) override { worker_func(this); }

private:
  RC cast(const Value &value, Value &cast_value) const;

private:
  std::shared_ptr<Expression> child_;      ///< 从这个表达式转换
  AttrType                    cast_type_;  ///< 想要转换成这个类型
};

/**
 * @brief 比较表达式
 * @ingroup Expression
 */
class ComparisonExpr : public Expression
{
public:
  ComparisonExpr(CompOp comp, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
  virtual ~ComparisonExpr();

  ExprType type() const override { return ExprType::COMPARISON; }
  RC       get_value(const Tuple &tuple, Value &value) const override;
  AttrType value_type() const override { return AttrType::BOOLEANS; }
  CompOp   comp() const { return comp_; }

  // 因为

  static RC check_comparison_with_subquery(Expression *expression, bool need_check_complex_subquery);
  /**
   * @brief 根据 ComparisonExpr 获得 `select` 结果。
   * select 的长度与chunk 的行数相同，表示每一行在ComparisonExpr
   * 计算后是否会被输出。
   */
  RC eval(Chunk &chunk, std::vector<uint8_t> &select) override;

  std::shared_ptr<Expression> &left() { return left_; }
  std::shared_ptr<Expression> &right() { return right_; }

  void check_or_get(std::function<void(Expression *)> &worker_func) override
  {
    if (left_ != nullptr) {
      worker_func(left_.get());
    }
    if (right_ != nullptr) {
      worker_func(right_.get());
    }
    worker_func(this);
  }
  /**
   * 尝试在没有tuple的情况下获取当前表达式的值
   * 在优化的时候，可能会使用到
   */
  RC try_get_value(Value &value) const override;

  /**
   * compare the two tuple cells
   * @param value the result of comparison
   */
  RC compare_value(const Value &left, const Value &right, bool &value) const;

  template <typename T>
  RC compare_column(const Column &left, const Column &right, std::vector<uint8_t> &result) const;

private:
  CompOp                      comp_;
  std::shared_ptr<Expression> left_;
  std::shared_ptr<Expression> right_;
};

/**
 * @brief 联结表达式
 * @ingroup Expression
 * 多个表达式使用同一种关系(AND或OR)来联结
 * 当前miniob仅有AND操作
 */
class ConjunctionExpr : public Expression
{
public:
  enum class Type
  {
    AND,
    OR,
  };

public:
  ConjunctionExpr(Type type, std::vector<std::shared_ptr<Expression>> &children);
  virtual ~ConjunctionExpr() = default;

  ExprType type() const override { return ExprType::CONJUNCTION; }
  AttrType value_type() const override { return AttrType::BOOLEANS; }
  RC       get_value(const Tuple &tuple, Value &value) const override;

  Type conjunction_type() const { return conjunction_type_; }

  void check_or_get(std::function<void(Expression *)> &worker_func) override
  {
    for (auto &expr : children_) {
      worker_func(expr.get());
    }
    worker_func(this);
  }
  std::vector<std::shared_ptr<Expression>> &children() { return children_; }

private:
  Type                                     conjunction_type_;
  std::vector<std::shared_ptr<Expression>> children_;
};

/**
 * @brief 算术表达式
 * @ingroup Expression
 */
class ArithmeticExpr : public Expression
{
public:
  enum class Type
  {
    ADD,
    SUB,
    MUL,
    DIV,
    NEGATIVE,
  };

public:
  ArithmeticExpr(Type type, Expression *left, Expression *right);
  ArithmeticExpr(Type type, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
  virtual ~ArithmeticExpr() = default;

  bool     equal(const Expression &other) const override;
  ExprType type() const override { return ExprType::ARITHMETIC; }

  AttrType value_type() const override;
  int      value_length() const override
  {
    if (!right_) {
      return left_->value_length();
    }
    return 4;  // sizeof(float) or sizeof(int)
  };

  RC get_value(const Tuple &tuple, Value &value) const override;

  RC get_column(Chunk &chunk, Column &column) override;

  RC try_get_value(Value &value) const override;

  Type arithmetic_type() const { return arithmetic_type_; }

  void check_or_get(std::function<void(Expression *)> &worker_func) override
  {
    if (left_ != nullptr) {
      worker_func(left_.get());
    }
    if (right_ != nullptr) {
      worker_func(right_.get());
    }
    worker_func(this);
  }

  std::shared_ptr<Expression> &left() { return left_; }
  std::shared_ptr<Expression> &right() { return right_; }

private:
  RC calc_value(const Value &left_value, const Value &right_value, Value &value) const;

  RC calc_column(const Column &left_column, const Column &right_column, Column &column) const;

  template <bool LEFT_CONSTANT, bool RIGHT_CONSTANT>
  RC execute_calc(const Column &left, const Column &right, Column &result, Type type, AttrType attr_type) const;

private:
  Type                        arithmetic_type_;
  std::shared_ptr<Expression> left_;
  std::shared_ptr<Expression> right_;
};

// 未绑定的聚合表达式。
class UnboundAggregateExpr : public Expression
{
public:
  UnboundAggregateExpr(const char *aggregate_name, Expression *child, const char *alias);
  virtual ~UnboundAggregateExpr() = default;

  ExprType type() const override { return ExprType::UNBOUND_AGGREGATION; }

  const char *aggregate_name() const { return aggregate_name_.c_str(); }

  std::shared_ptr<Expression> &child() { return child_; }

  void check_or_get(std::function<void(Expression *)> &worker_func) override
  {
    if (child_ != nullptr) {
      worker_func(child_.get());
    }
    worker_func(this);
  }
  RC       get_value(const Tuple &tuple, Value &value) const override { return RC::INTERNAL; }
  AttrType value_type() const override { return child_->value_type(); }

private:
  std::string                 aggregate_name_;
  std::string                 alias_;
  std::shared_ptr<Expression> child_;
};

// 聚合函数中不能有聚合函数，聚合函数必然在最顶端，这样就好处理一些。
class AggregateExpr : public Expression
{
public:
  enum class Type
  {
    COUNT,
    COUNT_STAR,
    SUM,
    AVG,
    MAX,
    MIN,
  };

public:
  AggregateExpr(Type type, Expression *child);
  AggregateExpr(Type type, std::shared_ptr<Expression> child);
  virtual ~AggregateExpr() = default;

  bool equal(const Expression &other) const override;

  ExprType type() const override { return ExprType::AGGREGATION; }

  AttrType value_type() const override
  {
    if (aggregate_type_ == Type::COUNT_STAR || aggregate_type_ == Type::COUNT) {
      return AttrType::INTS;
    }

    if (aggregate_type_ == Type::AVG) {
      return AttrType::FLOATS;
    }
    return child_->value_type();
  }

  int value_length() const override
  {
    if (aggregate_type_ == Type::COUNT_STAR || aggregate_type_ == Type::COUNT) {
      return 4;
    }

    if (aggregate_type_ == Type::AVG) {
      return 4;
    }
    return child_->value_length();
  }

  RC get_value(const Tuple &tuple, Value &value) const override;

  RC accumulate(const Tuple &tuple);

  RC evaluate(Value &value);

  RC get_column(Chunk &chunk, Column &column) override;

  void reset() { aggregator_->reset(); }

  Type aggregate_type() const { return aggregate_type_; }

  std::shared_ptr<Expression> &child() { return child_; }

  const std::shared_ptr<Expression> &child() const { return child_; }
  void                               check_or_get(std::function<void(Expression *)> &worker_func) override
  {
    if (child_ != nullptr) {
      worker_func(child_.get());
    }
    worker_func(this);
  }

  RC create_aggregator();

public:
  static RC type_from_string(const char *type_str, Type &type);

private:
  Type                        aggregate_type_;
  std::shared_ptr<Aggregator> aggregator_;
  std::shared_ptr<Expression> child_;
};

// 子查询和常量列表用一个算子处理，
class SubQuery_ValueList_Expression : public Expression
{
public:
  SubQuery_ValueList_Expression(const std::vector<std::shared_ptr<Expression>> &value_list)
      : is_sub_query(false), is_value_list(true), value_list_(value_list)
  {
    ASSERT(is_value_list ^ is_sub_query,"should not both false or both true");
  }

  SubQuery_ValueList_Expression(std::shared_ptr<SelectSqlNode> sql_node)
      : is_sub_query(true), is_value_list(false), sub_sql_node_(sql_node)
  {
    ASSERT(is_value_list ^ is_sub_query,"should not both false or both true");
  }

  RC open(Trx *trx);  // 打开，进行简单的处理，并进行一些检查。

  RC close();  // 如果是value_list则不进行任何处理应该。

  // get_value == next();
  RC get_value(const Tuple &tuple, Value &value) const override;
  RC next(const Tuple &tuple, Value &value) const;

  RC try_get_value(Value &value) const override;

  ExprType type() const override
  {
    ASSERT(is_value_list ^ is_sub_query,"should not both false or both true");
    return ExprType::SUBQUERY_OR_VALUELIST;
  }

  void set_prev_tuple(const Tuple *tuple);

  AttrType value_type() const override
  {
    return AttrType::UNDEFINED;  // 类型是不规则的。
  }

  void check_or_get(std::function<void(Expression *)> &worker_func) override
  {
    // 好好好，给这等着我呢
    // if (is_sub_query) {
    //   return;
    // }

    for (auto &child : value_list_) {
      worker_func(child.get());
    }
    worker_func(this);
  }

  // 从当前位置到最后的个数。
  // RC value_num(int32_t &num) const;

  RC create_stmt(Db *db, const std::unordered_map<std::string, Table *> &all_tables);
  RC create_logical();
  RC create_physical();

private:
  bool                                                       is_sub_query  = false;
  bool                                                       is_value_list = false;
  std::vector<std::shared_ptr<Expression>>                   value_list_;
  mutable std::vector<std::shared_ptr<Expression>>::iterator value_list_iterator_;
  std::shared_ptr<SelectSqlNode>                             sub_sql_node_;
  std::shared_ptr<SelectStmt>                                sub_stmt_;
  std::shared_ptr<LogicalOperator>                           sub_logical_operator_;
  std::shared_ptr<PhysicalOperator>                          sub_physical_operator_;
};

class VectorFunctionExpr : public Expression
{
public:
  enum class Type
  {
    L2_DISTANCE = 0,
    COSINE_DISTANCE,
    INNER_PRODUCT,
  };

  VectorFunctionExpr(Expression *left, Expression *right, Type type) : left_(left), right_(right), type_(type)
  {
    function_worker_ = create_function_worker(type_);
  }

  VectorFunctionExpr(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right, Type type)
      : left_(left), right_(right), type_(type)
  {
    function_worker_ = create_function_worker(type_);
  }

  static std::shared_ptr<FunctionWorker> create_function_worker(Type type);

  RC get_value(const Tuple &tuple, Value &value) const override;

  RC try_get_value(Value &value) const override { return RC::UNIMPLEMENTED; }

  AttrType value_type() const override { return AttrType::FLOATS; }

  ExprType type() const override { return ExprType::VECTOR_FUNCTION; }

  void check_or_get(std::function<void(Expression *)> &worker_func) override
  {
    if (left_ != nullptr) {
      worker_func(left_.get());
    }
    if (right_ != nullptr) {
      worker_func(right_.get());
    }
    worker_func(this);
  }

  std::shared_ptr<Expression> &left() { return left_; }
  std::shared_ptr<Expression> &right() { return right_; }
  Type                         function_type() { return type_; }

private:
  std::shared_ptr<Expression>     left_;
  std::shared_ptr<Expression>     right_;
  Type                            type_;
  std::shared_ptr<FunctionWorker> function_worker_;
};