
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

// 没有用。
struct DoNothingDeleter {
  template <typename T>
  void operator() (T*) const {}
};

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  LOG_WARN("parse error, msg : %s",msg);
  std::shared_ptr<ParsedSqlNode> error_sql_node = std::make_shared<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

// 检查聚合的位置是否合法，不合法则出错。
#define assert_is_not_aggregate(expression)                                              \
  if (expression->type() == ExprType::AGGREGATION) {                                     \
    yyerror (&yylloc, sql_string, sql_result, scanner, "here should not be aggregate."); \
  }                                                                                      \


bool try_expression_to_value(Expression *expression, Value &value)
{
  switch (expression->type()) {
    case ExprType::VALUE: {
      value = static_cast<ValueExpr *>(expression)->get_value();
      return true;
    } break;
    case ExprType::ARITHMETIC: {
      auto *expr = static_cast<ArithmeticExpr *>(expression);
      // value = expr->get_value(); 这里没有tuple，不能这么写。
      // 可能是单一个expr，
      if (expr->arithmetic_type() != ArithmeticExpr::Type::NEGATIVE && expr->left()->type() != ExprType::VALUE) {
        return false;
      }

      if (static_cast<ValueExpr *>(expr->left().get())->try_get_value(value) != RC::SUCCESS) {
        LOG_WARN("ArithmeticExpr can not cast to value.");
        return false;
      }
      Value tmp_value;
      tmp_value.set_type(value.attr_type());
      if (Value::negative(value, tmp_value) != RC::SUCCESS) {
        return false;
      }

      value = tmp_value;
      return true;
    } break;
    default : {
      return false;
    } break;
  }
  return false;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

UnboundAggregateExpr *create_aggregate_expression(const char *aggregate_name,
                                           Expression *child,
                                           const char *alias,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  UnboundAggregateExpr *expr = new UnboundAggregateExpr(aggregate_name, child, alias);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

%}

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

//标识tokens
%token  SEMICOLON
        BY
        CREATE
        ORDER_T
        HAVING_T
        LIKE_T
        COUNT_T
        ASC_T
        DESC_T
        SUM_T
        MIN_T
        MAX_T
        AVG_T
        NOT_T
        TEXT_T
        JOIN_T
        EXISTS_T
        IN_T
        UNIQUE_T
        AS_T
        NULL_T
        IS_T
        INNER_T
        DROP
        GROUP
        TABLE
        TABLES
        INDEX
        CALC
        SELECT
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        DATE_T
        STRING_T
        FLOAT_T
        VECTOR_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
        EXPLAIN
        STORAGE
        FORMAT
        EQ
        LT
        GT
        LE
        GE
        NE

/** union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据 **/
%union {
  ParsedSqlNode *                            sql_node;
  Value *                                    value;
  enum CompOp                                comp;
  RelAttrSqlNode *                           rel_attr;
  std::vector<AttrInfoSqlNode> *             attr_infos;
  AttrInfoSqlNode *                          attr_info;
  Expression *                               expression;
  std::vector<std::shared_ptr<Expression>> * expression_list;
  std::vector<Value> *                       value_list;
  std::vector<std::vector<Value>> *          insert_value_list;
  std::vector<RelAttrSqlNode> *              rel_attr_list;
  std::vector<std::string> *                 relation_list;
  std::vector<std::string> *                 index_attr_list;
  JoinSqlNode *                              join_node;
  std::vector<JoinSqlNode> *                 join_list;
  char *                                     string;
  UpdateNode *                               update_node;
  std::vector<UpdateNode> *                  update_node_list;
  int                                        number;
  float                                      floats;
  bool                                       boolean;
  OrderByType                                order_by_type;
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
%token <string> SSS
//非终结符

/** type 定义了各种解析后的结果输出的是什么类型。类型对应了 union 中的定义的成员变量名称 **/
%type <join_node>           join_node
%type <join_node>           from_node
%type <join_list>           from_node_list
%type <join_node>           join_list
%type <number>              type
/* %type <string>              alias_name */
%type <expression>          condition_list
%type <index_attr_list>     index_attr_list
%type <insert_value_list>   insert_value_list
%type <value_list>          insert_value
%type <value>               value
%type <update_node>         update_node
%type <update_node_list>    update_node_list
%type <number>              number
%type <string>              alias_name
/* %type <order_by_type>       order_by_type */
%type <rel_attr>            relation
%type <comp>                comp_op
%type <rel_attr>            rel_attr
%type <attr_infos>          attr_def_list
%type <attr_info>           attr_def
%type <value_list>          value_list
%type <expression>          where
%type <string>              storage_format
%type <boolean>             attr_def_nullable
/* %type <rel_attr_list>       rel_list */
%type <expression>          expression
%type <expression_list>     expression_list
%type <rel_attr_list>       group_by
%type <rel_attr_list>       order_by
%type <rel_attr_list>       rel_attr_list
%type <expression>          having
%type <sql_node>            calc_stmt
%type <sql_node>            select_stmt
%type <sql_node>            insert_stmt
%type <sql_node>            update_stmt
%type <sql_node>            delete_stmt
%type <sql_node>            create_table_stmt
%type <sql_node>            drop_table_stmt
%type <sql_node>            show_tables_stmt
%type <sql_node>            desc_table_stmt
%type <sql_node>            create_index_stmt
%type <sql_node>            drop_index_stmt
%type <sql_node>            sync_stmt
%type <sql_node>            begin_stmt
%type <sql_node>            commit_stmt
%type <sql_node>            rollback_stmt
%type <sql_node>            load_data_stmt
%type <sql_node>            explain_stmt
%type <sql_node>            set_variable_stmt
%type <sql_node>            help_stmt
%type <sql_node>            exit_stmt
%type <sql_node>            command_wrapper
// commands should be a list but I use a single command instead
%type <sql_node>            commands

// 声明结合的优先级 ：
// 比如 ： select * from t where a=2 and a=3 or a=4;
// 符合要求的是 4，即： select * from t where (a=2 and a=3) or a=4;
// 而非： select * from t where a=2 and (a=3 or a=4)
%left AND OR NE GE LE GT LT EQ
%left '+' '-'
%left '*' '/'

%nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon  //commands or sqls. parser starts here.
  {
    std::shared_ptr<ParsedSqlNode> sql_node = std::shared_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calc_stmt
  | select_stmt
  | insert_stmt
  | update_stmt
  | delete_stmt
  | create_table_stmt
  | drop_table_stmt
  | show_tables_stmt
  | desc_table_stmt
  | create_index_stmt
  | drop_index_stmt
  | sync_stmt
  | begin_stmt
  | commit_stmt
  | rollback_stmt
  | load_data_stmt
  | explain_stmt
  | set_variable_stmt
  | help_stmt
  | exit_stmt
    ;

exit_stmt:
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SCF_EXIT);
    };

help_stmt:
    HELP {
      $$ = new ParsedSqlNode(SCF_HELP);
    };

sync_stmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

begin_stmt:
    TRX_BEGIN  {
      $$ = new ParsedSqlNode(SCF_BEGIN);
    }
    ;

commit_stmt:
    TRX_COMMIT {
      $$ = new ParsedSqlNode(SCF_COMMIT);
    }
    ;

rollback_stmt:
    TRX_ROLLBACK  {
      $$ = new ParsedSqlNode(SCF_ROLLBACK);
    }
    ;

drop_table_stmt:    /*drop table 语句的语法解析树*/
    DROP TABLE ID {
      $$ = new ParsedSqlNode(SCF_DROP_TABLE);
      $$->drop_table.relation_name = $3;
      free($3);
    };

show_tables_stmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
    ;

desc_table_stmt:
    DESC_T ID  {
      $$ = new ParsedSqlNode(SCF_DESC_TABLE);
      $$->desc_table.relation_name = $2;
      free($2);
    }
    ;

create_index_stmt:    /*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE ID index_attr_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      create_index.index_name = $3;
      create_index.relation_name = $5;

      if ($8 != nullptr) {
        create_index.attr_names = *$8;
        delete $8;
      }

      create_index.unique = false;
      create_index.attr_names.push_back(std::string($7));
      std::reverse(create_index.attr_names.begin(), create_index.attr_names.end());
      free($3);
      free($5);
      free($7);
    }
    | CREATE UNIQUE_T INDEX ID ON ID LBRACE ID index_attr_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      create_index.index_name = $4;
      create_index.relation_name = $6;

      if ($9 != nullptr) {
        create_index.attr_names = *$9;
        delete $9;
      }

      create_index.unique = true;
      create_index.attr_names.push_back(std::string($8));
      std::reverse(create_index.attr_names.begin(), create_index.attr_names.end());
      free($4);
      free($6);
      free($8);
    }
    ;

index_attr_list:
  /* empty */
  {
    $$ = nullptr;
  }
  | COMMA ID index_attr_list
  {
    if ($3 != nullptr) {
      $$ = $3;
    } else {
      $$ = new std::vector<std::string>();
    }
    $$->emplace_back(std::string($2));
    free($2);
  }
  ;

drop_index_stmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(SCF_DROP_INDEX);
      $$->drop_index.index_name = $3;
      $$->drop_index.relation_name = $5;
      free($3);
      free($5);
    }
    ;

