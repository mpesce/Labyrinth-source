/*
 * HTUtils.h - Common types and utilities for WWW library
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Based on CERN libwww
 */

#ifndef HTUTILS_H
#define HTUTILS_H

#include <windows.h>
#include <winsock.h>

/* Boolean type */
#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/* Success/failure codes */
#define HT_LOADED       1
#define HT_ERROR        -1
#define HT_NO_DATA      -2
#define HT_RETRY        -3
#define HT_WOULD_BLOCK  -4

/* Line terminator for network protocols */
#define CR   '\r'
#define LF   '\n'
#define CRLF "\r\n"

/* Buffer sizes */
#define LINE_LENGTH     256
#define BUFFER_SIZE     4096

/* String allocation macros */
#define StrAllocCopy(dest, src) \
    { if (dest) free(dest); \
      dest = (char*)malloc(strlen(src) + 1); \
      if (dest) strcpy(dest, src); }

#define StrAllocCat(dest, src) \
    { if (src && *src) { \
        if (dest) { \
            char* tmp = (char*)malloc(strlen(dest) + strlen(src) + 1); \
            if (tmp) { strcpy(tmp, dest); strcat(tmp, src); \
                      free(dest); dest = tmp; } \
        } else { \
            dest = (char*)malloc(strlen(src) + 1); \
            if (dest) strcpy(dest, src); \
        } \
      } }

/* Network socket type */
typedef SOCKET HTSocket;
#define INVALID_SOCKET_VALUE INVALID_SOCKET

#endif /* HTUTILS_H */
