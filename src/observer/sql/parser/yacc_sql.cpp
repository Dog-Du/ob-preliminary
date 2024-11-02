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
  YYSYMBOL_ORDER_T = 6,                    /* ORDER_T  */
  YYSYMBOL_HAVING_T = 7,                   /* HAVING_T  */
  YYSYMBOL_LIKE_T = 8,                     /* LIKE_T  */
  YYSYMBOL_COUNT_T = 9,                    /* COUNT_T  */
  YYSYMBOL_ASC_T = 10,                     /* ASC_T  */
  YYSYMBOL_DESC_T = 11,                    /* DESC_T  */
  YYSYMBOL_SUM_T = 12,                     /* SUM_T  */
  YYSYMBOL_MIN_T = 13,                     /* MIN_T  */
  YYSYMBOL_MAX_T = 14,                     /* MAX_T  */
  YYSYMBOL_AVG_T = 15,                     /* AVG_T  */
  YYSYMBOL_NOT_T = 16,                     /* NOT_T  */
  YYSYMBOL_TEXT_T = 17,                    /* TEXT_T  */
  YYSYMBOL_JOIN_T = 18,                    /* JOIN_T  */
  YYSYMBOL_EXISTS_T = 19,                  /* EXISTS_T  */
  YYSYMBOL_IN_T = 20,                      /* IN_T  */
  YYSYMBOL_UNIQUE_T = 21,                  /* UNIQUE_T  */
  YYSYMBOL_AS_T = 22,                      /* AS_T  */
  YYSYMBOL_NULL_T = 23,                    /* NULL_T  */
  YYSYMBOL_IS_T = 24,                      /* IS_T  */
  YYSYMBOL_INNER_T = 25,                   /* INNER_T  */
  YYSYMBOL_DROP = 26,                      /* DROP  */
  YYSYMBOL_GROUP = 27,                     /* GROUP  */
  YYSYMBOL_TABLE = 28,                     /* TABLE  */
  YYSYMBOL_TABLES = 29,                    /* TABLES  */
  YYSYMBOL_INDEX = 30,                     /* INDEX  */
  YYSYMBOL_CALC = 31,                      /* CALC  */
  YYSYMBOL_SELECT = 32,                    /* SELECT  */
  YYSYMBOL_SHOW = 33,                      /* SHOW  */
  YYSYMBOL_SYNC = 34,                      /* SYNC  */
  YYSYMBOL_INSERT = 35,                    /* INSERT  */
  YYSYMBOL_DELETE = 36,                    /* DELETE  */
  YYSYMBOL_UPDATE = 37,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 38,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 39,                    /* RBRACE  */
  YYSYMBOL_COMMA = 40,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 41,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 42,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 43,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 44,                     /* INT_T  */
  YYSYMBOL_DATE_T = 45,                    /* DATE_T  */
  YYSYMBOL_STRING_T = 46,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 47,                   /* FLOAT_T  */
  YYSYMBOL_VECTOR_T = 48,                  /* VECTOR_T  */
  YYSYMBOL_HELP = 49,                      /* HELP  */
  YYSYMBOL_EXIT = 50,                      /* EXIT  */
  YYSYMBOL_DOT = 51,                       /* DOT  */
  YYSYMBOL_INTO = 52,                      /* INTO  */
  YYSYMBOL_VALUES = 53,                    /* VALUES  */
  YYSYMBOL_FROM = 54,                      /* FROM  */
  YYSYMBOL_WHERE = 55,                     /* WHERE  */
  YYSYMBOL_AND = 56,                       /* AND  */
  YYSYMBOL_SET = 57,                       /* SET  */
  YYSYMBOL_ON = 58,                        /* ON  */
  YYSYMBOL_LOAD = 59,                      /* LOAD  */
  YYSYMBOL_DATA = 60,                      /* DATA  */
  YYSYMBOL_INFILE = 61,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 62,                   /* EXPLAIN  */
  YYSYMBOL_STORAGE = 63,                   /* STORAGE  */
  YYSYMBOL_FORMAT = 64,                    /* FORMAT  */
  YYSYMBOL_EQ = 65,                        /* EQ  */
  YYSYMBOL_LT = 66,                        /* LT  */
  YYSYMBOL_GT = 67,                        /* GT  */
  YYSYMBOL_LE = 68,                        /* LE  */
  YYSYMBOL_GE = 69,                        /* GE  */
  YYSYMBOL_NE = 70,                        /* NE  */
  YYSYMBOL_NUMBER = 71,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 72,                     /* FLOAT  */
  YYSYMBOL_ID = 73,                        /* ID  */
  YYSYMBOL_SSS = 74,                       /* SSS  */
  YYSYMBOL_OR = 75,                        /* OR  */
  YYSYMBOL_76_ = 76,                       /* '+'  */
  YYSYMBOL_77_ = 77,                       /* '-'  */
  YYSYMBOL_78_ = 78,                       /* '*'  */
  YYSYMBOL_79_ = 79,                       /* '/'  */
  YYSYMBOL_UMINUS = 80,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 81,                  /* $accept  */
  YYSYMBOL_commands = 82,                  /* commands  */
  YYSYMBOL_command_wrapper = 83,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 84,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 85,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 86,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 87,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 88,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 89,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 90,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 91,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 92,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 93,         /* create_index_stmt  */
  YYSYMBOL_index_attr_list = 94,           /* index_attr_list  */
  YYSYMBOL_drop_index_stmt = 95,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 96,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 97,             /* attr_def_list  */
  YYSYMBOL_attr_def = 98,                  /* attr_def  */
  YYSYMBOL_attr_def_nullable = 99,         /* attr_def_nullable  */
  YYSYMBOL_number = 100,                   /* number  */
  YYSYMBOL_type = 101,                     /* type  */
  YYSYMBOL_insert_stmt = 102,              /* insert_stmt  */
  YYSYMBOL_insert_value_list = 103,        /* insert_value_list  */
  YYSYMBOL_insert_value = 104,             /* insert_value  */
  YYSYMBOL_value_list = 105,               /* value_list  */
  YYSYMBOL_value = 106,                    /* value  */
  YYSYMBOL_storage_format = 107,           /* storage_format  */
  YYSYMBOL_delete_stmt = 108,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 109,              /* update_stmt  */
  YYSYMBOL_update_node_list = 110,         /* update_node_list  */
  YYSYMBOL_update_node = 111,              /* update_node  */
  YYSYMBOL_select_stmt = 112,              /* select_stmt  */
  YYSYMBOL_order_by = 113,                 /* order_by  */
  YYSYMBOL_having = 114,                   /* having  */
  YYSYMBOL_group_by = 115,                 /* group_by  */
  YYSYMBOL_rel_attr_list = 116,            /* rel_attr_list  */
  YYSYMBOL_from_node = 117,                /* from_node  */
  YYSYMBOL_from_node_list = 118,           /* from_node_list  */
  YYSYMBOL_join_node = 119,                /* join_node  */
  YYSYMBOL_join_list = 120,                /* join_list  */
  YYSYMBOL_calc_stmt = 121,                /* calc_stmt  */
  YYSYMBOL_expression_list = 122,          /* expression_list  */
  YYSYMBOL_expression = 123,               /* expression  */
  YYSYMBOL_rel_attr = 124,                 /* rel_attr  */
  YYSYMBOL_relation = 125,                 /* relation  */
  YYSYMBOL_where = 126,                    /* where  */
  YYSYMBOL_condition_list = 127,           /* condition_list  */
  YYSYMBOL_comp_op = 128,                  /* comp_op  */
  YYSYMBOL_load_data_stmt = 129,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 130,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 131,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 132             /* opt_semicolon  */
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
#define YYFINAL  72
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   308

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  81
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  150
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  280

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   331


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
       2,     2,    78,    76,     2,    77,     2,    79,     2,     2,
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
      75,    80
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   285,   285,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   316,   322,   327,   333,   339,   345,   351,
     358,   364,   372,   391,   414,   417,   430,   441,   462,   469,
     476,   491,   510,   513,   526,   535,   544,   556,   559,   562,
     568,   572,   573,   574,   575,   576,   580,   597,   600,   613,
     634,   637,   655,   659,   663,   676,   684,   687,   694,   705,
     725,   728,   741,   751,   790,   793,   808,   811,   819,   822,
     837,   840,   853,   859,   862,   874,   889,   892,   905,   914,
     919,   931,   936,   941,   946,   951,   967,   971,   976,   979,
     985,   991,  1002,  1013,  1024,  1035,  1050,  1056,  1062,  1068,
    1076,  1084,  1092,  1100,  1108,  1116,  1124,  1132,  1140,  1150,
    1160,  1170,  1180,  1190,  1204,  1209,  1216,  1247,  1250,  1256,
    1260,  1267,  1274,  1278,  1285,  1286,  1287,  1288,  1289,  1290,
    1291,  1292,  1293,  1294,  1295,  1296,  1304,  1317,  1325,  1340,
    1341
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
  "CREATE", "ORDER_T", "HAVING_T", "LIKE_T", "COUNT_T", "ASC_T", "DESC_T",
  "SUM_T", "MIN_T", "MAX_T", "AVG_T", "NOT_T", "TEXT_T", "JOIN_T",
  "EXISTS_T", "IN_T", "UNIQUE_T", "AS_T", "NULL_T", "IS_T", "INNER_T",
  "DROP", "GROUP", "TABLE", "TABLES", "INDEX", "CALC", "SELECT", "SHOW",
  "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA",
  "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "DATE_T", "STRING_T",
  "FLOAT_T", "VECTOR_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM",
  "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EXPLAIN",
  "STORAGE", "FORMAT", "EQ", "LT", "GT", "LE", "GE", "NE", "NUMBER",
  "FLOAT", "ID", "SSS", "OR", "'+'", "'-'", "'*'", "'/'", "UMINUS",
  "$accept", "commands", "command_wrapper", "exit_stmt", "help_stmt",
  "sync_stmt", "begin_stmt", "commit_stmt", "rollback_stmt",
  "drop_table_stmt", "show_tables_stmt", "desc_table_stmt",
  "create_index_stmt", "index_attr_list", "drop_index_stmt",
  "create_table_stmt", "attr_def_list", "attr_def", "attr_def_nullable",
  "number", "type", "insert_stmt", "insert_value_list", "insert_value",
  "value_list", "value", "storage_format", "delete_stmt", "update_stmt",
  "update_node_list", "update_node", "select_stmt", "order_by", "having",
  "group_by", "rel_attr_list", "from_node", "from_node_list", "join_node",
  "join_list", "calc_stmt", "expression_list", "expression", "rel_attr",
  "relation", "where", "condition_list", "comp_op", "load_data_stmt",
  "explain_stmt", "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-221)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     170,    90,   -44,    20,   117,   117,     4,  -221,    -5,    12,
     -13,  -221,  -221,  -221,  -221,  -221,    11,    35,   170,    97,
      98,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,    95,    62,    77,  -221,    78,    79,   115,   116,   119,
     120,   122,  -221,    50,  -221,  -221,    21,  -221,   117,  -221,
    -221,  -221,   174,  -221,   107,  -221,    92,    93,   105,   103,
     102,  -221,  -221,  -221,  -221,   109,     6,   111,  -221,   112,
     117,   117,   117,   117,   117,   132,   134,  -221,  -221,   110,
     113,    41,  -221,   117,   117,   117,   117,   117,   125,   127,
     129,   136,   117,   126,   135,   183,   137,  -221,   143,   144,
      69,   100,   146,   158,   169,  -221,  -221,    89,     8,  -221,
    -221,  -221,   -54,   -54,  -221,  -221,   -15,   186,  -221,   203,
     192,    -3,  -221,   193,   173,    65,   187,   167,  -221,   214,
     201,   204,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,   171,    99,   176,  -221,   125,   129,   225,  -221,
     117,   223,   245,   117,    37,   -33,   136,   129,   117,   237,
     228,    96,  -221,  -221,  -221,  -221,  -221,    -2,   137,   229,
     194,   123,  -221,  -221,  -221,   186,   242,   125,   178,   192,
    -221,   117,    65,  -221,    14,  -221,   254,  -221,  -221,  -221,
    -221,  -221,  -221,   117,    -3,    -3,   193,  -221,    65,   198,
     199,   250,  -221,  -221,   205,  -221,   201,   -17,   234,  -221,
    -221,  -221,   271,   270,   220,   117,   240,   223,    65,  -221,
    -221,  -221,    65,  -221,  -221,  -221,  -221,   234,  -221,  -221,
     241,  -221,   183,   217,  -221,  -221,   209,   244,   136,    -3,
     278,    -3,   178,  -221,  -221,   246,    96,  -221,   221,   234,
    -221,   247,   -33,   284,  -221,    61,  -221,  -221,  -221,   216,
    -221,   136,  -221,   136,  -221,  -221,   247,   247,  -221,  -221
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,    26,    27,    28,    24,    23,     0,     0,     0,     0,
     149,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,    31,     0,     0,     0,     0,     0,
       0,     0,    65,     0,    62,    63,   106,    64,     0,    98,
      97,    88,    89,    99,     0,    30,     0,     0,     0,     0,
       0,   147,     1,   150,     2,     0,     0,     0,    29,     0,
       0,     0,     0,     0,     0,     0,     0,   107,   108,     0,
       0,   109,    96,     0,     0,     0,     0,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,   100,    95,   112,   115,   110,
     111,    90,    91,    92,    93,    94,   124,    83,    82,    86,
       0,     0,    68,    70,     0,   148,     0,     0,    38,     0,
      42,     0,    36,   105,   101,   103,   104,   102,   113,   114,
     116,   117,     0,   118,     0,   125,     0,   127,     0,    85,
       0,    57,     0,     0,     0,   128,     0,   127,     0,     0,
       0,    47,    51,    55,    52,    53,    54,    47,     0,     0,
       0,   121,   119,   120,   126,    83,    78,     0,    60,     0,
      56,     0,   132,   140,     0,   142,   144,   134,   135,   136,
     137,   138,   139,     0,     0,     0,    70,    69,    72,     0,
       0,     0,    48,    46,     0,    45,    42,    66,    34,   122,
     123,    84,     0,    76,     0,     0,     0,    57,   133,   141,
     143,   145,   129,   130,   131,    71,   146,    34,    49,    50,
       0,    43,     0,     0,    37,    41,     0,     0,     0,     0,
      74,     0,    60,    59,    58,     0,    47,    40,     0,    34,
      32,    80,    77,     0,    73,    86,    61,    33,    44,     0,
      35,     0,    79,     0,    87,    67,    80,    80,    81,    75
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -221,  -221,   272,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,  -221,  -220,  -221,  -221,    75,   114,  -173,  -221,
    -221,  -221,    66,   106,    42,  -221,  -221,  -221,  -221,    91,
     130,   -50,  -221,  -221,  -221,  -139,  -221,   118,   142,    34,
    -221,     3,    -4,   -99,   121,  -130,  -164,  -221,  -221,  -221,
    -221,  -221
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,   247,    31,    32,   179,   140,   213,   240,
     177,    33,   190,   161,   226,    60,   244,    34,    35,   167,
     133,    36,   264,   250,   223,   272,   127,   157,   128,   159,
      37,    61,   164,    63,   129,   132,   165,   203,    38,    39,
      40,    74
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      62,    62,   134,    85,   215,   242,    47,   154,    64,    48,
      49,    50,    51,   162,   211,     5,   163,   255,   150,   151,
      52,   212,   229,   204,    96,    97,   107,   186,   105,    44,
     152,    87,    88,    65,   230,    53,   214,   207,     5,   270,
     233,   234,   205,    89,   106,   193,   243,    66,    45,    62,
      46,   119,   120,   194,    92,   138,    86,   195,   155,    47,
      68,   196,    48,    49,    50,    51,    67,   134,    54,    55,
      56,    57,    90,    52,    58,    59,   110,   111,   112,   113,
     114,   153,     5,   268,    69,   262,   158,   265,    53,    62,
     122,   123,   124,   125,    91,    70,   121,    72,   135,   148,
     149,    73,   197,   198,   199,   200,   201,   202,   143,   182,
     183,    41,   211,    94,    95,    96,    97,   204,    42,   212,
      43,    54,    55,    56,    57,    75,    47,    58,    59,    48,
      49,    50,    51,   219,   220,    76,   205,   278,   279,   144,
      52,    94,    95,    96,    97,    94,    95,    96,    97,   261,
      77,    78,    79,    80,    81,    53,   188,    82,    83,   192,
      84,    98,   101,   103,   208,    99,   100,   245,   102,   108,
     109,   115,   276,   116,   277,     1,    94,    95,    96,    97,
     130,     2,   104,   117,   131,   145,   118,   228,    54,    55,
      56,    57,   257,   137,    58,    59,     3,   146,   126,   232,
     136,     4,     5,     6,     7,     8,     9,    10,   147,    56,
     139,    11,    12,    13,    93,     5,   141,   142,   225,    14,
      15,   252,    94,    95,    96,    97,   156,    16,   158,    17,
     160,   171,    18,   166,    94,    95,    96,    97,   168,   169,
     170,   178,   180,   187,   181,    94,    95,    96,    97,   184,
      94,    95,    96,    97,    94,    95,    96,    97,   172,   173,
     174,   175,   176,   189,   191,   209,   210,   218,   217,   222,
     231,   236,   237,   238,   246,   248,   239,   249,   251,   253,
     256,   258,   259,   260,   263,   267,   269,   271,   273,   275,
      71,   241,   216,   254,   266,   227,   206,   235,   185,   274,
       0,     0,     0,   221,     0,     0,     0,     0,   224
};

