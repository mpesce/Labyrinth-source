/*
 * HTTP.C - HTTP/1.0 protocol client
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

#include "../../include/WWW/HTTP.h"
#include "../../include/WWW/HTUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define LIBWWW_VERSION "2.17"
#define USER_AGENT "Labyrinth"
#define USER_AGENT_VERSION "1.0"

/* Local functions */
static char* read_line(HTSocket sock, char* buffer, int size);
static char* parse_header(const char* line, const char* header_name);
static int read_body(HTSocket sock, char** data, int content_length);

/*
 * Load a document via HTTP
 * Returns HT_LOADED on success, HT_ERROR on failure
 */
int HTTP_Get(const char* url, char** data, int* length)
{
    HTTPRequest request;
    HTTPResponse* response;
    HTSocket sock;
    int result = HT_ERROR;

    if (url == NULL || data == NULL || length == NULL) {
        return HT_ERROR;
    }

    /* Parse URL into request structure */
    memset(&request, 0, sizeof(HTTPRequest));
    request.url = strdup(url);
    request.method = "GET";
    request.user_agent = USER_AGENT;
    request.accept = "text/html, text/plain, */*";

    /* TODO: Parse host, port, path from URL */
    /* For now, assume default values */
    request.host = "localhost";
    request.port = 80;
    request.path = "/";

    /* Connect to server */
    sock = HTTP_Connect(request.host, request.port);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "HTTP: Unable to connect to remote host for `%s' (errno = %d).\n",
                url, WSAGetLastError());
        free(request.url);
        return HT_ERROR;
    }

    fprintf(stderr, "HTTP connected, socket %d\n", sock);

    /* Send request */
    if (!HTTP_SendRequest(sock, &request)) {
        closesocket(sock);
        free(request.url);
        return HT_ERROR;
    }

    /* Read response */
    response = HTTP_ReadResponse(sock);
    closesocket(sock);
    free(request.url);

    if (response == NULL) {
        return HT_ERROR;
    }

    /* Check status code */
    if (response->status == HTTP_OK) {
        *data = response->data;
        *length = response->data_length;
        response->data = NULL; /* Transfer ownership */
        result = HT_LOADED;
    } else if (response->status == HTTP_MOVED_PERMANENTLY ||
               response->status == HTTP_MOVED_TEMPORARILY) {
        /* Handle redirects */
        if (response->location != NULL) {
            result = HTTP_Get(response->location, data, length);
        }
    }

    HTTP_FreeResponse(response);
    return result;
}

/*
 * Connect to HTTP server
 * Returns socket on success, INVALID_SOCKET on failure
 */
HTSocket HTTP_Connect(const char* host, int port)
{
    HTSocket sock;
    struct sockaddr_in addr;
    struct hostent* hp;

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }

    /* Resolve hostname */
    hp = gethostbyname(host);
    if (hp == NULL) {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    /* Setup address structure */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);

    /* Connect */
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

/*
 * Send HTTP request
 * Returns TRUE on success, FALSE on failure
 */
BOOL HTTP_SendRequest(HTSocket sock, HTTPRequest* request)
{
    char buffer[4096];
    int len;

    if (request == NULL) {
        return FALSE;
    }

    /* Build request line */
    len = sprintf(buffer, "%s %s HTTP/1.0\r\n",
                  request->method, request->path);

    /* Add User-Agent header */
    len += sprintf(buffer + len, "User-Agent:  %s/%s  libwww/%s%c%c",
                   request->user_agent, USER_AGENT_VERSION, LIBWWW_VERSION, 13, 10);

    /* Add Accept header */
    if (request->accept != NULL) {
        len += sprintf(buffer + len, "Accept: %s%c%c", request->accept, 13, 10);
    }

    /* Add Host header */
    len += sprintf(buffer + len, "Host: %s\r\n", request->host);

    /* End of headers */
    len += sprintf(buffer + len, "\r\n");

    /* Send request */
    if (send(sock, buffer, len, 0) == SOCKET_ERROR) {
        return FALSE;
    }

    return TRUE;
}

/*
 * Read HTTP response
 * Returns response structure, NULL on failure
 */