create_table_stmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      free($3);

      std::vector<AttrInfoSqlNode> *src_attrs = $6;

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*$5);
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete $5;
      if ($8 != nullptr) {
        create_table.storage_format = $8;
        free($8);
      }
    }
    | CREATE TABLE ID AS_T select_stmt
    {
      $$ = $5;
      $$->flag = SCF_CREATE_TABLE;
      $$->create_table.relation_name = $3;
      free($3);
    }
    | CREATE TABLE ID select_stmt
    {
      $$ = $4;
      $$->flag = SCF_CREATE_TABLE;
      $$->create_table.relation_name = $3;
      free($3);
    }
    | CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE AS_T select_stmt
    {
      $$ = $9;
      $$->flag = SCF_CREATE_TABLE;
      $$->create_table.relation_name = $3;
      free($3);

      if ($6 != nullptr) {
        $$->create_table.attr_infos.swap(*$6);
        delete $6;
      }
      $$->create_table.attr_infos.emplace_back(*$5);
      std::reverse($$->create_table.attr_infos.begin(), $$->create_table.attr_infos.end());
      delete $5;
    }
    | CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE select_stmt
    {
      $$ = $8;
      $$->flag = SCF_CREATE_TABLE;
      $$->create_table.relation_name = $3;
      free($3);

      if ($6 != nullptr) {
        $$->create_table.attr_infos.swap(*$6);
        delete $6;
      }
      $$->create_table.attr_infos.emplace_back(*$5);
      std::reverse($$->create_table.attr_infos.begin(), $$->create_table.attr_infos.end());
      delete $5;
    }
    ;

