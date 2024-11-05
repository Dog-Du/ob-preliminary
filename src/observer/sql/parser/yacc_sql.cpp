/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


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


#line 175 "yacc_sql.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_BY = 4,                         /* BY  */
  YYSYMBOL_CREATE = 5,                     /* CREATE  */
  YYSYMBOL_LIMIT_T = 6,                    /* LIMIT_T  */
  YYSYMBOL_L2_DISTANCE_T = 7,              /* L2_DISTANCE_T  */
  YYSYMBOL_COSINE_DISTANCE_T = 8,          /* COSINE_DISTANCE_T  */
  YYSYMBOL_INNER_PRODUCT_T = 9,            /* INNER_PRODUCT_T  */
  YYSYMBOL_DISTANCE_T = 10,                /* DISTANCE_T  */
  YYSYMBOL_TYPE_T = 11,                    /* TYPE_T  */
  YYSYMBOL_PROBES_T = 12,                  /* PROBES_T  */
  YYSYMBOL_LISTS_T = 13,                   /* LISTS_T  */
  YYSYMBOL_WITH_T = 14,                    /* WITH_T  */
  YYSYMBOL_ORDER_T = 15,                   /* ORDER_T  */
  YYSYMBOL_HAVING_T = 16,                  /* HAVING_T  */
  YYSYMBOL_LIKE_T = 17,                    /* LIKE_T  */
  YYSYMBOL_COUNT_T = 18,                   /* COUNT_T  */
  YYSYMBOL_ASC_T = 19,                     /* ASC_T  */
  YYSYMBOL_DESC_T = 20,                    /* DESC_T  */
  YYSYMBOL_SUM_T = 21,                     /* SUM_T  */
  YYSYMBOL_MIN_T = 22,                     /* MIN_T  */
  YYSYMBOL_MAX_T = 23,                     /* MAX_T  */
  YYSYMBOL_AVG_T = 24,                     /* AVG_T  */
  YYSYMBOL_NOT_T = 25,                     /* NOT_T  */
  YYSYMBOL_TEXT_T = 26,                    /* TEXT_T  */
  YYSYMBOL_JOIN_T = 27,                    /* JOIN_T  */
  YYSYMBOL_EXISTS_T = 28,                  /* EXISTS_T  */
  YYSYMBOL_IN_T = 29,                      /* IN_T  */
  YYSYMBOL_UNIQUE_T = 30,                  /* UNIQUE_T  */
  YYSYMBOL_AS_T = 31,                      /* AS_T  */
  YYSYMBOL_NULL_T = 32,                    /* NULL_T  */
  YYSYMBOL_IS_T = 33,                      /* IS_T  */
  YYSYMBOL_INNER_T = 34,                   /* INNER_T  */
  YYSYMBOL_DROP = 35,                      /* DROP  */
  YYSYMBOL_GROUP = 36,                     /* GROUP  */
  YYSYMBOL_TABLE = 37,                     /* TABLE  */
  YYSYMBOL_TABLES = 38,                    /* TABLES  */
  YYSYMBOL_INDEX = 39,                     /* INDEX  */
  YYSYMBOL_CALC = 40,                      /* CALC  */
  YYSYMBOL_SELECT = 41,                    /* SELECT  */
  YYSYMBOL_SHOW = 42,                      /* SHOW  */
  YYSYMBOL_SYNC = 43,                      /* SYNC  */
  YYSYMBOL_INSERT = 44,                    /* INSERT  */
  YYSYMBOL_DELETE = 45,                    /* DELETE  */
  YYSYMBOL_UPDATE = 46,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 47,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 48,                    /* RBRACE  */
  YYSYMBOL_COMMA = 49,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 50,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 51,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 52,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 53,                     /* INT_T  */
  YYSYMBOL_DATE_T = 54,                    /* DATE_T  */
  YYSYMBOL_STRING_T = 55,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 56,                   /* FLOAT_T  */
  YYSYMBOL_VECTOR_T = 57,                  /* VECTOR_T  */
  YYSYMBOL_HELP = 58,                      /* HELP  */
  YYSYMBOL_EXIT = 59,                      /* EXIT  */
  YYSYMBOL_DOT = 60,                       /* DOT  */
  YYSYMBOL_INTO = 61,                      /* INTO  */
  YYSYMBOL_VALUES = 62,                    /* VALUES  */
  YYSYMBOL_FROM = 63,                      /* FROM  */
  YYSYMBOL_WHERE = 64,                     /* WHERE  */
  YYSYMBOL_AND = 65,                       /* AND  */
  YYSYMBOL_SET = 66,                       /* SET  */
  YYSYMBOL_ON = 67,                        /* ON  */
  YYSYMBOL_LOAD = 68,                      /* LOAD  */
  YYSYMBOL_DATA = 69,                      /* DATA  */
  YYSYMBOL_INFILE = 70,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 71,                   /* EXPLAIN  */
  YYSYMBOL_STORAGE = 72,                   /* STORAGE  */
  YYSYMBOL_FORMAT = 73,                    /* FORMAT  */
  YYSYMBOL_EQ = 74,                        /* EQ  */
  YYSYMBOL_LT = 75,                        /* LT  */
  YYSYMBOL_GT = 76,                        /* GT  */
  YYSYMBOL_LE = 77,                        /* LE  */
  YYSYMBOL_GE = 78,                        /* GE  */
  YYSYMBOL_NE = 79,                        /* NE  */
  YYSYMBOL_NUMBER = 80,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 81,                     /* FLOAT  */
  YYSYMBOL_ID = 82,                        /* ID  */
  YYSYMBOL_SSS = 83,                       /* SSS  */
  YYSYMBOL_VECTOR_VALUE = 84,              /* VECTOR_VALUE  */
  YYSYMBOL_OR = 85,                        /* OR  */
  YYSYMBOL_86_ = 86,                       /* '+'  */
  YYSYMBOL_87_ = 87,                       /* '-'  */
  YYSYMBOL_88_ = 88,                       /* '*'  */
  YYSYMBOL_89_ = 89,                       /* '/'  */
  YYSYMBOL_UMINUS = 90,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 91,                  /* $accept  */
  YYSYMBOL_commands = 92,                  /* commands  */
  YYSYMBOL_command_wrapper = 93,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 94,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 95,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 96,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 97,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 98,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 99,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 100,          /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 101,         /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 102,          /* desc_table_stmt  */
  YYSYMBOL_vector_function_type = 103,     /* vector_function_type  */
  YYSYMBOL_create_index_stmt = 104,        /* create_index_stmt  */
  YYSYMBOL_index_attr_list = 105,          /* index_attr_list  */
  YYSYMBOL_drop_index_stmt = 106,          /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 107,        /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 108,            /* attr_def_list  */
  YYSYMBOL_attr_def = 109,                 /* attr_def  */
  YYSYMBOL_attr_def_nullable = 110,        /* attr_def_nullable  */
  YYSYMBOL_number = 111,                   /* number  */
  YYSYMBOL_type = 112,                     /* type  */
  YYSYMBOL_insert_stmt = 113,              /* insert_stmt  */
  YYSYMBOL_insert_value_list = 114,        /* insert_value_list  */
  YYSYMBOL_insert_value = 115,             /* insert_value  */
  YYSYMBOL_value_list = 116,               /* value_list  */
  YYSYMBOL_value = 117,                    /* value  */
  YYSYMBOL_storage_format = 118,           /* storage_format  */
  YYSYMBOL_delete_stmt = 119,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 120,              /* update_stmt  */
  YYSYMBOL_update_node_list = 121,         /* update_node_list  */
  YYSYMBOL_update_node = 122,              /* update_node  */
  YYSYMBOL_select_stmt = 123,              /* select_stmt  */
  YYSYMBOL_limit = 124,                    /* limit  */
  YYSYMBOL_order_by = 125,                 /* order_by  */
  YYSYMBOL_order_by_list = 126,            /* order_by_list  */
  YYSYMBOL_order_by_node = 127,            /* order_by_node  */
  YYSYMBOL_having = 128,                   /* having  */
  YYSYMBOL_group_by = 129,                 /* group_by  */
  YYSYMBOL_rel_attr_list = 130,            /* rel_attr_list  */
  YYSYMBOL_from_node = 131,                /* from_node  */
  YYSYMBOL_from_node_list = 132,           /* from_node_list  */
  YYSYMBOL_join_node = 133,                /* join_node  */
  YYSYMBOL_join_list = 134,                /* join_list  */
  YYSYMBOL_calc_stmt = 135,                /* calc_stmt  */
  YYSYMBOL_alias_name = 136,               /* alias_name  */
  YYSYMBOL_expression_list = 137,          /* expression_list  */
  YYSYMBOL_expression = 138,               /* expression  */
  YYSYMBOL_rel_attr = 139,                 /* rel_attr  */
  YYSYMBOL_relation = 140,                 /* relation  */
  YYSYMBOL_where = 141,                    /* where  */
  YYSYMBOL_condition_list = 142,           /* condition_list  */
  YYSYMBOL_comp_op = 143,                  /* comp_op  */
  YYSYMBOL_load_data_stmt = 144,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 145,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 146,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 147             /* opt_semicolon  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  77
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   379

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  153
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  325

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   341


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    88,    86,     2,    87,     2,    89,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    90
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   303,   303,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   334,   340,   345,   351,   357,   363,   369,
     376,   382,   390,   394,   398,   405,   424,   443,   473,   476,
     489,   500,   521,   528,   535,   550,   569,   572,   585,   601,
     610,   622,   625,   628,   634,   638,   639,   640,   641,   642,
     646,   663,   666,   679,   700,   703,   721,   725,   729,   746,
     750,   763,   766,   773,   784,   804,   807,   820,   830,   871,
     874,   882,   885,   900,   903,   916,   922,   928,   938,   941,
     949,   952,   967,   970,   983,   989,   992,  1004,  1019,  1022,
    1035,  1045,  1048,  1052,  1059,  1069,  1086,  1091,  1096,  1101,
    1106,  1122,  1126,  1131,  1134,  1140,  1146,  1164,  1182,  1200,
    1218,  1236,  1241,  1246,  1255,  1325,  1333,  1495,  1500,  1507,
    1538,  1541,  1547,  1551,  1558,  1565,  1569,  1576,  1577,  1578,
    1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1595,
    1608,  1616,  1631,  1632
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "BY",
  "CREATE", "LIMIT_T", "L2_DISTANCE_T", "COSINE_DISTANCE_T",
  "INNER_PRODUCT_T", "DISTANCE_T", "TYPE_T", "PROBES_T", "LISTS_T",
  "WITH_T", "ORDER_T", "HAVING_T", "LIKE_T", "COUNT_T", "ASC_T", "DESC_T",
  "SUM_T", "MIN_T", "MAX_T", "AVG_T", "NOT_T", "TEXT_T", "JOIN_T",
  "EXISTS_T", "IN_T", "UNIQUE_T", "AS_T", "NULL_T", "IS_T", "INNER_T",
  "DROP", "GROUP", "TABLE", "TABLES", "INDEX", "CALC", "SELECT", "SHOW",
  "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA",
  "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "DATE_T", "STRING_T",
  "FLOAT_T", "VECTOR_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM",
  "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EXPLAIN",
  "STORAGE", "FORMAT", "EQ", "LT", "GT", "LE", "GE", "NE", "NUMBER",
  "FLOAT", "ID", "SSS", "VECTOR_VALUE", "OR", "'+'", "'-'", "'*'", "'/'",
  "UMINUS", "$accept", "commands", "command_wrapper", "exit_stmt",
  "help_stmt", "sync_stmt", "begin_stmt", "commit_stmt", "rollback_stmt",
  "drop_table_stmt", "show_tables_stmt", "desc_table_stmt",
  "vector_function_type", "create_index_stmt", "index_attr_list",
  "drop_index_stmt", "create_table_stmt", "attr_def_list", "attr_def",
  "attr_def_nullable", "number", "type", "insert_stmt",
  "insert_value_list", "insert_value", "value_list", "value",
  "storage_format", "delete_stmt", "update_stmt", "update_node_list",
  "update_node", "select_stmt", "limit", "order_by", "order_by_list",
  "order_by_node", "having", "group_by", "rel_attr_list", "from_node",
  "from_node_list", "join_node", "join_list", "calc_stmt", "alias_name",
  "expression_list", "expression", "rel_attr", "relation", "where",
  "condition_list", "comp_op", "load_data_stmt", "explain_stmt",
  "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-240)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     235,    -7,   -68,    29,   137,   137,   -16,  -240,   -30,    -8,
     -31,  -240,  -240,  -240,  -240,  -240,   -11,     0,   235,    76,
      78,  -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,
    -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,
    -240,    52,    24,    27,    72,  -240,    43,    50,    86,    90,
      96,   101,   102,   107,   110,   115,  -240,   106,  -240,  -240,
      75,  -240,  -240,   137,  -240,  -240,  -240,    30,  -240,   100,
    -240,    82,    84,   104,    93,   108,  -240,  -240,  -240,  -240,
      89,    79,   112,    98,  -240,   118,   137,   137,   137,   137,
     137,   137,   137,   137,   134,   147,   -24,  -240,   116,  -240,
     137,   137,   137,   137,   152,   120,   144,   141,   133,   137,
     139,   149,   182,   146,  -240,   156,   172,   174,   211,   222,
     233,    -9,     6,    88,   143,   148,  -240,  -240,  -240,  -240,
    -240,   -42,   -42,  -240,  -240,   137,   -20,   177,  -240,   223,
     212,    20,  -240,   209,   195,    53,   213,   190,  -240,   157,
     224,   236,   207,  -240,   137,   137,   137,  -240,  -240,  -240,
    -240,  -240,  -240,   208,  -240,   120,   141,   264,  -240,   137,
     243,   267,   137,   175,   -48,   133,   141,   137,   265,   257,
     -12,  -240,  -240,  -240,  -240,  -240,   -13,   146,   259,   230,
     258,   155,   159,   179,  -240,   177,   277,   120,   239,   212,
    -240,   137,    53,  -240,     7,  -240,   289,  -240,  -240,  -240,
    -240,  -240,  -240,   137,    20,    20,   209,  -240,    53,   234,
     241,   283,  -240,  -240,   237,  -240,   224,   -23,   269,   242,
    -240,  -240,  -240,  -240,   325,   314,   266,   137,   284,   243,
      53,  -240,  -240,  -240,    53,  -240,  -240,  -240,  -240,   269,
    -240,  -240,   286,  -240,   182,   262,  -240,  -240,   249,   288,
     269,   133,    20,   322,    20,   239,  -240,  -240,   290,   -12,
    -240,   268,   269,  -240,   291,   292,   -48,   336,   337,   -25,
    -240,  -240,  -240,   263,  -240,   330,   133,  -240,   137,   270,
    -240,  -240,  -240,   299,   292,   298,   -14,  -240,   338,  -240,
     137,  -240,  -240,  -240,   274,   298,   271,  -240,   302,   342,
     280,   114,  -240,  -240,  -240,   306,   343,   285,   278,   308,
     348,   287,   282,   315,  -240
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,    26,    27,    28,    24,    23,     0,     0,     0,     0,
     152,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,     0,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,     0,    66,    67,
     124,    68,    70,     0,   113,   112,   100,   101,   114,     0,
      30,     0,     0,     0,     0,     0,   150,     1,   153,     2,
       0,     0,     0,     0,    29,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,   102,
       0,     0,     0,     0,   104,     0,     0,   130,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,   110,   125,   126,
     103,   106,   107,   108,   109,     0,   127,    95,    94,    98,
       0,     0,    73,    75,     0,   151,     0,     0,    42,     0,
      46,     0,     0,    40,     0,     0,     0,   120,   116,   118,
     119,   117,   105,     0,   128,     0,   130,     0,    97,     0,
      61,     0,     0,     0,   131,     0,   130,     0,     0,     0,
      51,    55,    59,    56,    57,    58,    51,     0,     0,     0,
       0,     0,     0,     0,   129,    95,    90,     0,    64,     0,
      60,     0,   135,   143,     0,   145,   147,   137,   138,   139,
     140,   141,   142,     0,     0,     0,    75,    74,    77,     0,
       0,     0,    52,    50,     0,    49,    46,    71,    38,     0,
     121,   122,   123,    96,     0,    88,     0,     0,     0,    61,
     136,   144,   146,   148,   132,   133,   134,    76,   149,    38,
      53,    54,     0,    47,     0,     0,    41,    45,     0,     0,
      38,     0,     0,    81,     0,    64,    63,    62,     0,    51,
      44,     0,    38,    35,     0,    92,    89,     0,    79,    98,
      65,    36,    48,     0,    39,     0,     0,    91,     0,     0,
      78,    99,    72,     0,    92,    83,    85,    80,     0,    93,
       0,    82,    86,    87,     0,    83,     0,    84,     0,     0,
       0,     0,    32,    33,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -240,  -240,   346,  -240,  -240,  -240,  -240,  -240,  -240,  -240,
    -240,  -240,  -240,  -240,  -239,  -240,  -240,   140,   178,  -179,
    -240,  -240,  -240,   128,   169,   105,  -240,  -240,  -240,  -240,
     153,   196,   -55,  -240,  -240,    67,    73,  -240,  -240,    80,
    -240,   180,   214,    97,  -240,  -240,    -1,    -4,  -105,   181,
    -141,  -199,  -240,  -240,  -240,  -240,  -240
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,   315,    30,   259,    31,    32,   188,   150,   223,
     252,   186,    33,   200,   170,   238,    65,   256,    34,    35,
     176,   143,    36,   290,   278,   301,   295,   263,   235,   287,
     137,   166,   138,   168,    37,   104,    66,   173,    68,   139,
     142,   174,   213,    38,    39,    40,    79
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      67,    67,    94,   144,    69,   302,   303,   225,   254,   167,
     268,   163,   221,   221,    45,   245,   246,   214,     5,   222,
     222,   274,    70,    41,   241,   196,   114,    48,    49,    50,
      42,    71,    43,   284,   224,   217,   242,   215,    51,   157,
     214,    52,    53,    54,    55,   171,   102,   103,   172,   255,
      44,    73,    56,    67,   158,    72,    95,   148,   128,    97,
     215,    98,   164,   276,   129,   279,    46,    57,    47,    75,
     144,    74,   100,   101,   102,   103,    77,   100,   101,   102,
     103,    78,   118,   119,   120,   121,   122,   123,   124,   125,
     282,    80,   100,   101,   102,   103,   131,   132,   133,   134,
      58,    59,    60,    61,    62,   145,    81,    63,    64,    82,
     112,    83,    99,    48,    49,    50,   100,   101,   102,   103,
       5,   312,   313,   314,    51,    84,   113,    52,    53,    54,
      55,    67,    85,    86,   162,    96,   159,    87,    56,   100,
     101,   102,   103,    88,    48,    49,    50,     5,    89,    90,
     191,   192,   193,    57,    91,    51,   275,    92,    52,    53,
      54,    55,    93,   105,   106,   198,   107,   109,   202,    56,
     108,   111,   257,   218,   100,   101,   102,   103,   110,   115,
     116,   294,   126,   180,    57,   117,    58,    59,    60,    61,
      62,   160,   203,    63,    64,   127,   161,   240,   130,   270,
     204,   135,   136,   230,   205,   141,   140,   231,   206,   244,
     181,   182,   183,   184,   185,    60,   147,    58,    59,    60,
      61,    62,   146,     5,    63,    64,   165,   232,   149,   100,
     101,   102,   103,   265,   100,   101,   102,   103,   151,   152,
       1,   100,   101,   102,   103,   100,   101,   102,   103,   207,
     208,   209,   210,   211,   212,     2,   153,   167,   175,   169,
     154,   100,   101,   102,   103,   100,   101,   102,   103,   177,
       3,   155,   179,   187,   178,     4,     5,     6,     7,     8,
       9,    10,   156,   189,   296,    11,    12,    13,   237,   190,
     194,   197,   199,    14,    15,   201,   296,   100,   101,   102,
     103,    16,   219,    17,   220,   229,    18,   227,   100,   101,
     102,   103,   228,   234,   243,   250,   248,   251,   258,   100,
     101,   102,   103,   249,   260,   100,   101,   102,   103,   261,
     262,   272,   266,   264,   269,   271,   273,   277,   281,   285,
     288,   286,   283,   289,   293,   292,   298,   300,   306,   304,
     297,   309,   310,   308,   311,   316,   317,   320,   319,   318,
     321,   322,   323,   324,    76,   226,   253,   267,   239,   247,
     280,   216,   307,   305,   299,   233,   291,     0,   236,   195
};

static const yytype_int16 yycheck[] =
{
       4,     5,    57,   108,     5,    19,    20,   186,    31,    34,
     249,    31,    25,    25,    82,   214,   215,    65,    41,    32,
      32,   260,    38,    30,    17,   166,    81,     7,     8,     9,
      37,    61,    39,   272,    47,   176,    29,    85,    18,    48,
      65,    21,    22,    23,    24,    25,    88,    89,    28,    72,
      57,    82,    32,    57,    48,    63,    57,   112,    82,    63,
      85,    31,    82,   262,    88,   264,    37,    47,    39,    69,
     175,    82,    86,    87,    88,    89,     0,    86,    87,    88,
      89,     3,    86,    87,    88,    89,    90,    91,    92,    93,
     269,    39,    86,    87,    88,    89,   100,   101,   102,   103,
      80,    81,    82,    83,    84,   109,    82,    87,    88,    82,
      31,    39,    82,     7,     8,     9,    86,    87,    88,    89,
      41,     7,     8,     9,    18,    82,    47,    21,    22,    23,
      24,   135,    82,    47,   135,    60,    48,    47,    32,    86,
      87,    88,    89,    47,     7,     8,     9,    41,    47,    47,
     154,   155,   156,    47,    47,    18,   261,    47,    21,    22,
      23,    24,    47,    63,    82,   169,    82,    74,   172,    32,
      66,    82,   227,   177,    86,    87,    88,    89,    70,    67,
      82,   286,    48,    26,    47,    67,    80,    81,    82,    83,
      84,    48,    17,    87,    88,    48,    48,   201,    82,   254,
      25,    49,    82,    48,    29,    64,    62,    48,    33,   213,
      53,    54,    55,    56,    57,    82,    67,    80,    81,    82,
      83,    84,    83,    41,    87,    88,    49,    48,    82,    86,
      87,    88,    89,   237,    86,    87,    88,    89,    82,    67,
       5,    86,    87,    88,    89,    86,    87,    88,    89,    74,
      75,    76,    77,    78,    79,    20,    82,    34,    49,    47,
      49,    86,    87,    88,    89,    86,    87,    88,    89,    74,
      35,    49,    82,    49,    61,    40,    41,    42,    43,    44,
      45,    46,    49,    47,   288,    50,    51,    52,    49,    82,
      82,    27,    49,    58,    59,    28,   300,    86,    87,    88,
      89,    66,    37,    68,    47,    47,    71,    48,    86,    87,
      88,    89,    82,    36,    25,    32,    82,    80,    49,    86,
      87,    88,    89,    82,    82,    86,    87,    88,    89,     4,
      16,    82,    48,    67,    48,    73,    48,    15,    48,    48,
       4,    49,    74,     6,    14,    82,    47,    49,    74,    11,
      80,    49,    10,    82,    74,    49,    13,    49,    80,    74,
      12,    74,    80,    48,    18,   187,   226,   239,   199,   216,
     265,   175,   305,   300,   294,   195,   279,    -1,   197,   165
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    20,    35,    40,    41,    42,    43,    44,    45,
      46,    50,    51,    52,    58,    59,    66,    68,    71,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     104,   106,   107,   113,   119,   120,   123,   135,   144,   145,
     146,    30,    37,    39,    57,    82,    37,    39,     7,     8,
       9,    18,    21,    22,    23,    24,    32,    47,    80,    81,
      82,    83,    84,    87,    88,   117,   137,   138,   139,   137,
      38,    61,    63,    82,    82,    69,    93,     0,     3,   147,
      39,    82,    82,    39,    82,    82,    47,    47,    47,    47,
      47,    47,    47,    47,   123,   137,    60,   138,    31,    82,
      86,    87,    88,    89,   136,    63,    82,    82,    66,    74,
      70,    82,    31,    47,   123,    67,    82,    67,   138,   138,
     138,   138,   138,   138,   138,   138,    48,    48,    82,    88,
      82,   138,   138,   138,   138,    49,    82,   131,   133,   140,
      62,    64,   141,   122,   139,   138,    83,    67,   123,    82,
     109,    82,    67,    82,    49,    49,    49,    48,    48,    48,
      48,    48,   137,    31,    82,    49,   132,    34,   134,    47,
     115,    25,    28,   138,   142,    49,   121,    74,    61,    82,
      26,    53,    54,    55,    56,    57,   112,    49,   108,    47,
      82,   138,   138,   138,    82,   133,   141,    27,   138,    49,
     114,    28,   138,    17,    25,    29,    33,    74,    75,    76,
      77,    78,    79,   143,    65,    85,   122,   141,   138,    37,
      47,    25,    32,   110,    47,   110,   109,    48,    82,    47,
      48,    48,    48,   132,    36,   129,   140,    49,   116,   115,
     138,    17,    29,    25,   138,   142,   142,   121,    82,    82,
      32,    80,   111,   108,    31,    72,   118,   123,    49,   105,
      82,     4,    16,   128,    67,   138,    48,   114,   105,    48,
     123,    73,    82,    48,   105,   139,   142,    15,   125,   142,
     116,    48,   110,    74,   105,    48,    49,   130,     4,     6,
     124,   134,    82,    14,   139,   127,   138,    80,    47,   130,
      49,   126,    19,    20,    11,   127,    74,   126,    82,    49,
      10,    74,     7,     8,     9,   103,    49,    13,    74,    80,
      49,    12,    74,    80,    48
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    91,    92,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   103,   103,   104,   104,   104,   105,   105,
     106,   107,   107,   107,   107,   107,   108,   108,   109,   109,
     109,   110,   110,   110,   111,   112,   112,   112,   112,   112,
     113,   114,   114,   115,   116,   116,   117,   117,   117,   117,
     117,   118,   118,   119,   120,   121,   121,   122,   123,   124,
     124,   125,   125,   126,   126,   127,   127,   127,   128,   128,
     129,   129,   130,   130,   131,   132,   132,   133,   134,   134,
     135,   136,   136,   136,   137,   137,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   139,   139,   139,   140,   140,   140,
     141,   141,   142,   142,   142,   142,   142,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   144,
     145,   146,   147,   147
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     1,     1,     1,     9,    10,    28,     0,     3,
       5,     8,     5,     4,     9,     8,     0,     3,     6,     3,
       3,     0,     1,     2,     1,     1,     1,     1,     1,     1,
       6,     0,     3,     4,     0,     3,     1,     1,     1,     1,
       1,     0,     4,     4,     6,     0,     3,     3,    10,     0,
       2,     0,     4,     0,     3,     1,     2,     2,     0,     2,
       0,     4,     0,     3,     1,     0,     3,     2,     0,     6,
       2,     0,     1,     2,     2,     4,     3,     3,     3,     3,
       3,     2,     1,     1,     1,     3,     4,     4,     4,     4,
       4,     6,     6,     6,     1,     3,     3,     1,     2,     3,
       0,     2,     3,     3,     3,     2,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     2,     7,
       2,     4,     0,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 304 "yacc_sql.y"
  {
    std::shared_ptr<ParsedSqlNode> sql_node = std::shared_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1958 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 334 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1967 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 340 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1975 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 345 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1983 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 351 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1991 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 357 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1999 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 363 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 2007 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 369 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2017 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 376 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 2025 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC_T ID  */
