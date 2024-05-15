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

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
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


#line 115 "yacc_sql.cpp"

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
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_TABLES = 7,                     /* TABLES  */
  YYSYMBOL_INDEX = 8,                      /* INDEX  */
  YYSYMBOL_CALC = 9,                       /* CALC  */
  YYSYMBOL_SELECT = 10,                    /* SELECT  */
  YYSYMBOL_SUM = 11,                       /* SUM  */
  YYSYMBOL_MAX = 12,                       /* MAX  */
  YYSYMBOL_MIN = 13,                       /* MIN  */
  YYSYMBOL_COUNT = 14,                     /* COUNT  */
  YYSYMBOL_AVG = 15,                       /* AVG  */
  YYSYMBOL_IS_T = 16,                      /* IS_T  */
  YYSYMBOL_NOT = 17,                       /* NOT  */
  YYSYMBOL_NULL_T = 18,                    /* NULL_T  */
  YYSYMBOL_LIKE_T = 19,                    /* LIKE_T  */
  YYSYMBOL_INNER_T = 20,                   /* INNER_T  */
  YYSYMBOL_JOIN_T = 21,                    /* JOIN_T  */
  YYSYMBOL_IN_T = 22,                      /* IN_T  */
  YYSYMBOL_EXIST_T = 23,                   /* EXIST_T  */
  YYSYMBOL_UNIQUE_T = 24,                  /* UNIQUE_T  */
  YYSYMBOL_ORDER_T = 25,                   /* ORDER_T  */
  YYSYMBOL_BY_T = 26,                      /* BY_T  */
  YYSYMBOL_ASC_T = 27,                     /* ASC_T  */
  YYSYMBOL_DESC = 28,                      /* DESC  */
  YYSYMBOL_SHOW = 29,                      /* SHOW  */
  YYSYMBOL_SYNC = 30,                      /* SYNC  */
  YYSYMBOL_INSERT = 31,                    /* INSERT  */
  YYSYMBOL_DELETE = 32,                    /* DELETE  */
  YYSYMBOL_UPDATE = 33,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 34,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 35,                    /* RBRACE  */
  YYSYMBOL_COMMA = 36,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 37,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 38,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 39,              /* TRX_ROLLBACK  */
  YYSYMBOL_DATE_T = 40,                    /* DATE_T  */
  YYSYMBOL_INT_T = 41,                     /* INT_T  */
  YYSYMBOL_STRING_T = 42,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 43,                   /* FLOAT_T  */
  YYSYMBOL_HELP = 44,                      /* HELP  */
  YYSYMBOL_EXIT = 45,                      /* EXIT  */
  YYSYMBOL_DOT = 46,                       /* DOT  */
  YYSYMBOL_INTO = 47,                      /* INTO  */
  YYSYMBOL_VALUES = 48,                    /* VALUES  */
  YYSYMBOL_FROM = 49,                      /* FROM  */
  YYSYMBOL_WHERE = 50,                     /* WHERE  */
  YYSYMBOL_AND = 51,                       /* AND  */
  YYSYMBOL_SET = 52,                       /* SET  */
  YYSYMBOL_ON = 53,                        /* ON  */
  YYSYMBOL_LOAD = 54,                      /* LOAD  */
  YYSYMBOL_DATA = 55,                      /* DATA  */
  YYSYMBOL_INFILE = 56,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 57,                   /* EXPLAIN  */
  YYSYMBOL_EQ = 58,                        /* EQ  */
  YYSYMBOL_LT = 59,                        /* LT  */
  YYSYMBOL_GT = 60,                        /* GT  */
  YYSYMBOL_LE = 61,                        /* LE  */
  YYSYMBOL_GE = 62,                        /* GE  */
  YYSYMBOL_NE = 63,                        /* NE  */
  YYSYMBOL_NUMBER = 64,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 65,                     /* FLOAT  */
  YYSYMBOL_ID = 66,                        /* ID  */
  YYSYMBOL_DATE_STR = 67,                  /* DATE_STR  */
  YYSYMBOL_SSS = 68,                       /* SSS  */
  YYSYMBOL_69_ = 69,                       /* '+'  */
  YYSYMBOL_70_ = 70,                       /* '-'  */
  YYSYMBOL_71_ = 71,                       /* '*'  */
  YYSYMBOL_72_ = 72,                       /* '/'  */
  YYSYMBOL_UMINUS = 73,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 74,                  /* $accept  */
  YYSYMBOL_commands = 75,                  /* commands  */
  YYSYMBOL_command_wrapper = 76,           /* command_wrapper  */
  YYSYMBOL_avg_stmt = 77,                  /* avg_stmt  */
  YYSYMBOL_max_stmt = 78,                  /* max_stmt  */
  YYSYMBOL_min_stmt = 79,                  /* min_stmt  */
  YYSYMBOL_count_stmt = 80,                /* count_stmt  */
  YYSYMBOL_sum_stmt = 81,                  /* sum_stmt  */
  YYSYMBOL_exit_stmt = 82,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 83,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 84,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 85,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 86,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 87,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 88,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 89,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 90,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 91,         /* create_index_stmt  */
  YYSYMBOL_drop_index_stmt = 92,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 93,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 94,             /* attr_def_list  */
  YYSYMBOL_attr_def = 95,                  /* attr_def  */
  YYSYMBOL_number = 96,                    /* number  */
  YYSYMBOL_type = 97,                      /* type  */
  YYSYMBOL_insert_stmt = 98,               /* insert_stmt  */
  YYSYMBOL_value_list = 99,                /* value_list  */
  YYSYMBOL_value = 100,                    /* value  */
  YYSYMBOL_delete_stmt = 101,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 102,              /* update_stmt  */
  YYSYMBOL_update_list = 103,              /* update_list  */
  YYSYMBOL_select_stmt = 104,              /* select_stmt  */
  YYSYMBOL_order_by = 105,                 /* order_by  */
  YYSYMBOL_order_list = 106,               /* order_list  */
  YYSYMBOL_order_by_node = 107,            /* order_by_node  */
  YYSYMBOL_join_list = 108,                /* join_list  */
  YYSYMBOL_join = 109,                     /* join  */
  YYSYMBOL_calc_stmt = 110,                /* calc_stmt  */
  YYSYMBOL_expression_list = 111,          /* expression_list  */
  YYSYMBOL_expression = 112,               /* expression  */
  YYSYMBOL_select_attr = 113,              /* select_attr  */
  YYSYMBOL_rel_attr = 114,                 /* rel_attr  */
  YYSYMBOL_attr_list = 115,                /* attr_list  */
  YYSYMBOL_rel_list = 116,                 /* rel_list  */
  YYSYMBOL_where = 117,                    /* where  */
  YYSYMBOL_condition_list = 118,           /* condition_list  */
  YYSYMBOL_condition = 119,                /* condition  */
  YYSYMBOL_comp_op = 120,                  /* comp_op  */
  YYSYMBOL_load_data_stmt = 121,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 122,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 123,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 124             /* opt_semicolon  */
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
#define YYFINAL  78
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   308

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  51
/* YYNRULES -- Number of rules.  */
#define YYNRULES  141
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  289

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   324


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
       2,     2,    71,    69,     2,    70,     2,    72,     2,     2,
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
      65,    66,    67,    68,    73
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   229,   229,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   260,   266,   277,   283,   294,   300,   310,
     316,   324,   331,   337,   347,   353,   358,   364,   370,   376,
     382,   389,   395,   403,   419,   455,   465,   485,   488,   501,
     510,   519,   528,   537,   546,   557,   560,   561,   562,   563,
     566,   583,   586,   594,   598,   602,   606,   612,   620,   633,
     650,   660,   671,   681,   694,   730,   735,   748,   751,   761,
     766,   771,   780,   783,   796,   809,   819,   824,   835,   838,
     841,   844,   847,   851,   854,   862,   869,   881,   886,   893,
     896,   899,   902,   905,   912,   915,   929,   932,   942,   945,
     952,   955,   960,   967,   979,   991,  1003,  1015,  1032,  1048,
    1066,  1084,  1098,  1112,  1125,  1141,  1142,  1143,  1144,  1145,
    1146,  1147,  1148,  1149,  1150,  1151,  1152,  1156,  1169,  1177,
    1187,  1188
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
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "INDEX", "CALC", "SELECT", "SUM", "MAX",
  "MIN", "COUNT", "AVG", "IS_T", "NOT", "NULL_T", "LIKE_T", "INNER_T",
  "JOIN_T", "IN_T", "EXIST_T", "UNIQUE_T", "ORDER_T", "BY_T", "ASC_T",
  "DESC", "SHOW", "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE",
  "COMMA", "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "DATE_T", "INT_T",
  "STRING_T", "FLOAT_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM",
  "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EXPLAIN", "EQ",
  "LT", "GT", "LE", "GE", "NE", "NUMBER", "FLOAT", "ID", "DATE_STR", "SSS",
  "'+'", "'-'", "'*'", "'/'", "UMINUS", "$accept", "commands",
  "command_wrapper", "avg_stmt", "max_stmt", "min_stmt", "count_stmt",
  "sum_stmt", "exit_stmt", "help_stmt", "sync_stmt", "begin_stmt",
  "commit_stmt", "rollback_stmt", "drop_table_stmt", "show_tables_stmt",
  "desc_table_stmt", "create_index_stmt", "drop_index_stmt",
  "create_table_stmt", "attr_def_list", "attr_def", "number", "type",
  "insert_stmt", "value_list", "value", "delete_stmt", "update_stmt",
  "update_list", "select_stmt", "order_by", "order_list", "order_by_node",
  "join_list", "join", "calc_stmt", "expression_list", "expression",
  "select_attr", "rel_attr", "attr_list", "rel_list", "where",
  "condition_list", "condition", "comp_op", "load_data_stmt",
  "explain_stmt", "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-227)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     168,    29,    32,   115,     4,   -52,    13,  -227,   -17,    -3,
     -27,  -227,  -227,  -227,  -227,  -227,   -14,    52,   168,   129,
     131,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
    -227,    80,    81,   127,    90,    92,  -227,   115,  -227,  -227,
    -227,  -227,   115,  -227,  -227,   145,   114,   125,   126,   140,
     141,   130,  -227,  -227,  -227,  -227,  -227,  -227,   135,   150,
    -227,  -227,   128,   136,   139,   137,   147,  -227,  -227,  -227,
    -227,   170,   155,   143,  -227,   157,   -28,  -227,   115,   115,
     115,   115,   115,   152,   153,   158,    47,   160,   161,   162,
      14,  -227,   173,   179,   164,    55,   163,   166,   167,   181,
     169,  -227,  -227,    -7,    -7,  -227,  -227,   -25,   -24,    26,
      31,   188,    82,  -227,   200,   150,   203,    72,  -227,   180,
     179,  -227,   192,   101,   204,   207,   176,  -227,  -227,   177,
    -227,   178,  -227,   182,  -227,   184,  -227,  -227,   185,   186,
     225,  -227,    55,   223,   213,    -5,   108,   108,  -227,   202,
     -10,  -227,   248,  -227,  -227,  -227,  -227,    16,   166,   220,
     190,   226,   222,   224,   227,   229,   230,   200,   240,   179,
     225,   231,   232,    -5,   231,   233,   252,    69,  -227,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,    97,    87,    72,   260,
     235,   206,   255,  -227,   210,   204,  -227,   200,   209,  -227,
    -227,  -227,  -227,  -227,  -227,    14,   251,  -227,    55,   242,
      -5,   231,   243,   245,   108,  -227,  -227,  -227,  -227,  -227,
      -5,  -227,  -227,  -227,   246,   164,  -227,  -227,  -227,   247,
    -227,   249,   200,   234,   257,  -227,   231,  -227,   231,   250,
     253,  -227,   108,    14,   231,   254,   256,  -227,    86,  -227,
     258,    72,    14,  -227,   259,  -227,  -227,    14,  -227,   261,
    -227,   164,   268,  -227,  -227,  -227,   262,   104,  -227,  -227,
    -227,  -227,  -227,    14,  -227,  -227,  -227,   262,  -227
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    36,     0,     0,
       0,    37,    38,    39,    35,    34,     0,     0,     0,     0,
     140,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,     0,     0,    67,     0,    63,    64,
      65,    66,     0,    94,    85,    86,     0,     0,     0,     0,
       0,    97,    95,   103,   100,   101,   102,    99,     0,   104,
      42,    41,     0,     0,     0,     0,     0,   138,     1,   141,
       2,     0,     0,     0,    40,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,   108,     0,     0,     0,     0,     0,     0,
       0,    92,    87,    88,    89,    90,    91,     0,     0,     0,
       0,     0,     0,    98,   106,   104,     0,   110,    68,     0,
     108,   139,     0,     0,    47,     0,     0,    45,    32,     0,
      25,     0,    27,     0,    29,     0,    31,    23,     0,     0,
      82,   105,     0,     0,     0,     0,     0,     0,   109,   111,
       0,    69,     0,    59,    56,    57,    58,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,     0,   108,
      82,    61,     0,     0,    61,     0,   131,     0,   133,   135,
     125,   126,   127,   128,   129,   130,     0,     0,   110,     0,
      72,     0,     0,    53,     0,    47,    46,   106,     0,    33,
      26,    28,    30,    24,   107,     0,    75,    83,     0,     0,
       0,    61,     0,     0,     0,   132,   134,   136,   114,   116,
       0,   113,   115,   112,     0,     0,   137,    54,    55,     0,
      48,     0,   106,     0,     0,    74,    61,    60,    61,     0,
       0,   123,     0,     0,    61,     0,    73,    70,    49,    43,
       0,   110,     0,    62,     0,   124,   121,     0,   118,     0,
     117,     0,     0,    51,    44,    84,    77,    79,   122,   120,
     119,    71,    52,     0,    76,    80,    81,    77,    78
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -227,  -227,   272,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
      94,   123,  -227,  -227,  -227,  -172,  -104,  -227,  -227,  -226,
    -152,  -227,     8,    17,   117,  -227,  -227,   214,    98,  -227,
      -4,   183,  -171,   -98,  -194,  -227,  -155,  -227,  -227,  -227,
    -227
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    63,    64,    65,    66,    67,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
     169,   134,   239,   167,    33,   219,    53,    34,    35,   130,
      36,   245,   284,   276,   179,   180,    37,    54,    55,    68,
     157,   101,   150,   128,   158,   159,   196,    38,    39,    40,
      80
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      69,   131,   197,   185,   233,     4,   214,   111,    46,   257,
     138,   140,   223,    46,    70,    56,    57,    58,    59,    60,
      71,   139,   141,   156,   199,    56,    57,    58,    59,    60,
      72,   222,   161,   202,   203,    41,   241,    42,    44,    74,
      45,    89,    90,    91,    92,   281,    73,   234,   181,   250,
     204,   184,    75,    43,    48,    49,   200,    50,    51,    48,
      49,   142,    50,    51,    91,    92,   144,   275,   249,   253,
      61,   260,   143,    46,   263,    62,   264,   145,   255,   221,
      61,   216,   269,    56,    57,    58,    59,    60,   226,   153,
      46,   227,   228,   231,   156,   154,   125,   267,    56,    57,
      58,    59,    60,   272,   273,    46,   155,    76,    56,    57,
      58,    59,    60,   120,   246,    46,   248,   147,   121,    48,
      49,   230,    50,    51,   186,   187,   254,   188,   148,    78,
     189,   285,   286,    46,    79,    83,    48,    49,    61,    50,
      51,   163,   164,   165,   166,    86,    81,    82,    93,    47,
      87,    48,    49,    61,    50,    51,    84,   156,    85,    94,
      95,    48,    49,    61,    50,    51,   190,   191,   192,   193,
     194,   195,     1,     2,    96,    97,    98,     3,     4,    48,
      49,    88,    50,    51,    99,    52,   100,   113,   114,   115,
     116,   104,   229,   232,   102,   105,     5,     6,     7,     8,
       9,    10,   103,   106,   107,    11,    12,    13,   108,   109,
     110,   243,    14,    15,    89,    90,    91,    92,   117,   118,
      16,   126,    17,   146,   119,    18,   122,   123,   124,   127,
     129,   132,   133,   135,   136,   137,   149,   152,   160,   162,
     168,   170,   171,   172,   173,   178,   182,   183,   174,   268,
     175,   176,   177,   198,   201,   206,   207,   209,   277,   210,
     208,   215,   211,   279,   212,   213,   220,   218,   224,   225,
       4,   235,   236,   237,   238,   242,   244,   247,   251,   277,
     252,   256,   258,   262,   259,   265,   282,   261,   266,   270,
      77,   205,   271,   274,   278,   288,   280,   217,   283,   240,
     287,     0,   112,     0,     0,     0,     0,     0,   151
};