HTTPResponse* HTTP_ReadResponse(HTSocket sock)
{
    HTTPResponse* response;
    char buffer[MAX_LINE_LENGTH];
    char* line;
    char* header_value;

    response = (HTTPResponse*)calloc(1, sizeof(HTTPResponse));
    if (response == NULL) {
        return NULL;
    }

    /* Read status line */
    line = read_line(sock, buffer, sizeof(buffer));
    if (line == NULL) {
        free(response);
        return NULL;
    }

    response->status = HTTP_ParseStatusLine(line, &response->status_text);
    if (response->status == -1) {
        HTTP_FreeResponse(response);
        return NULL;
    }

    /* Read headers */
    while (1) {
        line = read_line(sock, buffer, sizeof(buffer));
        if (line == NULL || *line == '\0') {
            break; /* End of headers */
        }

        /* Parse Content-Type */
        header_value = parse_header(line, "Content-Type:");
        if (header_value != NULL) {
            response->content_type = strdup(header_value);
        }

        /* Parse Content-Length */
        header_value = parse_header(line, "Content-Length:");
        if (header_value != NULL) {
            response->content_length = atoi(header_value);
        }

        /* Parse Location */
        header_value = parse_header(line, "Location:");
        if (header_value != NULL) {
            response->location = strdup(header_value);
        }
    }

    /* Read body */
    if (read_body(sock, &response->data, response->content_length) < 0) {
        HTTP_FreeResponse(response);
        return NULL;
    }

    response->data_length = response->content_length;

    return response;
}

/*
 * Free HTTP response structure
 */
void HTTP_FreeResponse(HTTPResponse* response)
{
    if (response == NULL) {
        return;
    }

    if (response->status_text != NULL) {
        free(response->status_text);
    }
    if (response->content_type != NULL) {
        free(response->content_type);
    }
    if (response->location != NULL) {
        free(response->location);
    }
    if (response->data != NULL) {
        free(response->data);
    }

    free(response);
}

/*
 * Parse HTTP status line
 * Returns status code, -1 on error
 */
int HTTP_ParseStatusLine(const char* line, char** status_text)
{
    int status_code;
    char* p;
    char* text_start;

    if (line == NULL) {
        return -1;
    }

    /* Skip "HTTP/1.0 " or "HTTP/1.1 " */
    p = (char*)line;
    while (*p && *p != ' ') p++;
    if (*p == '\0') return -1;
    p++; /* Skip space */

    /* Read status code */
    status_code = atoi(p);

    /* Skip to status text */
    while (*p && *p != ' ') p++;
    if (*p == ' ') {
        p++;
        text_start = p;
        /* Trim trailing whitespace */
        while (*p && *p != '\r' && *p != '\n') p++;
        *status_text = (char*)malloc(p - text_start + 1);
        strncpy(*status_text, text_start, p - text_start);
        (*status_text)[p - text_start] = '\0';
    } else {
        *status_text = strdup("");
    }

    return status_code;
}

/*
 * Read a line from socket
 * Returns pointer to buffer on success, NULL on failure
 */
static char* read_line(HTSocket sock, char* buffer, int size)
{
    int i = 0;
    char c;

    while (i < size - 1) {
        if (recv(sock, &c, 1, 0) <= 0) {
            break;
        }

        if (c == '\n') {
            break;
        }

        if (c != '\r') {
            buffer[i++] = c;
        }
    }

    buffer[i] = '\0';
    return (i > 0 || buffer[0] == '\0') ? buffer : NULL;
}

/*
 * Parse header value
 * Returns pointer to value (trimmed), NULL if not found
 */
static char* parse_header(const char* line, const char* header_name)
{
    int len = strlen(header_name);
    char* value;

    if (strncmp(line, header_name, len) != 0) {
        return NULL;
    }

    value = (char*)line + len;
    /* Skip whitespace */
    while (*value == ' ' || *value == '\t') {
        value++;
    }

    return value;
}

/*
 * Read response body
 * Returns number of bytes read, -1 on error
 */
static int read_body(HTSocket sock, char** data, int content_length)
{
    char* buffer;
    int total_read = 0;
    int bytes_read;

    if (content_length > 0) {
        /* Read exactly content_length bytes */
        buffer = (char*)malloc(content_length + 1);
        if (buffer == NULL) {
            return -1;
        }

        while (total_read < content_length) {
            bytes_read = recv(sock, buffer + total_read,
                            content_length - total_read, 0);
            if (bytes_read <= 0) {
                break;
            }
            total_read += bytes_read;
        }

        buffer[total_read] = '\0';
        *data = buffer;
    } else {
        /* Read until connection closes */
        int buffer_size = BUFFER_SIZE;
        buffer = (char*)malloc(buffer_size);
        if (buffer == NULL) {
            return -1;
        }

        while (1) {
            bytes_read = recv(sock, buffer + total_read,
                            buffer_size - total_read - 1, 0);
            if (bytes_read <= 0) {
                break;
            }

            total_read += bytes_read;

            /* Expand buffer if needed */
            if (total_read >= buffer_size - 1) {
                buffer_size *= 2;
                buffer = (char*)realloc(buffer, buffer_size);
                if (buffer == NULL) {
                    return -1;
                }
            }
        }

        buffer[total_read] = '\0';
        *data = buffer;
    }

    return total_read;
}
