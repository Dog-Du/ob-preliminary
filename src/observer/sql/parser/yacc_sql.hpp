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
    ORDER_T = 265,                 /* ORDER_T  */
    HAVING_T = 266,                /* HAVING_T  */
    LIKE_T = 267,                  /* LIKE_T  */
    COUNT_T = 268,                 /* COUNT_T  */
    ASC_T = 269,                   /* ASC_T  */
    DESC_T = 270,                  /* DESC_T  */
    SUM_T = 271,                   /* SUM_T  */
    MIN_T = 272,                   /* MIN_T  */
    MAX_T = 273,                   /* MAX_T  */
    AVG_T = 274,                   /* AVG_T  */
    NOT_T = 275,                   /* NOT_T  */
    TEXT_T = 276,                  /* TEXT_T  */
    JOIN_T = 277,                  /* JOIN_T  */
    EXISTS_T = 278,                /* EXISTS_T  */
    IN_T = 279,                    /* IN_T  */
    UNIQUE_T = 280,                /* UNIQUE_T  */
    AS_T = 281,                    /* AS_T  */
    NULL_T = 282,                  /* NULL_T  */
    IS_T = 283,                    /* IS_T  */
    INNER_T = 284,                 /* INNER_T  */
    DROP = 285,                    /* DROP  */
    GROUP = 286,                   /* GROUP  */
    TABLE = 287,                   /* TABLE  */
    TABLES = 288,                  /* TABLES  */
    INDEX = 289,                   /* INDEX  */
    CALC = 290,                    /* CALC  */
    SELECT = 291,                  /* SELECT  */
    SHOW = 292,                    /* SHOW  */
    SYNC = 293,                    /* SYNC  */
    INSERT = 294,                  /* INSERT  */
    DELETE = 295,                  /* DELETE  */
    UPDATE = 296,                  /* UPDATE  */
    LBRACE = 297,                  /* LBRACE  */
    RBRACE = 298,                  /* RBRACE  */
    COMMA = 299,                   /* COMMA  */
    TRX_BEGIN = 300,               /* TRX_BEGIN  */
    TRX_COMMIT = 301,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 302,            /* TRX_ROLLBACK  */
    INT_T = 303,                   /* INT_T  */
    DATE_T = 304,                  /* DATE_T  */
    STRING_T = 305,                /* STRING_T  */
    FLOAT_T = 306,                 /* FLOAT_T  */
    VECTOR_T = 307,                /* VECTOR_T  */
    HELP = 308,                    /* HELP  */
    EXIT = 309,                    /* EXIT  */
    DOT = 310,                     /* DOT  */
    INTO = 311,                    /* INTO  */
    VALUES = 312,                  /* VALUES  */
    FROM = 313,                    /* FROM  */
    WHERE = 314,                   /* WHERE  */
    AND = 315,                     /* AND  */
    SET = 316,                     /* SET  */
    ON = 317,                      /* ON  */
    LOAD = 318,                    /* LOAD  */
    DATA = 319,                    /* DATA  */
    INFILE = 320,                  /* INFILE  */
    EXPLAIN = 321,                 /* EXPLAIN  */
    STORAGE = 322,                 /* STORAGE  */
    FORMAT = 323,                  /* FORMAT  */
    EQ = 324,                      /* EQ  */
    LT = 325,                      /* LT  */
    GT = 326,                      /* GT  */
    LE = 327,                      /* LE  */
    GE = 328,                      /* GE  */
    NE = 329,                      /* NE  */
    NUMBER = 330,                  /* NUMBER  */
    FLOAT = 331,                   /* FLOAT  */
    ID = 332,                      /* ID  */
    SSS = 333,                     /* SSS  */
    VECTOR_VALUE = 334,            /* VECTOR_VALUE  */
    OR = 335,                      /* OR  */
    UMINUS = 336                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 191 "yacc_sql.y"

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

#line 172 "yacc_sql.hpp"

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
