/*
 * vrml.l - Lexical analyzer for VRML 1.0
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Source: E:\labsrc\parser\VRML.L
 * Lexer: LEX/FLEX compatible
 *
 * VRML 1.0 Tokenization
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/QvNode.h"
#include "y.tab.h"
%}

%option noyywrap
%option case-insensitive
%option yylineno

DIGIT       [0-9]
HEXDIGIT    [0-9a-fA-F]
INTEGER     [-+]?{DIGIT}+
EXPONENT    [eE][-+]?{DIGIT}+
FLOAT       [-+]?({DIGIT}+\.{DIGIT}*|{DIGIT}*\.{DIGIT}+)({EXPONENT})?
IDENTIFIER  [a-zA-Z_][a-zA-Z0-9_]*
WHITESPACE  [ \t\r]+

%%

    /* VRML Header - must be first line */
^"#VRML V1.0 ascii".*   { return HEADER; }

    /* Comments */
#.*                     { /* Skip comments */ }

    /* Keywords - DEF/USE */
"DEF"                   { return DEF; }
"USE"                   { return USE; }

    /* Grouping Nodes */
"Separator"             { return SEPARATOR; }
"TransformSeparator"    { return TRANSFORMSEPARATOR; }
"Group"                 { return GROUP; }
"Switch"                { return SWITCH; }
"LevelOfDetail"         { return LEVELOFDETAIL; }

    /* Transform Nodes */
"Transform"             { return TRANSFORM; }
"Rotation"              { return ROTATION; }
"Translation"           { return TRANSLATION; }
"Scale"                 { return SCALE; }
"MatrixTransform"       { return MATRIXTRANSFORM; }

    /* Property Nodes */
"Material"              { return MATERIAL; }
"MaterialBinding"       { return MATERIALBINDING; }
"NormalBinding"         { return NORMALBINDING; }
"ShapeHints"            { return SHAPEHINTS; }
"Coordinate3"           { return COORDINATE3; }
"Normal"                { return NORMAL; }

    /* Texture Nodes */
"Texture2"              { return TEXTURE2; }
"Texture2Transform"     { return TEXTURE2TRANSFORM; }
"TextureCoordinate2"    { return TEXTURECOORDINATE2; }

    /* Geometry Nodes */
"Sphere"                { return SPHERE; }
"Cube"                  { return CUBE; }
"Cone"                  { return CONE; }
"Cylinder"              { return CYLINDER; }
"IndexedFaceSet"        { return INDEXEDFACESET; }
"IndexedLineSet"        { return INDEXEDLINESET; }
"PointSet"              { return POINTSET; }

    /* Light Nodes */
"DirectionalLight"      { return DIRECTIONALLIGHT; }
"PointLight"            { return POINTLIGHT; }
"SpotLight"             { return SPOTLIGHT; }

    /* Camera Nodes */
"OrthographicCamera"    { return ORTHOGRAPHICCAMERA; }
"PerspectiveCamera"     { return PERSPECTIVECAMERA; }

    /* WWW Nodes */
"WWWAnchor"             { return WWWANCHOR; }
"WWWInline"             { return WWWINLINE; }

    /* Info Node */
"Info"                  { return INFO; }

    /* Literals */
{FLOAT}                 {
                            yylval.floatval = (float)atof(yytext);
                            return FLOAT;
                        }

{INTEGER}               {
                            yylval.intval = atoi(yytext);
                            return INTEGER;
                        }

    /* String literals */
\"([^"\n]|\\.)*\"       {
                            /* Remove quotes */
                            yytext[yyleng-1] = '\0';
                            yylval.stringval = strdup(yytext+1);
                            return STRING;
                        }

    /* Identifiers (field names, node names) */
{IDENTIFIER}            {
                            yylval.stringval = strdup(yytext);
                            return IDENTIFIER;
                        }

    /* Delimiters */
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }
"["                     { return LBRACKET; }
"]"                     { return RBRACKET; }
","                     { return COMMA; }

    /* Whitespace */
{WHITESPACE}            { /* Skip whitespace */ }
\n                      { yylineno++; }

    /* Unknown character */
.                       {
                            fprintf(stderr, "Unexpected character: %c (line %d)\n",
                                    yytext[0], yylineno);
                        }

%%

/*
 * Reset lexer state
 */
void lex_reset(void)
{
    yylineno = 1;
    yyrestart(NULL);
}
