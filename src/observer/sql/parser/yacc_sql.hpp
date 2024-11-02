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
    ORDER_T = 261,                 /* ORDER_T  */
    HAVING_T = 262,                /* HAVING_T  */
    LIKE_T = 263,                  /* LIKE_T  */
    COUNT_T = 264,                 /* COUNT_T  */
    ASC_T = 265,                   /* ASC_T  */
    DESC_T = 266,                  /* DESC_T  */
    SUM_T = 267,                   /* SUM_T  */
    MIN_T = 268,                   /* MIN_T  */
    MAX_T = 269,                   /* MAX_T  */
    AVG_T = 270,                   /* AVG_T  */
    NOT_T = 271,                   /* NOT_T  */
    TEXT_T = 272,                  /* TEXT_T  */
    JOIN_T = 273,                  /* JOIN_T  */
    EXISTS_T = 274,                /* EXISTS_T  */
    IN_T = 275,                    /* IN_T  */
    UNIQUE_T = 276,                /* UNIQUE_T  */
    AS_T = 277,                    /* AS_T  */
    NULL_T = 278,                  /* NULL_T  */
    IS_T = 279,                    /* IS_T  */
    INNER_T = 280,                 /* INNER_T  */
    DROP = 281,                    /* DROP  */
    GROUP = 282,                   /* GROUP  */
    TABLE = 283,                   /* TABLE  */
    TABLES = 284,                  /* TABLES  */
    INDEX = 285,                   /* INDEX  */
    CALC = 286,                    /* CALC  */
    SELECT = 287,                  /* SELECT  */
    SHOW = 288,                    /* SHOW  */
    SYNC = 289,                    /* SYNC  */
    INSERT = 290,                  /* INSERT  */
    DELETE = 291,                  /* DELETE  */
    UPDATE = 292,                  /* UPDATE  */
    LBRACE = 293,                  /* LBRACE  */
    RBRACE = 294,                  /* RBRACE  */
    COMMA = 295,                   /* COMMA  */
    TRX_BEGIN = 296,               /* TRX_BEGIN  */
    TRX_COMMIT = 297,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 298,            /* TRX_ROLLBACK  */
    INT_T = 299,                   /* INT_T  */
    DATE_T = 300,                  /* DATE_T  */
    STRING_T = 301,                /* STRING_T  */
    FLOAT_T = 302,                 /* FLOAT_T  */
    VECTOR_T = 303,                /* VECTOR_T  */
    HELP = 304,                    /* HELP  */
    EXIT = 305,                    /* EXIT  */
    DOT = 306,                     /* DOT  */
    INTO = 307,                    /* INTO  */
    VALUES = 308,                  /* VALUES  */
    FROM = 309,                    /* FROM  */
    WHERE = 310,                   /* WHERE  */
    AND = 311,                     /* AND  */
    SET = 312,                     /* SET  */
    ON = 313,                      /* ON  */
    LOAD = 314,                    /* LOAD  */
    DATA = 315,                    /* DATA  */
    INFILE = 316,                  /* INFILE  */
    EXPLAIN = 317,                 /* EXPLAIN  */
    STORAGE = 318,                 /* STORAGE  */
    FORMAT = 319,                  /* FORMAT  */
    EQ = 320,                      /* EQ  */
    LT = 321,                      /* LT  */
    GT = 322,                      /* GT  */
    LE = 323,                      /* LE  */
    GE = 324,                      /* GE  */
    NE = 325,                      /* NE  */
    NUMBER = 326,                  /* NUMBER  */
    FLOAT = 327,                   /* FLOAT  */
    ID = 328,                      /* ID  */
    SSS = 329,                     /* SSS  */
    OR = 330,                      /* OR  */
    UMINUS = 331                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 187 "yacc_sql.y"

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

#line 165 "yacc_sql.hpp"

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
