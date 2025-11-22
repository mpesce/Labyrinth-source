/*
 * HTParse.c - URL parsing utilities
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Source: E:\labsrc\WWW\HTPARSE.C
 */

#include "../../include/WWW/HTParse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Hex to decimal conversion */
static int hex_to_int(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

/* Check if character needs escaping */
static BOOL needs_escape(unsigned char c, unsigned char mask)
{
    if (c <= 32 || c >= 127) return TRUE;
    if (strchr(" <>\"#%{}|\\^~[]`", c)) return TRUE;
    return FALSE;
}

/*
 * Parse URL and extract components
 * Returns allocated string with requested components
 */
char* HTParse(const char* url, const char* base, int wanted)
{
    char* result;
    char* access = NULL;
    char* host = NULL;
    char* path = NULL;
    char* anchor = NULL;
    char* p;
    int result_len = 0;

    if (url == NULL) {
        return NULL;
    }

    /* If URL is relative and base is provided, combine them */
    if (base != NULL && strchr(url, ':') == NULL) {
        return HTRelative(url, base);
    }

    /* Parse access (scheme) */
    if (wanted & PARSE_ACCESS) {
        p = (char*)strchr(url, ':');
        if (p != NULL) {
            int len = p - url;
            access = (char*)malloc(len + 1);
            strncpy(access, url, len);
            access[len] = '\0';
            /* Convert to lowercase */
            for (int i = 0; i < len; i++) {
                access[i] = tolower(access[i]);
            }
        }
    }

    /* Parse host and path */
    p = (char*)strchr(url, ':');
    if (p != NULL && p[1] == '/' && p[2] == '/') {
        /* URL has host component */
        p += 3; /* Skip "://" */

        if (wanted & PARSE_HOST) {
            char* end = strchr(p, '/');
            if (end == NULL) end = strchr(p, '#');
            if (end == NULL) end = p + strlen(p);

            int len = end - p;
            host = (char*)malloc(len + 1);
            strncpy(host, p, len);
            host[len] = '\0';
        }

        /* Find start of path */
        p = strchr(p, '/');
        if (p == NULL) {
            p = (char*)url + strlen(url);
        }
    } else if (p != NULL) {
        /* Scheme but no host (e.g., file:) */
        p++; /* Skip ':' */
    } else {
        /* No scheme, relative URL */
        p = (char*)url;
    }

    /* Parse path and anchor */
    if (wanted & (PARSE_PATH | PARSE_ANCHOR)) {
        char* anchor_start = strchr(p, '#');

        if (wanted & PARSE_ANCHOR && anchor_start != NULL) {
            anchor = strdup(anchor_start + 1);
        }

        if (wanted & PARSE_PATH) {
            int len = anchor_start ? (anchor_start - p) : strlen(p);
            path = (char*)malloc(len + 1);
            strncpy(path, p, len);
            path[len] = '\0';
        }
    }

    /* Build result string */
    result_len = 1; /* For null terminator */
    if (access) result_len += strlen(access) + (wanted & PARSE_PUNCTUATION ? 3 : 1);
    if (host) result_len += strlen(host) + (wanted & PARSE_PUNCTUATION ? 2 : 0);
    if (path) result_len += strlen(path);
    if (anchor) result_len += strlen(anchor) + 1;

    result = (char*)malloc(result_len);
    result[0] = '\0';

    if (access) {
        strcat(result, access);
        if (wanted & PARSE_PUNCTUATION) {
            strcat(result, "://");
        } else {
            strcat(result, ":");
        }
    }

    if (host) {
        strcat(result, host);
        if (wanted & PARSE_PUNCTUATION && path && path[0] != '/') {
            strcat(result, "/");
        }
    }

    if (path) {
        strcat(result, path);
    }

    if (anchor) {
        strcat(result, "#");
        strcat(result, anchor);
    }

    /* Free temporary buffers */
    if (access) free(access);
    if (host) free(host);
    if (path) free(path);
    if (anchor) free(anchor);

    return result;
}

/*
 * Simplify a URL path (remove ./ and ../)
 * Modifies path in place
 */
void HTSimplify(char* path)
{
    char* p;
    char* q;

    if (path == NULL) return;

    /* Remove ./ */
    p = path;
    while ((p = strstr(p, "./")) != NULL) {
        if (p == path || p[-1] == '/') {
            strcpy(p, p + 2);
        } else {
            p += 2;
        }
    }

    /* Remove ../ */
    p = path;
    while ((p = strstr(p, "../")) != NULL) {
        if (p > path) {
            /* Find previous slash */
            q = p - 1;
            while (q > path && *q != '/') q--;
            if (*q == '/') {
                strcpy(q + 1, p + 3);
                p = q + 1;
            } else {
                strcpy(q, p + 3);
                p = q;
            }
        } else {
            strcpy(p, p + 3);
        }
    }

    /* Remove trailing /. */
    p = strstr(path, "/.");
    if (p && p[2] == '\0') {
        p[1] = '\0';
    }
}

/*
 * Escape special characters in URL
 * Returns allocated string
 */
char* HTEscape(const char* str, unsigned char mask)
{
    char* result;
    char* p;
    int count = 0;

    if (str == NULL) return NULL;

    /* Count characters that need escaping */
    for (const char* s = str; *s; s++) {
        if (needs_escape(*s, mask)) {
            count += 2; /* %XX takes 3 chars but replaces 1 */
        }
    }

    result = (char*)malloc(strlen(str) + count + 1);
    p = result;

    for (const char* s = str; *s; s++) {
        if (needs_escape(*s, mask)) {
            sprintf(p, "%%%02X", (unsigned char)*s);
            p += 3;
        } else {
            *p++ = *s;
        }
    }

    *p = '\0';
    return result;
}

/*
 * Unescape URL-encoded string (%XX)
 * Returns allocated string
 */
char* HTUnEscape(const char* str)
{
    char* result;
    char* p;

    if (str == NULL) return NULL;

    result = strdup(str);
    p = result;

    while (*str) {
        if (*str == '%' && isxdigit(str[1]) && isxdigit(str[2])) {
            *p++ = (hex_to_int(str[1]) << 4) | hex_to_int(str[2]);
            str += 3;
        } else if (*str == '+') {
            *p++ = ' ';
            str++;
        } else {
            *p++ = *str++;
        }
    }

    *p = '\0';
    return result;
}

/*
 * Extract access scheme from URL (e.g., "http", "ftp")
 * Returns pointer to static buffer
 */
const char* HTParse_Access(const char* url)
{
    static char access[32];
    char* p;

    if (url == NULL) return NULL;

    p = strchr(url, ':');
    if (p == NULL) return NULL;

    int len = p - url;
    if (len >= sizeof(access)) len = sizeof(access) - 1;

    strncpy(access, url, len);
    access[len] = '\0';

    /* Convert to lowercase */
    for (int i = 0; i < len; i++) {
        access[i] = tolower(access[i]);
    }

    return access;
}

/*
 * Extract hostname from URL
 * Returns allocated string
 */
char* HTParse_Host(const char* url)
{
    char* p;
    char* end;
    char* host;
    int len;

    if (url == NULL) return NULL;

    p = strchr(url, ':');
    if (p == NULL || p[1] != '/' || p[2] != '/') {
        return NULL;
    }

    p += 3; /* Skip "://" */

    /* Find end of hostname (before port, path, or anchor) */
    end = p;
    while (*end && *end != ':' && *end != '/' && *end != '#') {
        end++;
    }

    len = end - p;
    host = (char*)malloc(len + 1);
    strncpy(host, p, len);
    host[len] = '\0';

    return host;
}

/*
 * Extract port from URL (returns -1 if not specified)
 */
int HTParse_Port(const char* url)
{
    char* p;
    char* colon;

    if (url == NULL) return -1;

    p = strchr(url, ':');
    if (p == NULL || p[1] != '/' || p[2] != '/') {
        return -1;
    }

    p += 3; /* Skip "://" */

    /* Find port separator */
    colon = strchr(p, ':');
    if (colon == NULL) {
        return -1; /* No port specified */
    }

    /* Check if this is before the path */
    char* slash = strchr(p, '/');
    if (slash != NULL && colon > slash) {
        return -1; /* Colon is in the path, not the port */
    }

    return atoi(colon + 1);
}

/*
 * Extract path from URL
 * Returns allocated string
 */
char* HTParse_Path(const char* url)
{
    char* p;
    char* end;
    char* path;
    int len;

    if (url == NULL) return NULL;

    p = strchr(url, ':');
    if (p != NULL && p[1] == '/' && p[2] == '/') {
        /* Skip host part */
        p += 3;
        p = strchr(p, '/');
        if (p == NULL) {
            return strdup("/");
        }
    } else if (p != NULL) {
        /* Scheme but no host (e.g., file:) */
        p++;
    } else {
        /* No scheme, entire URL is path */
        p = (char*)url;
    }

    /* Find end of path (before anchor) */
    end = strchr(p, '#');
    if (end == NULL) {
        end = p + strlen(p);
    }

    len = end - p;
    path = (char*)malloc(len + 1);
    strncpy(path, p, len);
    path[len] = '\0';

    return path;
}

/*
 * Extract anchor/fragment from URL
 * Returns allocated string
 */
char* HTParse_Anchor(const char* url)
{
    char* p;

    if (url == NULL) return NULL;

    p = strchr(url, '#');
    if (p == NULL) return NULL;

    return strdup(p + 1);
}

/*
 * Combine relative URL with base URL
 * Returns allocated string
 */
char* HTRelative(const char* relative, const char* base)
{
    char* result;
    char* base_path;
    char* p;

    if (relative == NULL) return NULL;
    if (base == NULL) return strdup(relative);

    /* If relative URL has a scheme, it's absolute */
    if (strchr(relative, ':') != NULL) {
        return strdup(relative);
    }

    /* If relative starts with /, it's absolute path */
    if (relative[0] == '/') {
        char* access = (char*)HTParse_Access(base);
        char* host = HTParse_Host(base);
        int port = HTParse_Port(base);

        if (access && host) {
            if (port > 0) {
                result = (char*)malloc(strlen(access) + strlen(host) +
                                      strlen(relative) + 32);
                sprintf(result, "%s://%s:%d%s", access, host, port, relative);
            } else {
                result = (char*)malloc(strlen(access) + strlen(host) +
                                      strlen(relative) + 16);
                sprintf(result, "%s://%s%s", access, host, relative);
            }
            free(host);
            return result;
        }
    }

    /* Relative to base path */
    base_path = HTParse_Path(base);
    if (base_path == NULL) {
        return strdup(relative);
    }

    /* Find last slash in base path */
    p = strrchr(base_path, '/');
    if (p != NULL) {
        p[1] = '\0'; /* Keep trailing slash */
    }

    /* Combine base and relative */
    result = (char*)malloc(strlen(base_path) + strlen(relative) + 1);
    sprintf(result, "%s%s", base_path, relative);

    /* Simplify path */
    HTSimplify(result);

    /* Add access and host back */
    {
        char* access = (char*)HTParse_Access(base);
        char* host = HTParse_Host(base);
        int port = HTParse_Port(base);
        char* full_url;

        if (access && host) {
            if (port > 0) {
                full_url = (char*)malloc(strlen(access) + strlen(host) +
                                        strlen(result) + 32);
                sprintf(full_url, "%s://%s:%d%s", access, host, port, result);
            } else {
                full_url = (char*)malloc(strlen(access) + strlen(host) +
                                        strlen(result) + 16);
                sprintf(full_url, "%s://%s%s", access, host, result);
            }
            free(host);
            free(result);
            result = full_url;
        }
    }

    free(base_path);
    return result;
}
