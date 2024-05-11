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
  YYSYMBOL_DESC = 24,                      /* DESC  */
  YYSYMBOL_SHOW = 25,                      /* SHOW  */
  YYSYMBOL_SYNC = 26,                      /* SYNC  */
  YYSYMBOL_INSERT = 27,                    /* INSERT  */
  YYSYMBOL_DELETE = 28,                    /* DELETE  */
  YYSYMBOL_UPDATE = 29,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 30,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 31,                    /* RBRACE  */
  YYSYMBOL_COMMA = 32,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 33,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 34,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 35,              /* TRX_ROLLBACK  */
  YYSYMBOL_DATE_T = 36,                    /* DATE_T  */
  YYSYMBOL_INT_T = 37,                     /* INT_T  */
  YYSYMBOL_STRING_T = 38,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 39,                   /* FLOAT_T  */
  YYSYMBOL_HELP = 40,                      /* HELP  */
  YYSYMBOL_EXIT = 41,                      /* EXIT  */
  YYSYMBOL_DOT = 42,                       /* DOT  */
  YYSYMBOL_INTO = 43,                      /* INTO  */
  YYSYMBOL_VALUES = 44,                    /* VALUES  */
  YYSYMBOL_FROM = 45,                      /* FROM  */
  YYSYMBOL_WHERE = 46,                     /* WHERE  */
  YYSYMBOL_AND = 47,                       /* AND  */
  YYSYMBOL_SET = 48,                       /* SET  */
  YYSYMBOL_ON = 49,                        /* ON  */
  YYSYMBOL_LOAD = 50,                      /* LOAD  */
  YYSYMBOL_DATA = 51,                      /* DATA  */
  YYSYMBOL_INFILE = 52,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 53,                   /* EXPLAIN  */
  YYSYMBOL_EQ = 54,                        /* EQ  */
  YYSYMBOL_LT = 55,                        /* LT  */
  YYSYMBOL_GT = 56,                        /* GT  */
  YYSYMBOL_LE = 57,                        /* LE  */
  YYSYMBOL_GE = 58,                        /* GE  */
  YYSYMBOL_NE = 59,                        /* NE  */
  YYSYMBOL_NUMBER = 60,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 61,                     /* FLOAT  */
  YYSYMBOL_ID = 62,                        /* ID  */
  YYSYMBOL_DATE_STR = 63,                  /* DATE_STR  */
  YYSYMBOL_SSS = 64,                       /* SSS  */
  YYSYMBOL_65_ = 65,                       /* '+'  */
  YYSYMBOL_66_ = 66,                       /* '-'  */
  YYSYMBOL_67_ = 67,                       /* '*'  */
  YYSYMBOL_68_ = 68,                       /* '/'  */
  YYSYMBOL_UMINUS = 69,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 70,                  /* $accept  */
  YYSYMBOL_commands = 71,                  /* commands  */
  YYSYMBOL_command_wrapper = 72,           /* command_wrapper  */
  YYSYMBOL_avg_stmt = 73,                  /* avg_stmt  */
  YYSYMBOL_max_stmt = 74,                  /* max_stmt  */
  YYSYMBOL_min_stmt = 75,                  /* min_stmt  */
  YYSYMBOL_count_stmt = 76,                /* count_stmt  */
  YYSYMBOL_sum_stmt = 77,                  /* sum_stmt  */
  YYSYMBOL_exit_stmt = 78,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 79,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 80,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 81,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 82,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 83,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 84,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 85,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 86,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 87,         /* create_index_stmt  */
  YYSYMBOL_drop_index_stmt = 88,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 89,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 90,             /* attr_def_list  */
  YYSYMBOL_attr_def = 91,                  /* attr_def  */
  YYSYMBOL_number = 92,                    /* number  */
  YYSYMBOL_type = 93,                      /* type  */
  YYSYMBOL_insert_stmt = 94,               /* insert_stmt  */
  YYSYMBOL_value_list = 95,                /* value_list  */
  YYSYMBOL_value = 96,                     /* value  */
  YYSYMBOL_delete_stmt = 97,               /* delete_stmt  */
  YYSYMBOL_update_stmt = 98,               /* update_stmt  */
  YYSYMBOL_update_list = 99,               /* update_list  */
  YYSYMBOL_select_stmt = 100,              /* select_stmt  */
  YYSYMBOL_join_list = 101,                /* join_list  */
  YYSYMBOL_join = 102,                     /* join  */
  YYSYMBOL_calc_stmt = 103,                /* calc_stmt  */
  YYSYMBOL_expression_list = 104,          /* expression_list  */
  YYSYMBOL_expression = 105,               /* expression  */
  YYSYMBOL_select_attr = 106,              /* select_attr  */
  YYSYMBOL_rel_attr = 107,                 /* rel_attr  */
  YYSYMBOL_attr_list = 108,                /* attr_list  */
  YYSYMBOL_rel_list = 109,                 /* rel_list  */
  YYSYMBOL_where = 110,                    /* where  */
  YYSYMBOL_condition_list = 111,           /* condition_list  */
  YYSYMBOL_condition = 112,                /* condition  */
  YYSYMBOL_comp_op = 113,                  /* comp_op  */
  YYSYMBOL_load_data_stmt = 114,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 115,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 116,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 117             /* opt_semicolon  */
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
#define YYLAST   273

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  70
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  263

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   320


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
       2,     2,    67,    65,     2,    66,     2,    68,     2,     2,
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
      69
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   213,   213,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   244,   250,   261,   267,   278,   284,   294,
     300,   308,   315,   321,   331,   337,   342,   348,   354,   360,
     366,   373,   379,   387,   401,   411,   431,   434,   447,   456,
     465,   474,   483,   492,   503,   506,   507,   508,   509,   512,
     529,   532,   540,   544,   548,   552,   558,   566,   579,   596,
     606,   619,   648,   651,   664,   677,   687,   692,   703,   706,
     709,   712,   715,   719,   722,   730,   737,   749,   754,   761,
     764,   767,   770,   773,   780,   783,   797,   800,   813,   816,
     822,   825,   830,   837,   849,   861,   873,   885,   902,   918,
     936,   954,   968,   982,   995,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1026,  1039,  1047,
    1057,  1058
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
  "JOIN_T", "IN_T", "EXIST_T", "DESC", "SHOW", "SYNC", "INSERT", "DELETE",
  "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN", "TRX_COMMIT",
  "TRX_ROLLBACK", "DATE_T", "INT_T", "STRING_T", "FLOAT_T", "HELP", "EXIT",
  "DOT", "INTO", "VALUES", "FROM", "WHERE", "AND", "SET", "ON", "LOAD",
  "DATA", "INFILE", "EXPLAIN", "EQ", "LT", "GT", "LE", "GE", "NE",
  "NUMBER", "FLOAT", "ID", "DATE_STR", "SSS", "'+'", "'-'", "'*'", "'/'",
  "UMINUS", "$accept", "commands", "command_wrapper", "avg_stmt",
  "max_stmt", "min_stmt", "count_stmt", "sum_stmt", "exit_stmt",
  "help_stmt", "sync_stmt", "begin_stmt", "commit_stmt", "rollback_stmt",
  "drop_table_stmt", "show_tables_stmt", "desc_table_stmt",
  "create_index_stmt", "drop_index_stmt", "create_table_stmt",
  "attr_def_list", "attr_def", "number", "type", "insert_stmt",
  "value_list", "value", "delete_stmt", "update_stmt", "update_list",
  "select_stmt", "join_list", "join", "calc_stmt", "expression_list",
  "expression", "select_attr", "rel_attr", "attr_list", "rel_list",
  "where", "condition_list", "condition", "comp_op", "load_data_stmt",
  "explain_stmt", "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-184)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     128,    34,    39,   147,    -6,   -45,    43,  -184,    14,    10,
       5,  -184,  -184,  -184,  -184,  -184,    24,    20,   128,    74,
      84,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,
    -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,
    -184,    26,    31,    41,    48,  -184,   147,  -184,  -184,  -184,
    -184,   147,  -184,  -184,    76,    75,    82,    85,    97,    98,
      72,  -184,  -184,  -184,  -184,  -184,  -184,    89,    99,  -184,
    -184,    68,    73,    88,   104,    87,  -184,  -184,  -184,  -184,
     121,   117,  -184,   118,    12,  -184,   147,   147,   147,   147,
     147,   108,   129,   131,   -44,   132,   134,   135,     0,  -184,
     116,   133,   136,   122,   120,   137,   143,   144,  -184,  -184,
     -42,   -42,  -184,  -184,   -11,    17,    64,    65,   149,    71,
    -184,   177,    99,   182,    21,  -184,   160,   133,  -184,   172,
      81,   184,   187,  -184,  -184,   156,  -184,   157,  -184,   158,
    -184,   159,  -184,  -184,   161,   162,   202,  -184,   122,   203,
     195,     9,   173,   173,  -184,   186,   122,  -184,   228,  -184,
    -184,  -184,  -184,    11,   137,   204,   174,   206,   208,   209,
     210,   211,   177,   222,   133,   202,   212,   215,     9,   212,
     216,   229,     2,  -184,  -184,  -184,  -184,  -184,  -184,  -184,
    -184,   111,    86,    21,   217,   188,   233,  -184,   192,   184,
    -184,   223,  -184,  -184,  -184,  -184,  -184,  -184,     0,  -184,
    -184,   122,   224,     9,   212,   225,   226,   173,  -184,  -184,
    -184,  -184,  -184,     9,  -184,  -184,  -184,   136,  -184,  -184,
    -184,   227,  -184,  -184,   213,   212,  -184,   212,   230,   232,
    -184,   173,     0,   212,   234,  -184,    35,    21,  -184,   235,
    -184,  -184,     0,  -184,   236,  -184,   241,  -184,  -184,  -184,
    -184,  -184,  -184
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    36,     0,     0,
       0,    37,    38,    39,    35,    34,     0,     0,     0,     0,
     130,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,     0,    66,     0,    62,    63,    64,
      65,     0,    84,    75,    76,     0,     0,     0,     0,     0,
      87,    85,    93,    90,    91,    92,    89,     0,    94,    42,
      41,     0,     0,     0,     0,     0,   128,     1,   131,     2,
       0,     0,    40,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
       0,    98,     0,     0,     0,     0,     0,     0,    82,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,     0,
      88,    96,    94,     0,   100,    67,     0,    98,   129,     0,
       0,    46,     0,    44,    32,     0,    25,     0,    27,     0,
      29,     0,    31,    23,     0,     0,    72,    95,     0,     0,
       0,     0,     0,     0,    99,   101,     0,    68,     0,    58,
      55,    56,    57,    49,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,    98,    72,    60,     0,     0,    60,
       0,   121,     0,   123,   125,   115,   116,   117,   118,   119,
     120,     0,     0,   100,    70,     0,     0,    52,     0,    46,
      45,     0,    33,    26,    28,    30,    24,    97,     0,    71,
      73,     0,     0,     0,    60,     0,     0,     0,   122,   124,
     126,   104,   106,     0,   103,   105,   102,     0,   127,    53,
      54,     0,    47,    43,     0,    60,    59,    60,     0,     0,
     113,     0,     0,    60,     0,    69,    48,   100,    61,     0,
     114,   111,     0,   108,     0,   107,     0,    50,    74,   112,
     110,   109,    51
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -184,  -184,   242,  -184,  -184,  -184,  -184,  -184,  -184,  -184,
    -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,
      54,   100,  -184,  -184,  -184,  -177,  -102,  -184,  -184,    42,
    -148,    93,  -184,  -184,   185,   113,  -184,    -4,   148,   101,
    -111,  -183,  -184,  -149,  -184,  -184,  -184,  -184
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    19,    20,    62,    63,    64,    65,    66,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
     165,   131,   231,   163,    33,   212,    52,    34,    35,   127,
      36,   174,   175,    37,    53,    54,    67,   153,    99,   146,
     125,   154,   155,   191,    38,    39,    40,    79
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      68,   128,   216,   180,   192,    55,    56,    57,    58,    59,
     226,    55,    56,    57,    58,    59,   157,    69,   117,     4,
     134,   219,   152,   118,   220,    89,    90,    45,   196,   197,
     215,   135,    55,    56,    57,    58,    59,   239,   149,    45,
      41,   198,    42,   108,   150,    43,   176,    44,   136,   179,
      70,   151,   256,   257,   194,    72,    60,    71,   248,   137,
     249,    61,    60,   209,   258,   238,   254,    73,   242,    47,
      48,    75,    49,    50,    77,   244,   214,    87,    88,    89,
      90,    47,    48,    60,    49,    50,    74,    78,    80,   221,
     224,   152,   252,    81,   122,   138,   140,    55,    56,    57,
      58,    59,   143,    82,    45,    91,   139,   141,    86,   235,
      83,   237,    92,   144,    96,    93,   223,   159,   160,   161,
     162,   243,    55,    56,    57,    58,    59,    94,    95,    45,
     100,    98,     1,     2,    97,   101,   102,     3,     4,   104,
      45,    87,    88,    89,    90,   152,    47,    48,    60,    49,
      50,   105,     5,     6,     7,     8,     9,    10,   103,    84,
     123,    11,    12,    13,    85,    45,   106,   107,    14,    15,
     114,    47,    48,    60,    49,    50,    16,    46,    17,   124,
     142,    18,    47,    48,   129,    49,    50,   222,   225,   181,
     182,   115,   183,   116,   119,   184,   120,   121,   126,   130,
     110,   111,   112,   113,   234,   132,   133,    47,    48,   145,
      49,    50,   148,    51,   156,   158,   164,   166,   167,   168,
     169,   170,   173,   171,   172,   178,   177,   185,   186,   187,
     188,   189,   190,   193,   195,   200,   201,   202,   253,   203,
     204,   205,   206,   208,   211,   213,   218,   217,   260,   227,
     228,   229,   230,   232,   233,   236,   240,   241,   246,   262,
      76,   250,   247,   251,   199,   255,   259,   261,   210,   245,
     147,   109,     0,   207
};