#line 382 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2035 "yacc_sql.cpp"
    break;

  case 32: /* vector_function_type: L2_DISTANCE_T  */
#line 391 "yacc_sql.y"
  {
    (yyval.number) = static_cast<int>(VectorFunctionExpr::Type::L2_DISTANCE);
  }
#line 2043 "yacc_sql.cpp"
    break;

  case 33: /* vector_function_type: COSINE_DISTANCE_T  */
#line 395 "yacc_sql.y"
  {
    (yyval.number) = static_cast<int>(VectorFunctionExpr::Type::COSINE_DISTANCE);
  }
#line 2051 "yacc_sql.cpp"
    break;

  case 34: /* vector_function_type: INNER_PRODUCT_T  */
#line 399 "yacc_sql.y"
  {
    (yyval.number) = static_cast<int>(VectorFunctionExpr::Type::INNER_PRODUCT);
  }
#line 2059 "yacc_sql.cpp"
    break;

  case 35: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID index_attr_list RBRACE  */
#line 406 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);

      if ((yyvsp[-1].index_attr_list) != nullptr) {
        create_index.attr_names = *(yyvsp[-1].index_attr_list);
        delete (yyvsp[-1].index_attr_list);
      }

      create_index.unique = false;
      create_index.attr_names.push_back(std::string((yyvsp[-2].string)));
      std::reverse(create_index.attr_names.begin(), create_index.attr_names.end());
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 2082 "yacc_sql.cpp"
    break;

  case 36: /* create_index_stmt: CREATE UNIQUE_T INDEX ID ON ID LBRACE ID index_attr_list RBRACE  */
