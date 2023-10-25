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
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "syntax.y"

    #define YYSTYPE treeNode*
    #include "lex.yy.c"
    #include "treeNode.h"
    #include "string.h"

    treeNode* root;

    void yyerror(const char *s);
    
    void print_B_error(const char *cause, size_t lineno)
    { fprintf(yyout, "Error type B at Line %d: %s \';\'\n", lineno); }

#line 85 "syntax.tab.c"

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

#include "syntax.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TYPE = 3,                       /* TYPE  */
  YYSYMBOL_STRUCT = 4,                     /* STRUCT  */
  YYSYMBOL_IF = 5,                         /* IF  */
  YYSYMBOL_ELSE = 6,                       /* ELSE  */
  YYSYMBOL_WHILE = 7,                      /* WHILE  */
  YYSYMBOL_FOR = 8,                        /* FOR  */
  YYSYMBOL_RETURN = 9,                     /* RETURN  */
  YYSYMBOL_INCLUDE = 10,                   /* INCLUDE  */
  YYSYMBOL_SHARP = 11,                     /* SHARP  */
  YYSYMBOL_DOT = 12,                       /* DOT  */
  YYSYMBOL_SEMI = 13,                      /* SEMI  */
  YYSYMBOL_COLON = 14,                     /* COLON  */
  YYSYMBOL_COMMA = 15,                     /* COMMA  */
  YYSYMBOL_ASSIGN = 16,                    /* ASSIGN  */
  YYSYMBOL_LT = 17,                        /* LT  */
  YYSYMBOL_LE = 18,                        /* LE  */
  YYSYMBOL_GT = 19,                        /* GT  */
  YYSYMBOL_GE = 20,                        /* GE  */
  YYSYMBOL_NE = 21,                        /* NE  */
  YYSYMBOL_EQ = 22,                        /* EQ  */
  YYSYMBOL_PLUS = 23,                      /* PLUS  */
  YYSYMBOL_MINUS = 24,                     /* MINUS  */
  YYSYMBOL_MUL = 25,                       /* MUL  */
  YYSYMBOL_DIV = 26,                       /* DIV  */
  YYSYMBOL_AND = 27,                       /* AND  */
  YYSYMBOL_OR = 28,                        /* OR  */
  YYSYMBOL_NOT = 29,                       /* NOT  */
  YYSYMBOL_LP = 30,                        /* LP  */
  YYSYMBOL_RP = 31,                        /* RP  */
  YYSYMBOL_LB = 32,                        /* LB  */
  YYSYMBOL_RB = 33,                        /* RB  */
  YYSYMBOL_LC = 34,                        /* LC  */
  YYSYMBOL_RC = 35,                        /* RC  */
  YYSYMBOL_ABSTR = 36,                     /* ABSTR  */
  YYSYMBOL_ID = 37,                        /* ID  */
  YYSYMBOL_INT = 38,                       /* INT  */
  YYSYMBOL_FLOAT = 39,                     /* FLOAT  */
  YYSYMBOL_CHAR = 40,                      /* CHAR  */
  YYSYMBOL_INVALID_CHAR = 41,              /* INVALID_CHAR  */
  YYSYMBOL_INVALID_ID = 42,                /* INVALID_ID  */
  YYSYMBOL_SINGLE_LINE_COMMENT = 43,       /* SINGLE_LINE_COMMENT  */
  YYSYMBOL_MULTI_LINE_COMMENT = 44,        /* MULTI_LINE_COMMENT  */
  YYSYMBOL_LOWER_ELSE = 45,                /* LOWER_ELSE  */
  YYSYMBOL_LOWER_FOR = 46,                 /* LOWER_FOR  */
  YYSYMBOL_UPPER_FOR = 47,                 /* UPPER_FOR  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_Program = 49,                   /* Program  */
  YYSYMBOL_IncDef = 50,                    /* IncDef  */
  YYSYMBOL_IncDefList = 51,                /* IncDefList  */
  YYSYMBOL_ExtDefList = 52,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 53,                    /* ExtDef  */
  YYSYMBOL_ExtDecList = 54,                /* ExtDecList  */
  YYSYMBOL_Specifier = 55,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 56,           /* StructSpecifier  */
  YYSYMBOL_VarDec = 57,                    /* VarDec  */
  YYSYMBOL_FunDec = 58,                    /* FunDec  */
  YYSYMBOL_VarList = 59,                   /* VarList  */
  YYSYMBOL_ParamDec = 60,                  /* ParamDec  */
  YYSYMBOL_CompSt = 61,                    /* CompSt  */
  YYSYMBOL_StmtList = 62,                  /* StmtList  */
  YYSYMBOL_Stmt = 63,                      /* Stmt  */
  YYSYMBOL_DefList = 64,                   /* DefList  */
  YYSYMBOL_Def = 65,                       /* Def  */
  YYSYMBOL_DecList = 66,                   /* DecList  */
  YYSYMBOL_Dec = 67,                       /* Dec  */
  YYSYMBOL_Exp = 68,                       /* Exp  */
  YYSYMBOL_Args = 69                       /* Args  */
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
typedef yytype_uint8 yy_state_t;

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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   339

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  136

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   302


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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    40,    40,    43,    45,    46,    48,    49,    52,    53,
      54,    55,    56,    59,    60,    64,    65,    68,    69,    77,
      78,    81,    82,    85,    86,    89,    93,    96,    97,   100,
     101,   102,   103,   104,   105,   106,   111,   112,   115,   118,
     119,   122,   123,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   153,   154
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TYPE", "STRUCT", "IF",
  "ELSE", "WHILE", "FOR", "RETURN", "INCLUDE", "SHARP", "DOT", "SEMI",
  "COLON", "COMMA", "ASSIGN", "LT", "LE", "GT", "GE", "NE", "EQ", "PLUS",
  "MINUS", "MUL", "DIV", "AND", "OR", "NOT", "LP", "RP", "LB", "RB", "LC",
  "RC", "ABSTR", "ID", "INT", "FLOAT", "CHAR", "INVALID_CHAR",
  "INVALID_ID", "SINGLE_LINE_COMMENT", "MULTI_LINE_COMMENT", "LOWER_ELSE",
  "LOWER_FOR", "UPPER_FOR", "$accept", "Program", "IncDef", "IncDefList",
  "ExtDefList", "ExtDef", "ExtDecList", "Specifier", "StructSpecifier",
  "VarDec", "FunDec", "VarList", "ParamDec", "CompSt", "StmtList", "Stmt",
  "DefList", "Def", "DecList", "Dec", "Exp", "Args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-54)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -3,     5,    18,    -3,    10,   -16,   -54,   -54,   -54,   -15,
     -54,    10,     6,   -54,   -54,   -13,   -54,   -54,   -54,    20,
      32,    29,    28,    10,    22,   -54,    26,    27,    10,   -54,
      26,    33,    10,   -54,    26,    41,    49,   -54,   -54,    40,
      47,    35,    61,    60,   -54,   -54,    48,   -54,    10,   -54,
      52,    53,    59,   299,   299,   299,   299,    75,   -54,   -54,
     -54,   -54,    44,    47,    94,   299,   -54,    26,   -54,   299,
     299,    10,   111,   283,    15,   128,   287,   -54,   -54,    54,
     -54,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   231,   -54,   145,   162,    77,
     -54,   -54,   -54,   180,   110,   -54,   248,    34,    34,    34,
      34,    34,    34,   283,   283,    15,    15,   281,   265,    76,
      47,    47,   299,   299,   -54,   -54,   119,   -54,   197,   -54,
      47,   299,   -54,   214,    47,   -54
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     0,     4,     6,     0,     1,     5,    15,     0,
       2,     6,     0,    16,     3,    18,     7,    12,     9,    19,
      11,    13,     0,    36,     0,     8,     0,     0,    36,    10,
       0,     0,    36,    22,     0,     0,    24,    19,    14,     0,
      27,    41,     0,    39,    17,    37,    25,    21,     0,    20,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    30,     0,    27,     0,     0,    38,     0,    23,     0,
       0,    36,     0,    57,    58,     0,     0,    26,    28,     0,
      29,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    42,    40,     0,     0,     0,
      31,    56,    60,    68,     0,    62,    43,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    44,    45,     0,
       0,     0,     0,     0,    59,    61,    32,    34,     0,    67,
       0,     0,    33,     0,     0,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -54,   -54,   -54,   139,   147,   -54,   149,     0,   -54,   -25,
     -54,   143,   -54,   189,   164,   200,   -22,   -54,   161,   -54,
     -53,   121
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     4,    10,    11,    20,    30,    13,    21,
      22,    35,    36,    61,    62,    63,    31,    32,    42,    43,
      64,   104
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      72,    73,    74,    75,    12,    41,    40,    17,     1,    46,
      45,    12,    95,     8,     9,     5,    97,    98,     6,    18,
      14,    23,    15,   103,    34,     8,     9,    79,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    41,    19,    26,    25,    79,    94,    34,    99,
      24,    65,    50,    33,    51,    52,    53,    88,    89,    90,
      91,    27,    28,    37,    48,    39,    94,    27,    44,   128,
     103,    54,    47,    49,    66,    67,    55,    56,   133,    77,
      27,    28,    69,    70,    57,    58,    59,    60,    79,    71,
     122,   105,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    76,    79,    80,    94,   125,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    79,   100,   130,    94,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      79,   124,     7,    94,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    79,    16,   101,
      94,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    79,    38,   120,    94,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    68,    79,   121,    94,   123,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    79,
     131,    29,    94,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    79,    78,    96,    94,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    79,   129,   134,    94,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      79,     0,     0,    94,     0,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    79,     0,     0,
      94,     0,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    79,     0,    79,     0,    94,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    90,    91,
       0,    54,     0,    94,     0,    94,    55,    56,   102,     0,
     126,   127,     0,    54,    57,    58,    59,    60,    55,    56,
     132,     0,     0,     0,   135,     0,    57,    58,    59,    60
};