static const yytype_int16 yycheck[] =
{
       4,     5,   101,    53,   177,    22,     9,    22,     5,    12,
      13,    14,    15,    16,    16,    32,    19,   237,    10,    11,
      23,    23,     8,    56,    78,    79,    76,   157,    22,    73,
      22,    10,    11,    29,    20,    38,    38,   167,    32,   259,
     204,   205,    75,    22,    38,     8,    63,    52,    28,    53,
      30,    10,    11,    16,    58,   105,    53,    20,    73,     9,
      73,    24,    12,    13,    14,    15,    54,   166,    71,    72,
      73,    74,    51,    23,    77,    78,    80,    81,    82,    83,
      84,    73,    32,   256,    73,   249,    25,   251,    38,    93,
      94,    95,    96,    97,    73,    60,    93,     0,   102,    10,
      11,     3,    65,    66,    67,    68,    69,    70,    39,    10,
      11,    21,    16,    76,    77,    78,    79,    56,    28,    23,
      30,    71,    72,    73,    74,    30,     9,    77,    78,    12,
      13,    14,    15,    10,    11,    73,    75,   276,   277,    39,
      23,    76,    77,    78,    79,    76,    77,    78,    79,   248,
      73,    73,    73,    38,    38,    38,   160,    38,    38,   163,
      38,    54,    57,    61,   168,    73,    73,   217,    65,    58,
      58,    39,   271,    39,   273,     5,    76,    77,    78,    79,
      53,    11,    73,    73,    55,    39,    73,   191,    71,    72,
      73,    74,   242,    58,    77,    78,    26,    39,    73,   203,
      74,    31,    32,    33,    34,    35,    36,    37,    39,    73,
      73,    41,    42,    43,    40,    32,    73,    73,    40,    49,
      50,   225,    76,    77,    78,    79,    40,    57,    25,    59,
      38,    17,    62,    40,    76,    77,    78,    79,    65,    52,
      73,    40,    38,    18,    73,    76,    77,    78,    79,    73,
      76,    77,    78,    79,    76,    77,    78,    79,    44,    45,
      46,    47,    48,    40,    19,    28,    38,    73,    39,    27,
      16,    73,    73,    23,    40,     4,    71,     7,    58,    39,
      39,    64,    73,    39,     6,    39,    65,    40,     4,    73,
      18,   216,   178,   227,   252,   189,   166,   206,   156,   265,
      -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,   187
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    11,    26,    31,    32,    33,    34,    35,    36,
      37,    41,    42,    43,    49,    50,    57,    59,    62,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    95,    96,   102,   108,   109,   112,   121,   129,   130,
     131,    21,    28,    30,    73,    28,    30,     9,    12,    13,
      14,    15,    23,    38,    71,    72,    73,    74,    77,    78,
     106,   122,   123,   124,   122,    29,    52,    54,    73,    73,
      60,    83,     0,     3,   132,    30,    73,    73,    73,    73,
      38,    38,    38,    38,    38,   112,   122,    10,    11,    22,
      51,    73,   123,    40,    76,    77,    78,    79,    54,    73,
      73,    57,    65,    61,    73,    22,    38,   112,    58,    58,
     123,   123,   123,   123,   123,    39,    39,    73,    73,    10,
      11,   122,   123,   123,   123,   123,    73,   117,   119,   125,
      53,    55,   126,   111,   124,   123,    74,    58,   112,    73,
      98,    73,    73,    39,    39,    39,    39,    39,    10,    11,
      10,    11,    22,    73,    22,    73,    40,   118,    25,   120,
      38,   104,    16,    19,   123,   127,    40,   110,    65,    52,
      73,    17,    44,    45,    46,    47,    48,   101,    40,    97,
      38,    73,    10,    11,    73,   119,   126,    18,   123,    40,
     103,    19,   123,     8,    16,    20,    24,    65,    66,    67,
      68,    69,    70,   128,    56,    75,   111,   126,   123,    28,
      38,    16,    23,    99,    38,    99,    98,    39,    73,    10,
      11,   118,    27,   115,   125,    40,   105,   104,   123,     8,
      20,    16,   123,   127,   127,   110,    73,    73,    23,    71,
     100,    97,    22,    63,   107,   112,    40,    94,     4,     7,
     114,    58,   123,    39,   103,    94,    39,   112,    64,    73,
      39,   124,   127,     6,   113,   127,   105,    39,    99,    65,
      94,    40,   116,     4,   120,    73,   124,   124,   116,   116
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    81,    82,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    93,    94,    94,    95,    96,    96,    96,
      96,    96,    97,    97,    98,    98,    98,    99,    99,    99,
     100,   101,   101,   101,   101,   101,   102,   103,   103,   104,
     105,   105,   106,   106,   106,   106,   107,   107,   108,   109,
     110,   110,   111,   112,   113,   113,   114,   114,   115,   115,
     116,   116,   117,   118,   118,   119,   120,   120,   121,   122,
     122,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   125,   125,   125,   126,   126,   127,
     127,   127,   127,   127,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   129,   130,   131,   132,
     132
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     9,    10,     0,     3,     5,     8,     5,     4,
       9,     8,     0,     3,     6,     3,     3,     0,     1,     2,
       1,     1,     1,     1,     1,     1,     6,     0,     3,     4,
       0,     3,     1,     1,     1,     1,     0,     4,     4,     6,
       0,     3,     3,     9,     0,     4,     0,     2,     0,     4,
       0,     3,     1,     0,     3,     2,     0,     6,     2,     1,
       3,     3,     3,     3,     3,     3,     2,     1,     1,     1,
       3,     4,     4,     4,     4,     4,     1,     2,     2,     2,
       3,     3,     3,     4,     4,     3,     4,     4,     4,     5,
       5,     5,     6,     6,     1,     2,     3,     0,     2,     3,
       3,     3,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     1,     2,     7,     2,     4,     0,
       1
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
#line 286 "yacc_sql.y"
  {
    std::shared_ptr<ParsedSqlNode> sql_node = std::shared_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1909 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 316 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1918 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 322 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1926 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 327 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1934 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 333 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1942 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 339 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1950 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 345 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1958 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 351 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1968 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 358 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1976 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC_T ID  */
#line 364 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1986 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID index_attr_list RBRACE  */
#line 373 "yacc_sql.y"
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
#line 2009 "yacc_sql.cpp"
    break;

  case 33: /* create_index_stmt: CREATE UNIQUE_T INDEX ID ON ID LBRACE ID index_attr_list RBRACE  */
#line 392 "yacc_sql.y"
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
#line 2032 "yacc_sql.cpp"
    break;

  case 34: /* index_attr_list: %empty  */
#line 414 "yacc_sql.y"
  {
    (yyval.index_attr_list) = nullptr;
  }
#line 2040 "yacc_sql.cpp"
    break;

  case 35: /* index_attr_list: COMMA ID index_attr_list  */
#line 418 "yacc_sql.y"
  {
    if ((yyvsp[0].index_attr_list) != nullptr) {
      (yyval.index_attr_list) = (yyvsp[0].index_attr_list);
    } else {
      (yyval.index_attr_list) = new std::vector<std::string>();
    }
    (yyval.index_attr_list)->emplace_back(std::string((yyvsp[-1].string)));
    free((yyvsp[-1].string));
  }
#line 2054 "yacc_sql.cpp"
    break;

  case 36: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 431 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2066 "yacc_sql.cpp"
    break;

  case 37: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 442 "yacc_sql.y"
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
#line 2091 "yacc_sql.cpp"
    break;

  case 38: /* create_table_stmt: CREATE TABLE ID AS_T select_stmt  */
#line 463 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      (yyval.sql_node)->create_table.relation_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2102 "yacc_sql.cpp"
    break;

  case 39: /* create_table_stmt: CREATE TABLE ID select_stmt  */
#line 470 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      (yyval.sql_node)->create_table.relation_name = (yyvsp[-1].string);
      free((yyvsp[-1].string));
    }
#line 2113 "yacc_sql.cpp"
    break;

  case 40: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE AS_T select_stmt  */
#line 477 "yacc_sql.y"
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
#line 2132 "yacc_sql.cpp"
    break;

  case 41: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE select_stmt  */
#line 492 "yacc_sql.y"
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
#line 2151 "yacc_sql.cpp"
    break;

  case 42: /* attr_def_list: %empty  */
#line 510 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2159 "yacc_sql.cpp"
    break;

  case 43: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 514 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2173 "yacc_sql.cpp"
    break;

  case 44: /* attr_def: ID type LBRACE number RBRACE attr_def_nullable  */
#line 527 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-5].string));
    }
