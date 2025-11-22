/*
 * HTParse.h - URL parsing utilities
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Source: E:\labsrc\WWW\HTPARSE.C
 *
 * URL parsing and manipulation functions for HTTP, FTP, File, etc.
 */

#ifndef HTPARSE_H
#define HTPARSE_H

#include "HTUtils.h"

/* URL components flags */
#define PARSE_ACCESS    0x01    /* Access scheme (http, ftp, file, etc.) */
#define PARSE_HOST      0x02    /* Host name */
#define PARSE_PATH      0x04    /* Path */
#define PARSE_ANCHOR    0x08    /* Fragment/anchor (#...) */
#define PARSE_PUNCTUATION 0x10  /* Include :// and / separators */
#define PARSE_ALL       0xFF    /* Parse everything */

/*
 * Parse URL and extract components
 * Returns allocated string with requested components
 */
char* HTParse(const char* url, const char* base, int wanted);

/*
 * Simplify a URL path (remove ./ and ../)
 * Modifies path in place
 */
void HTSimplify(char* path);

/*
 * Escape special characters in URL
 * Returns allocated string
 */
char* HTEscape(const char* str, unsigned char mask);

/*
 * Unescape URL-encoded string (%XX)
 * Returns allocated string
 */
char* HTUnEscape(const char* str);

/*
 * Extract access scheme from URL (e.g., "http", "ftp")
 * Returns pointer to static buffer
 */
const char* HTParse_Access(const char* url);

/*
 * Extract hostname from URL
 * Returns allocated string
 */
char* HTParse_Host(const char* url);

/*
 * Extract port from URL (returns -1 if not specified)
 */
int HTParse_Port(const char* url);

/*
 * Extract path from URL
 * Returns allocated string
 */
char* HTParse_Path(const char* url);

/*
 * Extract anchor/fragment from URL
 * Returns allocated string
 */
char* HTParse_Anchor(const char* url);

/*
 * Combine relative URL with base URL
 * Returns allocated string
 */
char* HTRelative(const char* relative, const char* base);

#endif /* HTPARSE_H */