static const yytype_int16 yycheck[] =
{
       4,   105,   157,   155,   198,    10,   177,    35,    18,   235,
      35,    35,   184,    18,    66,    11,    12,    13,    14,    15,
       7,    46,    46,   127,    34,    11,    12,    13,    14,    15,
      47,   183,   130,    17,    18,     6,   207,     8,     6,    66,
       8,    69,    70,    71,    72,   271,    49,   199,   152,   221,
      34,   155,    66,    24,    64,    65,   160,    67,    68,    64,
      65,    35,    67,    68,    71,    72,    35,   261,   220,   224,
      66,   242,    46,    18,   246,    71,   248,    46,   230,   183,
      66,   179,   254,    11,    12,    13,    14,    15,    19,    17,
      18,    22,   196,   197,   198,    23,   100,   252,    11,    12,
      13,    14,    15,    17,    18,    18,    34,    55,    11,    12,
      13,    14,    15,    66,   218,    18,   220,    35,    71,    64,
      65,    34,    67,    68,    16,    17,   230,    19,    46,     0,
      22,    27,    28,    18,     3,     8,    64,    65,    66,    67,
      68,    40,    41,    42,    43,    47,    66,    66,    34,    34,
      52,    64,    65,    66,    67,    68,    66,   261,    66,    34,
      34,    64,    65,    66,    67,    68,    58,    59,    60,    61,
      62,    63,     4,     5,    34,    34,    46,     9,    10,    64,
      65,    36,    67,    68,    49,    70,    36,    89,    90,    91,
      92,    52,   196,   197,    66,    58,    28,    29,    30,    31,
      32,    33,    66,    56,    34,    37,    38,    39,    53,    66,
      53,   215,    44,    45,    69,    70,    71,    72,    66,    66,
      52,    48,    54,    35,    66,    57,    66,    66,    66,    50,
      66,    68,    66,    66,    53,    66,    36,    34,    58,    47,
      36,    34,    66,    66,    66,    20,    23,    34,    66,   253,
      66,    66,    66,    51,     6,    35,    66,    35,   262,    35,
      34,    21,    35,   267,    35,    35,    34,    36,    35,    17,
      10,    36,    66,    18,    64,    66,    25,    35,    35,   283,
      35,    35,    35,    26,    35,    35,    18,    53,    35,    35,
      18,   168,    36,    35,    35,   287,    35,   180,    36,   205,
     283,    -1,    88,    -1,    -1,    -1,    -1,    -1,   125
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,     9,    10,    28,    29,    30,    31,    32,
      33,    37,    38,    39,    44,    45,    52,    54,    57,    75,
      76,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    98,   101,   102,   104,   110,   121,   122,
     123,     6,     8,    24,     6,     8,    18,    34,    64,    65,
      67,    68,    70,   100,   111,   112,    11,    12,    13,    14,
      15,    66,    71,    77,    78,    79,    80,    81,   113,   114,
      66,     7,    47,    49,    66,    66,    55,    76,     0,     3,
     124,    66,    66,     8,    66,    66,   112,   112,    36,    69,
      70,    71,    72,    34,    34,    34,    34,    34,    46,    49,
      36,   115,    66,    66,    52,    58,    56,    34,    53,    66,
      53,    35,   111,   112,   112,   112,   112,    66,    66,    66,
      66,    71,    66,    66,    66,   114,    48,    50,   117,    66,
     103,   100,    68,    66,    95,    66,    53,    66,    35,    46,
      35,    46,    35,    46,    35,    46,    35,    35,    46,    36,
     116,   115,    34,    17,    23,    34,   100,   114,   118,   119,
      58,   117,    47,    40,    41,    42,    43,    97,    36,    94,
      34,    66,    66,    66,    66,    66,    66,    66,    20,   108,
     109,   100,    23,    34,   100,   104,    16,    17,    19,    22,
      58,    59,    60,    61,    62,    63,   120,   120,    51,    34,
     100,     6,    17,    18,    34,    95,    35,    66,    34,    35,
      35,    35,    35,    35,   116,    21,   117,   108,    36,    99,
      34,   100,   104,    99,    35,    17,    19,    22,   100,   114,
      34,   100,   114,   118,   104,    36,    66,    18,    64,    96,
      94,   116,    66,   114,    25,   105,   100,    35,   100,   104,
      99,    35,    35,   120,   100,   104,    35,   103,    35,    35,
     116,    53,    26,    99,    99,    35,    35,   120,   114,    99,
      35,    36,    17,    18,    35,   118,   107,   114,    35,   114,
      35,   103,    18,    36,   106,    27,    28,   107,   106
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    74,    75,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    77,    77,    78,    78,    79,    79,    80,
      80,    80,    81,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    91,    92,    93,    94,    94,    95,
      95,    95,    95,    95,    95,    96,    97,    97,    97,    97,
      98,    99,    99,   100,   100,   100,   100,   100,   101,   102,
     103,   103,   103,   103,   104,   105,   105,   106,   106,   107,
     107,   107,   108,   108,   109,   110,   111,   111,   112,   112,
     112,   112,   112,   112,   112,   113,   113,   114,   114,   114,
     114,   114,   114,   114,   115,   115,   116,   116,   117,   117,
     118,   118,   118,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   121,   122,   123,
     124,   124
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     6,     4,     6,     4,     6,     4,
       6,     4,     4,     6,     1,     1,     1,     1,     1,     1,
       3,     2,     2,     9,    10,     5,     7,     0,     3,     5,
       2,     6,     7,     3,     4,     1,     1,     1,     1,     1,
       8,     0,     3,     1,     1,     1,     1,     1,     4,     5,
       5,     7,     3,     5,     8,     0,     4,     0,     3,     1,
       2,     2,     0,     2,     5,     2,     1,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     2,     1,     3,     1,
       1,     1,     1,     1,     0,     3,     0,     3,     0,     2,
       0,     1,     3,     3,     3,     3,     3,     5,     5,     6,
       6,     5,     6,     4,     5,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     1,     2,     7,     2,     4,
       0,     1
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
#line 230 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1838 "yacc_sql.cpp"
    break;

  case 23: /* avg_stmt: AVG LBRACE ID RBRACE  */
#line 260 "yacc_sql.y"
                       {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::AVG_TYPE;
    free((yyvsp[-1].string));
  }
#line 1849 "yacc_sql.cpp"
    break;

  case 24: /* avg_stmt: AVG LBRACE ID DOT ID RBRACE  */
#line 266 "yacc_sql.y"
                                {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::AVG_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1862 "yacc_sql.cpp"
    break;

  case 25: /* max_stmt: MAX LBRACE ID RBRACE  */
#line 277 "yacc_sql.y"
                       {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::MAX_TYPE;
    free((yyvsp[-1].string));
  }
#line 1873 "yacc_sql.cpp"
    break;

  case 26: /* max_stmt: MAX LBRACE ID DOT ID RBRACE  */
#line 283 "yacc_sql.y"
                                {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::MAX_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1886 "yacc_sql.cpp"
    break;

  case 27: /* min_stmt: MIN LBRACE ID RBRACE  */
#line 294 "yacc_sql.y"
                       {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::MIN_TYPE;
    free((yyvsp[-1].string));
  }
#line 1897 "yacc_sql.cpp"
    break;

  case 28: /* min_stmt: MIN LBRACE ID DOT ID RBRACE  */
#line 300 "yacc_sql.y"
                                {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::MIN_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1910 "yacc_sql.cpp"
    break;

  case 29: /* count_stmt: COUNT LBRACE ID RBRACE  */
#line 310 "yacc_sql.y"
                         {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::COUNT_TYPE;
    free((yyvsp[-1].string));
  }
#line 1921 "yacc_sql.cpp"
    break;

  case 30: /* count_stmt: COUNT LBRACE ID DOT ID RBRACE  */
#line 316 "yacc_sql.y"
                                  {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::COUNT_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1934 "yacc_sql.cpp"
    break;

  case 31: /* count_stmt: COUNT LBRACE '*' RBRACE  */
#line 324 "yacc_sql.y"
                            {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = "*";
    (yyval.rel_attr)->agg_type = AggregationType::COUNT_STAR_TYPE;
  }
#line 1944 "yacc_sql.cpp"
    break;

  case 32: /* sum_stmt: SUM LBRACE ID RBRACE  */
#line 331 "yacc_sql.y"
                       {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::SUM_TYPE;
    free((yyvsp[-1].string));
  }
#line 1955 "yacc_sql.cpp"
    break;

  case 33: /* sum_stmt: SUM LBRACE ID DOT ID RBRACE  */
#line 337 "yacc_sql.y"
                                {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::SUM_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1968 "yacc_sql.cpp"
    break;

  case 34: /* exit_stmt: EXIT  */
#line 347 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1977 "yacc_sql.cpp"
    break;

  case 35: /* help_stmt: HELP  */
#line 353 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1985 "yacc_sql.cpp"
    break;

  case 36: /* sync_stmt: SYNC  */
#line 358 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1993 "yacc_sql.cpp"
    break;

  case 37: /* begin_stmt: TRX_BEGIN  */
#line 364 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 2001 "yacc_sql.cpp"
    break;

  case 38: /* commit_stmt: TRX_COMMIT  */
#line 370 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 2009 "yacc_sql.cpp"
    break;

  case 39: /* rollback_stmt: TRX_ROLLBACK  */
#line 376 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 2017 "yacc_sql.cpp"
    break;

  case 40: /* drop_table_stmt: DROP TABLE ID  */
#line 382 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2027 "yacc_sql.cpp"
    break;

  case 41: /* show_tables_stmt: SHOW TABLES  */
#line 389 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 2035 "yacc_sql.cpp"
    break;

  case 42: /* desc_table_stmt: DESC ID  */
#line 395 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2045 "yacc_sql.cpp"
    break;

  case 43: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID rel_list RBRACE  */
#line 404 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);

      (yyvsp[-1].relation_list)->emplace_back(std::string((yyvsp[-2].string)));
      free((yyvsp[-2].string));

      create_index.is_unique = false;
      create_index.attrs.swap(*(yyvsp[-1].relation_list));
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      delete (yyvsp[-1].relation_list);
    }
#line 2065 "yacc_sql.cpp"
    break;

  case 44: /* create_index_stmt: CREATE UNIQUE_T INDEX ID ON ID LBRACE ID rel_list RBRACE  */
#line 420 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);

      (yyvsp[-1].relation_list)->emplace_back(std::string((yyvsp[-2].string)));
      free((yyvsp[-2].string));

      create_index.is_unique = true;
      create_index.attrs.swap(*(yyvsp[-1].relation_list));
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      delete (yyvsp[-1].relation_list);
    }
#line 2085 "yacc_sql.cpp"
    break;

  case 45: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 456 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2097 "yacc_sql.cpp"
    break;

  case 46: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE  */
#line 466 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-4].string);
      free((yyvsp[-4].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-1].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-2].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-2].attr_info);
    }
#line 2118 "yacc_sql.cpp"
    break;

  case 47: /* attr_def_list: %empty  */
#line 485 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2126 "yacc_sql.cpp"
    break;

  case 48: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 489 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2140 "yacc_sql.cpp"
    break;

  case 49: /* attr_def: ID type LBRACE number RBRACE  */
#line 502 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number) + 1;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-4].string));
    }