attr_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA attr_def attr_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<AttrInfoSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

attr_def:
    ID type LBRACE number RBRACE attr_def_nullable
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->nullable = $6;
      free($1);
    }
    | ID type attr_def_nullable
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = 4;
      $$->nullable = $3;
      free($1);
    }
    | ID TEXT_T attr_def_nullable
    {
      $$ = new AttrInfoSqlNode;
      $$->type = AttrType::CHARS;
      $$->name = $1;
      $$->length = TEXT_LENGTH - 1;
      $$->nullable = $3;
      free($1);
    }
    ;

attr_def_nullable:
  /* empty */ {
    $$ = true;
  }
  | NULL_T {
    $$ = true;
  }
  | NOT_T NULL_T {
    $$ = false;
  }
  ;

number:
    NUMBER {$$ = $1;}
    ;

type:
    INT_T      { $$ = static_cast<int>(AttrType::INTS); }
    | STRING_T { $$ = static_cast<int>(AttrType::CHARS); }
    | FLOAT_T  { $$ = static_cast<int>(AttrType::FLOATS); }
    | VECTOR_T { $$ = static_cast<int>(AttrType::VECTORS); }
    | DATE_T { $$ = static_cast<int>(AttrType::DATES); }
    ;

insert_stmt:        /*insert   语句的语法解析树*/
    INSERT INTO ID VALUES insert_value insert_value_list
    {
      $$ = new ParsedSqlNode(SCF_INSERT);
      $$->insertion.relation_name = $3;
      if ($6 != nullptr) {
        $$->insertion.values.swap(*$6);
        delete $6;
      }
      $$->insertion.values.emplace_back(*$5);
      std::reverse($$->insertion.values.begin(), $$->insertion.values.end());
      delete $5;
      free($3);
    }
    ;

insert_value_list:
  /* empty */
  {
    $$ = nullptr;
  }
  | COMMA insert_value insert_value_list
  {
    if ($3 != nullptr) {
      $$ = $3;
    } else {
      $$ = new std::vector<std::vector<Value>>;
    }

    $$->emplace_back(*$2);
    delete $2;
  }