static const yytype_int16 yycheck[] =
{
       4,   103,   179,   151,   153,    11,    12,    13,    14,    15,
     193,    11,    12,    13,    14,    15,   127,    62,    62,    10,
      31,    19,   124,    67,    22,    67,    68,    18,    17,    18,
     178,    42,    11,    12,    13,    14,    15,   214,    17,    18,
       6,    30,     8,    31,    23,     6,   148,     8,    31,   151,
       7,    30,    17,    18,   156,    45,    62,    43,   235,    42,
     237,    67,    62,   174,   247,   213,   243,    62,   217,    60,
      61,    51,    63,    64,     0,   223,   178,    65,    66,    67,
      68,    60,    61,    62,    63,    64,    62,     3,    62,   191,
     192,   193,   241,    62,    98,    31,    31,    11,    12,    13,
      14,    15,    31,    62,    18,    30,    42,    42,    32,   211,
      62,   213,    30,    42,    42,    30,    30,    36,    37,    38,
      39,   223,    11,    12,    13,    14,    15,    30,    30,    18,
      62,    32,     4,     5,    45,    62,    48,     9,    10,    52,
      18,    65,    66,    67,    68,   247,    60,    61,    62,    63,
      64,    30,    24,    25,    26,    27,    28,    29,    54,    46,
      44,    33,    34,    35,    51,    18,    49,    49,    40,    41,
      62,    60,    61,    62,    63,    64,    48,    30,    50,    46,
      31,    53,    60,    61,    64,    63,    64,   191,   192,    16,
      17,    62,    19,    62,    62,    22,    62,    62,    62,    62,
      87,    88,    89,    90,   208,    62,    62,    60,    61,    32,
      63,    64,    30,    66,    54,    43,    32,    30,    62,    62,
      62,    62,    20,    62,    62,    30,    23,    54,    55,    56,
      57,    58,    59,    47,     6,    31,    62,    31,   242,    31,
      31,    31,    31,    21,    32,    30,    17,    31,   252,    32,
      62,    18,    60,   199,    31,    31,    31,    31,    31,    18,
      18,    31,    49,    31,   164,    31,    31,    31,   175,   227,
     122,    86,    -1,   172
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,     9,    10,    24,    25,    26,    27,    28,
      29,    33,    34,    35,    40,    41,    48,    50,    53,    71,
      72,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    94,    97,    98,   100,   103,   114,   115,
     116,     6,     8,     6,     8,    18,    30,    60,    61,    63,
      64,    66,    96,   104,   105,    11,    12,    13,    14,    15,
      62,    67,    73,    74,    75,    76,    77,   106,   107,    62,
       7,    43,    45,    62,    62,    51,    72,     0,     3,   117,
      62,    62,    62,    62,   105,   105,    32,    65,    66,    67,
      68,    30,    30,    30,    30,    30,    42,    45,    32,   108,
      62,    62,    48,    54,    52,    30,    49,    49,    31,   104,
     105,   105,   105,   105,    62,    62,    62,    62,    67,    62,
      62,    62,   107,    44,    46,   110,    62,    99,    96,    64,
      62,    91,    62,    62,    31,    42,    31,    42,    31,    42,
      31,    42,    31,    31,    42,    32,   109,   108,    30,    17,
      23,    30,    96,   107,   111,   112,    54,   110,    43,    36,
      37,    38,    39,    93,    32,    90,    30,    62,    62,    62,
      62,    62,    62,    20,   101,   102,    96,    23,    30,    96,
     100,    16,    17,    19,    22,    54,    55,    56,    57,    58,
      59,   113,   113,    47,    96,     6,    17,    18,    30,    91,
      31,    62,    31,    31,    31,    31,    31,   109,    21,   110,
     101,    32,    95,    30,    96,   100,    95,    31,    17,    19,
      22,    96,   107,    30,    96,   107,   111,    32,    62,    18,
      60,    92,    90,    31,   107,    96,    31,    96,   100,    95,
      31,    31,   113,    96,   100,    99,    31,    49,    95,    95,
      31,    31,   113,   107,    95,    31,    17,    18,   111,    31,
     107,    31,    18
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    70,    71,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    73,    73,    74,    74,    75,    75,    76,
      76,    76,    77,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    90,    91,    91,
      91,    91,    91,    91,    92,    93,    93,    93,    93,    94,
      95,    95,    96,    96,    96,    96,    96,    97,    98,    99,
      99,   100,   101,   101,   102,   103,   104,   104,   105,   105,
     105,   105,   105,   105,   105,   106,   106,   107,   107,   107,
     107,   107,   107,   107,   108,   108,   109,   109,   110,   110,
     111,   111,   111,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   114,   115,   116,
     117,   117
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     6,     4,     6,     4,     6,     4,
       6,     4,     4,     6,     1,     1,     1,     1,     1,     1,
       3,     2,     2,     8,     5,     7,     0,     3,     5,     2,
       6,     7,     3,     4,     1,     1,     1,     1,     1,     8,
       0,     3,     1,     1,     1,     1,     1,     4,     5,     5,
       3,     7,     0,     2,     5,     2,     1,     3,     3,     3,
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
#line 214 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1813 "yacc_sql.cpp"
    break;

  case 23: /* avg_stmt: AVG LBRACE ID RBRACE  */
#line 244 "yacc_sql.y"
                       {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::AVG_TYPE;
    free((yyvsp[-1].string));
  }
#line 1824 "yacc_sql.cpp"
    break;

  case 24: /* avg_stmt: AVG LBRACE ID DOT ID RBRACE  */
#line 250 "yacc_sql.y"
                                {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::AVG_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1837 "yacc_sql.cpp"
    break;

  case 25: /* max_stmt: MAX LBRACE ID RBRACE  */
#line 261 "yacc_sql.y"
                       {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::MAX_TYPE;
    free((yyvsp[-1].string));
  }
#line 1848 "yacc_sql.cpp"
    break;

  case 26: /* max_stmt: MAX LBRACE ID DOT ID RBRACE  */
#line 267 "yacc_sql.y"
                                {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::MAX_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1861 "yacc_sql.cpp"
    break;

  case 27: /* min_stmt: MIN LBRACE ID RBRACE  */
#line 278 "yacc_sql.y"
                       {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::MIN_TYPE;
    free((yyvsp[-1].string));
  }
#line 1872 "yacc_sql.cpp"
    break;

  case 28: /* min_stmt: MIN LBRACE ID DOT ID RBRACE  */
#line 284 "yacc_sql.y"
                                {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::MIN_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1885 "yacc_sql.cpp"
    break;

  case 29: /* count_stmt: COUNT LBRACE ID RBRACE  */
#line 294 "yacc_sql.y"
                         {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::COUNT_TYPE;
    free((yyvsp[-1].string));
  }
#line 1896 "yacc_sql.cpp"
    break;

  case 30: /* count_stmt: COUNT LBRACE ID DOT ID RBRACE  */
#line 300 "yacc_sql.y"
                                  {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::COUNT_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1909 "yacc_sql.cpp"
    break;

  case 31: /* count_stmt: COUNT LBRACE '*' RBRACE  */
#line 308 "yacc_sql.y"
                            {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = "*";
    (yyval.rel_attr)->agg_type = AggregationType::COUNT_STAR_TYPE;
  }
#line 1919 "yacc_sql.cpp"
    break;

  case 32: /* sum_stmt: SUM LBRACE ID RBRACE  */
#line 315 "yacc_sql.y"
                       {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::SUM_TYPE;
    free((yyvsp[-1].string));
  }
#line 1930 "yacc_sql.cpp"
    break;

  case 33: /* sum_stmt: SUM LBRACE ID DOT ID RBRACE  */
#line 321 "yacc_sql.y"
                                {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
    (yyval.rel_attr)->relation_name = (yyvsp[-3].string);
    (yyval.rel_attr)->agg_type = ((yyval.rel_attr)->attribute_name == "*") ? AggregationType::ERROR_TYPE : AggregationType::SUM_TYPE;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 1943 "yacc_sql.cpp"
    break;

  case 34: /* exit_stmt: EXIT  */
#line 331 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1952 "yacc_sql.cpp"
    break;

  case 35: /* help_stmt: HELP  */
#line 337 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1960 "yacc_sql.cpp"
    break;

  case 36: /* sync_stmt: SYNC  */
#line 342 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1968 "yacc_sql.cpp"
    break;

  case 37: /* begin_stmt: TRX_BEGIN  */
#line 348 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1976 "yacc_sql.cpp"
    break;

  case 38: /* commit_stmt: TRX_COMMIT  */
#line 354 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1984 "yacc_sql.cpp"
    break;

  case 39: /* rollback_stmt: TRX_ROLLBACK  */
#line 360 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1992 "yacc_sql.cpp"
    break;

  case 40: /* drop_table_stmt: DROP TABLE ID  */
#line 366 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2002 "yacc_sql.cpp"
    break;

  case 41: /* show_tables_stmt: SHOW TABLES  */
#line 373 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 2010 "yacc_sql.cpp"
    break;

  case 42: /* desc_table_stmt: DESC ID  */
#line 379 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2020 "yacc_sql.cpp"
    break;

  case 43: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID RBRACE  */
#line 388 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-5].string);
      create_index.relation_name = (yyvsp[-3].string);
      create_index.attribute_name = (yyvsp[-1].string);
      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 2035 "yacc_sql.cpp"
    break;

  case 44: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 402 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2047 "yacc_sql.cpp"
    break;

  case 45: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE  */
#line 412 "yacc_sql.y"
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
#line 2068 "yacc_sql.cpp"
    break;

  case 46: /* attr_def_list: %empty  */
#line 431 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2076 "yacc_sql.cpp"
    break;

  case 47: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 435 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2090 "yacc_sql.cpp"
    break;

  case 48: /* attr_def: ID type LBRACE number RBRACE  */
#line 448 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number) + 1;
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-4].string));
    }
#line 2103 "yacc_sql.cpp"
    break;

  case 49: /* attr_def: ID type  */
#line 457 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[0].number);
      (yyval.attr_info)->name = (yyvsp[-1].string);
      (yyval.attr_info)->length = VALUE_SIZE_NOT_CHARS;
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-1].string));
    }
