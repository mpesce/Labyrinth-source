/*
 * QvParser.h - VRML 1.0 parser interface
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Parser interface for reading VRML files
 * Uses YACC-generated parser (vrml.y) and LEX lexer (vrml.l)
 */

#ifndef QV_PARSER_H
#define QV_PARSER_H

#include "QvNode.h"
#include <stdio.h>

/*
 * Parse VRML from file
 * Returns root node on success, NULL on failure
 */
QvNode* QvParser_ReadFile(const char* filename);

/*
 * Parse VRML from file pointer
 * Returns root node on success, NULL on failure
 */
QvNode* QvParser_ReadFILE(FILE* fp);

/*
 * Parse VRML from string
 * Returns root node on success, NULL on failure
 */
QvNode* QvParser_ReadString(const char* vrml_string);

/*
 * Get last parse error message
 * Returns error string or NULL if no error
 */
const char* QvParser_GetError(void);

/*
 * Reset parser state
 * Call before parsing a new file
 */
void QvParser_Reset(void);

/* YACC-generated functions */
#ifdef __cplusplus
extern "C" {
#endif

int yyparse(void);
int yylex(void);
void yyerror(const char* s);
void yyrestart(FILE* fp);
QvNode* vrml_get_root(void);

/* Lexer input */
extern FILE* yyin;
extern int yylineno;

#ifdef __cplusplus
}
#endif

#endif /* QV_PARSER_H */