#line 2153 "yacc_sql.cpp"
    break;

  case 50: /* attr_def: ID type  */
#line 511 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[0].number);
      (yyval.attr_info)->name = (yyvsp[-1].string);
      (yyval.attr_info)->length = VALUE_SIZE_NOT_CHARS;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-1].string));
    }
#line 2166 "yacc_sql.cpp"
    break;

  case 51: /* attr_def: ID type LBRACE number RBRACE NULL_T  */
#line 520 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number) + 1;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-5].string));
    }
#line 2179 "yacc_sql.cpp"
    break;

  case 52: /* attr_def: ID type LBRACE number RBRACE NOT NULL_T  */
#line 529 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-5].number);
      (yyval.attr_info)->name = (yyvsp[-6].string);
      (yyval.attr_info)->length = (yyvsp[-3].number) + 1;
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-6].string));
    }
#line 2192 "yacc_sql.cpp"
    break;

  case 53: /* attr_def: ID type NULL_T  */
#line 538 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = VALUE_SIZE_NOT_CHARS;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-2].string));
    }
#line 2205 "yacc_sql.cpp"
    break;

  case 54: /* attr_def: ID type NOT NULL_T  */
#line 547 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-2].number);
      (yyval.attr_info)->name = (yyvsp[-3].string);
      (yyval.attr_info)->length = VALUE_SIZE_NOT_CHARS;
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-3].string));
    }