#line 2186 "yacc_sql.cpp"
    break;

  case 45: /* attr_def: ID type attr_def_nullable  */
#line 536 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2199 "yacc_sql.cpp"
    break;

  case 46: /* attr_def: ID TEXT_T attr_def_nullable  */
#line 545 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = AttrType::CHARS;
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = TEXT_LENGTH - 1;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2212 "yacc_sql.cpp"
    break;

  case 47: /* attr_def_nullable: %empty  */
#line 556 "yacc_sql.y"
              {
    (yyval.boolean) = true;
  }
#line 2220 "yacc_sql.cpp"
    break;

  case 48: /* attr_def_nullable: NULL_T  */
#line 559 "yacc_sql.y"
           {
    (yyval.boolean) = true;
  }
#line 2228 "yacc_sql.cpp"
    break;

  case 49: /* attr_def_nullable: NOT_T NULL_T  */
#line 562 "yacc_sql.y"
                 {
    (yyval.boolean) = false;
  }
#line 2236 "yacc_sql.cpp"
    break;

  case 50: /* number: NUMBER  */
#line 568 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2242 "yacc_sql.cpp"
    break;

  case 51: /* type: INT_T  */
#line 572 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2248 "yacc_sql.cpp"
    break;

  case 52: /* type: STRING_T  */