insert_value:
  LBRACE expression value_list RBRACE
  {
    if ($3 != nullptr) {
      $$ = $3;
    } else {
      $$ = new std::vector<Value>;
    }
    Value tmp_value;

    if (try_expression_to_value($2, tmp_value) == false) {
      yyerror (&yylloc, sql_string, sql_result, scanner, "try_expression_to_value failed in value_list");
    }

    $$->emplace_back(tmp_value);
    std::reverse($$->begin(), $$->end());
    delete $2;
  }
  ;

value_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA expression value_list  {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Value>;
      }
      Value tmp_value;

      if (try_expression_to_value($2, tmp_value) == false) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "try_expression_to_value failed in value_list");
      }

      $$->emplace_back(tmp_value);
      delete $2;
    }
    ;

value:
    NUMBER {
      $$ = new Value((int)$1);
      @$ = @1;
    }
    |FLOAT {
      $$ = new Value((float)$1);
      @$ = @1;
    }
    |SSS {
      int len = strlen($1) - 2;
      if (len >= TEXT_LENGTH) {
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsg);
      }

      char *tmp = common::substr($1,1,len);
      $$ = new Value();
      $$->set_type(AttrType::CHARS);
      $$->set_data(tmp, len);
      free(tmp);
      free($1);
    }
    |NULL_T {
      $$ = new Value(INT_NULL);
      @$ = @1;
    }
    ;

storage_format:
    /* empty */
    {
      $$ = nullptr;
    }
    | STORAGE FORMAT EQ ID
    {
      $$ = $4;
    }
    ;

delete_stmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID where
    {
      $$ = new ParsedSqlNode(SCF_DELETE);
      $$->deletion.relation_name = $3;

      $$->deletion.conditions.reset($4);
      free($3);
    }
    ;

update_stmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET update_node update_node_list where
    {
      $$ = new ParsedSqlNode(SCF_UPDATE);
      if ($5 != nullptr) {
        $$->update.update_list.swap(*$5);
        delete $5;
      }

      $$->update.conditions.reset($6);
      $$->update.rel_name = $2;
      $$->update.update_list.push_back(*$4);
      std::reverse($$->update.update_list.begin(), $$->update.update_list.end());

      free($2);
      delete $4;
    }
    ;

update_node_list:
  /* empty */
  {
    $$ = nullptr;
  }
  | COMMA update_node update_node_list
  {
    if ($3 != nullptr) {
      $$ = $3;
    } else {
      $$ = new std::vector<UpdateNode>();
    }
    $$->push_back(*$2);
    delete $2;
  }
  ;

update_node:
  rel_attr EQ expression
  {
    $$ = new UpdateNode();
    $$->rel_attr = *$1;
    $$->expression.reset($3);
    delete $1;
  }
  ;

select_stmt:        /*  select 语句的语法解析树*/
    SELECT expression_list FROM from_node from_node_list where group_by having order_by
    {
      $$ = new ParsedSqlNode(SCF_SELECT);
      if ($2 != nullptr) {
        $$->selection.expressions.swap(*$2);
        delete $2;
      }

      if ($5 != nullptr) {
        $$->selection.relations.swap(*$5);
        delete $5;
      }

      $$->selection.relations.emplace_back(*$4);
      std::reverse($$->selection.relations.begin(), $$->selection.relations.end());
      delete $4;

      $$->selection.conditions.reset($6);

      if ($7 != nullptr) {
        $$->selection.group_by.swap(*$7);
        delete $7;
      }

      if ($8 != nullptr) {
        // 这里不能delete，而是把所有权交给了having
        $$->selection.having = std::shared_ptr<Expression>($8);
        // delete $8;
      }

      if ($9 != nullptr) {
        $$->selection.order_by = *$9;
        delete $9;
      }
    }
    ;

order_by:
  /* empty */
  {
    $$ = nullptr;
  }
  | ORDER_T BY rel_attr rel_attr_list
  {
    if ($4 != nullptr) {
      $$ = $4;
    } else {
      $$ = new std::vector<RelAttrSqlNode>();
    }
    $$->push_back(*$3);
    delete $3;
    std::reverse($$->begin(), $$->end());
  }
  ;

having:
  /* empty */
  {
    $$ = nullptr;
  }
  | HAVING_T condition_list
  {
    $$ = $2;
  }
  ;

group_by:
    /* empty */
    {
      $$ = nullptr;
    }
    | GROUP BY rel_attr rel_attr_list {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrSqlNode>();
      }

      $$->push_back(*$3);
      delete $3;
      std::reverse($$->begin(), $$->end());
    }
    ;