#line 2218 "yacc_sql.cpp"
    break;

  case 55: /* number: NUMBER  */
#line 557 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2224 "yacc_sql.cpp"
    break;

  case 56: /* type: INT_T  */
#line 560 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 2230 "yacc_sql.cpp"
    break;

  case 57: /* type: STRING_T  */
#line 561 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 2236 "yacc_sql.cpp"
    break;

  case 58: /* type: FLOAT_T  */
#line 562 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 2242 "yacc_sql.cpp"
    break;

  case 59: /* type: DATE_T  */
#line 563 "yacc_sql.y"
             { (yyval.number)=DATES; }
#line 2248 "yacc_sql.cpp"
    break;

  case 60: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE  */
#line 567 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-5].string);
      if ((yyvsp[-1].value_list) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[-1].value_list));
        delete (yyvsp[-1].value_list);
      }
      (yyval.sql_node)->insertion.values.emplace_back(*(yyvsp[-2].value));
      std::reverse((yyval.sql_node)->insertion.values.begin(), (yyval.sql_node)->insertion.values.end());
      delete (yyvsp[-2].value);
      free((yyvsp[-5].string));
    }
#line 2265 "yacc_sql.cpp"
    break;

  case 61: /* value_list: %empty  */
#line 583 "yacc_sql.y"
    {
      (yyval.value_list) = new std::vector<Value>;
    }