#line 573 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2254 "yacc_sql.cpp"
    break;

  case 53: /* type: FLOAT_T  */
#line 574 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2260 "yacc_sql.cpp"
    break;

  case 54: /* type: VECTOR_T  */
#line 575 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2266 "yacc_sql.cpp"
    break;

  case 55: /* type: DATE_T  */
#line 576 "yacc_sql.y"
             { (yyval.number) = static_cast<int>(AttrType::DATES); }
#line 2272 "yacc_sql.cpp"
    break;

  case 56: /* insert_stmt: INSERT INTO ID VALUES insert_value insert_value_list  */
#line 581 "yacc_sql.y"
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
#line 2289 "yacc_sql.cpp"
    break;

  case 57: /* insert_value_list: %empty  */
#line 597 "yacc_sql.y"
  {
    (yyval.insert_value_list) = nullptr;
  }
#line 2297 "yacc_sql.cpp"
    break;

  case 58: /* insert_value_list: COMMA insert_value insert_value_list  */
#line 601 "yacc_sql.y"
  {
    if ((yyvsp[0].insert_value_list) != nullptr) {
      (yyval.insert_value_list) = (yyvsp[0].insert_value_list);
    } else {
      (yyval.insert_value_list) = new std::vector<std::vector<Value>>;
    }

    (yyval.insert_value_list)->emplace_back(*(yyvsp[-1].value_list));
    delete (yyvsp[-1].value_list);
  }