rel_attr_list:
  /* empty */
  {
    $$ = nullptr;
  }
  | COMMA rel_attr rel_attr_list
  {
    if ($3 == nullptr) {
      $$ = new std::vector<RelAttrSqlNode>();
    } else {
      $$ = $3;
    }
    $$->push_back(*$2);
    delete $2;
  }
  ;

from_node:
  join_node {
    $$ = $1;
  }
  ;

from_node_list:
  /* empty */ {
    $$ = nullptr;
  }
  | COMMA join_node from_node_list {
    if ($3 != nullptr ) {
      $$ = $3;
    } else {
      $$ = new std::vector<JoinSqlNode>;
    }
    $$->emplace_back(*$2);
    delete $2;
  }
  ;

join_node:
  relation join_list {
    if ($2 != nullptr) {
      $$ = $2;
    } else {
      $$ =  new JoinSqlNode;
    }

    std::swap($$->first_rel, *$1);
    std::reverse($$->join_rel_list.begin(), $$->join_rel_list.end());
    std::reverse($$->conditions.begin(), $$->conditions.end());
    delete $1;
  }
  ;

join_list:
  /* empty */ {
    $$ = nullptr;
  }
  | INNER_T JOIN_T relation ON condition_list join_list {
    if ($6 != nullptr) {
      $$ = $6;
    } else {
      $$ = new JoinSqlNode;
    }
    $$->join_rel_list.emplace_back(*$3);
    $$->conditions.emplace_back($5);
    delete $3;
  }
  ;

calc_stmt:
    CALC expression_list
    {
      $$ = new ParsedSqlNode(SCF_CALC);
      $$->calc.expressions.swap(*$2);
      delete $2;
    }
    ;

alias_name:
  /* empty */
  {
    $$ = nullptr;
  }
  | ID
  {
    $$ = $1;
  }
  | AS_T ID
  {
    $$ = $2;
  }
  ;

expression_list:
    expression alias_name
    {
      $$ = new std::vector<std::shared_ptr<Expression>>;

      if ($2 != nullptr) {
        $1->set_name($2);
        free($2);
      }
      $$->emplace_back($1);
    }
    | expression alias_name COMMA expression_list
    {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<std::shared_ptr<Expression>>;
      }

      if ($2 != nullptr) {
        $1->set_name($2);
        free($2);
      }
      $$->emplace($$->begin(), $1);
    }
    ;