#line 2273 "yacc_sql.cpp"
    break;

  case 62: /* value_list: COMMA value value_list  */
#line 586 "yacc_sql.y"
                              {
      (yyval.value_list) = (yyvsp[0].value_list);

      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2284 "yacc_sql.cpp"
    break;

  case 63: /* value: NUMBER  */
#line 594 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2293 "yacc_sql.cpp"
    break;

  case 64: /* value: FLOAT  */
#line 598 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2302 "yacc_sql.cpp"
    break;

  case 65: /* value: DATE_STR  */
#line 602 "yacc_sql.y"
              {
      (yyval.value) = new Value(str_to_date((yyvsp[0].string)));
      (yyloc) = (yylsp[0]);
    }
#line 2311 "yacc_sql.cpp"
    break;

  case 66: /* value: SSS  */
#line 606 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2322 "yacc_sql.cpp"
    break;

  case 67: /* value: NULL_T  */
#line 612 "yacc_sql.y"
            {
      (yyval.value) = new Value(false);
      (yyval.value)->set_null();
      (yyloc) = (yylsp[0]);
    }
#line 2332 "yacc_sql.cpp"
    break;

  case 68: /* delete_stmt: DELETE FROM ID where  */
#line 621 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2346 "yacc_sql.cpp"
    break;

  case 69: /* update_stmt: UPDATE ID SET update_list where  */
#line 634 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      (yyval.sql_node)->update.relation_name = (yyvsp[-3].string);
      (yyval.sql_node)->update.update_values.swap(*(yyvsp[-1].update_list));
      std::reverse((yyval.sql_node)->update.update_values.begin(), (yyval.sql_node)->update.update_values.end());

      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->update.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-3].string));
      delete (yyvsp[-1].update_list);
    }
