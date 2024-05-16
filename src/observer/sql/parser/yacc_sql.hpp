/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
# define YY_YY_YACC_SQL_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SEMICOLON = 258,               /* SEMICOLON  */
    CREATE = 259,                  /* CREATE  */
    DROP = 260,                    /* DROP  */
    TABLE = 261,                   /* TABLE  */
    TABLES = 262,                  /* TABLES  */
    INDEX = 263,                   /* INDEX  */
    CALC = 264,                    /* CALC  */
    SELECT = 265,                  /* SELECT  */
    SUM = 266,                     /* SUM  */
    MAX = 267,                     /* MAX  */
    MIN = 268,                     /* MIN  */
    COUNT = 269,                   /* COUNT  */
    AVG = 270,                     /* AVG  */
    IS_T = 271,                    /* IS_T  */
    NOT = 272,                     /* NOT  */
    NULL_T = 273,                  /* NULL_T  */
    LIKE_T = 274,                  /* LIKE_T  */
    INNER_T = 275,                 /* INNER_T  */
    JOIN_T = 276,                  /* JOIN_T  */
    IN_T = 277,                    /* IN_T  */
    EXIST_T = 278,                 /* EXIST_T  */
    UNIQUE_T = 279,                /* UNIQUE_T  */
    ORDER_T = 280,                 /* ORDER_T  */
    BY_T = 281,                    /* BY_T  */
    ASC_T = 282,                   /* ASC_T  */
    TEXT_T = 283,                  /* TEXT_T  */
    DESC = 284,                    /* DESC  */
    SHOW = 285,                    /* SHOW  */
    SYNC = 286,                    /* SYNC  */
    INSERT = 287,                  /* INSERT  */
    DELETE = 288,                  /* DELETE  */
    UPDATE = 289,                  /* UPDATE  */
    LBRACE = 290,                  /* LBRACE  */
    RBRACE = 291,                  /* RBRACE  */
    COMMA = 292,                   /* COMMA  */
    TRX_BEGIN = 293,               /* TRX_BEGIN  */
    TRX_COMMIT = 294,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 295,            /* TRX_ROLLBACK  */
    DATE_T = 296,                  /* DATE_T  */
    INT_T = 297,                   /* INT_T  */
    STRING_T = 298,                /* STRING_T  */
    FLOAT_T = 299,                 /* FLOAT_T  */
    HELP = 300,                    /* HELP  */
    EXIT = 301,                    /* EXIT  */
    DOT = 302,                     /* DOT  */
    INTO = 303,                    /* INTO  */
    VALUES = 304,                  /* VALUES  */
    FROM = 305,                    /* FROM  */
    WHERE = 306,                   /* WHERE  */
    AND = 307,                     /* AND  */
    SET = 308,                     /* SET  */
    ON = 309,                      /* ON  */
    LOAD = 310,                    /* LOAD  */
    DATA = 311,                    /* DATA  */
    INFILE = 312,                  /* INFILE  */
    EXPLAIN = 313,                 /* EXPLAIN  */
    EQ = 314,                      /* EQ  */
    LT = 315,                      /* LT  */
    GT = 316,                      /* GT  */
    LE = 317,                      /* LE  */
    GE = 318,                      /* GE  */
    NE = 319,                      /* NE  */
    NUMBER = 320,                  /* NUMBER  */
    FLOAT = 321,                   /* FLOAT  */
    ID = 322,                      /* ID  */
    DATE_STR = 323,                /* DATE_STR  */
    SSS = 324,                     /* SSS  */
    UMINUS = 325                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 130 "yacc_sql.y"

  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;

  JoinSqlNode *                     join;
  std::vector<JoinSqlNode> *        join_list;

  std::vector<update_value> *       update_list;

  std::vector<OrderBySqlNode> *     order_by_list;
  OrderBySqlNode *                  order_by_node;

  Value *                           value;
  enum CompOp                       comp;
  RelAttrSqlNode *                  rel_attr;
  std::vector<AttrInfoSqlNode> *    attr_infos;
  AttrInfoSqlNode *                 attr_info;
  Expression *                      expression;
  std::vector<Expression *> *       expression_list;
  std::vector<Value> *              value_list;
  std::vector<ConditionSqlNode> *   condition_list;
  std::vector<RelAttrSqlNode> *     rel_attr_list;
  std::vector<std::string> *        relation_list;
  char *                            string;
  int                               number;
  float                             floats;

#line 162 "yacc_sql.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner);


#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