#line 2312 "yacc_sql.cpp"
    break;

  case 59: /* insert_value: LBRACE expression value_list RBRACE  */
#line 614 "yacc_sql.y"
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
#line 2333 "yacc_sql.cpp"
    break;

  case 60: /* value_list: %empty  */
#line 634 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 2341 "yacc_sql.cpp"
    break;

  case 61: /* value_list: COMMA expression value_list  */
#line 637 "yacc_sql.y"
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
#line 2361 "yacc_sql.cpp"
    break;

  case 62: /* value: NUMBER  */
#line 655 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2370 "yacc_sql.cpp"
    break;

  case 63: /* value: FLOAT  */
#line 659 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2379 "yacc_sql.cpp"
    break;

  case 64: /* value: SSS  */
#line 663 "yacc_sql.y"
         {
      int len = strlen((yyvsp[0].string)) - 2;
      if (len >= TEXT_LENGTH) {
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsg);
      }

      char *tmp = common::substr((yyvsp[0].string),1,len);
      (yyval.value) = new Value();
      (yyval.value)->set_type(AttrType::CHARS);
      (yyval.value)->set_data(tmp, len);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2397 "yacc_sql.cpp"
    break;

  case 65: /* value: NULL_T  */
#line 676 "yacc_sql.y"
            {
      (yyval.value) = new Value(INT_NULL);
      (yyloc) = (yylsp[0]);
    }
#line 2406 "yacc_sql.cpp"
    break;

  case 66: /* storage_format: %empty  */
#line 684 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2414 "yacc_sql.cpp"
    break;

  case 67: /* storage_format: STORAGE FORMAT EQ ID  */
#line 688 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2422 "yacc_sql.cpp"
    break;

  case 68: /* delete_stmt: DELETE FROM ID where  */
#line 695 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);

      (yyval.sql_node)->deletion.conditions.reset((yyvsp[0].expression));
      free((yyvsp[-1].string));
    }
#line 2434 "yacc_sql.cpp"
    break;

  case 69: /* update_stmt: UPDATE ID SET update_node update_node_list where  */
#line 706 "yacc_sql.y"
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
#line 2454 "yacc_sql.cpp"
    break;

  case 70: /* update_node_list: %empty  */
#line 725 "yacc_sql.y"
  {
    (yyval.update_node_list) = nullptr;
  }
#line 2462 "yacc_sql.cpp"
    break;

  case 71: /* update_node_list: COMMA update_node update_node_list  */
#line 729 "yacc_sql.y"
  {
    if ((yyvsp[0].update_node_list) != nullptr) {
      (yyval.update_node_list) = (yyvsp[0].update_node_list);
    } else {
      (yyval.update_node_list) = new std::vector<UpdateNode>();
    }
    (yyval.update_node_list)->push_back(*(yyvsp[-1].update_node));
    delete (yyvsp[-1].update_node);
  }