#line 2364 "yacc_sql.cpp"
    break;

  case 70: /* update_list: ID EQ value COMMA update_list  */
#line 651 "yacc_sql.y"
  {
    (yyval.update_list) = (yyvsp[0].update_list);
    update_value tmp;
    tmp.attribute_name = (yyvsp[-4].string);
    tmp.value = *(yyvsp[-2].value);
    (yyval.update_list)->emplace_back(tmp);
    free((yyvsp[-4].string));
    delete (yyvsp[-2].value);
  }
#line 2378 "yacc_sql.cpp"
    break;

  case 71: /* update_list: ID EQ LBRACE select_stmt RBRACE COMMA update_list  */
#line 661 "yacc_sql.y"
  {
    (yyval.update_list) = (yyvsp[0].update_list);
    update_value tmp((yyvsp[-3].sql_node)->selection);
    tmp.attribute_name = (yyvsp[-6].string);
    tmp.is_sub = true;

    (yyval.update_list)->emplace_back(tmp);
    free((yyvsp[-6].string));
    delete (yyvsp[-3].sql_node);
  }
#line 2393 "yacc_sql.cpp"
    break;

  case 72: /* update_list: ID EQ value  */
#line 672 "yacc_sql.y"
  {
    (yyval.update_list) = new std::vector<update_value>;
    update_value tmp;
    tmp.attribute_name = (yyvsp[-2].string);
    tmp.value = *(yyvsp[0].value);
    (yyval.update_list)->emplace_back(tmp);
    free((yyvsp[-2].string));
    delete (yyvsp[0].value);
  }
#line 2407 "yacc_sql.cpp"
    break;

  case 73: /* update_list: ID EQ LBRACE select_stmt RBRACE  */
#line 682 "yacc_sql.y"
  {
    (yyval.update_list) = new std::vector<update_value>;
    update_value tmp((yyvsp[-1].sql_node)->selection);
    tmp.attribute_name = (yyvsp[-4].string);
    tmp.is_sub = true;
    (yyval.update_list)->emplace_back(tmp);
    free((yyvsp[-4].string));
    delete (yyvsp[-1].sql_node);
  }
#line 2421 "yacc_sql.cpp"
    break;

  case 74: /* select_stmt: SELECT select_attr FROM ID rel_list join_list where order_by  */
#line 695 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-6].rel_attr_list) != nullptr) {
        (yyval.sql_node)->selection.attributes.swap(*(yyvsp[-6].rel_attr_list));
        delete (yyvsp[-6].rel_attr_list);
      }
      if ((yyvsp[-3].relation_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-3].relation_list));
        delete (yyvsp[-3].relation_list);
      }
      (yyval.sql_node)->selection.relations.push_back((yyvsp[-4].string));
      std::reverse((yyval.sql_node)->selection.relations.begin(), (yyval.sql_node)->selection.relations.end());

      if ((yyvsp[-2].join_list) != nullptr) {
        std::reverse((yyvsp[-2].join_list)->begin(), (yyvsp[-2].join_list)->end());
        (yyval.sql_node)->selection.joins.swap(*(yyvsp[-2].join_list));
        delete (yyvsp[-2].join_list);
      }

      if ((yyvsp[-1].condition_list) != nullptr) {
        (yyval.sql_node)->selection.conditions.swap(*(yyvsp[-1].condition_list));
        delete (yyvsp[-1].condition_list);
      }

      if ((yyvsp[0].order_by_list) != nullptr) {
        (yyval.sql_node)->selection.order_bys.swap(*(yyvsp[0].order_by_list));
        delete (yyvsp[0].order_by_list);
      }

      free((yyvsp[-4].string));
    }
#line 2457 "yacc_sql.cpp"
    break;

  case 75: /* order_by: %empty  */
#line 730 "yacc_sql.y"
  {
    (yyval.order_by_list) = nullptr;
  }
#line 2465 "yacc_sql.cpp"
    break;

  case 76: /* order_by: ORDER_T BY_T order_by_node order_list  */
#line 736 "yacc_sql.y"
  {
    (yyval.order_by_list) = (yyvsp[0].order_by_list);
    (yyval.order_by_list)->emplace_back(*(yyvsp[-1].order_by_node));
    delete (yyvsp[-1].order_by_node);

    /* 翻转 */
    std::reverse((yyval.order_by_list)->begin(), (yyval.order_by_list)->end());
  }
#line 2478 "yacc_sql.cpp"
    break;

  case 77: /* order_list: %empty  */
#line 748 "yacc_sql.y"
  {
    (yyval.order_by_list) = new std::vector<OrderBySqlNode>;
  }
#line 2486 "yacc_sql.cpp"
    break;

  case 78: /* order_list: COMMA order_by_node order_list  */
#line 752 "yacc_sql.y"
  {
    (yyval.order_by_list) = (yyvsp[0].order_by_list);

    (yyval.order_by_list)->emplace_back(*(yyvsp[-1].order_by_node));
    delete (yyvsp[-1].order_by_node);
  }
#line 2497 "yacc_sql.cpp"
    break;

  case 79: /* order_by_node: rel_attr  */
#line 762 "yacc_sql.y"
  {
    (yyval.order_by_node) = new OrderBySqlNode(*(yyvsp[0].rel_attr), true);
    delete (yyvsp[0].rel_attr);
  }
#line 2506 "yacc_sql.cpp"
    break;

  case 80: /* order_by_node: rel_attr ASC_T  */
#line 767 "yacc_sql.y"
  {
    (yyval.order_by_node) = new OrderBySqlNode(*(yyvsp[-1].rel_attr), true);
    delete (yyvsp[-1].rel_attr);
  }
#line 2515 "yacc_sql.cpp"
    break;

  case 81: /* order_by_node: rel_attr DESC  */
#line 772 "yacc_sql.y"
  {
    (yyval.order_by_node) = new OrderBySqlNode(*(yyvsp[-1].rel_attr), false);
    delete (yyvsp[-1].rel_attr);
  }
#line 2524 "yacc_sql.cpp"
    break;

  case 82: /* join_list: %empty  */
#line 780 "yacc_sql.y"
  {
    (yyval.join_list) = nullptr;
  }
#line 2532 "yacc_sql.cpp"
    break;

  case 83: /* join_list: join join_list  */
#line 784 "yacc_sql.y"
  {
    if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new std::vector<JoinSqlNode>;
      }
    (yyval.join_list)->emplace_back(*(yyvsp[-1].join));
    delete (yyvsp[-1].join);
  }
#line 2546 "yacc_sql.cpp"
    break;

  case 84: /* join: INNER_T JOIN_T rel_attr ON condition_list  */