expression:
    expression '+' expression {
      // assert_is_not_aggregate($1);
      // assert_is_not_aggregate($3);
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::ADD, $1, $3, sql_string, &@$);
    }
    | expression '-' expression {
      // assert_is_not_aggregate($1);
      // assert_is_not_aggregate($3);
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::SUB, $1, $3, sql_string, &@$);
    }
    | expression '*' expression {
      // // assert_is_not_aggregate($1);
      // // assert_is_not_aggregate($3);
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::MUL, $1, $3, sql_string, &@$);
    }
    | expression '/' expression {
      // // assert_is_not_aggregate($1);
      // // assert_is_not_aggregate($3);
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::DIV, $1, $3, sql_string, &@$);
    }
    | LBRACE expression_list RBRACE {
      // for (auto &x : *$2) {
      //  // assert_is_not_aggregate(x.get());
      // }

      if ($2->size() == 1) {
        $$ = $2->front().get();
        // delete $2;
        // TODO: 这里没有delete，会造成内存泄漏，但是懒得整了。
        // 已解决，构造释放函数。-> 好吧，没解决
      } else {
        $$ = new SubQuery_ValueList_Expression(*$2);
        delete $2;
      }
      $$->set_name(token_name(sql_string, &@$));
    }
    | '-' expression %prec UMINUS {
      // // assert_is_not_aggregate($2);
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, $2, nullptr, sql_string, &@$);
    }
    | value {
      $$ = new ValueExpr(*$1);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    | '*' {
      $$ = new FieldExpr("*", "*", "*");
    }
    | rel_attr {
      RelAttrSqlNode *node = $1;
      $$ = new FieldExpr(node->relation_name, node->attribute_name, node->alias);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    | LBRACE select_stmt RBRACE
    {
      $$ = new SubQuery_ValueList_Expression(std::make_shared<SelectSqlNode>($2->selection));
      $$->set_name(token_name(sql_string, &@$));
      delete $2;
    }
    | SUM_T LBRACE expression RBRACE /* alias_name */
    {
      // assert_is_not_aggregate($3);
      $$ = new AggregateExpr(AggregateExpr::Type::SUM, $3);
      RC rc = static_cast<AggregateExpr *>($$)->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      $$->set_name(token_name(sql_string, &@$));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
    | AVG_T LBRACE expression RBRACE /* alias_name */
    {
      // assert_is_not_aggregate($3);
      $$ = new AggregateExpr(AggregateExpr::Type::AVG, $3);
      RC rc = static_cast<AggregateExpr *>($$)->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      $$->set_name(token_name(sql_string, &@$));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
    | MIN_T LBRACE expression RBRACE /* alias_name */
    {
      // assert_is_not_aggregate($3);
      $$ = new AggregateExpr(AggregateExpr::Type::MIN, $3);
      RC rc = static_cast<AggregateExpr *>($$)->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      $$->set_name(token_name(sql_string, &@$));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
    | MAX_T LBRACE expression RBRACE /* alias_name */
    {
      // assert_is_not_aggregate($3);
      $$ = new AggregateExpr(AggregateExpr::Type::MAX, $3);
      RC rc = static_cast<AggregateExpr *>($$)->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      $$->set_name(token_name(sql_string, &@$));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
    | COUNT_T LBRACE expression RBRACE /* alias_name */
    {
      // assert_is_not_aggregate($3);
      $$ = new AggregateExpr(AggregateExpr::Type::COUNT, $3);
      RC rc = static_cast<AggregateExpr *>($$)->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      $$->set_name(token_name(sql_string, &@$));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
    // your code here
    ;

rel_attr:
    ID {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      $$->order_by_type = OrderByType::ASC;
      free($1);
    }
    | ID ASC_T {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      $$->order_by_type = OrderByType::ASC;
      free($1);
    }
    | ID DESC_T {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      $$->order_by_type = OrderByType::DESC;
      free($1);
    }

/*
    | ID ID {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      $$->order_by_type = OrderByType::ASC;
      $$->alias = $2;
      free($2);
      free($1);
    }
    | ID ID ASC_T {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      $$->order_by_type = OrderByType::ASC;
      $$->alias = $2;
      free($2);
      free($1);
    }
    | ID ID DESC_T {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      $$->order_by_type = OrderByType::DESC;
      $$->alias = $2;
      free($2);
      free($1);
    }
    | ID AS_T ID {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      $$->order_by_type = OrderByType::ASC;
      $$->alias = $3;
      free($1);
      free($3);
    }
    | ID AS_T ID ASC_T {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      $$->order_by_type = OrderByType::ASC;
      $$->alias = $3;
      free($1);
      free($3);
    }
    | ID AS_T ID DESC_T {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      $$->order_by_type = OrderByType::DESC;
      $$->alias = $3;
      free($1);
      free($3);
    }
*/
    | ID DOT ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      free($3);
    }
    | ID DOT '*' {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";
      $$->order_by_type = OrderByType::ASC;
      free($1);
    }
    | ID DOT ID ASC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      free($3);
    }
    | ID DOT '*' ASC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";
      $$->order_by_type = OrderByType::ASC;
      free($1);
    }
    | ID DOT ID DESC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->order_by_type = OrderByType::DESC;
      free($1);
      free($3);
    }
    | ID DOT '*' DESC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";
      $$->order_by_type = OrderByType::DESC;
      free($1);
    }
/*
    | ID DOT ID ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->alias = $4;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      free($3);
      free($4);
    }
    | ID DOT '*' ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";
      $$->alias = $4;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      free($4);
    }
    | ID DOT ID ID ASC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->alias = $4;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      free($3);
      free($4);
    }
    | ID DOT '*' ID ASC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";
      $$->alias = $4;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      free($4);
    }
    | ID DOT ID ID DESC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->alias = $4;
      $$->order_by_type = OrderByType::DESC;
      free($1);
      free($3);
      free($4);
    }
    | ID DOT '*' ID DESC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";
      $$->alias = $4;
      $$->order_by_type = OrderByType::DESC;
      free($1);
      // free($3);
      free($4);
    }
    | ID DOT ID AS_T ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->alias = $5;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      free($3);
      free($5);
    }
    | ID DOT '*' AS_T ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";
      $$->alias = $5;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      // free($3);
      free($5);
    }
    | ID DOT ID AS_T ID ASC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->alias = $5;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      free($3);
      free($5);
    }
    | ID DOT '*' AS_T ID ASC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";
      $$->alias = $5;
      $$->order_by_type = OrderByType::ASC;
      free($1);
      // free($3);
      free($5);
    }
    | ID DOT ID AS_T ID DESC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->alias = $5;
      $$->order_by_type = OrderByType::DESC;
      free($1);
      free($3);
      free($5);
    }
    | ID DOT '*' AS_T ID DESC_T {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";
      $$->alias = $5;
      $$->order_by_type = OrderByType::DESC;
      free($1);
      // free($3);
      free($5);
    }
*/
    ;


relation:
    ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name = $1;
      free($1);
    }
    | ID ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name = $1;
      $$->alias = $2;
      free($1);
      free($2);
    }
    | ID AS_T ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name = $1;
      $$->alias = $3;
      free($1);
      free($3);
    }
    ;