#line 2476 "yacc_sql.cpp"
    break;

  case 72: /* update_node: rel_attr EQ expression  */
#line 742 "yacc_sql.y"
  {
    (yyval.update_node) = new UpdateNode();
    (yyval.update_node)->rel_attr = *(yyvsp[-2].rel_attr);
    (yyval.update_node)->expression.reset((yyvsp[0].expression));
    delete (yyvsp[-2].rel_attr);
  }
#line 2487 "yacc_sql.cpp"
    break;

  case 73: /* select_stmt: SELECT expression_list FROM from_node from_node_list where group_by having order_by  */
#line 752 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-7].expression_list) != nullptr) {
        (yyval.sql_node)->selection.expressions.swap(*(yyvsp[-7].expression_list));
        delete (yyvsp[-7].expression_list);
      }

      if ((yyvsp[-4].join_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-4].join_list));
        delete (yyvsp[-4].join_list);
      }

      (yyval.sql_node)->selection.relations.emplace_back(*(yyvsp[-5].join_node));
      std::reverse((yyval.sql_node)->selection.relations.begin(), (yyval.sql_node)->selection.relations.end());
      delete (yyvsp[-5].join_node);

      (yyval.sql_node)->selection.conditions.reset((yyvsp[-3].expression));

      if ((yyvsp[-2].rel_attr_list) != nullptr) {
        (yyval.sql_node)->selection.group_by.swap(*(yyvsp[-2].rel_attr_list));
        delete (yyvsp[-2].rel_attr_list);
      }

      if ((yyvsp[-1].expression) != nullptr) {
        // 这里不能delete，而是把所有权交给了having
        (yyval.sql_node)->selection.having = std::shared_ptr<Expression>((yyvsp[-1].expression));
        // delete $8;
      }

      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.sql_node)->selection.order_by = *(yyvsp[0].rel_attr_list);
        delete (yyvsp[0].rel_attr_list);
      }
    }
#line 2526 "yacc_sql.cpp"
    break;

  case 74: /* order_by: %empty  */
#line 790 "yacc_sql.y"
  {
    (yyval.rel_attr_list) = nullptr;
  }
#line 2534 "yacc_sql.cpp"
    break;

  case 75: /* order_by: ORDER_T BY rel_attr rel_attr_list  */
#line 794 "yacc_sql.y"
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
#line 2549 "yacc_sql.cpp"
    break;

  case 76: /* having: %empty  */
#line 808 "yacc_sql.y"
  {
    (yyval.expression) = nullptr;
  }
#line 2557 "yacc_sql.cpp"
    break;

  case 77: /* having: HAVING_T condition_list  */
#line 812 "yacc_sql.y"
  {
    (yyval.expression) = (yyvsp[0].expression);
  }
#line 2565 "yacc_sql.cpp"
    break;

  case 78: /* group_by: %empty  */
#line 819 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2573 "yacc_sql.cpp"
    break;

  case 79: /* group_by: GROUP BY rel_attr rel_attr_list  */
#line 822 "yacc_sql.y"
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
#line 2589 "yacc_sql.cpp"
    break;

  case 80: /* rel_attr_list: %empty  */
#line 837 "yacc_sql.y"
  {
    (yyval.rel_attr_list) = nullptr;
  }
#line 2597 "yacc_sql.cpp"
    break;

  case 81: /* rel_attr_list: COMMA rel_attr rel_attr_list  */
#line 841 "yacc_sql.y"
  {
    if ((yyvsp[0].rel_attr_list) == nullptr) {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>();
    } else {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
    }
    (yyval.rel_attr_list)->push_back(*(yyvsp[-1].rel_attr));
    delete (yyvsp[-1].rel_attr);
  }
#line 2611 "yacc_sql.cpp"
    break;

  case 82: /* from_node: join_node  */
#line 853 "yacc_sql.y"
            {
    (yyval.join_node) = (yyvsp[0].join_node);
  }
#line 2619 "yacc_sql.cpp"
    break;

  case 83: /* from_node_list: %empty  */
#line 859 "yacc_sql.y"
              {
    (yyval.join_list) = nullptr;
  }
#line 2627 "yacc_sql.cpp"
    break;

  case 84: /* from_node_list: COMMA join_node from_node_list  */
#line 862 "yacc_sql.y"
                                   {
    if ((yyvsp[0].join_list) != nullptr ) {
      (yyval.join_list) = (yyvsp[0].join_list);
    } else {
      (yyval.join_list) = new std::vector<JoinSqlNode>;
    }
    (yyval.join_list)->emplace_back(*(yyvsp[-1].join_node));
    delete (yyvsp[-1].join_node);
  }
#line 2641 "yacc_sql.cpp"
    break;

  case 85: /* join_node: relation join_list  */
#line 874 "yacc_sql.y"
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
#line 2658 "yacc_sql.cpp"
    break;

  case 86: /* join_list: %empty  */
#line 889 "yacc_sql.y"
              {
    (yyval.join_node) = nullptr;
  }
#line 2666 "yacc_sql.cpp"
    break;

  case 87: /* join_list: INNER_T JOIN_T relation ON condition_list join_list  */
#line 892 "yacc_sql.y"
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
#line 2681 "yacc_sql.cpp"
    break;

  case 88: /* calc_stmt: CALC expression_list  */
#line 906 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2691 "yacc_sql.cpp"
    break;

  case 89: /* expression_list: expression  */
#line 915 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::shared_ptr<Expression>>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2700 "yacc_sql.cpp"
    break;

  case 90: /* expression_list: expression COMMA expression_list  */
#line 920 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<std::shared_ptr<Expression>>;
      }
      (yyval.expression_list)->emplace((yyval.expression_list)->begin(), (yyvsp[-2].expression));
    }
#line 2713 "yacc_sql.cpp"
    break;

  case 91: /* expression: expression '+' expression  */