#line 797 "yacc_sql.y"
  {
    (yyval.join) = new JoinSqlNode;
    (yyval.join)->joined_rel = *(yyvsp[-2].rel_attr);
    /* 这里rel_attr会把表名识别成属性名，交换一下 */
    (yyval.join)->joined_rel.relation_name.swap((yyval.join)->joined_rel.attribute_name);
    (yyval.join)->conditions.swap(*(yyvsp[0].condition_list));
    delete (yyvsp[-2].rel_attr);
    delete (yyvsp[0].condition_list);
  }
#line 2560 "yacc_sql.cpp"
    break;

  case 85: /* calc_stmt: CALC expression_list  */
#line 810 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2571 "yacc_sql.cpp"
    break;

  case 86: /* expression_list: expression  */
#line 820 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<Expression*>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2580 "yacc_sql.cpp"
    break;

  case 87: /* expression_list: expression COMMA expression_list  */
#line 825 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-2].expression));
    }
#line 2593 "yacc_sql.cpp"
    break;

  case 88: /* expression: expression '+' expression  */
#line 835 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2601 "yacc_sql.cpp"
    break;

  case 89: /* expression: expression '-' expression  */
#line 838 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2609 "yacc_sql.cpp"
    break;

  case 90: /* expression: expression '*' expression  */
#line 841 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2617 "yacc_sql.cpp"
    break;

  case 91: /* expression: expression '/' expression  */
#line 844 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2625 "yacc_sql.cpp"
    break;

  case 92: /* expression: LBRACE expression RBRACE  */
#line 847 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2634 "yacc_sql.cpp"
    break;

  case 93: /* expression: '-' expression  */
#line 851 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2642 "yacc_sql.cpp"
    break;

  case 94: /* expression: value  */
#line 854 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2652 "yacc_sql.cpp"
    break;

  case 95: /* select_attr: '*'  */
#line 862 "yacc_sql.y"
        {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      RelAttrSqlNode attr;
      attr.relation_name  = "";
      attr.attribute_name = "*";
      (yyval.rel_attr_list)->emplace_back(attr);
    }
#line 2664 "yacc_sql.cpp"
    break;

  case 96: /* select_attr: rel_attr attr_list  */
#line 869 "yacc_sql.y"
                         {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2678 "yacc_sql.cpp"
    break;

  case 97: /* rel_attr: ID  */
#line 881 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2688 "yacc_sql.cpp"
    break;

  case 98: /* rel_attr: ID DOT ID  */
#line 886 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2700 "yacc_sql.cpp"
    break;

  case 99: /* rel_attr: sum_stmt  */
#line 893 "yacc_sql.y"
               {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2708 "yacc_sql.cpp"
    break;

  case 100: /* rel_attr: max_stmt  */
#line 896 "yacc_sql.y"
               {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2716 "yacc_sql.cpp"
    break;

  case 101: /* rel_attr: min_stmt  */
#line 899 "yacc_sql.y"
               {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2724 "yacc_sql.cpp"
    break;

  case 102: /* rel_attr: count_stmt  */
#line 902 "yacc_sql.y"
                 {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2732 "yacc_sql.cpp"
    break;

  case 103: /* rel_attr: avg_stmt  */
#line 905 "yacc_sql.y"
               {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2740 "yacc_sql.cpp"
    break;

  case 104: /* attr_list: %empty  */
#line 912 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2748 "yacc_sql.cpp"
    break;

  case 105: /* attr_list: COMMA rel_attr attr_list  */
#line 915 "yacc_sql.y"
                               { /*我超，递归*/
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }

      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2763 "yacc_sql.cpp"
    break;

  case 106: /* rel_list: %empty  */
#line 929 "yacc_sql.y"
    {
      (yyval.relation_list) = new std::vector<std::string>;
    }
#line 2771 "yacc_sql.cpp"
    break;

  case 107: /* rel_list: COMMA ID rel_list  */
#line 932 "yacc_sql.y"
                        {
      (yyval.relation_list) = (yyvsp[0].relation_list);

      (yyval.relation_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 2782 "yacc_sql.cpp"
    break;

  case 108: /* where: %empty  */
#line 942 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2790 "yacc_sql.cpp"
    break;

  case 109: /* where: WHERE condition_list  */
#line 945 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 2798 "yacc_sql.cpp"
    break;

  case 110: /* condition_list: %empty  */
#line 952 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2806 "yacc_sql.cpp"
    break;

  case 111: /* condition_list: condition  */
#line 955 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 2816 "yacc_sql.cpp"
    break;

  case 112: /* condition_list: condition AND condition_list  */
#line 960 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 2826 "yacc_sql.cpp"
    break;

  case 113: /* condition: rel_attr comp_op value  */
#line 968 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->left_attr = *(yyvsp[-2].rel_attr);
      (yyval.condition)->right_is_attr = 0;
      (yyval.condition)->right_value = *(yyvsp[0].value);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].rel_attr);
      delete (yyvsp[0].value);
    }
#line 2842 "yacc_sql.cpp"
    break;

  case 114: /* condition: value comp_op value  */
#line 980 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 0;
      (yyval.condition)->left_value = *(yyvsp[-2].value);
      (yyval.condition)->right_is_attr = 0;
      (yyval.condition)->right_value = *(yyvsp[0].value);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].value);
      delete (yyvsp[0].value);
    }
#line 2858 "yacc_sql.cpp"
    break;

  case 115: /* condition: rel_attr comp_op rel_attr  */
#line 992 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->left_attr = *(yyvsp[-2].rel_attr);
      (yyval.condition)->right_is_attr = 1;
      (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].rel_attr);
      delete (yyvsp[0].rel_attr);
    }
#line 2874 "yacc_sql.cpp"
    break;

  case 116: /* condition: value comp_op rel_attr  */
#line 1004 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 0;
      (yyval.condition)->left_value = *(yyvsp[-2].value);
      (yyval.condition)->right_is_attr = 1;
      (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].value);
      delete (yyvsp[0].rel_attr);
    }
#line 2890 "yacc_sql.cpp"
    break;

  case 117: /* condition: rel_attr comp_op LBRACE select_stmt RBRACE  */
#line 1016 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->right_is_attr = 3; /* 设置2为sub查询 */
      (yyval.condition)->left_attr = *(yyvsp[-4].rel_attr);

      delete (yyvsp[-4].rel_attr);

      SelectSqlNode *tmp = new SelectSqlNode((yyvsp[-1].sql_node)->selection);
      tmp->is_sub = true;
      delete (yyvsp[-1].sql_node);

      (yyval.condition)->right_sql.reset(tmp);

      (yyval.condition)->comp = (yyvsp[-3].comp);
    }
