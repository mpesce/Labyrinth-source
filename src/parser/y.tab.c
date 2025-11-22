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
#line 15 "vrml.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/QvNode.h"
#include "../include/QvSeparator.h"
#include "../include/QvGroup.h"
#include "../include/QvSwitch.h"
#include "../include/QvLevelOfDetail.h"
#include "../include/QvTransformSeparator.h"
#include "../include/QvTransform.h"
#include "../include/QvRotation.h"
#include "../include/QvTranslation.h"
#include "../include/QvScale.h"
#include "../include/QvMatrixTransform.h"
#include "../include/QvMaterial.h"
#include "../include/QvMaterialBinding.h"
#include "../include/QvNormalBinding.h"
#include "../include/QvShapeHints.h"
#include "../include/QvTexture2.h"
#include "../include/QvTexture2Transform.h"
#include "../include/QvTextureCoordinate2.h"
#include "../include/QvCoordinate3.h"
#include "../include/QvNormal.h"
#include "../include/QvSphere.h"
#include "../include/QvCube.h"
#include "../include/QvCone.h"
#include "../include/QvCylinder.h"
#include "../include/QvIndexedFaceSet.h"
#include "../include/QvIndexedLineSet.h"
#include "../include/QvPointSet.h"
#include "../include/QvDirectionalLight.h"
#include "../include/QvPointLight.h"
#include "../include/QvSpotLight.h"
#include "../include/QvOrthographicCamera.h"
#include "../include/QvPerspectiveCamera.h"
#include "../include/QvWWWAnchor.h"
#include "../include/QvWWWInline.h"
#include "../include/QvInfo.h"
#include "../include/QvUnknownNode.h"

/* Global root node */
static QvNode* vrml_root = NULL;

/* Name dictionary for DEF/USE */
static void* name_dict = NULL;

/* Error messages from strings analysis */
void yyerror(const char* s);
int yylex(void);

/* Forward declarations */
QvNode* create_node(const char* type);
void add_child_to_group(QvNode* parent, QvNode* child);

/* Exported functions for C++ parser interface */
#ifdef __cplusplus
extern "C" {
#endif
    int yyparse(void);
    QvNode* vrml_get_root(void);
#ifdef __cplusplus
}
#endif