static const yytype_int16 yycheck[] =
{
      53,    54,    55,    56,     4,    30,    28,     1,    11,    34,
      32,    11,    65,     3,     4,    10,    69,    70,     0,    13,
      36,    34,    37,    76,    24,     3,     4,    12,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    67,    37,    15,    13,    12,    32,    48,    71,
      30,    16,     5,    31,     7,     8,     9,    23,    24,    25,
      26,    32,    34,    37,    15,    38,    32,    32,    35,   122,
     123,    24,    31,    33,    13,    15,    29,    30,   131,    35,
      32,    34,    30,    30,    37,    38,    39,    40,    12,    30,
      13,    37,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    30,    12,    13,    32,    33,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    12,    13,     6,    32,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      12,    31,     3,    32,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    12,    11,    31,
      32,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    12,    26,    31,    32,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    48,    12,    31,    32,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    12,
      13,    22,    32,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    12,    63,    67,    32,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    12,   123,    31,    32,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      12,    -1,    -1,    32,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    12,    -1,    -1,
      32,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    12,    -1,    12,    -1,    32,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    25,    26,
      -1,    24,    -1,    32,    -1,    32,    29,    30,    31,    -1,
     120,   121,    -1,    24,    37,    38,    39,    40,    29,    30,
     130,    -1,    -1,    -1,   134,    -1,    37,    38,    39,    40
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    11,    49,    50,    51,    10,     0,    51,     3,     4,
      52,    53,    55,    56,    36,    37,    52,     1,    13,    37,
      54,    57,    58,    34,    30,    13,    15,    32,    34,    61,
      55,    64,    65,    31,    55,    59,    60,    37,    54,    38,
      64,    57,    66,    67,    35,    64,    57,    31,    15,    33,
       5,     7,     8,     9,    24,    29,    30,    37,    38,    39,
      40,    61,    62,    63,    68,    16,    13,    15,    59,    30,
      30,    30,    68,    68,    68,    68,    30,    35,    62,    12,
      13,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    32,    68,    66,    68,    68,    64,
      13,    31,    31,    68,    69,    37,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      31,    31,    13,    15,    31,    33,    63,    63,    68,    69,
       6,    13,    63,    68,    31,    63
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    50,    51,    51,    52,    52,    53,    53,
      53,    53,    53,    54,    54,    55,    55,    56,    56,    57,
      57,    58,    58,    59,    59,    60,    61,    62,    62,    63,
      63,    63,    63,    63,    63,    63,    64,    64,    65,    66,
      66,    67,    67,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    69,    69
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     3,     0,     2,     0,     2,     3,     2,
       3,     2,     2,     1,     3,     1,     1,     5,     2,     1,
       4,     4,     3,     3,     1,     2,     4,     0,     2,     2,
       1,     3,     5,     7,     5,     9,     0,     2,     3,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     4,
       3,     4,     3,     1,     1,     1,     1,     3,     1
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
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
      yychar = yylex ();
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: IncDefList ExtDefList  */
#line 40 "syntax.y"
                                { addn(yyval, "Program", 2, yyvsp[-1], yyvsp[0]); root = yyval;}
#line 1267 "syntax.tab.c"
    break;

  case 3: /* IncDef: SHARP INCLUDE ABSTR  */
#line 43 "syntax.y"
                             { addn(yyval, "IncDef", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1273 "syntax.tab.c"
    break;

  case 4: /* IncDefList: %empty  */
#line 45 "syntax.y"
                        { add0(yyval, "IncDefList"); }
#line 1279 "syntax.tab.c"
    break;

  case 5: /* IncDefList: IncDef IncDefList  */
#line 46 "syntax.y"
                        { addn(yyval, "IncDefList", 2, yyvsp[-1], yyvsp[0]); }
#line 1285 "syntax.tab.c"
    break;

  case 6: /* ExtDefList: %empty  */
#line 48 "syntax.y"
                        { add0(yyval, "ExtDefList"); }
#line 1291 "syntax.tab.c"
    break;

  case 7: /* ExtDefList: ExtDef ExtDefList  */
#line 49 "syntax.y"
                        { addn(yyval, "ExtDefList", 2, yyvsp[-1], yyvsp[0]); }
#line 1297 "syntax.tab.c"
    break;

  case 8: /* ExtDef: Specifier ExtDecList SEMI  */
#line 52 "syntax.y"
                                    { addn(yyval, "ExtDef", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1303 "syntax.tab.c"
    break;

  case 9: /* ExtDef: Specifier SEMI  */
#line 53 "syntax.y"
                                    { addn(yyval, "ExtDef", 2, yyvsp[-1], yyvsp[0]); }
#line 1309 "syntax.tab.c"
    break;

  case 10: /* ExtDef: Specifier FunDec CompSt  */
#line 54 "syntax.y"
                                    { addn(yyval, "ExtDef", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1315 "syntax.tab.c"
    break;

  case 11: /* ExtDef: Specifier ExtDecList  */
#line 55 "syntax.y"
                                    { has_error = 1; print_B_error("Missing semicolon", yyvsp[-1]->lineno); }
#line 1321 "syntax.tab.c"
    break;

  case 12: /* ExtDef: Specifier error  */
#line 56 "syntax.y"
                                    { has_error = 1; print_B_error("Missing semicolon", yyvsp[-1]->lineno); }
#line 1327 "syntax.tab.c"
    break;

  case 13: /* ExtDecList: VarDec  */
#line 59 "syntax.y"
                                { add1(yyval, "ExtDecList", 1, yyvsp[0]); }
#line 1333 "syntax.tab.c"
    break;

  case 14: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 60 "syntax.y"
                                { addn(yyval, "ExtDecList", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1339 "syntax.tab.c"
    break;

  case 15: /* Specifier: TYPE  */
#line 64 "syntax.y"
                        { add1(yyval, "Specifier", 1, yyvsp[0]); }
#line 1345 "syntax.tab.c"
    break;

  case 16: /* Specifier: StructSpecifier  */
#line 65 "syntax.y"
                        { add1(yyval, "Specifier", 1, yyvsp[0]); }
#line 1351 "syntax.tab.c"
    break;

  case 17: /* StructSpecifier: STRUCT ID LC DefList RC  */
#line 68 "syntax.y"
                                            { addn(yyval, "StructSpecifier", 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1357 "syntax.tab.c"
    break;

  case 18: /* StructSpecifier: STRUCT ID  */
#line 69 "syntax.y"
                                            { addn(yyval, "StructSpecifier", 2, yyvsp[-1], yyvsp[0]); }
#line 1363 "syntax.tab.c"
    break;

  case 19: /* VarDec: ID  */
#line 77 "syntax.y"
                        { add1(yyval, "VarDec", 1, yyvsp[0]); }
#line 1369 "syntax.tab.c"
    break;

  case 20: /* VarDec: VarDec LB INT RB  */
#line 78 "syntax.y"
                        { addn(yyval, "VarDec", 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1375 "syntax.tab.c"
    break;

  case 21: /* FunDec: ID LP VarList RP  */
#line 81 "syntax.y"
                            { addn(yyval, "FunDec", 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1381 "syntax.tab.c"
    break;

  case 22: /* FunDec: ID LP RP  */
#line 82 "syntax.y"
                            { addn(yyval, "FunDec", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1387 "syntax.tab.c"
    break;

  case 23: /* VarList: ParamDec COMMA VarList  */
#line 85 "syntax.y"
                                    { addn(yyval, "VarList", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1393 "syntax.tab.c"
    break;

  case 24: /* VarList: ParamDec  */
#line 86 "syntax.y"
                                    { add1(yyval, "VarList", 1, yyvsp[0]); }
#line 1399 "syntax.tab.c"
    break;

  case 25: /* ParamDec: Specifier VarDec  */
#line 89 "syntax.y"
                            { addn(yyval, "ParamDec", 2, yyvsp[-1], yyvsp[0]); }
#line 1405 "syntax.tab.c"
    break;

  case 26: /* CompSt: LC DefList StmtList RC  */
#line 93 "syntax.y"
                                { addn(yyval, "CompSt", 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1411 "syntax.tab.c"
    break;

  case 27: /* StmtList: %empty  */
#line 96 "syntax.y"
           { add0(yyval, "StmtList"); }
#line 1417 "syntax.tab.c"
    break;

  case 28: /* StmtList: Stmt StmtList  */
#line 97 "syntax.y"
                    { addn(yyval, "StmtList", 2, yyvsp[-1], yyvsp[0]); }
#line 1423 "syntax.tab.c"
    break;

  case 29: /* Stmt: Exp SEMI  */
#line 100 "syntax.y"
                                                { addn(yyval, "Stmt", 2, yyvsp[-1], yyvsp[0]); }
#line 1429 "syntax.tab.c"
    break;

  case 30: /* Stmt: CompSt  */
#line 101 "syntax.y"
                                                { add1(yyval, "Stmt", 1, yyvsp[0]); }
#line 1435 "syntax.tab.c"
    break;

  case 31: /* Stmt: RETURN Exp SEMI  */
#line 102 "syntax.y"
                                                { addn(yyval, "Stmt", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1441 "syntax.tab.c"
    break;

  case 32: /* Stmt: IF LP Exp RP Stmt  */
#line 103 "syntax.y"
                                                { addn(yyval, "Stmt", 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1447 "syntax.tab.c"
    break;

  case 33: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 104 "syntax.y"
                                                { addn(yyval, "Stmt", 7, yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1453 "syntax.tab.c"
    break;

  case 34: /* Stmt: WHILE LP Exp RP Stmt  */
#line 105 "syntax.y"
                                                { addn(yyval, "Stmt", 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1459 "syntax.tab.c"
    break;

  case 35: /* Stmt: FOR LP DefList SEMI Exp SEMI Exp RP Stmt  */
#line 106 "syntax.y"
                                                                { addn(yyval, "Stmt", 9, yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1465 "syntax.tab.c"
    break;

  case 36: /* DefList: %empty  */
#line 111 "syntax.y"
                    { add0(yyval, "DefList"); }
#line 1471 "syntax.tab.c"
    break;

  case 37: /* DefList: Def DefList  */
#line 112 "syntax.y"
                    { addn(yyval, "DefList", 2, yyvsp[-1], yyvsp[0]); }
#line 1477 "syntax.tab.c"
    break;

  case 38: /* Def: Specifier DecList SEMI  */
#line 115 "syntax.y"
                             { addn(yyval, "Def", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1483 "syntax.tab.c"
    break;

  case 39: /* DecList: Dec  */
#line 118 "syntax.y"
                        { add1(yyval, "DecList", 1, yyvsp[0]); }
#line 1489 "syntax.tab.c"
    break;

  case 40: /* DecList: Dec COMMA DecList  */
#line 119 "syntax.y"
                        { addn(yyval, "DecList", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1495 "syntax.tab.c"
    break;

  case 41: /* Dec: VarDec  */
#line 122 "syntax.y"
                        { add1(yyval, "Dec", 1, yyvsp[0]); }
#line 1501 "syntax.tab.c"
    break;

  case 42: /* Dec: VarDec ASSIGN Exp  */
#line 123 "syntax.y"
                        { addn(yyval, "Dec", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1507 "syntax.tab.c"
    break;

  case 43: /* Exp: Exp ASSIGN Exp  */
#line 127 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1513 "syntax.tab.c"
    break;

  case 44: /* Exp: Exp AND Exp  */
#line 128 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1519 "syntax.tab.c"
    break;

  case 45: /* Exp: Exp OR Exp  */
#line 129 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1525 "syntax.tab.c"
    break;

  case 46: /* Exp: Exp LT Exp  */
#line 130 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1531 "syntax.tab.c"
    break;

  case 47: /* Exp: Exp LE Exp  */
#line 131 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1537 "syntax.tab.c"
    break;

  case 48: /* Exp: Exp GT Exp  */
#line 132 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1543 "syntax.tab.c"
    break;

  case 49: /* Exp: Exp GE Exp  */
#line 133 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1549 "syntax.tab.c"
    break;

  case 50: /* Exp: Exp NE Exp  */
#line 134 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1555 "syntax.tab.c"
    break;

  case 51: /* Exp: Exp EQ Exp  */
#line 135 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1561 "syntax.tab.c"
    break;

  case 52: /* Exp: Exp PLUS Exp  */
#line 136 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1567 "syntax.tab.c"
    break;

  case 53: /* Exp: Exp MINUS Exp  */
#line 137 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1573 "syntax.tab.c"
    break;

  case 54: /* Exp: Exp MUL Exp  */
#line 138 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1579 "syntax.tab.c"
    break;

  case 55: /* Exp: Exp DIV Exp  */
#line 139 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1585 "syntax.tab.c"
    break;

  case 56: /* Exp: LP Exp RP  */
#line 140 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1591 "syntax.tab.c"
    break;

  case 57: /* Exp: MINUS Exp  */
#line 141 "syntax.y"
                        { addn(yyval, "Exp", 2, yyvsp[-1], yyvsp[0]); }
#line 1597 "syntax.tab.c"
    break;

  case 58: /* Exp: NOT Exp  */
#line 142 "syntax.y"
                        { addn(yyval, "Exp", 2, yyvsp[-1], yyvsp[0]); }
#line 1603 "syntax.tab.c"
    break;

  case 59: /* Exp: ID LP Args RP  */
#line 143 "syntax.y"
                        { addn(yyval, "Exp", 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1609 "syntax.tab.c"
    break;

  case 60: /* Exp: ID LP RP  */
#line 144 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1615 "syntax.tab.c"
    break;

  case 61: /* Exp: Exp LB Exp RB  */
#line 145 "syntax.y"
                        { addn(yyval, "Exp", 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1621 "syntax.tab.c"
    break;

  case 62: /* Exp: Exp DOT ID  */
#line 146 "syntax.y"
                        { addn(yyval, "Exp", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1627 "syntax.tab.c"
    break;

  case 63: /* Exp: ID  */
#line 147 "syntax.y"
                        { add1(yyval, "Exp", 1, yyvsp[0]); }
#line 1633 "syntax.tab.c"
    break;

  case 64: /* Exp: INT  */
#line 148 "syntax.y"
                        { add1(yyval, "Exp", 1, yyvsp[0]); }
#line 1639 "syntax.tab.c"
    break;

  case 65: /* Exp: FLOAT  */
#line 149 "syntax.y"
                        { add1(yyval, "Exp", 1, yyvsp[0]); }
#line 1645 "syntax.tab.c"
    break;

  case 66: /* Exp: CHAR  */
#line 150 "syntax.y"
                        { add1(yyval, "Exp", 1, yyvsp[0]); }
#line 1651 "syntax.tab.c"
    break;

  case 67: /* Args: Exp COMMA Args  */
#line 153 "syntax.y"
                        { addn(yyval, "Args", 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1657 "syntax.tab.c"
    break;

  case 68: /* Args: Exp  */
#line 154 "syntax.y"
                        { add1(yyval, "Args", 1, yyvsp[0]); }
#line 1663 "syntax.tab.c"
    break;


#line 1667 "syntax.tab.c"

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
      yyerror (YY_("syntax error"));
    }

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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
  yyerror (YY_("memory exhausted"));
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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 157 "syntax.y"


void yyerror(const char *s)
{
    fprintf(stderr, "YYERROR: %s\n", s);
}

int main(int argc, char **argv)
{
    FILE* file_in;
    FILE* file_out;
    if(argc == 2 || argc == 3) {
        file_in = fopen(argv[1], "r");
        if (file_in == NULL) {
            perror("Error opening input file");
            return 1;
        }
        if(argc == 2) {
            int la = strlen(argv[1]);
            argv[1][la-3] = 'o';
            argv[1][la-2] = 'u';
            argv[1][la-1] = 't';
            file_out = fopen(argv[1], "w");
        } else if(argc == 3) {
            file_out = fopen(argv[2], "w");
        }
        if (file_out == NULL) {
            perror("Error opening output file");
            return 1;
        }
        // Redirect Flex to read from file
    } else if(argc == 1) {
        file_in = stdin;
        file_out = stdout;
    } else {
        fprintf(stderr, "Parameters Error!\nShould be %s [input_file] [output_file]\n", argv[0]);
        return -1;
    }
    yyin = file_in;
    yyout = file_out;
    yyparse();
    if (!has_error)
        output_tree(root, 0);
    fclose(file_in);
    fclose(file_out);
    return 0;
}