#line 2911 "yacc_sql.cpp"
    break;

  case 118: /* condition: LBRACE select_stmt RBRACE comp_op rel_attr  */
#line 1033 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->right_is_attr = 1;
      (yyval.condition)->left_is_attr = 3;
      (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);

      delete (yyvsp[0].rel_attr);

      SelectSqlNode *tmp = new SelectSqlNode((yyvsp[-3].sql_node)->selection);
      tmp->is_sub = true;
      delete (yyvsp[-3].sql_node);

      (yyval.condition)->left_sql.reset(tmp);
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 2931 "yacc_sql.cpp"
    break;

  case 119: /* condition: rel_attr comp_op LBRACE value value_list RBRACE  */
#line 1049 "yacc_sql.y"
    {
      (yyvsp[-1].value_list)->emplace_back(*(yyvsp[-2].value));
      delete (yyvsp[-2].value);

      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->right_is_attr = 2; /* 设置3表示value_list集合 */
      (yyval.condition)->left_attr = *(yyvsp[-5].rel_attr);

      delete (yyvsp[-5].rel_attr);

      (yyval.condition)->comp = (yyvsp[-4].comp);

      (yyval.condition)->right_value_list.swap(*(yyvsp[-1].value_list));

      delete (yyvsp[-1].value_list);
    }
#line 2953 "yacc_sql.cpp"
    break;

  case 120: /* condition: LBRACE value value_list RBRACE comp_op rel_attr  */
#line 1067 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 2;
      (yyval.condition)->right_is_attr = 1;
      (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);

      delete (yyvsp[0].rel_attr);

      (yyval.condition)->comp = (yyvsp[-1].comp);

      (yyvsp[-3].value_list)->emplace_back(*(yyvsp[-4].value));
      delete (yyvsp[-4].value);

      (yyval.condition)->left_value_list.swap(*(yyvsp[-3].value_list));

      delete (yyvsp[-3].value_list);
    }
#line 2975 "yacc_sql.cpp"
    break;

  case 121: /* condition: EXIST_T LBRACE value value_list RBRACE  */
#line 1085 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = -1; /* 无效位 */
      (yyval.condition)->right_is_attr = 2;

      (yyval.condition)->right_value_list.swap(*(yyvsp[-1].value_list));
      delete (yyvsp[-1].value_list);

      (yyval.condition)->right_value_list.emplace_back(*(yyvsp[-2].value));
      delete (yyvsp[-2].value);

      (yyval.condition)->comp = EXIST;
    }
#line 2993 "yacc_sql.cpp"
    break;

  case 122: /* condition: NOT EXIST_T LBRACE value value_list RBRACE  */
#line 1099 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = -1;
      (yyval.condition)->right_is_attr = 2;

      (yyval.condition)->right_value_list.swap(*(yyvsp[-1].value_list));
      delete (yyvsp[-1].value_list);

      (yyval.condition)->right_value_list.emplace_back(*(yyvsp[-2].value));
      delete (yyvsp[-2].value);

      (yyval.condition)->comp = EXIST_NOT;
    }
#line 3011 "yacc_sql.cpp"
    break;

  case 123: /* condition: EXIST_T LBRACE select_stmt RBRACE  */
#line 1113 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->right_is_attr = 3;
      (yyval.condition)->left_is_attr = -1;

      SelectSqlNode *tmp = new SelectSqlNode((yyvsp[-1].sql_node)->selection);
      tmp->is_sub = true;
      delete (yyvsp[-1].sql_node);

      (yyval.condition)->right_sql.reset(tmp);
      (yyval.condition)->comp = EXIST;
    }
#line 3028 "yacc_sql.cpp"
    break;

  case 124: /* condition: NOT EXIST_T LBRACE select_stmt RBRACE  */
#line 1126 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->right_is_attr = 3;
      (yyval.condition)->left_is_attr = -1;

      SelectSqlNode *tmp = new SelectSqlNode((yyvsp[-1].sql_node)->selection);
      tmp->is_sub = true;
      delete (yyvsp[-1].sql_node);

      (yyval.condition)->right_sql.reset(tmp);
      (yyval.condition)->comp = EXIST_NOT;
    }
#line 3045 "yacc_sql.cpp"
    break;

  case 125: /* comp_op: EQ  */
#line 1141 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3051 "yacc_sql.cpp"
    break;

  case 126: /* comp_op: LT  */
#line 1142 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3057 "yacc_sql.cpp"
    break;

  case 127: /* comp_op: GT  */
#line 1143 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3063 "yacc_sql.cpp"
    break;

  case 128: /* comp_op: LE  */
#line 1144 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3069 "yacc_sql.cpp"
    break;

  case 129: /* comp_op: GE  */
#line 1145 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3075 "yacc_sql.cpp"
    break;

  case 130: /* comp_op: NE  */
#line 1146 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3081 "yacc_sql.cpp"
    break;

  case 131: /* comp_op: IS_T  */
#line 1147 "yacc_sql.y"
           { (yyval.comp) = IS; }
#line 3087 "yacc_sql.cpp"
    break;

  case 132: /* comp_op: IS_T NOT  */
#line 1148 "yacc_sql.y"
               { (yyval.comp) = IS_NOT; }
#line 3093 "yacc_sql.cpp"
    break;

  case 133: /* comp_op: LIKE_T  */
#line 1149 "yacc_sql.y"
             { (yyval.comp) = LIKE; }
#line 3099 "yacc_sql.cpp"
    break;

  case 134: /* comp_op: NOT LIKE_T  */
#line 1150 "yacc_sql.y"
                 { (yyval.comp) = LIKE_NOT; }
#line 3105 "yacc_sql.cpp"
    break;

  case 135: /* comp_op: IN_T  */
#line 1151 "yacc_sql.y"
           { (yyval.comp) = IN; }
#line 3111 "yacc_sql.cpp"
    break;

  case 136: /* comp_op: NOT IN_T  */
#line 1152 "yacc_sql.y"
               { (yyval.comp) = IN_NOT; }
#line 3117 "yacc_sql.cpp"
    break;

  case 137: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1157 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);

      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3131 "yacc_sql.cpp"
    break;

  case 138: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1170 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3140 "yacc_sql.cpp"
    break;

  case 139: /* set_variable_stmt: SET ID EQ value  */
#line 1178 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3152 "yacc_sql.cpp"
    break;


#line 3156 "yacc_sql.cpp"

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

#line 1190 "yacc_sql.y"

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