#line 2116 "yacc_sql.cpp"
    break;

  case 50: /* attr_def: ID type LBRACE number RBRACE NULL_T  */
#line 466 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number) + 1;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-5].string));
    }
#line 2129 "yacc_sql.cpp"
    break;

  case 51: /* attr_def: ID type LBRACE number RBRACE NOT NULL_T  */
#line 475 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-5].number);
      (yyval.attr_info)->name = (yyvsp[-6].string);
      (yyval.attr_info)->length = (yyvsp[-3].number) + 1;
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-6].string));
    }
#line 2142 "yacc_sql.cpp"
    break;

  case 52: /* attr_def: ID type NULL_T  */
#line 484 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = VALUE_SIZE_NOT_CHARS;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-2].string));
    }
#line 2155 "yacc_sql.cpp"
    break;

  case 53: /* attr_def: ID type NOT NULL_T  */
#line 493 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-2].number);
      (yyval.attr_info)->name = (yyvsp[-3].string);
      (yyval.attr_info)->length = VALUE_SIZE_NOT_CHARS;
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-3].string));
    }
#line 2168 "yacc_sql.cpp"
    break;

  case 54: /* number: NUMBER  */
#line 503 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2174 "yacc_sql.cpp"
    break;

  case 55: /* type: INT_T  */