#line 931 "yacc_sql.y"
                              {
      // assert_is_not_aggregate($1);
      // assert_is_not_aggregate($3);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2723 "yacc_sql.cpp"
    break;

  case 92: /* expression: expression '-' expression  */
#line 936 "yacc_sql.y"
                                {
      // assert_is_not_aggregate($1);
      // assert_is_not_aggregate($3);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2733 "yacc_sql.cpp"
    break;

  case 93: /* expression: expression '*' expression  */
#line 941 "yacc_sql.y"
                                {
      // // assert_is_not_aggregate($1);
      // // assert_is_not_aggregate($3);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2743 "yacc_sql.cpp"
    break;

  case 94: /* expression: expression '/' expression  */
#line 946 "yacc_sql.y"
                                {
      // // assert_is_not_aggregate($1);
      // // assert_is_not_aggregate($3);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2753 "yacc_sql.cpp"
    break;

  case 95: /* expression: LBRACE expression_list RBRACE  */
#line 951 "yacc_sql.y"
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
#line 2774 "yacc_sql.cpp"
    break;

  case 96: /* expression: '-' expression  */
#line 967 "yacc_sql.y"
                                  {
      // // assert_is_not_aggregate($2);
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2783 "yacc_sql.cpp"
    break;

  case 97: /* expression: value  */
#line 971 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2793 "yacc_sql.cpp"
    break;

  case 98: /* expression: '*'  */
#line 976 "yacc_sql.y"
          {
      (yyval.expression) = new FieldExpr("*", "*", "*");
    }
#line 2801 "yacc_sql.cpp"
    break;

  case 99: /* expression: rel_attr  */
#line 979 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new FieldExpr(node->relation_name, node->attribute_name, node->alias);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 2812 "yacc_sql.cpp"
    break;

  case 100: /* expression: LBRACE select_stmt RBRACE  */
#line 986 "yacc_sql.y"
    {
      (yyval.expression) = new SubQuery_ValueList_Expression(std::make_shared<SelectSqlNode>((yyvsp[-1].sql_node)->selection));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[-1].sql_node);
    }
#line 2822 "yacc_sql.cpp"
    break;

  case 101: /* expression: SUM_T LBRACE expression RBRACE  */
#line 992 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::SUM, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2837 "yacc_sql.cpp"
    break;

  case 102: /* expression: AVG_T LBRACE expression RBRACE  */
#line 1003 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::AVG, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2852 "yacc_sql.cpp"
    break;

  case 103: /* expression: MIN_T LBRACE expression RBRACE  */
#line 1014 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::MIN, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2867 "yacc_sql.cpp"
    break;

  case 104: /* expression: MAX_T LBRACE expression RBRACE  */
#line 1025 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::MAX, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2882 "yacc_sql.cpp"
    break;

  case 105: /* expression: COUNT_T LBRACE expression RBRACE  */
#line 1036 "yacc_sql.y"
    {
      // assert_is_not_aggregate($3);
      (yyval.expression) = new AggregateExpr(AggregateExpr::Type::COUNT, (yyvsp[-1].expression));
      RC rc = static_cast<AggregateExpr *>((yyval.expression))->create_aggregator();

      if (rc != RC::SUCCESS) {
        yyerror (&yylloc, sql_string, sql_result, scanner, "create_aggregator failed.");
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2897 "yacc_sql.cpp"
    break;

  case 106: /* rel_attr: ID  */
#line 1050 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      free((yyvsp[0].string));
    }
#line 2908 "yacc_sql.cpp"
    break;

  case 107: /* rel_attr: ID ASC_T  */
#line 1056 "yacc_sql.y"
               {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      free((yyvsp[-1].string));
    }
#line 2919 "yacc_sql.cpp"
    break;

  case 108: /* rel_attr: ID DESC_T  */
#line 1062 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->order_by_type = OrderByType::DESC;
      free((yyvsp[-1].string));
    }
#line 2930 "yacc_sql.cpp"
    break;

  case 109: /* rel_attr: ID ID  */
#line 1068 "yacc_sql.y"
            {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      free((yyvsp[0].string));
      free((yyvsp[-1].string));
    }
#line 2943 "yacc_sql.cpp"
    break;

  case 110: /* rel_attr: ID ID ASC_T  */
#line 1076 "yacc_sql.y"
                  {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-2].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      (yyval.rel_attr)->alias = (yyvsp[-1].string);
      free((yyvsp[-1].string));
      free((yyvsp[-2].string));
    }
#line 2956 "yacc_sql.cpp"
    break;

  case 111: /* rel_attr: ID ID DESC_T  */
#line 1084 "yacc_sql.y"
                   {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-2].string);
      (yyval.rel_attr)->order_by_type = OrderByType::DESC;
      (yyval.rel_attr)->alias = (yyvsp[-1].string);
      free((yyvsp[-1].string));
      free((yyvsp[-2].string));
    }
#line 2969 "yacc_sql.cpp"
    break;

  case 112: /* rel_attr: ID AS_T ID  */
#line 1092 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-2].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2982 "yacc_sql.cpp"
    break;

  case 113: /* rel_attr: ID AS_T ID ASC_T  */
#line 1100 "yacc_sql.y"
                       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-3].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      (yyval.rel_attr)->alias = (yyvsp[-1].string);
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 2995 "yacc_sql.cpp"
    break;

  case 114: /* rel_attr: ID AS_T ID DESC_T  */
#line 1108 "yacc_sql.y"
                        {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-3].string);
      (yyval.rel_attr)->order_by_type = OrderByType::DESC;
      (yyval.rel_attr)->alias = (yyvsp[-1].string);
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 3008 "yacc_sql.cpp"
    break;

  case 115: /* rel_attr: ID DOT ID  */
#line 1116 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 3021 "yacc_sql.cpp"
    break;

  case 116: /* rel_attr: ID DOT ID ASC_T  */
#line 1124 "yacc_sql.y"
                      {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-3].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 3034 "yacc_sql.cpp"
    break;

  case 117: /* rel_attr: ID DOT ID DESC_T  */
#line 1132 "yacc_sql.y"
                       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-3].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->order_by_type = OrderByType::DESC;
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 3047 "yacc_sql.cpp"
    break;

  case 118: /* rel_attr: ID DOT ID ID  */
#line 1140 "yacc_sql.y"
                   {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-3].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
      free((yyvsp[0].string));
    }
#line 3062 "yacc_sql.cpp"
    break;

  case 119: /* rel_attr: ID DOT ID ID ASC_T  */
#line 1150 "yacc_sql.y"
                         {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-4].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[-2].string);
      (yyval.rel_attr)->alias = (yyvsp[-1].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
      free((yyvsp[-1].string));
    }
#line 3077 "yacc_sql.cpp"
    break;

  case 120: /* rel_attr: ID DOT ID ID DESC_T  */
#line 1160 "yacc_sql.y"
                          {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-4].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[-2].string);
      (yyval.rel_attr)->alias = (yyvsp[-1].string);
      (yyval.rel_attr)->order_by_type = OrderByType::DESC;
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
      free((yyvsp[-1].string));
    }