#line 425 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);

      if ((yyvsp[-1].index_attr_list) != nullptr) {
        create_index.attr_names = *(yyvsp[-1].index_attr_list);
        delete (yyvsp[-1].index_attr_list);
      }

      create_index.unique = true;
      create_index.attr_names.push_back(std::string((yyvsp[-2].string)));
      std::reverse(create_index.attr_names.begin(), create_index.attr_names.end());
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 2105 "yacc_sql.cpp"
    break;

  case 37: /* create_index_stmt: CREATE VECTOR_T INDEX ID ON ID LBRACE ID index_attr_list RBRACE WITH_T LBRACE TYPE_T EQ ID COMMA DISTANCE_T EQ vector_function_type COMMA LISTS_T EQ NUMBER COMMA PROBES_T EQ NUMBER RBRACE  */
#line 444 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-24].string);
      create_index.relation_name = (yyvsp[-22].string);

      if ((yyvsp[-19].index_attr_list) != nullptr) {
        create_index.attr_names = *(yyvsp[-19].index_attr_list);
        delete (yyvsp[-19].index_attr_list);
      }

      create_index.unique = false;
      create_index.is_vector_index = true;
      create_index.attr_names.push_back(std::string((yyvsp[-20].string)));
      std::reverse(create_index.attr_names.begin(), create_index.attr_names.end());
      create_index.distance_type = static_cast<int>((yyvsp[-9].number));
      create_index.algorithm_type = (yyvsp[-13].string);
      create_index.lists = (yyvsp[-5].number);
      create_index.probes = (yyvsp[-1].number);
      free((yyvsp[-13].string));
      // free($19);
      free((yyvsp[-24].string));
      free((yyvsp[-22].string));
      free((yyvsp[-20].string));
    }
