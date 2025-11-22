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
#line 90 "vrml.y"

    int             intval;
    float           floatval;
    char*           stringval;
    QvNode*         node;
    struct {
        float x, y;
    } vec2;
    struct {
        float x, y, z;
    } vec3;
    struct {
        float x, y, z, angle;
    } rot;

#line 179 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