/* 用不到了
rel_list:
    relation {
      $$ = new std::vector<RelAttrSqlNode>();
      $$->emplace_back($1);
      delete $1;
    }
    | relation COMMA rel_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }

      $$->insert($$->begin(), $1);
      delete $1;
    }
    ;
*/

where:
    /* empty */
    {
      $$ = nullptr;
    }
    | WHERE condition_list {
      $$ = $2;
    }
    ;

condition_list:
    expression comp_op expression
    {
      $$ = new ComparisonExpr($2, std::shared_ptr<Expression>($1), std::shared_ptr<Expression>($3));
    }
    | condition_list AND condition_list
    {
      std::vector<std::shared_ptr<Expression>> tmp;
      tmp.push_back(std::shared_ptr<Expression>($1));
      tmp.push_back(std::shared_ptr<Expression>($3));
      $$ = new ConjunctionExpr(ConjunctionExpr::Type::AND, tmp);
    }
    | condition_list OR condition_list
    {
      std::vector<std::shared_ptr<Expression>> tmp;
      tmp.push_back(std::shared_ptr<Expression>($1));
      tmp.push_back(std::shared_ptr<Expression>($3));
      $$ = new ConjunctionExpr(ConjunctionExpr::Type::OR, tmp);
    }
    | EXISTS_T expression
    {
      $$ = new ComparisonExpr(EXISTS, std::shared_ptr<Expression>($2), nullptr);
    }
    | NOT_T EXISTS_T expression
    {
      $$ = new ComparisonExpr(NOT_EXISTS, std::shared_ptr<Expression>($3), nullptr);
    }
    ;

comp_op:
      EQ { $$ = EQUAL_TO; }
    | LT { $$ = LESS_THAN; }
    | GT { $$ = GREAT_THAN; }
    | LE { $$ = LESS_EQUAL; }
    | GE { $$ = GREAT_EQUAL; }
    | NE { $$ = NOT_EQUAL; }
    | LIKE_T { $$ = LIKE; }
    | NOT_T LIKE_T { $$ = NOT_LIKE; }
    | IN_T { $$ = IN; }
    | NOT_T IN_T { $$ = NOT_IN; }
    | IS_T { $$ = IS; }
    | IS_T NOT_T { $$ = NOT_IS; }
    ;





load_data_stmt:
    LOAD DATA INFILE SSS INTO TABLE ID
    {
      char *tmp_file_name = common::substr($4, 1, strlen($4) - 2);

      $$ = new ParsedSqlNode(SCF_LOAD_DATA);
      $$->load_data.relation_name = $7;
      $$->load_data.file_name = tmp_file_name;
      free($7);
      free(tmp_file_name);
    }
    ;

explain_stmt:
    EXPLAIN command_wrapper
    {
      $$ = new ParsedSqlNode(SCF_EXPLAIN);
      $$->explain.sql_node = std::shared_ptr<ParsedSqlNode>($2);
    }
    ;

set_variable_stmt:
    SET ID EQ expression
    {
      $$ = new ParsedSqlNode(SCF_SET_VARIABLE);
      $$->set_variable.name  = $2;

      if (try_expression_to_value($4, $$->set_variable.value) == false) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "try_expression_to_value failed in set_variable_stmt");
      }

      // $$->set_variable.value = *$4;
      free($2);
      delete $4;
    }
    ;

opt_semicolon: /*empty*/
    | SEMICOLON
    ;
%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
