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
    LIKE_T = 261,                  /* LIKE_T  */
    NOT_T = 262,                   /* NOT_T  */
    TEXT_T = 263,                  /* TEXT_T  */
    JOIN_T = 264,                  /* JOIN_T  */
    AS_T = 265,                    /* AS_T  */
    NULL_T = 266,                  /* NULL_T  */
    IS_T = 267,                    /* IS_T  */
    INNER_T = 268,                 /* INNER_T  */
    DROP = 269,                    /* DROP  */
    GROUP = 270,                   /* GROUP  */
    TABLE = 271,                   /* TABLE  */
    TABLES = 272,                  /* TABLES  */
    INDEX = 273,                   /* INDEX  */
    CALC = 274,                    /* CALC  */
    SELECT = 275,                  /* SELECT  */
    DESC = 276,                    /* DESC  */
    SHOW = 277,                    /* SHOW  */
    SYNC = 278,                    /* SYNC  */
    INSERT = 279,                  /* INSERT  */
    DELETE = 280,                  /* DELETE  */
    UPDATE = 281,                  /* UPDATE  */
    LBRACE = 282,                  /* LBRACE  */
    RBRACE = 283,                  /* RBRACE  */
    COMMA = 284,                   /* COMMA  */
    TRX_BEGIN = 285,               /* TRX_BEGIN  */
    TRX_COMMIT = 286,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 287,            /* TRX_ROLLBACK  */
    INT_T = 288,                   /* INT_T  */
    DATE_T = 289,                  /* DATE_T  */
    STRING_T = 290,                /* STRING_T  */
    FLOAT_T = 291,                 /* FLOAT_T  */
    VECTOR_T = 292,                /* VECTOR_T  */
    HELP = 293,                    /* HELP  */
    EXIT = 294,                    /* EXIT  */
    DOT = 295,                     /* DOT  */
    INTO = 296,                    /* INTO  */
    VALUES = 297,                  /* VALUES  */
    FROM = 298,                    /* FROM  */
    WHERE = 299,                   /* WHERE  */
    AND = 300,                     /* AND  */
    SET = 301,                     /* SET  */
    ON = 302,                      /* ON  */
    LOAD = 303,                    /* LOAD  */
    DATA = 304,                    /* DATA  */
    INFILE = 305,                  /* INFILE  */
    EXPLAIN = 306,                 /* EXPLAIN  */
    STORAGE = 307,                 /* STORAGE  */
    FORMAT = 308,                  /* FORMAT  */
    EQ = 309,                      /* EQ  */
    LT = 310,                      /* LT  */
    GT = 311,                      /* GT  */
    LE = 312,                      /* LE  */
    GE = 313,                      /* GE  */
    NE = 314,                      /* NE  */
    NUMBER = 315,                  /* NUMBER  */
    FLOAT = 316,                   /* FLOAT  */
    ID = 317,                      /* ID  */
    SSS = 318,                     /* SSS  */
    OR = 319,                      /* OR  */
    UMINUS = 320                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 162 "yacc_sql.y"

  ParsedSqlNode *                            sql_node;
  Value *                                    value;
  enum CompOp                                comp;
  RelAttrSqlNode *                           rel_attr;
  std::vector<AttrInfoSqlNode> *             attr_infos;
  AttrInfoSqlNode *                          attr_info;
  Expression *                               expression;
  std::vector<std::shared_ptr<Expression>> * expression_list;
  std::vector<Value> *                       value_list;
  std::vector<RelAttrSqlNode> *              rel_attr_list;
  std::vector<std::string> *                 relation_list;
  JoinSqlNode *                              join_node;
  std::vector<JoinSqlNode> *                 join_list;
  char *                                     string;
  int                                        number;
  float                                      floats;
  bool                                       boolean;

#line 149 "yacc_sql.hpp"

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