#line 137 "y.tab.c"

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

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    HEADER = 258,                  /* HEADER  */
    DEF = 259,                     /* DEF  */
    USE = 260,                     /* USE  */
    LBRACE = 261,                  /* LBRACE  */
    RBRACE = 262,                  /* RBRACE  */
    LBRACKET = 263,                /* LBRACKET  */
    RBRACKET = 264,                /* RBRACKET  */
    COMMA = 265,                   /* COMMA  */
    SEPARATOR = 266,               /* SEPARATOR  */
    TRANSFORMSEPARATOR = 267,      /* TRANSFORMSEPARATOR  */
    GROUP = 268,                   /* GROUP  */
    SWITCH = 269,                  /* SWITCH  */
    LEVELOFDETAIL = 270,           /* LEVELOFDETAIL  */
    TRANSFORM = 271,               /* TRANSFORM  */
    ROTATION = 272,                /* ROTATION  */
    TRANSLATION = 273,             /* TRANSLATION  */
    SCALE = 274,                   /* SCALE  */
    MATRIXTRANSFORM = 275,         /* MATRIXTRANSFORM  */
    MATERIAL = 276,                /* MATERIAL  */
    MATERIALBINDING = 277,         /* MATERIALBINDING  */
    NORMALBINDING = 278,           /* NORMALBINDING  */
    SHAPEHINTS = 279,              /* SHAPEHINTS  */
    COORDINATE3 = 280,             /* COORDINATE3  */
    NORMAL = 281,                  /* NORMAL  */
    TEXTURE2 = 282,                /* TEXTURE2  */
    TEXTURE2TRANSFORM = 283,       /* TEXTURE2TRANSFORM  */
    TEXTURECOORDINATE2 = 284,      /* TEXTURECOORDINATE2  */
    SPHERE = 285,                  /* SPHERE  */
    CUBE = 286,                    /* CUBE  */
    CONE = 287,                    /* CONE  */
    CYLINDER = 288,                /* CYLINDER  */
    INDEXEDFACESET = 289,          /* INDEXEDFACESET  */
    INDEXEDLINESET = 290,          /* INDEXEDLINESET  */
    POINTSET = 291,                /* POINTSET  */
    DIRECTIONALLIGHT = 292,        /* DIRECTIONALLIGHT  */
    POINTLIGHT = 293,              /* POINTLIGHT  */
    SPOTLIGHT = 294,               /* SPOTLIGHT  */
    ORTHOGRAPHICCAMERA = 295,      /* ORTHOGRAPHICCAMERA  */
    PERSPECTIVECAMERA = 296,       /* PERSPECTIVECAMERA  */
    WWWANCHOR = 297,               /* WWWANCHOR  */
    WWWINLINE = 298,               /* WWWINLINE  */
    INFO = 299,                    /* INFO  */
    IDENTIFIER = 300,              /* IDENTIFIER  */
    STRING = 301,                  /* STRING  */
    INTEGER = 302,                 /* INTEGER  */
    FLOAT = 303                    /* FLOAT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define HEADER 258
#define DEF 259
#define USE 260
#define LBRACE 261
#define RBRACE 262
#define LBRACKET 263
#define RBRACKET 264
#define COMMA 265
#define SEPARATOR 266
#define TRANSFORMSEPARATOR 267
#define GROUP 268
#define SWITCH 269
#define LEVELOFDETAIL 270
#define TRANSFORM 271
#define ROTATION 272
#define TRANSLATION 273
#define SCALE 274
#define MATRIXTRANSFORM 275
#define MATERIAL 276
#define MATERIALBINDING 277
#define NORMALBINDING 278
#define SHAPEHINTS 279
#define COORDINATE3 280
#define NORMAL 281
#define TEXTURE2 282
#define TEXTURE2TRANSFORM 283
#define TEXTURECOORDINATE2 284
#define SPHERE 285
#define CUBE 286
#define CONE 287
#define CYLINDER 288
#define INDEXEDFACESET 289
#define INDEXEDLINESET 290
#define POINTSET 291
#define DIRECTIONALLIGHT 292
#define POINTLIGHT 293
#define SPOTLIGHT 294
#define ORTHOGRAPHICCAMERA 295
#define PERSPECTIVECAMERA 296
#define WWWANCHOR 297
#define WWWINLINE 298
#define INFO 299
#define IDENTIFIER 300
#define STRING 301
#define INTEGER 302
#define FLOAT 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 81 "vrml.y"

    int             intval;
    float           floatval;
    char*           stringval;
    QvNode*         node;

#line 293 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_HEADER = 3,                     /* HEADER  */
  YYSYMBOL_DEF = 4,                        /* DEF  */
  YYSYMBOL_USE = 5,                        /* USE  */
  YYSYMBOL_LBRACE = 6,                     /* LBRACE  */
  YYSYMBOL_RBRACE = 7,                     /* RBRACE  */
  YYSYMBOL_LBRACKET = 8,                   /* LBRACKET  */
  YYSYMBOL_RBRACKET = 9,                   /* RBRACKET  */
  YYSYMBOL_COMMA = 10,                     /* COMMA  */
  YYSYMBOL_SEPARATOR = 11,                 /* SEPARATOR  */
  YYSYMBOL_TRANSFORMSEPARATOR = 12,        /* TRANSFORMSEPARATOR  */
  YYSYMBOL_GROUP = 13,                     /* GROUP  */
  YYSYMBOL_SWITCH = 14,                    /* SWITCH  */
  YYSYMBOL_LEVELOFDETAIL = 15,             /* LEVELOFDETAIL  */
  YYSYMBOL_TRANSFORM = 16,                 /* TRANSFORM  */
  YYSYMBOL_ROTATION = 17,                  /* ROTATION  */
  YYSYMBOL_TRANSLATION = 18,               /* TRANSLATION  */
  YYSYMBOL_SCALE = 19,                     /* SCALE  */
  YYSYMBOL_MATRIXTRANSFORM = 20,           /* MATRIXTRANSFORM  */
  YYSYMBOL_MATERIAL = 21,                  /* MATERIAL  */
  YYSYMBOL_MATERIALBINDING = 22,           /* MATERIALBINDING  */
  YYSYMBOL_NORMALBINDING = 23,             /* NORMALBINDING  */
  YYSYMBOL_SHAPEHINTS = 24,                /* SHAPEHINTS  */
  YYSYMBOL_COORDINATE3 = 25,               /* COORDINATE3  */
  YYSYMBOL_NORMAL = 26,                    /* NORMAL  */
  YYSYMBOL_TEXTURE2 = 27,                  /* TEXTURE2  */
  YYSYMBOL_TEXTURE2TRANSFORM = 28,         /* TEXTURE2TRANSFORM  */
  YYSYMBOL_TEXTURECOORDINATE2 = 29,        /* TEXTURECOORDINATE2  */
  YYSYMBOL_SPHERE = 30,                    /* SPHERE  */
  YYSYMBOL_CUBE = 31,                      /* CUBE  */
  YYSYMBOL_CONE = 32,                      /* CONE  */
  YYSYMBOL_CYLINDER = 33,                  /* CYLINDER  */
  YYSYMBOL_INDEXEDFACESET = 34,            /* INDEXEDFACESET  */
  YYSYMBOL_INDEXEDLINESET = 35,            /* INDEXEDLINESET  */
  YYSYMBOL_POINTSET = 36,                  /* POINTSET  */
  YYSYMBOL_DIRECTIONALLIGHT = 37,          /* DIRECTIONALLIGHT  */
  YYSYMBOL_POINTLIGHT = 38,                /* POINTLIGHT  */
  YYSYMBOL_SPOTLIGHT = 39,                 /* SPOTLIGHT  */
  YYSYMBOL_ORTHOGRAPHICCAMERA = 40,        /* ORTHOGRAPHICCAMERA  */
  YYSYMBOL_PERSPECTIVECAMERA = 41,         /* PERSPECTIVECAMERA  */
  YYSYMBOL_WWWANCHOR = 42,                 /* WWWANCHOR  */
  YYSYMBOL_WWWINLINE = 43,                 /* WWWINLINE  */
  YYSYMBOL_INFO = 44,                      /* INFO  */
  YYSYMBOL_IDENTIFIER = 45,                /* IDENTIFIER  */
  YYSYMBOL_STRING = 46,                    /* STRING  */
  YYSYMBOL_INTEGER = 47,                   /* INTEGER  */
  YYSYMBOL_FLOAT = 48,                     /* FLOAT  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_vrmlFile = 50,                  /* vrmlFile  */
  YYSYMBOL_vrmlScene = 51,                 /* vrmlScene  */
  YYSYMBOL_node = 52,                      /* node  */
  YYSYMBOL_definedNode = 53,               /* definedNode  */
  YYSYMBOL_usedNode = 54,                  /* usedNode  */
  YYSYMBOL_nodeGuts = 55,                  /* nodeGuts  */
  YYSYMBOL_nodeName = 56,                  /* nodeName  */
  YYSYMBOL_nodeBody = 57,                  /* nodeBody  */
  YYSYMBOL_fieldDeclaration = 58,          /* fieldDeclaration  */
  YYSYMBOL_fieldValue = 59,                /* fieldValue  */
  YYSYMBOL_vec2f = 60,                     /* vec2f  */
  YYSYMBOL_vec3f = 61,                     /* vec3f  */
  YYSYMBOL_rotation = 62,                  /* rotation  */
  YYSYMBOL_fieldArray = 63,                /* fieldArray  */
  YYSYMBOL_fieldValueList = 64             /* fieldValueList  */
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
typedef yytype_int8 yy_state_t;

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   124

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  74

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


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
      45,    46,    47,    48
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   126,   126,   134,   137,   152,   156,   160,   167,   179,
     196,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   241,   243,   244,   248,
     257,   258,   259,   260,   261,   262,   263,   267,   271,   275,
     279,   282,   284,   285,   286
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
  "\"end of file\"", "error", "\"invalid token\"", "HEADER", "DEF", "USE",
  "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "COMMA", "SEPARATOR",
  "TRANSFORMSEPARATOR", "GROUP", "SWITCH", "LEVELOFDETAIL", "TRANSFORM",
  "ROTATION", "TRANSLATION", "SCALE", "MATRIXTRANSFORM", "MATERIAL",
  "MATERIALBINDING", "NORMALBINDING", "SHAPEHINTS", "COORDINATE3",
  "NORMAL", "TEXTURE2", "TEXTURE2TRANSFORM", "TEXTURECOORDINATE2",
  "SPHERE", "CUBE", "CONE", "CYLINDER", "INDEXEDFACESET", "INDEXEDLINESET",
  "POINTSET", "DIRECTIONALLIGHT", "POINTLIGHT", "SPOTLIGHT",
  "ORTHOGRAPHICCAMERA", "PERSPECTIVECAMERA", "WWWANCHOR", "WWWINLINE",
  "INFO", "IDENTIFIER", "STRING", "INTEGER", "FLOAT", "$accept",
  "vrmlFile", "vrmlScene", "node", "definedNode", "usedNode", "nodeGuts",
  "nodeName", "nodeBody", "fieldDeclaration", "fieldValue", "vec2f",
  "vec3f", "rotation", "fieldArray", "fieldValueList", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-41)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -1,   -41,     4,    38,   -41,   -40,     2,   -41,   -41,   -41,
     -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,
     -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,
     -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,
     -41,   -41,   -41,   -41,   -41,   -41,    42,    38,   -41,   -41,
     -41,    -4,   -41,    -2,   -41,   -41,    -2,   -41,   -41,    43,
     -41,   -41,   -41,   -41,   -41,   -41,    76,    45,   -41,    -2,
     -41,    46,   -41,   -41
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     3,     0,     2,     1,     0,     0,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     4,     6,     7,     5,     0,     0,     9,    46,
       8,     0,    10,    45,    48,    47,    61,    52,    50,    51,
      49,    53,    54,    55,    56,    62,     0,    57,    60,     0,
      64,    58,    63,    59
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -41,   -41,   -41,    41,   -41,   -41,   -41,   -41,   -41,   -41,
      31,   -41,   -41,   -41,   -41,   -41
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,    42,    43,    44,    45,    46,    51,    55,
      60,    61,    62,    63,    64,    66
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       5,     6,     1,    52,     4,    47,    56,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    53,     5,     6,    57,    58,    59,    48,    49,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    56,    68,    69,    65,    50,     0,
       0,    67,    54,    71,    73,     0,     0,    70,     0,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59
};