#line 506 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 2180 "yacc_sql.cpp"
    break;

  case 56: /* type: STRING_T  */
#line 507 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 2186 "yacc_sql.cpp"
    break;

  case 57: /* type: FLOAT_T  */
#line 508 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 2192 "yacc_sql.cpp"
    break;

  case 58: /* type: DATE_T  */
#line 509 "yacc_sql.y"
             { (yyval.number)=DATES; }
#line 2198 "yacc_sql.cpp"
    break;

  case 59: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE  */
#line 513 "yacc_sql.y"
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
#line 2215 "yacc_sql.cpp"
    break;

  case 60: /* value_list: %empty  */
#line 529 "yacc_sql.y"
    {
      (yyval.value_list) = new std::vector<Value>;
    }
#line 2223 "yacc_sql.cpp"
    break;

  case 61: /* value_list: COMMA value value_list  */
#line 532 "yacc_sql.y"
                              {
      (yyval.value_list) = (yyvsp[0].value_list);

      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2234 "yacc_sql.cpp"
    break;

  case 62: /* value: NUMBER  */
#line 540 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2243 "yacc_sql.cpp"
    break;

  case 63: /* value: FLOAT  */
#line 544 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2252 "yacc_sql.cpp"
    break;

  case 64: /* value: DATE_STR  */
#line 548 "yacc_sql.y"
              {
      (yyval.value) = new Value(str_to_date((yyvsp[0].string)));
      (yyloc) = (yylsp[0]);
    }
#line 2261 "yacc_sql.cpp"
    break;

  case 65: /* value: SSS  */
#line 552 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2272 "yacc_sql.cpp"
    break;

  case 66: /* value: NULL_T  */
#line 558 "yacc_sql.y"
            {
      (yyval.value) = new Value(false);
      (yyval.value)->set_null();
      (yyloc) = (yylsp[0]);
    }
#line 2282 "yacc_sql.cpp"
    break;

  case 67: /* delete_stmt: DELETE FROM ID where  */
#line 567 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2296 "yacc_sql.cpp"
    break;

  case 68: /* update_stmt: UPDATE ID SET update_list where  */
#line 580 "yacc_sql.y"
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
#line 2314 "yacc_sql.cpp"
    break;

  case 69: /* update_list: ID EQ value COMMA update_list  */
#line 597 "yacc_sql.y"
  {
    (yyval.update_list) = (yyvsp[0].update_list);
    update_value tmp;
    tmp.attribute_name = (yyvsp[-4].string);
    tmp.value = *(yyvsp[-2].value);
    (yyval.update_list)->emplace_back(tmp);
    free((yyvsp[-4].string));
    delete (yyvsp[-2].value);
  }
#line 2328 "yacc_sql.cpp"
    break;

  case 70: /* update_list: ID EQ value  */
#line 607 "yacc_sql.y"
  {
    (yyval.update_list) = new std::vector<update_value>;
    update_value tmp;
    tmp.attribute_name = (yyvsp[-2].string);
    tmp.value = *(yyvsp[0].value);
    (yyval.update_list)->emplace_back(tmp);
    free((yyvsp[-2].string));
    delete (yyvsp[0].value);
  }
#line 2342 "yacc_sql.cpp"
    break;

  case 71: /* select_stmt: SELECT select_attr FROM ID rel_list join_list where  */
#line 620 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-5].rel_attr_list) != nullptr) {
        (yyval.sql_node)->selection.attributes.swap(*(yyvsp[-5].rel_attr_list));
        delete (yyvsp[-5].rel_attr_list);
      }
      if ((yyvsp[-2].relation_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-2].relation_list));
        delete (yyvsp[-2].relation_list);
      }
      (yyval.sql_node)->selection.relations.push_back((yyvsp[-3].string));
      std::reverse((yyval.sql_node)->selection.relations.begin(), (yyval.sql_node)->selection.relations.end());

      if ((yyvsp[-1].join_list) != nullptr) {
        (yyval.sql_node)->selection.joins.swap(*(yyvsp[-1].join_list));
        delete (yyvsp[-1].join_list);
      }

      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->selection.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-3].string));
    }
