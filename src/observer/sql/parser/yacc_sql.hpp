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
    L2_DISTANCE_T = 261,           /* L2_DISTANCE_T  */
    COSINE_DISTANCE_T = 262,       /* COSINE_DISTANCE_T  */
    INNER_PRODUCT_T = 263,         /* INNER_PRODUCT_T  */
    ORDER_T = 264,                 /* ORDER_T  */
    HAVING_T = 265,                /* HAVING_T  */
    LIKE_T = 266,                  /* LIKE_T  */
    COUNT_T = 267,                 /* COUNT_T  */
    ASC_T = 268,                   /* ASC_T  */
    DESC_T = 269,                  /* DESC_T  */
    SUM_T = 270,                   /* SUM_T  */
    MIN_T = 271,                   /* MIN_T  */
    MAX_T = 272,                   /* MAX_T  */
    AVG_T = 273,                   /* AVG_T  */
    NOT_T = 274,                   /* NOT_T  */
    TEXT_T = 275,                  /* TEXT_T  */
    JOIN_T = 276,                  /* JOIN_T  */
    EXISTS_T = 277,                /* EXISTS_T  */
    IN_T = 278,                    /* IN_T  */
    UNIQUE_T = 279,                /* UNIQUE_T  */
    AS_T = 280,                    /* AS_T  */
    NULL_T = 281,                  /* NULL_T  */
    IS_T = 282,                    /* IS_T  */
    INNER_T = 283,                 /* INNER_T  */
    DROP = 284,                    /* DROP  */
    GROUP = 285,                   /* GROUP  */
    TABLE = 286,                   /* TABLE  */
    TABLES = 287,                  /* TABLES  */
    INDEX = 288,                   /* INDEX  */
    CALC = 289,                    /* CALC  */
    SELECT = 290,                  /* SELECT  */
    SHOW = 291,                    /* SHOW  */
    SYNC = 292,                    /* SYNC  */
    INSERT = 293,                  /* INSERT  */
    DELETE = 294,                  /* DELETE  */
    UPDATE = 295,                  /* UPDATE  */
    LBRACE = 296,                  /* LBRACE  */
    RBRACE = 297,                  /* RBRACE  */
    COMMA = 298,                   /* COMMA  */
    TRX_BEGIN = 299,               /* TRX_BEGIN  */
    TRX_COMMIT = 300,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 301,            /* TRX_ROLLBACK  */
    INT_T = 302,                   /* INT_T  */
    DATE_T = 303,                  /* DATE_T  */
    STRING_T = 304,                /* STRING_T  */
    FLOAT_T = 305,                 /* FLOAT_T  */
    VECTOR_T = 306,                /* VECTOR_T  */
    HELP = 307,                    /* HELP  */
    EXIT = 308,                    /* EXIT  */
    DOT = 309,                     /* DOT  */
    INTO = 310,                    /* INTO  */
    VALUES = 311,                  /* VALUES  */
    FROM = 312,                    /* FROM  */
    WHERE = 313,                   /* WHERE  */
    AND = 314,                     /* AND  */
    SET = 315,                     /* SET  */
    ON = 316,                      /* ON  */
    LOAD = 317,                    /* LOAD  */
    DATA = 318,                    /* DATA  */
    INFILE = 319,                  /* INFILE  */
    EXPLAIN = 320,                 /* EXPLAIN  */
    STORAGE = 321,                 /* STORAGE  */
    FORMAT = 322,                  /* FORMAT  */
    EQ = 323,                      /* EQ  */
    LT = 324,                      /* LT  */
    GT = 325,                      /* GT  */
    LE = 326,                      /* LE  */
    GE = 327,                      /* GE  */
    NE = 328,                      /* NE  */
    NUMBER = 329,                  /* NUMBER  */
    FLOAT = 330,                   /* FLOAT  */
    ID = 331,                      /* ID  */
    SSS = 332,                     /* SSS  */
    VECTOR_VALUE = 333,            /* VECTOR_VALUE  */
    OR = 334,                      /* OR  */
    UMINUS = 335                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 190 "yacc_sql.y"

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

#line 169 "yacc_sql.hpp"

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
