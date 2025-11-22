/*
 * HTTP.h - HTTP/1.0 protocol client
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Source: E:\labsrc\WWW\HTTP.C
 *
 * Strings from analysis:
 * - "HTTP/1.0"
 * - "Accept: %s%c%c"
 * - "User-Agent:  %s/%s  libwww/%s%c%c"
 * - "HTTP connected, socket %d"
 * - "HTTP: Unable to connect to remote host for `%s' (errno = %d)."
 */

#ifndef HTTP_H
#define HTTP_H

#include "HTUtils.h"

/* HTTP status codes */
#define HTTP_OK                 200
#define HTTP_CREATED            201
#define HTTP_ACCEPTED           202
#define HTTP_NO_CONTENT         204
#define HTTP_MOVED_PERMANENTLY  301
#define HTTP_MOVED_TEMPORARILY  302
#define HTTP_NOT_MODIFIED       304
#define HTTP_BAD_REQUEST        400
#define HTTP_UNAUTHORIZED       401
#define HTTP_FORBIDDEN          403
#define HTTP_NOT_FOUND          404
#define HTTP_INTERNAL_ERROR     500
#define HTTP_NOT_IMPLEMENTED    501

/* HTTP request structure */
typedef struct {
    char* url;              /* Full URL */
    char* host;             /* Hostname */
    int port;               /* Port number (default 80) */
    char* path;             /* Path component */
    char* method;           /* GET, POST, etc. */
    char* user_agent;       /* User-Agent header */
    char* accept;           /* Accept header */
} HTTPRequest;

/* HTTP response structure */
typedef struct {
    int status;             /* HTTP status code */
    char* status_text;      /* Status text */
    char* content_type;     /* Content-Type header */
    int content_length;     /* Content-Length header */
    char* location;         /* Location header (for redirects) */
    char* data;             /* Response body */
    int data_length;        /* Length of response body */
} HTTPResponse;

/*
 * Load a document via HTTP
 * Returns HT_LOADED on success, HT_ERROR on failure
 */
int HTTP_Get(const char* url, char** data, int* length);

/*
 * Connect to HTTP server
 * Returns socket on success, INVALID_SOCKET on failure
 */
HTSocket HTTP_Connect(const char* host, int port);

/*
 * Send HTTP request
 * Returns TRUE on success, FALSE on failure
 */
BOOL HTTP_SendRequest(HTSocket sock, HTTPRequest* request);

/*
 * Read HTTP response
 * Returns response structure, NULL on failure
 */
HTTPResponse* HTTP_ReadResponse(HTSocket sock);

/*
 * Free HTTP response structure
 */
void HTTP_FreeResponse(HTTPResponse* response);

/*
 * Parse HTTP status line
 * Returns status code, -1 on error
 */
int HTTP_ParseStatusLine(const char* line, char** status_text);

#endif /* HTTP_H */