#line 2371 "yacc_sql.cpp"
    break;

  case 72: /* join_list: %empty  */
#line 648 "yacc_sql.y"
  {
    (yyval.join_list) = nullptr;
  }
#line 2379 "yacc_sql.cpp"
    break;

  case 73: /* join_list: join join_list  */
#line 652 "yacc_sql.y"
  {
    if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new std::vector<JoinSqlNode>;
      }
    (yyval.join_list)->emplace_back(*(yyvsp[-1].join));
    delete (yyvsp[-1].join);
  }
#line 2393 "yacc_sql.cpp"
    break;

  case 74: /* join: INNER_T JOIN_T rel_attr ON condition_list  */
#line 665 "yacc_sql.y"
  {
    (yyval.join) = new JoinSqlNode;
    (yyval.join)->joined_rel = *(yyvsp[-2].rel_attr);
    /* 这里rel_attr会把表名识别成属性名，交换一下 */
    (yyval.join)->joined_rel.relation_name.swap((yyval.join)->joined_rel.attribute_name);
    (yyval.join)->conditions.swap(*(yyvsp[0].condition_list));
    delete (yyvsp[-2].rel_attr);
    delete (yyvsp[0].condition_list);
  }
#line 2407 "yacc_sql.cpp"
    break;

  case 75: /* calc_stmt: CALC expression_list  */