#line 2135 "yacc_sql.cpp"
    break;

  case 38: /* index_attr_list: %empty  */
#line 473 "yacc_sql.y"
  {
    (yyval.index_attr_list) = nullptr;
  }
#line 2143 "yacc_sql.cpp"
    break;

  case 39: /* index_attr_list: COMMA ID index_attr_list  */
#line 477 "yacc_sql.y"
  {
    if ((yyvsp[0].index_attr_list) != nullptr) {
      (yyval.index_attr_list) = (yyvsp[0].index_attr_list);
    } else {
      (yyval.index_attr_list) = new std::vector<std::string>();
    }
    (yyval.index_attr_list)->emplace_back(std::string((yyvsp[-1].string)));
    free((yyvsp[-1].string));
  }
#line 2157 "yacc_sql.cpp"
    break;

  case 40: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 490 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2169 "yacc_sql.cpp"
    break;

  case 41: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 501 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-2].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-3].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-3].attr_info);
      if ((yyvsp[0].string) != nullptr) {
        create_table.storage_format = (yyvsp[0].string);
        free((yyvsp[0].string));
      }
    }
#line 2194 "yacc_sql.cpp"
    break;

  case 42: /* create_table_stmt: CREATE TABLE ID AS_T select_stmt  */
#line 522 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      (yyval.sql_node)->create_table.relation_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2205 "yacc_sql.cpp"
    break;

  case 43: /* create_table_stmt: CREATE TABLE ID select_stmt  */
