/*
 * QvParser.cpp - VRML 1.0 parser implementation
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * High-level parser interface
 */

#include "../../include/QvParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Last error message */
static char error_buffer[512] = {0};

/* YACC external variables and functions */
extern "C" {
    extern FILE* yyin;
    extern int yylineno;
    int yyparse(void);
    QvNode* vrml_get_root(void);
}

/*
 * Parse VRML from file
 * Returns root node on success, NULL on failure
 */
QvNode* QvParser_ReadFile(const char* filename)
{
    FILE* fp;
    QvNode* root;

    if (filename == NULL) {
        snprintf(error_buffer, sizeof(error_buffer),
                 "QvParser_ReadFile: NULL filename");
        return NULL;
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        snprintf(error_buffer, sizeof(error_buffer),
                 "QvParser_ReadFile: Cannot open file '%s'", filename);
        return NULL;
    }

    root = QvParser_ReadFILE(fp);
    fclose(fp);

    return root;
}

/*
 * Parse VRML from file pointer
 * Returns root node on success, NULL on failure
 */
QvNode* QvParser_ReadFILE(FILE* fp)
{
    QvNode* root;

    if (fp == NULL) {
        snprintf(error_buffer, sizeof(error_buffer),
                 "QvParser_ReadFILE: NULL file pointer");
        return NULL;
    }

    /* Reset parser state */
    QvParser_Reset();

    /* Set YACC input */
    yyin = fp;
    yylineno = 1;

    /* Parse */
    error_buffer[0] = '\0';
    if (yyparse() != 0) {
        /* Parse error */
        if (error_buffer[0] == '\0') {
            snprintf(error_buffer, sizeof(error_buffer),
                     "VRML parse error at line %d", yylineno);
        }
        return NULL;
    }

    /* Get parsed scene */
    root = vrml_get_root();
    if (root == NULL) {
        snprintf(error_buffer, sizeof(error_buffer),
                 "VRML parse returned empty scene");
        return NULL;
    }

    return root;
}

/*
 * Parse VRML from string
 * Returns root node on success, NULL on failure
 */
QvNode* QvParser_ReadString(const char* vrml_string)
{
    FILE* fp;
    QvNode* root;

    if (vrml_string == NULL) {
        snprintf(error_buffer, sizeof(error_buffer),
                 "QvParser_ReadString: NULL string");
        return NULL;
    }

    /* Create temporary file from string */
#ifdef _WIN32
    fp = tmpfile();
#else
    fp = tmpfile();
#endif

    if (fp == NULL) {
        snprintf(error_buffer, sizeof(error_buffer),
                 "QvParser_ReadString: Cannot create temporary file");
        return NULL;
    }

    /* Write string to temp file */
    fwrite(vrml_string, 1, strlen(vrml_string), fp);
    rewind(fp);

    /* Parse */
    root = QvParser_ReadFILE(fp);
    fclose(fp);

    return root;
}

/*
 * Get last parse error message
 * Returns error string or NULL if no error
 */
const char* QvParser_GetError(void)
{
    return error_buffer[0] != '\0' ? error_buffer : NULL;
}

/*
 * Reset parser state
 * Call before parsing a new file
 */
void QvParser_Reset(void)
{
    error_buffer[0] = '\0';
    yylineno = 1;
    /* Note: yyrestart() will be called when yyin is set */
}

/*
 * Error handler called by YACC
 */
extern "C" void yyerror(const char* s)
{
    snprintf(error_buffer, sizeof(error_buffer),
             "VRML error at line %d: %s", yylineno, s);
    fprintf(stderr, "%s\n", error_buffer);
}