#line 678 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2418 "yacc_sql.cpp"
    break;

  case 76: /* expression_list: expression  */
#line 688 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<Expression*>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2427 "yacc_sql.cpp"
    break;

  case 77: /* expression_list: expression COMMA expression_list  */
#line 693 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-2].expression));
    }
#line 2440 "yacc_sql.cpp"
    break;

  case 78: /* expression: expression '+' expression  */
#line 703 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2448 "yacc_sql.cpp"
    break;

  case 79: /* expression: expression '-' expression  */
#line 706 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2456 "yacc_sql.cpp"
    break;

  case 80: /* expression: expression '*' expression  */
#line 709 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2464 "yacc_sql.cpp"
    break;

  case 81: /* expression: expression '/' expression  */
#line 712 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2472 "yacc_sql.cpp"
    break;

  case 82: /* expression: LBRACE expression RBRACE  */
#line 715 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2481 "yacc_sql.cpp"
    break;

  case 83: /* expression: '-' expression  */
#line 719 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2489 "yacc_sql.cpp"
    break;

  case 84: /* expression: value  */
#line 722 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2499 "yacc_sql.cpp"
    break;

  case 85: /* select_attr: '*'  */
#line 730 "yacc_sql.y"
        {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      RelAttrSqlNode attr;
      attr.relation_name  = "";
      attr.attribute_name = "*";
      (yyval.rel_attr_list)->emplace_back(attr);
    }