#line 529 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      (yyval.sql_node)->create_table.relation_name = (yyvsp[-1].string);
      free((yyvsp[-1].string));
    }
#line 2216 "yacc_sql.cpp"
    break;

  case 44: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE AS_T select_stmt  */
#line 536 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      (yyval.sql_node)->create_table.relation_name = (yyvsp[-6].string);
      free((yyvsp[-6].string));

      if ((yyvsp[-3].attr_infos) != nullptr) {
        (yyval.sql_node)->create_table.attr_infos.swap(*(yyvsp[-3].attr_infos));
        delete (yyvsp[-3].attr_infos);
      }
      (yyval.sql_node)->create_table.attr_infos.emplace_back(*(yyvsp[-4].attr_info));
      std::reverse((yyval.sql_node)->create_table.attr_infos.begin(), (yyval.sql_node)->create_table.attr_infos.end());
      delete (yyvsp[-4].attr_info);
    }
#line 2235 "yacc_sql.cpp"
    break;

  case 45: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE select_stmt  */
#line 551 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      (yyval.sql_node)->create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));

      if ((yyvsp[-2].attr_infos) != nullptr) {
        (yyval.sql_node)->create_table.attr_infos.swap(*(yyvsp[-2].attr_infos));
        delete (yyvsp[-2].attr_infos);
      }
      (yyval.sql_node)->create_table.attr_infos.emplace_back(*(yyvsp[-3].attr_info));
      std::reverse((yyval.sql_node)->create_table.attr_infos.begin(), (yyval.sql_node)->create_table.attr_infos.end());
      delete (yyvsp[-3].attr_info);
    }
#line 2254 "yacc_sql.cpp"
    break;

  case 46: /* attr_def_list: %empty  */
#line 569 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2262 "yacc_sql.cpp"
    break;

  case 47: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 573 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2276 "yacc_sql.cpp"
    break;

  case 48: /* attr_def: ID type LBRACE number RBRACE attr_def_nullable  */
#line 586 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);

      if ((yyval.attr_info)->type == AttrType::VECTORS) {
        if ((yyval.attr_info)->length > MAX_VECTOR_LENGTH) {
          yyerror (&yylloc, sql_string, sql_result, scanner, "vector length too large");
        }
        (yyval.attr_info)->length *= VECTOR_UNIT_SIZE;
      }
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-5].string));
    }
#line 2296 "yacc_sql.cpp"
    break;

  case 49: /* attr_def: ID type attr_def_nullable  */
#line 602 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2309 "yacc_sql.cpp"
    break;

  case 50: /* attr_def: ID TEXT_T attr_def_nullable  */
#line 611 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = AttrType::CHARS;
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = TEXT_LENGTH - 1;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2322 "yacc_sql.cpp"
    break;

  case 51: /* attr_def_nullable: %empty  */
#line 622 "yacc_sql.y"
              {
    (yyval.boolean) = true;
  }
#line 2330 "yacc_sql.cpp"
    break;

  case 52: /* attr_def_nullable: NULL_T  */
#line 625 "yacc_sql.y"
           {
    (yyval.boolean) = true;
  }
#line 2338 "yacc_sql.cpp"
    break;

  case 53: /* attr_def_nullable: NOT_T NULL_T  */
#line 628 "yacc_sql.y"
                 {
    (yyval.boolean) = false;
  }
#line 2346 "yacc_sql.cpp"
    break;

  case 54: /* number: NUMBER  */
#line 634 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2352 "yacc_sql.cpp"
    break;

  case 55: /* type: INT_T  */
#line 638 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2358 "yacc_sql.cpp"
    break;

  case 56: /* type: STRING_T  */
#line 639 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2364 "yacc_sql.cpp"
    break;

  case 57: /* type: FLOAT_T  */
#line 640 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2370 "yacc_sql.cpp"
    break;

  case 58: /* type: VECTOR_T  */
#line 641 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2376 "yacc_sql.cpp"
    break;

  case 59: /* type: DATE_T  */
#line 642 "yacc_sql.y"
             { (yyval.number) = static_cast<int>(AttrType::DATES); }
#line 2382 "yacc_sql.cpp"
    break;

  case 60: /* insert_stmt: INSERT INTO ID VALUES insert_value insert_value_list  */
#line 647 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-3].string);
      if ((yyvsp[0].insert_value_list) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[0].insert_value_list));
        delete (yyvsp[0].insert_value_list);
      }
      (yyval.sql_node)->insertion.values.emplace_back(*(yyvsp[-1].value_list));
      std::reverse((yyval.sql_node)->insertion.values.begin(), (yyval.sql_node)->insertion.values.end());
      delete (yyvsp[-1].value_list);
      free((yyvsp[-3].string));
    }
#line 2399 "yacc_sql.cpp"
    break;

  case 61: /* insert_value_list: %empty  */
#line 663 "yacc_sql.y"
  {
    (yyval.insert_value_list) = nullptr;
  }
#line 2407 "yacc_sql.cpp"
    break;

  case 62: /* insert_value_list: COMMA insert_value insert_value_list  */
#line 667 "yacc_sql.y"
  {
    if ((yyvsp[0].insert_value_list) != nullptr) {
      (yyval.insert_value_list) = (yyvsp[0].insert_value_list);
    } else {
      (yyval.insert_value_list) = new std::vector<std::vector<Value>>;
    }

    (yyval.insert_value_list)->emplace_back(*(yyvsp[-1].value_list));
    delete (yyvsp[-1].value_list);
  }
#line 2422 "yacc_sql.cpp"
    break;

  case 63: /* insert_value: LBRACE expression value_list RBRACE  */
#line 680 "yacc_sql.y"
  {
    if ((yyvsp[-1].value_list) != nullptr) {
      (yyval.value_list) = (yyvsp[-1].value_list);
    } else {
      (yyval.value_list) = new std::vector<Value>;
    }
    Value tmp_value;

    if (try_expression_to_value((yyvsp[-2].expression), tmp_value) == false) {
      yyerror (&yylloc, sql_string, sql_result, scanner, "try_expression_to_value failed in value_list");
    }

    (yyval.value_list)->emplace_back(tmp_value);
    std::reverse((yyval.value_list)->begin(), (yyval.value_list)->end());
    delete (yyvsp[-2].expression);
  }
#line 2443 "yacc_sql.cpp"
    break;

  case 64: /* value_list: %empty  */
#line 700 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 2451 "yacc_sql.cpp"
    break;

  case 65: /* value_list: COMMA expression value_list  */
#line 703 "yacc_sql.y"
                                   {
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      Value tmp_value;

      if (try_expression_to_value((yyvsp[-1].expression), tmp_value) == false) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "try_expression_to_value failed in value_list");
      }

      (yyval.value_list)->emplace_back(tmp_value);
      delete (yyvsp[-1].expression);
    }
#line 2471 "yacc_sql.cpp"
    break;

  case 66: /* value: NUMBER  */
#line 721 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2480 "yacc_sql.cpp"
    break;

  case 67: /* value: FLOAT  */
#line 725 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2489 "yacc_sql.cpp"
    break;

  case 68: /* value: SSS  */
#line 729 "yacc_sql.y"
         {
      int len = strlen((yyvsp[0].string)) - 2;
      if (len >= TEXT_LENGTH) {
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsg);
      }

      char *tmp = common::substr((yyvsp[0].string),1,len);
      (yyval.value) = new Value();

      if (Value::cast_vector_from_str(tmp, len, *(yyval.value)) != RC::SUCCESS) {
        (yyval.value)->set_type(AttrType::CHARS);
        (yyval.value)->set_data(tmp, len);
      }

      free(tmp);
      free((yyvsp[0].string));
    }
#line 2511 "yacc_sql.cpp"
    break;

  case 69: /* value: NULL_T  */
#line 746 "yacc_sql.y"
            {
      (yyval.value) = new Value(INT_NULL);
      (yyloc) = (yylsp[0]);
    }
