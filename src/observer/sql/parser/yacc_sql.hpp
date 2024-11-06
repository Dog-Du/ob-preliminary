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
    VIEW_T = 262,                  /* VIEW_T  */
    L2_DISTANCE_T = 263,           /* L2_DISTANCE_T  */
    COSINE_DISTANCE_T = 264,       /* COSINE_DISTANCE_T  */
    INNER_PRODUCT_T = 265,         /* INNER_PRODUCT_T  */
    DISTANCE_T = 266,              /* DISTANCE_T  */
    TYPE_T = 267,                  /* TYPE_T  */
    PROBES_T = 268,                /* PROBES_T  */
    LISTS_T = 269,                 /* LISTS_T  */
    WITH_T = 270,                  /* WITH_T  */
    ORDER_T = 271,                 /* ORDER_T  */
    HAVING_T = 272,                /* HAVING_T  */
    LIKE_T = 273,                  /* LIKE_T  */
    COUNT_T = 274,                 /* COUNT_T  */
    ASC_T = 275,                   /* ASC_T  */
    DESC_T = 276,                  /* DESC_T  */
    SUM_T = 277,                   /* SUM_T  */
    MIN_T = 278,                   /* MIN_T  */
    MAX_T = 279,                   /* MAX_T  */
    AVG_T = 280,                   /* AVG_T  */
    NOT_T = 281,                   /* NOT_T  */
    TEXT_T = 282,                  /* TEXT_T  */
    JOIN_T = 283,                  /* JOIN_T  */
    EXISTS_T = 284,                /* EXISTS_T  */
    IN_T = 285,                    /* IN_T  */
    UNIQUE_T = 286,                /* UNIQUE_T  */
    AS_T = 287,                    /* AS_T  */
    NULL_T = 288,                  /* NULL_T  */
    IS_T = 289,                    /* IS_T  */
    INNER_T = 290,                 /* INNER_T  */
    DROP = 291,                    /* DROP  */
    GROUP = 292,                   /* GROUP  */
    TABLE = 293,                   /* TABLE  */
    TABLES = 294,                  /* TABLES  */
    INDEX = 295,                   /* INDEX  */
    CALC = 296,                    /* CALC  */
    SELECT = 297,                  /* SELECT  */
    SHOW = 298,                    /* SHOW  */
    SYNC = 299,                    /* SYNC  */
    INSERT = 300,                  /* INSERT  */
    DELETE = 301,                  /* DELETE  */
    UPDATE = 302,                  /* UPDATE  */
    LBRACE = 303,                  /* LBRACE  */
    RBRACE = 304,                  /* RBRACE  */
    COMMA = 305,                   /* COMMA  */
    TRX_BEGIN = 306,               /* TRX_BEGIN  */
    TRX_COMMIT = 307,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 308,            /* TRX_ROLLBACK  */
    INT_T = 309,                   /* INT_T  */
    DATE_T = 310,                  /* DATE_T  */
    STRING_T = 311,                /* STRING_T  */
    FLOAT_T = 312,                 /* FLOAT_T  */
    VECTOR_T = 313,                /* VECTOR_T  */
    HELP = 314,                    /* HELP  */
    EXIT = 315,                    /* EXIT  */
    DOT = 316,                     /* DOT  */
    INTO = 317,                    /* INTO  */
    VALUES = 318,                  /* VALUES  */
    FROM = 319,                    /* FROM  */
    WHERE = 320,                   /* WHERE  */
    AND = 321,                     /* AND  */
    SET = 322,                     /* SET  */
    ON = 323,                      /* ON  */
    LOAD = 324,                    /* LOAD  */
    DATA = 325,                    /* DATA  */
    INFILE = 326,                  /* INFILE  */
    EXPLAIN = 327,                 /* EXPLAIN  */
    STORAGE = 328,                 /* STORAGE  */
    FORMAT = 329,                  /* FORMAT  */
    EQ = 330,                      /* EQ  */
    LT = 331,                      /* LT  */
    GT = 332,                      /* GT  */
    LE = 333,                      /* LE  */
    GE = 334,                      /* GE  */
    NE = 335,                      /* NE  */
    NUMBER = 336,                  /* NUMBER  */
    FLOAT = 337,                   /* FLOAT  */
    ID = 338,                      /* ID  */
    SSS = 339,                     /* SSS  */
    VECTOR_VALUE = 340,            /* VECTOR_VALUE  */
    OR = 341,                      /* OR  */
    UMINUS = 342                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 197 "yacc_sql.y"

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

#line 178 "yacc_sql.hpp"

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