#line 2511 "yacc_sql.cpp"
    break;

  case 86: /* select_attr: rel_attr attr_list  */
#line 737 "yacc_sql.y"
                         {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2525 "yacc_sql.cpp"
    break;

  case 87: /* rel_attr: ID  */
#line 749 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2535 "yacc_sql.cpp"
    break;

  case 88: /* rel_attr: ID DOT ID  */
#line 754 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2547 "yacc_sql.cpp"
    break;

  case 89: /* rel_attr: sum_stmt  */
#line 761 "yacc_sql.y"
               {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2555 "yacc_sql.cpp"
    break;

  case 90: /* rel_attr: max_stmt  */
#line 764 "yacc_sql.y"
               {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2563 "yacc_sql.cpp"
    break;

  case 91: /* rel_attr: min_stmt  */
#line 767 "yacc_sql.y"
               {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2571 "yacc_sql.cpp"
    break;

  case 92: /* rel_attr: count_stmt  */
#line 770 "yacc_sql.y"
                 {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2579 "yacc_sql.cpp"
    break;

  case 93: /* rel_attr: avg_stmt  */
#line 773 "yacc_sql.y"
               {
      (yyval.rel_attr) = (yyvsp[0].rel_attr);
    }
#line 2587 "yacc_sql.cpp"
    break;

  case 94: /* attr_list: %empty  */
#line 780 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2595 "yacc_sql.cpp"
    break;

  case 95: /* attr_list: COMMA rel_attr attr_list  */
#line 783 "yacc_sql.y"
                               { /*我超，递归*/
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }

      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2610 "yacc_sql.cpp"
    break;

  case 96: /* rel_list: %empty  */
#line 797 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2618 "yacc_sql.cpp"
    break;

  case 97: /* rel_list: COMMA ID rel_list  */
#line 800 "yacc_sql.y"
                        {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }

      (yyval.relation_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 2633 "yacc_sql.cpp"
    break;

  case 98: /* where: %empty  */
#line 813 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2641 "yacc_sql.cpp"
    break;

  case 99: /* where: WHERE condition_list  */
#line 816 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 2649 "yacc_sql.cpp"
    break;

  case 100: /* condition_list: %empty  */
#line 822 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2657 "yacc_sql.cpp"
    break;

  case 101: /* condition_list: condition  */
#line 825 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 2667 "yacc_sql.cpp"
    break;

  case 102: /* condition_list: condition AND condition_list  */
#line 830 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 2677 "yacc_sql.cpp"
    break;

  case 103: /* condition: rel_attr comp_op value  */
#line 838 "yacc_sql.y"
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
#line 2693 "yacc_sql.cpp"
    break;

  case 104: /* condition: value comp_op value  */
#line 850 "yacc_sql.y"
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
#line 2709 "yacc_sql.cpp"
    break;

  case 105: /* condition: rel_attr comp_op rel_attr  */
#line 862 "yacc_sql.y"
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
#line 2725 "yacc_sql.cpp"
    break;

  case 106: /* condition: value comp_op rel_attr  */
#line 874 "yacc_sql.y"
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
#line 2741 "yacc_sql.cpp"
    break;

  case 107: /* condition: rel_attr comp_op LBRACE select_stmt RBRACE  */
#line 886 "yacc_sql.y"
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
#line 2762 "yacc_sql.cpp"
    break;

  case 108: /* condition: LBRACE select_stmt RBRACE comp_op rel_attr  */
#line 903 "yacc_sql.y"
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
#line 2782 "yacc_sql.cpp"
    break;

  case 109: /* condition: rel_attr comp_op LBRACE value value_list RBRACE  */
#line 919 "yacc_sql.y"
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
#line 2804 "yacc_sql.cpp"
    break;

  case 110: /* condition: LBRACE value value_list RBRACE comp_op rel_attr  */
#line 937 "yacc_sql.y"
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
#line 2826 "yacc_sql.cpp"
    break;

  case 111: /* condition: EXIST_T LBRACE value value_list RBRACE  */
#line 955 "yacc_sql.y"
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
#line 2844 "yacc_sql.cpp"
    break;

  case 112: /* condition: NOT EXIST_T LBRACE value value_list RBRACE  */
#line 969 "yacc_sql.y"
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
#line 2862 "yacc_sql.cpp"
    break;

  case 113: /* condition: EXIST_T LBRACE select_stmt RBRACE  */
#line 983 "yacc_sql.y"
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
#line 2879 "yacc_sql.cpp"
    break;

  case 114: /* condition: NOT EXIST_T LBRACE select_stmt RBRACE  */
#line 996 "yacc_sql.y"
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
#line 2896 "yacc_sql.cpp"
    break;

  case 115: /* comp_op: EQ  */
#line 1011 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 2902 "yacc_sql.cpp"
    break;

  case 116: /* comp_op: LT  */
#line 1012 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 2908 "yacc_sql.cpp"
    break;

  case 117: /* comp_op: GT  */
#line 1013 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 2914 "yacc_sql.cpp"
    break;

  case 118: /* comp_op: LE  */
#line 1014 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 2920 "yacc_sql.cpp"
    break;

  case 119: /* comp_op: GE  */
#line 1015 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 2926 "yacc_sql.cpp"
    break;

  case 120: /* comp_op: NE  */
#line 1016 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 2932 "yacc_sql.cpp"
    break;

  case 121: /* comp_op: IS_T  */
#line 1017 "yacc_sql.y"
           { (yyval.comp) = IS; }
#line 2938 "yacc_sql.cpp"
    break;

  case 122: /* comp_op: IS_T NOT  */
#line 1018 "yacc_sql.y"
               { (yyval.comp) = IS_NOT; }
#line 2944 "yacc_sql.cpp"
    break;

  case 123: /* comp_op: LIKE_T  */
#line 1019 "yacc_sql.y"
             { (yyval.comp) = LIKE; }
#line 2950 "yacc_sql.cpp"
    break;

  case 124: /* comp_op: NOT LIKE_T  */
#line 1020 "yacc_sql.y"
                 { (yyval.comp) = LIKE_NOT; }
#line 2956 "yacc_sql.cpp"
    break;

  case 125: /* comp_op: IN_T  */
#line 1021 "yacc_sql.y"
           { (yyval.comp) = IN; }
#line 2962 "yacc_sql.cpp"
    break;

  case 126: /* comp_op: NOT IN_T  */
#line 1022 "yacc_sql.y"
               { (yyval.comp) = IN_NOT; }
#line 2968 "yacc_sql.cpp"
    break;

  case 127: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1027 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);

      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 2982 "yacc_sql.cpp"
    break;

  case 128: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1040 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 2991 "yacc_sql.cpp"
    break;

  case 129: /* set_variable_stmt: SET ID EQ value  */
#line 1048 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3003 "yacc_sql.cpp"
    break;


#line 3007 "yacc_sql.cpp"

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

#line 1060 "yacc_sql.y"

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