#line 2520 "yacc_sql.cpp"
    break;

  case 70: /* value: VECTOR_VALUE  */
#line 750 "yacc_sql.y"
                  {
      int len = strlen((yyvsp[0].string));
      (yyval.value) = new Value();
      if (Value::cast_vector_from_str((yyvsp[0].string), len, *(yyval.value)) != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "cast failed.");
      }

      free((yyvsp[0].string));
    }
#line 2534 "yacc_sql.cpp"
    break;

  case 71: /* storage_format: %empty  */
#line 763 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2542 "yacc_sql.cpp"
    break;

  case 72: /* storage_format: STORAGE FORMAT EQ ID  */
#line 767 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2550 "yacc_sql.cpp"
    break;

  case 73: /* delete_stmt: DELETE FROM ID where  */
#line 774 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);

      (yyval.sql_node)->deletion.conditions.reset((yyvsp[0].expression));
      free((yyvsp[-1].string));
    }
#line 2562 "yacc_sql.cpp"
    break;

  case 74: /* update_stmt: UPDATE ID SET update_node update_node_list where  */
#line 785 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      if ((yyvsp[-1].update_node_list) != nullptr) {
        (yyval.sql_node)->update.update_list.swap(*(yyvsp[-1].update_node_list));
        delete (yyvsp[-1].update_node_list);
      }

      (yyval.sql_node)->update.conditions.reset((yyvsp[0].expression));
      (yyval.sql_node)->update.rel_name = (yyvsp[-4].string);
      (yyval.sql_node)->update.update_list.push_back(*(yyvsp[-2].update_node));
      std::reverse((yyval.sql_node)->update.update_list.begin(), (yyval.sql_node)->update.update_list.end());

      free((yyvsp[-4].string));
      delete (yyvsp[-2].update_node);
    }
#line 2582 "yacc_sql.cpp"
    break;

  case 75: /* update_node_list: %empty  */
#line 804 "yacc_sql.y"
  {
    (yyval.update_node_list) = nullptr;
  }
#line 2590 "yacc_sql.cpp"
    break;

  case 76: /* update_node_list: COMMA update_node update_node_list  */
#line 808 "yacc_sql.y"
  {
    if ((yyvsp[0].update_node_list) != nullptr) {
      (yyval.update_node_list) = (yyvsp[0].update_node_list);
    } else {
      (yyval.update_node_list) = new std::vector<UpdateNode>();
    }
    (yyval.update_node_list)->push_back(*(yyvsp[-1].update_node));
    delete (yyvsp[-1].update_node);
  }
#line 2604 "yacc_sql.cpp"
    break;

  case 77: /* update_node: rel_attr EQ expression  */
#line 821 "yacc_sql.y"
  {
    (yyval.update_node) = new UpdateNode();
    (yyval.update_node)->rel_attr = *(yyvsp[-2].rel_attr);
    (yyval.update_node)->expression.reset((yyvsp[0].expression));
    delete (yyvsp[-2].rel_attr);
  }
#line 2615 "yacc_sql.cpp"
    break;

  case 78: /* select_stmt: SELECT expression_list FROM from_node from_node_list where group_by having order_by limit  */
#line 831 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-8].expression_list) != nullptr) {
        (yyval.sql_node)->selection.expressions.swap(*(yyvsp[-8].expression_list));
        delete (yyvsp[-8].expression_list);
      }

      if ((yyvsp[-5].join_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-5].join_list));
        delete (yyvsp[-5].join_list);
      }

      (yyval.sql_node)->selection.relations.emplace_back(*(yyvsp[-6].join_node));
      std::reverse((yyval.sql_node)->selection.relations.begin(), (yyval.sql_node)->selection.relations.end());
      delete (yyvsp[-6].join_node);

      (yyval.sql_node)->selection.conditions.reset((yyvsp[-4].expression));

      if ((yyvsp[-3].rel_attr_list) != nullptr) {
        (yyval.sql_node)->selection.group_by.swap(*(yyvsp[-3].rel_attr_list));
        delete (yyvsp[-3].rel_attr_list);
      }

      if ((yyvsp[-2].expression) != nullptr) {
        // 这里不能delete，而是把所有权交给了having
        (yyval.sql_node)->selection.having = std::shared_ptr<Expression>((yyvsp[-2].expression));
        // delete $8;
      }

      if ((yyvsp[-1].order_by_list) != nullptr) {
        (yyval.sql_node)->selection.order_by = *(yyvsp[-1].order_by_list);
        delete (yyvsp[-1].order_by_list);
      }

      (yyval.sql_node)->selection.limit = (yyvsp[0].number);
    }
#line 2656 "yacc_sql.cpp"
    break;

  case 79: /* limit: %empty  */
#line 871 "yacc_sql.y"
  {
    (yyval.number) = INT32_MAX;
  }
#line 2664 "yacc_sql.cpp"
    break;

  case 80: /* limit: LIMIT_T NUMBER  */
#line 875 "yacc_sql.y"
  {
    (yyval.number) = (yyvsp[0].number);
  }
#line 2672 "yacc_sql.cpp"
    break;

  case 81: /* order_by: %empty  */
#line 882 "yacc_sql.y"
  {
    (yyval.order_by_list) = nullptr;
  }
#line 2680 "yacc_sql.cpp"
    break;

  case 82: /* order_by: ORDER_T BY order_by_node order_by_list  */
#line 886 "yacc_sql.y"
  {
    if ((yyvsp[0].order_by_list) != nullptr) {
      (yyval.order_by_list) = (yyvsp[0].order_by_list);
    } else {
      (yyval.order_by_list) = new std::vector<OrderByNode>();
    }
    (yyval.order_by_list)->push_back(*(yyvsp[-1].order_by_node));
    delete (yyvsp[-1].order_by_node);
    std::reverse((yyval.order_by_list)->begin(), (yyval.order_by_list)->end());
  }
#line 2695 "yacc_sql.cpp"
    break;

  case 83: /* order_by_list: %empty  */
#line 900 "yacc_sql.y"
  {
    (yyval.order_by_list) = nullptr;
  }
#line 2703 "yacc_sql.cpp"
    break;

  case 84: /* order_by_list: COMMA order_by_node order_by_list  */
#line 904 "yacc_sql.y"
  {
    if ((yyvsp[0].order_by_list) != nullptr) {
      (yyval.order_by_list) = (yyvsp[0].order_by_list);
    } else {
      (yyval.order_by_list) = new std::vector<OrderByNode>();
    }
    (yyval.order_by_list)->push_back(*(yyvsp[-1].order_by_node));
    delete (yyvsp[-1].order_by_node);
  }
#line 2717 "yacc_sql.cpp"
    break;

  case 85: /* order_by_node: expression  */
#line 917 "yacc_sql.y"
  {
    (yyval.order_by_node) = new OrderByNode();
    (yyval.order_by_node)->order_by_expression.reset((yyvsp[0].expression));
    (yyval.order_by_node)->type = OrderByType::ASC;
  }
#line 2727 "yacc_sql.cpp"
    break;

  case 86: /* order_by_node: expression ASC_T  */
#line 923 "yacc_sql.y"
  {
    (yyval.order_by_node) = new OrderByNode();
    (yyval.order_by_node)->order_by_expression.reset((yyvsp[-1].expression));
    (yyval.order_by_node)->type = OrderByType::ASC;
  }
#line 2737 "yacc_sql.cpp"
    break;

  case 87: /* order_by_node: expression DESC_T  */
#line 929 "yacc_sql.y"
  {
    (yyval.order_by_node) = new OrderByNode();
    (yyval.order_by_node)->order_by_expression.reset((yyvsp[-1].expression));
    (yyval.order_by_node)->type = OrderByType::DESC;
  }
#line 2747 "yacc_sql.cpp"
    break;

  case 88: /* having: %empty  */
#line 938 "yacc_sql.y"
  {
    (yyval.expression) = nullptr;
  }
#line 2755 "yacc_sql.cpp"
    break;

  case 89: /* having: HAVING_T condition_list  */
#line 942 "yacc_sql.y"
  {
    (yyval.expression) = (yyvsp[0].expression);
  }
#line 2763 "yacc_sql.cpp"
    break;

  case 90: /* group_by: %empty  */
