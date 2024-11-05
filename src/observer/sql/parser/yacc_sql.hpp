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
    BY = 259,                      /* BY  */
    CREATE = 260,                  /* CREATE  */
    LIMIT_T = 261,                 /* LIMIT_T  */
    L2_DISTANCE_T = 262,           /* L2_DISTANCE_T  */
    COSINE_DISTANCE_T = 263,       /* COSINE_DISTANCE_T  */
    INNER_PRODUCT_T = 264,         /* INNER_PRODUCT_T  */
    DISTANCE_T = 265,              /* DISTANCE_T  */
    TYPE_T = 266,                  /* TYPE_T  */
    PROBES_T = 267,                /* PROBES_T  */
    LISTS_T = 268,                 /* LISTS_T  */
    WITH_T = 269,                  /* WITH_T  */
    ORDER_T = 270,                 /* ORDER_T  */
    HAVING_T = 271,                /* HAVING_T  */
    LIKE_T = 272,                  /* LIKE_T  */
    COUNT_T = 273,                 /* COUNT_T  */
    ASC_T = 274,                   /* ASC_T  */
    DESC_T = 275,                  /* DESC_T  */
    SUM_T = 276,                   /* SUM_T  */
    MIN_T = 277,                   /* MIN_T  */
    MAX_T = 278,                   /* MAX_T  */
    AVG_T = 279,                   /* AVG_T  */
    NOT_T = 280,                   /* NOT_T  */
    TEXT_T = 281,                  /* TEXT_T  */
    JOIN_T = 282,                  /* JOIN_T  */
    EXISTS_T = 283,                /* EXISTS_T  */
    IN_T = 284,                    /* IN_T  */
    UNIQUE_T = 285,                /* UNIQUE_T  */
    AS_T = 286,                    /* AS_T  */
    NULL_T = 287,                  /* NULL_T  */
    IS_T = 288,                    /* IS_T  */
    INNER_T = 289,                 /* INNER_T  */
    DROP = 290,                    /* DROP  */
    GROUP = 291,                   /* GROUP  */
    TABLE = 292,                   /* TABLE  */
    TABLES = 293,                  /* TABLES  */
    INDEX = 294,                   /* INDEX  */
    CALC = 295,                    /* CALC  */
    SELECT = 296,                  /* SELECT  */
    SHOW = 297,                    /* SHOW  */
    SYNC = 298,                    /* SYNC  */
    INSERT = 299,                  /* INSERT  */
    DELETE = 300,                  /* DELETE  */
    UPDATE = 301,                  /* UPDATE  */
    LBRACE = 302,                  /* LBRACE  */
    RBRACE = 303,                  /* RBRACE  */
    COMMA = 304,                   /* COMMA  */
    TRX_BEGIN = 305,               /* TRX_BEGIN  */
    TRX_COMMIT = 306,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 307,            /* TRX_ROLLBACK  */
    INT_T = 308,                   /* INT_T  */
    DATE_T = 309,                  /* DATE_T  */
    STRING_T = 310,                /* STRING_T  */
    FLOAT_T = 311,                 /* FLOAT_T  */
    VECTOR_T = 312,                /* VECTOR_T  */
    HELP = 313,                    /* HELP  */
    EXIT = 314,                    /* EXIT  */
    DOT = 315,                     /* DOT  */
    INTO = 316,                    /* INTO  */
    VALUES = 317,                  /* VALUES  */
    FROM = 318,                    /* FROM  */
    WHERE = 319,                   /* WHERE  */
    AND = 320,                     /* AND  */
    SET = 321,                     /* SET  */
    ON = 322,                      /* ON  */
    LOAD = 323,                    /* LOAD  */
    DATA = 324,                    /* DATA  */
    INFILE = 325,                  /* INFILE  */
    EXPLAIN = 326,                 /* EXPLAIN  */
    STORAGE = 327,                 /* STORAGE  */
    FORMAT = 328,                  /* FORMAT  */
    EQ = 329,                      /* EQ  */
    LT = 330,                      /* LT  */
    GT = 331,                      /* GT  */
    LE = 332,                      /* LE  */
    GE = 333,                      /* GE  */
    NE = 334,                      /* NE  */
    NUMBER = 335,                  /* NUMBER  */
    FLOAT = 336,                   /* FLOAT  */
    ID = 337,                      /* ID  */
    SSS = 338,                     /* SSS  */
    VECTOR_VALUE = 339,            /* VECTOR_VALUE  */
    OR = 340,                      /* OR  */
    UMINUS = 341                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 196 "yacc_sql.y"

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
  OrderByNode*                               order_by_node;
  std::vector<OrderByNode> *                 order_by_list;

#line 177 "yacc_sql.hpp"

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