#line 3092 "yacc_sql.cpp"
    break;

  case 121: /* rel_attr: ID DOT ID AS_T ID  */
#line 1170 "yacc_sql.y"
                        {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-4].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[-2].string);
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 3107 "yacc_sql.cpp"
    break;

  case 122: /* rel_attr: ID DOT ID AS_T ID ASC_T  */
#line 1180 "yacc_sql.y"
                              {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-5].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[-3].string);
      (yyval.rel_attr)->alias = (yyvsp[-1].string);
      (yyval.rel_attr)->order_by_type = OrderByType::ASC;
      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 3122 "yacc_sql.cpp"
    break;

  case 123: /* rel_attr: ID DOT ID AS_T ID DESC_T  */
#line 1190 "yacc_sql.y"
                               {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-5].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[-3].string);
      (yyval.rel_attr)->alias = (yyvsp[-1].string);
      (yyval.rel_attr)->order_by_type = OrderByType::DESC;
      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 3137 "yacc_sql.cpp"
    break;

  case 124: /* relation: ID  */
#line 1204 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 3147 "yacc_sql.cpp"
    break;

  case 125: /* relation: ID ID  */
#line 1209 "yacc_sql.y"
            {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name = (yyvsp[-1].string);
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      free((yyvsp[-1].string));
      free((yyvsp[0].string));
    }
#line 3159 "yacc_sql.cpp"
    break;

  case 126: /* relation: ID AS_T ID  */
#line 1216 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name = (yyvsp[-2].string);
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 3171 "yacc_sql.cpp"
    break;

  case 127: /* where: %empty  */
#line 1247 "yacc_sql.y"
    {
      (yyval.expression) = nullptr;
    }
#line 3179 "yacc_sql.cpp"
    break;

  case 128: /* where: WHERE condition_list  */
#line 1250 "yacc_sql.y"
                           {
      (yyval.expression) = (yyvsp[0].expression);
    }
#line 3187 "yacc_sql.cpp"
    break;

  case 129: /* condition_list: expression comp_op expression  */
#line 1257 "yacc_sql.y"
    {
      (yyval.expression) = new ComparisonExpr((yyvsp[-1].comp), std::shared_ptr<Expression>((yyvsp[-2].expression)), std::shared_ptr<Expression>((yyvsp[0].expression)));
    }
#line 3195 "yacc_sql.cpp"
    break;

  case 130: /* condition_list: condition_list AND condition_list  */
#line 1261 "yacc_sql.y"
    {
      std::vector<std::shared_ptr<Expression>> tmp;
      tmp.push_back(std::shared_ptr<Expression>((yyvsp[-2].expression)));
      tmp.push_back(std::shared_ptr<Expression>((yyvsp[0].expression)));
      (yyval.expression) = new ConjunctionExpr(ConjunctionExpr::Type::AND, tmp);
    }
#line 3206 "yacc_sql.cpp"
    break;

  case 131: /* condition_list: condition_list OR condition_list  */
#line 1268 "yacc_sql.y"
    {
      std::vector<std::shared_ptr<Expression>> tmp;
      tmp.push_back(std::shared_ptr<Expression>((yyvsp[-2].expression)));
      tmp.push_back(std::shared_ptr<Expression>((yyvsp[0].expression)));
      (yyval.expression) = new ConjunctionExpr(ConjunctionExpr::Type::OR, tmp);
    }
#line 3217 "yacc_sql.cpp"
    break;

  case 132: /* condition_list: EXISTS_T expression  */
#line 1275 "yacc_sql.y"
    {
      (yyval.expression) = new ComparisonExpr(EXISTS, std::shared_ptr<Expression>((yyvsp[0].expression)), nullptr);
    }
#line 3225 "yacc_sql.cpp"
    break;

  case 133: /* condition_list: NOT_T EXISTS_T expression  */
#line 1279 "yacc_sql.y"
    {
      (yyval.expression) = new ComparisonExpr(NOT_EXISTS, std::shared_ptr<Expression>((yyvsp[0].expression)), nullptr);
    }
#line 3233 "yacc_sql.cpp"
    break;

  case 134: /* comp_op: EQ  */
#line 1285 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3239 "yacc_sql.cpp"
    break;

  case 135: /* comp_op: LT  */
#line 1286 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3245 "yacc_sql.cpp"
    break;

  case 136: /* comp_op: GT  */
#line 1287 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3251 "yacc_sql.cpp"
    break;

  case 137: /* comp_op: LE  */
#line 1288 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3257 "yacc_sql.cpp"
    break;

  case 138: /* comp_op: GE  */
#line 1289 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3263 "yacc_sql.cpp"
    break;

  case 139: /* comp_op: NE  */
#line 1290 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3269 "yacc_sql.cpp"
    break;

  case 140: /* comp_op: LIKE_T  */
#line 1291 "yacc_sql.y"
             { (yyval.comp) = LIKE; }
#line 3275 "yacc_sql.cpp"
    break;

  case 141: /* comp_op: NOT_T LIKE_T  */
#line 1292 "yacc_sql.y"
                   { (yyval.comp) = NOT_LIKE; }
#line 3281 "yacc_sql.cpp"
    break;

  case 142: /* comp_op: IN_T  */
#line 1293 "yacc_sql.y"
           { (yyval.comp) = IN; }
#line 3287 "yacc_sql.cpp"
    break;

  case 143: /* comp_op: NOT_T IN_T  */
#line 1294 "yacc_sql.y"
                 { (yyval.comp) = NOT_IN; }
#line 3293 "yacc_sql.cpp"
    break;

  case 144: /* comp_op: IS_T  */
#line 1295 "yacc_sql.y"
           { (yyval.comp) = IS; }
#line 3299 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: IS_T NOT_T  */
#line 1296 "yacc_sql.y"
                 { (yyval.comp) = NOT_IS; }
#line 3305 "yacc_sql.cpp"
    break;

  case 146: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1305 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);

      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3319 "yacc_sql.cpp"
    break;

  case 147: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1318 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::shared_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3328 "yacc_sql.cpp"
    break;

  case 148: /* set_variable_stmt: SET ID EQ expression  */
#line 1326 "yacc_sql.y"
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
#line 3345 "yacc_sql.cpp"
    break;


#line 3349 "yacc_sql.cpp"

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

#line 1343 "yacc_sql.y"

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
