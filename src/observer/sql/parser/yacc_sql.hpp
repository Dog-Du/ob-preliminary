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
    DESC = 280,                    /* DESC  */
    SHOW = 281,                    /* SHOW  */
    SYNC = 282,                    /* SYNC  */
    INSERT = 283,                  /* INSERT  */
    DELETE = 284,                  /* DELETE  */
    UPDATE = 285,                  /* UPDATE  */
    LBRACE = 286,                  /* LBRACE  */
    RBRACE = 287,                  /* RBRACE  */
    COMMA = 288,                   /* COMMA  */
    TRX_BEGIN = 289,               /* TRX_BEGIN  */
    TRX_COMMIT = 290,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 291,            /* TRX_ROLLBACK  */
    DATE_T = 292,                  /* DATE_T  */
    INT_T = 293,                   /* INT_T  */
    STRING_T = 294,                /* STRING_T  */
    FLOAT_T = 295,                 /* FLOAT_T  */
    HELP = 296,                    /* HELP  */
    EXIT = 297,                    /* EXIT  */
    DOT = 298,                     /* DOT  */
    INTO = 299,                    /* INTO  */
    VALUES = 300,                  /* VALUES  */
    FROM = 301,                    /* FROM  */
    WHERE = 302,                   /* WHERE  */
    AND = 303,                     /* AND  */
    SET = 304,                     /* SET  */
    ON = 305,                      /* ON  */
    LOAD = 306,                    /* LOAD  */
    DATA = 307,                    /* DATA  */
    INFILE = 308,                  /* INFILE  */
    EXPLAIN = 309,                 /* EXPLAIN  */
    EQ = 310,                      /* EQ  */
    LT = 311,                      /* LT  */
    GT = 312,                      /* GT  */
    LE = 313,                      /* LE  */
    GE = 314,                      /* GE  */
    NE = 315,                      /* NE  */
    NUMBER = 316,                  /* NUMBER  */
    FLOAT = 317,                   /* FLOAT  */
    ID = 318,                      /* ID  */
    DATE_STR = 319,                /* DATE_STR  */
    SSS = 320,                     /* SSS  */
    UMINUS = 321                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 124 "yacc_sql.y"

  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;

  JoinSqlNode *                     join;
  std::vector<JoinSqlNode> *        join_list;

  std::vector<update_value> *       update_list;

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

#line 155 "yacc_sql.hpp"

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