#line 949 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2771 "yacc_sql.cpp"
    break;

  case 91: /* group_by: GROUP BY rel_attr rel_attr_list  */
#line 952 "yacc_sql.y"
                                      {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>();
      }

      (yyval.rel_attr_list)->push_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
      std::reverse((yyval.rel_attr_list)->begin(), (yyval.rel_attr_list)->end());
    }
#line 2787 "yacc_sql.cpp"
    break;

  case 92: /* rel_attr_list: %empty  */
#line 967 "yacc_sql.y"
  {
    (yyval.rel_attr_list) = nullptr;
  }
#line 2795 "yacc_sql.cpp"
    break;

  case 93: /* rel_attr_list: COMMA rel_attr rel_attr_list  */
#line 971 "yacc_sql.y"
  {
    if ((yyvsp[0].rel_attr_list) == nullptr) {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>();
    } else {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
    }
    (yyval.rel_attr_list)->push_back(*(yyvsp[-1].rel_attr));
    delete (yyvsp[-1].rel_attr);
  }
#line 2809 "yacc_sql.cpp"
    break;

  case 94: /* from_node: join_node  */
#line 983 "yacc_sql.y"
            {
    (yyval.join_node) = (yyvsp[0].join_node);
  }
#line 2817 "yacc_sql.cpp"
    break;

  case 95: /* from_node_list: %empty  */
#line 989 "yacc_sql.y"
              {
    (yyval.join_list) = nullptr;
  }
#line 2825 "yacc_sql.cpp"
    break;

  case 96: /* from_node_list: COMMA join_node from_node_list  */
#line 992 "yacc_sql.y"
                                   {
    if ((yyvsp[0].join_list) != nullptr ) {
      (yyval.join_list) = (yyvsp[0].join_list);
    } else {
      (yyval.join_list) = new std::vector<JoinSqlNode>;
    }
    (yyval.join_list)->emplace_back(*(yyvsp[-1].join_node));
    delete (yyvsp[-1].join_node);
  }
#line 2839 "yacc_sql.cpp"
    break;

  case 97: /* join_node: relation join_list  */
#line 1004 "yacc_sql.y"
                     {
    if ((yyvsp[0].join_node) != nullptr) {
      (yyval.join_node) = (yyvsp[0].join_node);
    } else {
      (yyval.join_node) =  new JoinSqlNode;
    }

    std::swap((yyval.join_node)->first_rel, *(yyvsp[-1].rel_attr));
    std::reverse((yyval.join_node)->join_rel_list.begin(), (yyval.join_node)->join_rel_list.end());
    std::reverse((yyval.join_node)->conditions.begin(), (yyval.join_node)->conditions.end());
    delete (yyvsp[-1].rel_attr);
  }
#line 2856 "yacc_sql.cpp"
    break;

  case 98: /* join_list: %empty  */
#line 1019 "yacc_sql.y"
              {
    (yyval.join_node) = nullptr;
  }
#line 2864 "yacc_sql.cpp"
    break;

  case 99: /* join_list: INNER_T JOIN_T relation ON condition_list join_list  */
#line 1022 "yacc_sql.y"
                                                        {
    if ((yyvsp[0].join_node) != nullptr) {
      (yyval.join_node) = (yyvsp[0].join_node);
    } else {
      (yyval.join_node) = new JoinSqlNode;
    }
    (yyval.join_node)->join_rel_list.emplace_back(*(yyvsp[-3].rel_attr));
    (yyval.join_node)->conditions.emplace_back((yyvsp[-1].expression));
    delete (yyvsp[-3].rel_attr);
  }
#line 2879 "yacc_sql.cpp"
    break;

  case 100: /* calc_stmt: CALC expression_list  */
#line 1036 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2889 "yacc_sql.cpp"
    break;

  case 101: /* alias_name: %empty  */
#line 1045 "yacc_sql.y"
  {
    (yyval.string) = nullptr;
  }
#line 2897 "yacc_sql.cpp"
    break;

  case 102: /* alias_name: ID  */
#line 1049 "yacc_sql.y"
  {
    (yyval.string) = (yyvsp[0].string);
  }
#line 2905 "yacc_sql.cpp"
    break;

  case 103: /* alias_name: AS_T ID  */
#line 1053 "yacc_sql.y"
  {
    (yyval.string) = (yyvsp[0].string);
  }
#line 2913 "yacc_sql.cpp"
    break;

  case 104: /* expression_list: expression alias_name  */
#line 1060 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::shared_ptr<Expression>>;

      if ((yyvsp[0].string) != nullptr) {
        (yyvsp[-1].expression)->set_alias((yyvsp[0].string));
        free((yyvsp[0].string));
      }
      (yyval.expression_list)->emplace_back((yyvsp[-1].expression));
    }
#line 2927 "yacc_sql.cpp"
    break;

  case 105: /* expression_list: expression alias_name COMMA expression_list  */
#line 1070 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<std::shared_ptr<Expression>>;
      }

      if ((yyvsp[-2].string) != nullptr) {
        (yyvsp[-3].expression)->set_alias((yyvsp[-2].string));
        free((yyvsp[-2].string));
      }
      (yyval.expression_list)->emplace((yyval.expression_list)->begin(), (yyvsp[-3].expression));
    }
#line 2945 "yacc_sql.cpp"
    break;

  case 106: /* expression: expression '+' expression  */