static const yytype_int8 yycheck[] =
{
       4,     5,     3,     7,     0,    45,     8,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     4,     5,    46,    47,    48,    45,     6,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,     8,     9,    10,    56,    47,    -1,
      -1,    48,    51,    48,    48,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,    48
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    50,    51,     0,     4,     5,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    52,    53,    54,    55,    56,    45,    45,     6,
      52,    57,     7,    45,    52,    58,     8,    46,    47,    48,
      59,    60,    61,    62,    63,    59,    64,    48,     9,    10,
      59,    48,    59,    48
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    51,    51,    52,    52,    52,    53,    54,
      55,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    57,    57,    57,    58,
      59,    59,    59,    59,    59,    59,    59,    60,    61,    62,
      63,    64,    64,    64,    64
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     2,     1,     1,     1,     3,     2,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     4,
       3,     0,     1,     3,     2
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
  case 2: /* vrmlFile: HEADER vrmlScene  */
#line 127 "vrml.y"
    {
        vrml_root = (yyvsp[0].node);
    }
#line 1405 "y.tab.c"
    break;

  case 3: /* vrmlScene: %empty  */
#line 134 "vrml.y"
    {
        (yyval.node) = NULL;
    }
#line 1413 "y.tab.c"
    break;

  case 4: /* vrmlScene: vrmlScene node  */
#line 138 "vrml.y"
    {
        if ((yyvsp[-1].node) == NULL) {
            /* Create implicit root Separator */
            (yyval.node) = new QvSeparator;
        } else {
            (yyval.node) = (yyvsp[-1].node);
        }
        if ((yyvsp[0].node) != NULL) {
            add_child_to_group((yyval.node), (yyvsp[0].node));
        }
    }
#line 1429 "y.tab.c"
    break;

  case 5: /* node: nodeGuts  */
#line 153 "vrml.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1437 "y.tab.c"
    break;

  case 6: /* node: definedNode  */
#line 157 "vrml.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1445 "y.tab.c"
    break;

  case 7: /* node: usedNode  */
#line 161 "vrml.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1453 "y.tab.c"
    break;

  case 8: /* definedNode: DEF IDENTIFIER node  */
#line 168 "vrml.y"
    {
        /* DEF name node - assign name and add to dictionary */
        (yyval.node) = (yyvsp[0].node);
        if ((yyval.node) != NULL) {
            (yyval.node)->setName((yyvsp[-1].stringval));
        }
        free((yyvsp[-1].stringval));
    }
#line 1466 "y.tab.c"
    break;

  case 9: /* usedNode: USE IDENTIFIER  */
#line 180 "vrml.y"
    {
        /* USE name - look up node in dictionary */
        /* Error: "Premature end of file after USE" */
        QvNode* existing = QvNode::findByName((yyvsp[0].stringval));
        if (existing != NULL) {
            existing->ref();
            (yyval.node) = existing;
        } else {
            fprintf(stderr, "Unknown USE name: %s\n", (yyvsp[0].stringval));
            (yyval.node) = NULL;
        }
        free((yyvsp[0].stringval));
    }
#line 1484 "y.tab.c"
    break;

  case 10: /* nodeGuts: nodeName LBRACE nodeBody RBRACE  */
#line 197 "vrml.y"
    {
        (yyval.node) = create_node((yyvsp[-3].stringval));
        free((yyvsp[-3].stringval));
    }
#line 1493 "y.tab.c"
    break;

  case 11: /* nodeName: SEPARATOR  */
#line 204 "vrml.y"
                        { (yyval.stringval) = strdup("Separator"); }
#line 1499 "y.tab.c"
    break;

  case 12: /* nodeName: TRANSFORMSEPARATOR  */
#line 205 "vrml.y"
                         { (yyval.stringval) = strdup("TransformSeparator"); }
#line 1505 "y.tab.c"
    break;

  case 13: /* nodeName: GROUP  */
#line 206 "vrml.y"
                        { (yyval.stringval) = strdup("Group"); }
#line 1511 "y.tab.c"
    break;

  case 14: /* nodeName: SWITCH  */
#line 207 "vrml.y"
                        { (yyval.stringval) = strdup("Switch"); }
#line 1517 "y.tab.c"
    break;

  case 15: /* nodeName: LEVELOFDETAIL  */
#line 208 "vrml.y"
                        { (yyval.stringval) = strdup("LevelOfDetail"); }
#line 1523 "y.tab.c"
    break;

  case 16: /* nodeName: TRANSFORM  */
#line 209 "vrml.y"
                        { (yyval.stringval) = strdup("Transform"); }
#line 1529 "y.tab.c"
    break;

  case 17: /* nodeName: ROTATION  */
#line 210 "vrml.y"
                        { (yyval.stringval) = strdup("Rotation"); }
#line 1535 "y.tab.c"
    break;

  case 18: /* nodeName: TRANSLATION  */
#line 211 "vrml.y"
                        { (yyval.stringval) = strdup("Translation"); }
#line 1541 "y.tab.c"
    break;

  case 19: /* nodeName: SCALE  */
#line 212 "vrml.y"
                        { (yyval.stringval) = strdup("Scale"); }
#line 1547 "y.tab.c"
    break;

  case 20: /* nodeName: MATRIXTRANSFORM  */
#line 213 "vrml.y"
                        { (yyval.stringval) = strdup("MatrixTransform"); }
#line 1553 "y.tab.c"
    break;

  case 21: /* nodeName: MATERIAL  */
#line 214 "vrml.y"
                        { (yyval.stringval) = strdup("Material"); }
#line 1559 "y.tab.c"
    break;

  case 22: /* nodeName: MATERIALBINDING  */
#line 215 "vrml.y"
                        { (yyval.stringval) = strdup("MaterialBinding"); }
#line 1565 "y.tab.c"
    break;

  case 23: /* nodeName: NORMALBINDING  */
#line 216 "vrml.y"
                        { (yyval.stringval) = strdup("NormalBinding"); }
#line 1571 "y.tab.c"
    break;

  case 24: /* nodeName: SHAPEHINTS  */
#line 217 "vrml.y"
                        { (yyval.stringval) = strdup("ShapeHints"); }
#line 1577 "y.tab.c"
    break;

  case 25: /* nodeName: COORDINATE3  */
#line 218 "vrml.y"
                        { (yyval.stringval) = strdup("Coordinate3"); }
#line 1583 "y.tab.c"
    break;

  case 26: /* nodeName: NORMAL  */
#line 219 "vrml.y"
                        { (yyval.stringval) = strdup("Normal"); }
#line 1589 "y.tab.c"
    break;

  case 27: /* nodeName: TEXTURE2  */
#line 220 "vrml.y"
                        { (yyval.stringval) = strdup("Texture2"); }
#line 1595 "y.tab.c"
    break;

  case 28: /* nodeName: TEXTURE2TRANSFORM  */
#line 221 "vrml.y"
                        { (yyval.stringval) = strdup("Texture2Transform"); }
#line 1601 "y.tab.c"
    break;

  case 29: /* nodeName: TEXTURECOORDINATE2  */
#line 222 "vrml.y"
                         { (yyval.stringval) = strdup("TextureCoordinate2"); }
#line 1607 "y.tab.c"
    break;

  case 30: /* nodeName: SPHERE  */
#line 223 "vrml.y"
                        { (yyval.stringval) = strdup("Sphere"); }
#line 1613 "y.tab.c"
    break;

  case 31: /* nodeName: CUBE  */
#line 224 "vrml.y"
                        { (yyval.stringval) = strdup("Cube"); }
#line 1619 "y.tab.c"
    break;

  case 32: /* nodeName: CONE  */
#line 225 "vrml.y"
                        { (yyval.stringval) = strdup("Cone"); }
#line 1625 "y.tab.c"
    break;

  case 33: /* nodeName: CYLINDER  */
#line 226 "vrml.y"
                        { (yyval.stringval) = strdup("Cylinder"); }
#line 1631 "y.tab.c"
    break;

  case 34: /* nodeName: INDEXEDFACESET  */
#line 227 "vrml.y"
                        { (yyval.stringval) = strdup("IndexedFaceSet"); }
#line 1637 "y.tab.c"
    break;

  case 35: /* nodeName: INDEXEDLINESET  */
#line 228 "vrml.y"
                        { (yyval.stringval) = strdup("IndexedLineSet"); }
#line 1643 "y.tab.c"
    break;

  case 36: /* nodeName: POINTSET  */
#line 229 "vrml.y"
                        { (yyval.stringval) = strdup("PointSet"); }
#line 1649 "y.tab.c"
    break;

  case 37: /* nodeName: DIRECTIONALLIGHT  */
#line 230 "vrml.y"
                        { (yyval.stringval) = strdup("DirectionalLight"); }
#line 1655 "y.tab.c"
    break;

  case 38: /* nodeName: POINTLIGHT  */
#line 231 "vrml.y"
                        { (yyval.stringval) = strdup("PointLight"); }
#line 1661 "y.tab.c"
    break;

  case 39: /* nodeName: SPOTLIGHT  */
#line 232 "vrml.y"
                        { (yyval.stringval) = strdup("SpotLight"); }
#line 1667 "y.tab.c"
    break;

  case 40: /* nodeName: ORTHOGRAPHICCAMERA  */
#line 233 "vrml.y"
                         { (yyval.stringval) = strdup("OrthographicCamera"); }
#line 1673 "y.tab.c"
    break;

  case 41: /* nodeName: PERSPECTIVECAMERA  */
#line 234 "vrml.y"
                        { (yyval.stringval) = strdup("PerspectiveCamera"); }
#line 1679 "y.tab.c"
    break;

  case 42: /* nodeName: WWWANCHOR  */
#line 235 "vrml.y"
                        { (yyval.stringval) = strdup("WWWAnchor"); }
#line 1685 "y.tab.c"
    break;

  case 43: /* nodeName: WWWINLINE  */
#line 236 "vrml.y"
                        { (yyval.stringval) = strdup("WWWInline"); }
#line 1691 "y.tab.c"
    break;

  case 44: /* nodeName: INFO  */
#line 237 "vrml.y"
                        { (yyval.stringval) = strdup("Info"); }
#line 1697 "y.tab.c"
    break;

  case 45: /* nodeName: IDENTIFIER  */
#line 238 "vrml.y"
                        { (yyval.stringval) = (yyvsp[0].stringval); /* Unknown node type */ }
#line 1703 "y.tab.c"
    break;

  case 49: /* fieldDeclaration: IDENTIFIER fieldValue  */
#line 249 "vrml.y"
    {
        /* Field assignments handled in QvNode::readInstance() */
        /* Error strings: "Unknown field", "Couldn't read value for field" */
        free((yyvsp[-1].stringval));
    }
#line 1713 "y.tab.c"
    break;


#line 1717 "y.tab.c"

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

#line 289 "vrml.y"


/*
 * Error handler
 * Errors from strings analysis:
 * - "VRML error in %s(): %s"
 * - "VRML read error: %s"
 */
void yyerror(const char* s)
{
    fprintf(stderr, "VRML error: %s\n", s);
}

/*
 * Create node by type name
 * Unknown nodes become QvUnknownNode
 */
QvNode* create_node(const char* type)
{
    if (strcmp(type, "Separator") == 0) return new QvSeparator;
    if (strcmp(type, "TransformSeparator") == 0) return new QvTransformSeparator;
    if (strcmp(type, "Group") == 0) return new QvGroup;
    if (strcmp(type, "Switch") == 0) return new QvSwitch;
    if (strcmp(type, "LevelOfDetail") == 0) return new QvLevelOfDetail;
    if (strcmp(type, "Transform") == 0) return new QvTransform;
    if (strcmp(type, "Rotation") == 0) return new QvRotation;
    if (strcmp(type, "Translation") == 0) return new QvTranslation;
    if (strcmp(type, "Scale") == 0) return new QvScale;
    if (strcmp(type, "MatrixTransform") == 0) return new QvMatrixTransform;
    if (strcmp(type, "Material") == 0) return new QvMaterial;
    if (strcmp(type, "MaterialBinding") == 0) return new QvMaterialBinding;
    if (strcmp(type, "NormalBinding") == 0) return new QvNormalBinding;
    if (strcmp(type, "ShapeHints") == 0) return new QvShapeHints;
    if (strcmp(type, "Coordinate3") == 0) return new QvCoordinate3;
    if (strcmp(type, "Normal") == 0) return new QvNormal;
    if (strcmp(type, "Texture2") == 0) return new QvTexture2;
    if (strcmp(type, "Texture2Transform") == 0) return new QvTexture2Transform;
    if (strcmp(type, "TextureCoordinate2") == 0) return new QvTextureCoordinate2;
    if (strcmp(type, "Sphere") == 0) return new QvSphere;
    if (strcmp(type, "Cube") == 0) return new QvCube;
    if (strcmp(type, "Cone") == 0) return new QvCone;
    if (strcmp(type, "Cylinder") == 0) return new QvCylinder;
    if (strcmp(type, "IndexedFaceSet") == 0) return new QvIndexedFaceSet;
    if (strcmp(type, "IndexedLineSet") == 0) return new QvIndexedLineSet;
    if (strcmp(type, "PointSet") == 0) return new QvPointSet;
    if (strcmp(type, "DirectionalLight") == 0) return new QvDirectionalLight;
    if (strcmp(type, "PointLight") == 0) return new QvPointLight;
    if (strcmp(type, "SpotLight") == 0) return new QvSpotLight;
    if (strcmp(type, "OrthographicCamera") == 0) return new QvOrthographicCamera;
    if (strcmp(type, "PerspectiveCamera") == 0) return new QvPerspectiveCamera;
    if (strcmp(type, "WWWAnchor") == 0) return new QvWWWAnchor;
    if (strcmp(type, "WWWInline") == 0) return new QvWWWInline;
    if (strcmp(type, "Info") == 0) return new QvInfo;

    /* Unknown node */
    fprintf(stderr, "Unknown node type: %s\n", type);
    return new QvUnknownNode;
}

/*
 * Add child to group node
 */
void add_child_to_group(QvNode* parent, QvNode* child)
{
    if (parent == NULL || child == NULL) {
        return;
    }

    /* Use dynamic_cast or check node type */
    QvGroup* group = (QvGroup*)parent;
    if (group != NULL) {
        group->addChild(child);
    }
}

/*
 * Get parsed scene root
 */
QvNode* vrml_get_root(void)
{
    return vrml_root;
}