#line 1086 "yacc_sql.y"
                              {
      // assert_is_not_aggregate($1);
      // assert_is_not_aggregate($3);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2955 "yacc_sql.cpp"
    break;

  case 107: /* expression: expression '-' expression  */
#line 1091 "yacc_sql.y"
                                {
      // assert_is_not_aggregate($1);
      // assert_is_not_aggregate($3);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2965 "yacc_sql.cpp"
    break;

  case 108: /* expression: expression '*' expression  */
#line 1096 "yacc_sql.y"
                                {
      // // assert_is_not_aggregate($1);
      // // assert_is_not_aggregate($3);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2975 "yacc_sql.cpp"
    break;

  case 109: /* expression: expression '/' expression  */
#line 1101 "yacc_sql.y"
                                {
      // // assert_is_not_aggregate($1);
      // // assert_is_not_aggregate($3);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2985 "yacc_sql.cpp"
    break;

  case 110: /* expression: LBRACE expression_list RBRACE  */
#line 1106 "yacc_sql.y"
                                    {
      // for (auto &x : *$2) {
      //  // assert_is_not_aggregate(x.get());
      // }

      if ((yyvsp[-1].expression_list)->size() == 1) {
        (yyval.expression) = (yyvsp[-1].expression_list)->front().get();
        // delete $2;
        // TODO: 这里没有delete，会造成内存泄漏，但是懒得整了。
        // 已解决，构造释放函数。-> 好吧，没解决
      } else {
        (yyval.expression) = new SubQuery_ValueList_Expression(*(yyvsp[-1].expression_list));
        delete (yyvsp[-1].expression_list);
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 3006 "yacc_sql.cpp"
    break;

  case 111: /* expression: '-' expression  */
#line 1122 "yacc_sql.y"
                                  {
      // // assert_is_not_aggregate($2);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 3015 "yacc_sql.cpp"
    break;

  case 112: /* expression: value  */
#line 1126 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 3025 "yacc_sql.cpp"
    break;

  case 113: /* expression: '*'  */
#line 1131 "yacc_sql.y"
          {
      (yyval.expression) = new FieldExpr("*", "*", "*");
    }
#line 3033 "yacc_sql.cpp"
    break;

  case 114: /* expression: rel_attr  */
#line 1134 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new FieldExpr(node->relation_name, node->attribute_name, node->alias);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 3044 "yacc_sql.cpp"
    break;

  case 115: /* expression: LBRACE select_stmt RBRACE  */
#line 1141 "yacc_sql.y"
    {
      (yyval.expression) = new SubQuery_ValueList_Expression(std::make_shared<SelectSqlNode>((yyvsp[-1].sql_node)->selection));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[-1].sql_node);
    }
#line 3054 "yacc_sql.cpp"
    break;

  case 116: /* expression: SUM_T LBRACE expression RBRACE  */
#line 1147 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::SUM, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
#line 3076 "yacc_sql.cpp"
    break;

  case 117: /* expression: AVG_T LBRACE expression RBRACE  */
#line 1165 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::AVG, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
#line 3098 "yacc_sql.cpp"
    break;

  case 118: /* expression: MIN_T LBRACE expression RBRACE  */
#line 1183 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::MIN, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
#line 3120 "yacc_sql.cpp"
    break;

  case 119: /* expression: MAX_T LBRACE expression RBRACE  */
#line 1201 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::MAX, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
#line 3142 "yacc_sql.cpp"
    break;

  case 120: /* expression: COUNT_T LBRACE expression RBRACE  */
#line 1219 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::COUNT, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));

      /*
      if ($5 != nullptr) {
        $$->set_name($5);
        free($5);
      }
      */
    }
#line 3164 "yacc_sql.cpp"
    break;

  case 121: /* expression: L2_DISTANCE_T LBRACE expression COMMA expression RBRACE  */
#line 1237 "yacc_sql.y"
    {
      (yyval.expression) = new VectorFunctionExpr((yyvsp[-3].expression), (yyvsp[-1].expression), VectorFunctionExpr::Type::L2_DISTANCE);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 3173 "yacc_sql.cpp"
    break;

  case 122: /* expression: COSINE_DISTANCE_T LBRACE expression COMMA expression RBRACE  */
#line 1242 "yacc_sql.y"
    {
      (yyval.expression) = new VectorFunctionExpr((yyvsp[-3].expression), (yyvsp[-1].expression), VectorFunctionExpr::Type::COSINE_DISTANCE);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 3182 "yacc_sql.cpp"
    break;

  case 123: /* expression: INNER_PRODUCT_T LBRACE expression COMMA expression RBRACE  */
#line 1247 "yacc_sql.y"
    {
      (yyval.expression) = new VectorFunctionExpr((yyvsp[-3].expression), (yyvsp[-1].expression), VectorFunctionExpr::Type::INNER_PRODUCT);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 3191 "yacc_sql.cpp"
    break;

  case 124: /* rel_attr: ID  */
#line 1255 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      // $$->order_by_type = OrderByType::ASC;
      free((yyvsp[0].string));
    }
#line 3202 "yacc_sql.cpp"
    break;

  case 125: /* rel_attr: ID DOT ID  */
#line 1325 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      // $$->order_by_type = OrderByType::ASC;
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 3215 "yacc_sql.cpp"
    break;

  case 126: /* rel_attr: ID DOT '*'  */
#line 1333 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = "*";
      // $$->order_by_type = OrderByType::ASC;
      free((yyvsp[-2].string));
    }
#line 3227 "yacc_sql.cpp"
    break;

  case 127: /* relation: ID  */
#line 1495 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 3237 "yacc_sql.cpp"
    break;

  case 128: /* relation: ID ID  */
#line 1500 "yacc_sql.y"
            {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name = (yyvsp[-1].string);
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      free((yyvsp[-1].string));
      free((yyvsp[0].string));
    }
#line 3249 "yacc_sql.cpp"
    break;

  case 129: /* relation: ID AS_T ID  */
#line 1507 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name = (yyvsp[-2].string);
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 3261 "yacc_sql.cpp"
    break;

  case 130: /* where: %empty  */
#line 1538 "yacc_sql.y"
    {
      (yyval.expression) = nullptr;
    }
#line 3269 "yacc_sql.cpp"
    break;

  case 131: /* where: WHERE condition_list  */
#line 1541 "yacc_sql.y"
                           {
      (yyval.expression) = (yyvsp[0].expression);
    }
#line 3277 "yacc_sql.cpp"
    break;

  case 132: /* condition_list: expression comp_op expression  */
#line 1548 "yacc_sql.y"
    {
      (yyval.expression) = new ComparisonExpr((yyvsp[-1].comp), std::shared_ptr<Expression>((yyvsp[-2].expression)), std::shared_ptr<Expression>((yyvsp[0].expression)));
    }
#line 3285 "yacc_sql.cpp"
    break;

  case 133: /* condition_list: condition_list AND condition_list  */
#line 1552 "yacc_sql.y"
    {
      std::vector<std::shared_ptr<Expression>> tmp;
      tmp.push_back(std::shared_ptr<Expression>((yyvsp[-2].expression)));
      tmp.push_back(std::shared_ptr<Expression>((yyvsp[0].expression)));
      (yyval.expression) = new ConjunctionExpr(ConjunctionExpr::Type::AND, tmp);
    }
#line 3296 "yacc_sql.cpp"
    break;

  case 134: /* condition_list: condition_list OR condition_list  */
#line 1559 "yacc_sql.y"
    {
      std::vector<std::shared_ptr<Expression>> tmp;
      tmp.push_back(std::shared_ptr<Expression>((yyvsp[-2].expression)));
      tmp.push_back(std::shared_ptr<Expression>((yyvsp[0].expression)));
      (yyval.expression) = new ConjunctionExpr(ConjunctionExpr::Type::OR, tmp);
    }
#line 3307 "yacc_sql.cpp"
    break;

  case 135: /* condition_list: EXISTS_T expression  */
#line 1566 "yacc_sql.y"
    {
      (yyval.expression) = new ComparisonExpr(EXISTS, std::shared_ptr<Expression>((yyvsp[0].expression)), nullptr);
    }
#line 3315 "yacc_sql.cpp"
    break;

  case 136: /* condition_list: NOT_T EXISTS_T expression  */
#line 1570 "yacc_sql.y"
    {
      (yyval.expression) = new ComparisonExpr(NOT_EXISTS, std::shared_ptr<Expression>((yyvsp[0].expression)), nullptr);
    }
#line 3323 "yacc_sql.cpp"
    break;

  case 137: /* comp_op: EQ  */
#line 1576 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3329 "yacc_sql.cpp"
    break;

  case 138: /* comp_op: LT  */
#line 1577 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3335 "yacc_sql.cpp"
    break;

  case 139: /* comp_op: GT  */
#line 1578 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3341 "yacc_sql.cpp"
    break;

  case 140: /* comp_op: LE  */
#line 1579 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3347 "yacc_sql.cpp"
    break;

  case 141: /* comp_op: GE  */
#line 1580 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3353 "yacc_sql.cpp"
    break;

  case 142: /* comp_op: NE  */
#line 1581 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3359 "yacc_sql.cpp"
    break;

  case 143: /* comp_op: LIKE_T  */
#line 1582 "yacc_sql.y"
             { (yyval.comp) = LIKE; }
#line 3365 "yacc_sql.cpp"
    break;

  case 144: /* comp_op: NOT_T LIKE_T  */
#line 1583 "yacc_sql.y"
                   { (yyval.comp) = NOT_LIKE; }
#line 3371 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: IN_T  */
#line 1584 "yacc_sql.y"
           { (yyval.comp) = IN; }
#line 3377 "yacc_sql.cpp"
    break;

  case 146: /* comp_op: NOT_T IN_T  */
#line 1585 "yacc_sql.y"
                 { (yyval.comp) = NOT_IN; }
#line 3383 "yacc_sql.cpp"
    break;

  case 147: /* comp_op: IS_T  */
#line 1586 "yacc_sql.y"
           { (yyval.comp) = IS; }
#line 3389 "yacc_sql.cpp"
    break;

  case 148: /* comp_op: IS_T NOT_T  */
#line 1587 "yacc_sql.y"
                 { (yyval.comp) = NOT_IS; }
#line 3395 "yacc_sql.cpp"
    break;

  case 149: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1596 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);

      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3409 "yacc_sql.cpp"
    break;

  case 150: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1609 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::shared_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3418 "yacc_sql.cpp"
    break;

  case 151: /* set_variable_stmt: SET ID EQ expression  */
#line 1617 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);

      if (try_expression_to_value((yyvsp[0].expression), (yyval.sql_node)->set_variable.value) == false) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "try_expression_to_value failed in set_variable_stmt");
      }

      // $$->set_variable.value = *$4;
      free((yyvsp[-2].string));
      delete (yyvsp[0].expression);
    }
#line 3435 "yacc_sql.cpp"
    break;


#line 3439 "yacc_sql.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1634 "yacc_sql.y"

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
