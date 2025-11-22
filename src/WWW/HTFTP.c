/*
 * HTFTP.c - FTP protocol client
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Source: E:\labsrc\WWW\HTFTP.C
 */

#include "../../include/WWW/HTFTP.h"
#include "../../include/WWW/HTUtils.h"
#include "../../include/WWW/HTParse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Local functions */
static char* read_response(HTSocket sock, int* code);
static HTSocket create_data_connection(FTPConnection* conn);
static int read_data(HTSocket sock, char** data);

/*
 * Load a file via FTP
 * Returns HT_LOADED on success, HT_ERROR on failure
 */
int FTP_Get(const char* url, char** data, int* length)
{
    FTPConnection* conn;
    char* host;
    char* path;
    char* filename;
    char* p;
    int port;
    int result = HT_ERROR;

    if (url == NULL || data == NULL || length == NULL) {
        return HT_ERROR;
    }

    /* Parse URL */
    host = HTParse_Host(url);
    if (host == NULL) {
        return HT_ERROR;
    }

    port = HTParse_Port(url);
    if (port == -1) {
        port = 21; /* Default FTP port */
    }

    path = HTParse_Path(url);

    /* Connect to server */
    conn = FTP_Connect(host, port);
    if (conn == NULL) {
        fprintf(stderr, "FTP: Unable to connect to %s\n", host);
        free(host);
        if (path) free(path);
        return HT_ERROR;
    }

    /* Login (anonymous) */
    if (!FTP_Login(conn, "anonymous", "labyrinth@")) {
        fprintf(stderr, "FTP: Login failed\n");
        FTP_Close(conn);
        free(host);
        if (path) free(path);
        return HT_ERROR;
    }

    /* Set binary mode */
    FTP_SetType(conn, 'I');

    /* Change to directory if path contains directories */
    if (path != NULL && strlen(path) > 0) {
        p = strrchr(path, '/');
        if (p != NULL && p != path) {
            *p = '\0';
            filename = p + 1;
            FTP_ChangeDir(conn, path);
            *p = '/';
        } else {
            filename = path + 1;
        }

        /* Retrieve file */
        result = FTP_Retrieve(conn, filename, data, length);
    }

    FTP_Close(conn);
    free(host);
    if (path) free(path);

    return result;
}

/*
 * Connect to FTP server
 * Returns connection structure, NULL on failure
 */
FTPConnection* FTP_Connect(const char* host, int port)
{
    FTPConnection* conn;
    struct sockaddr_in addr;
    struct hostent* hp;
    char* response;
    int code;

    conn = (FTPConnection*)calloc(1, sizeof(FTPConnection));
    if (conn == NULL) {
        return NULL;
    }

    conn->host = strdup(host);
    conn->port = port;
    conn->passive = TRUE; /* Use passive mode by default */

    /* Create control socket */
    conn->control = socket(AF_INET, SOCK_STREAM, 0);
    if (conn->control == INVALID_SOCKET) {
        free(conn->host);
        free(conn);
        return NULL;
    }

    /* Resolve hostname */
    hp = gethostbyname(host);
    if (hp == NULL) {
        CLOSE_SOCKET(conn->control);
        free(conn->host);
        free(conn);
        return NULL;
    }

    /* Setup address structure */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);

    /* Connect */
    if (connect(conn->control, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        CLOSE_SOCKET(conn->control);
        free(conn->host);
        free(conn);
        return NULL;
    }

    /* Read welcome message */
    response = read_response(conn->control, &code);
    if (code != FTP_READY) {
        if (response) free(response);
        CLOSE_SOCKET(conn->control);
        free(conn->host);
        free(conn);
        return NULL;
    }

    if (response) free(response);
    return conn;
}

/*
 * Login to FTP server
 * Returns TRUE on success, FALSE on failure
 */
BOOL FTP_Login(FTPConnection* conn, const char* user, const char* password)
{
    char* response;
    int code;

    if (conn == NULL) {
        return FALSE;
    }

    /* Send USER command */
    code = FTP_Command(conn, user != NULL ? user : "anonymous", &response);
    if (response) free(response);

    if (code == FTP_LOGIN_OK) {
        return TRUE; /* No password needed */
    }

    if (code != FTP_NEED_PASSWORD) {
        return FALSE;
    }

    /* Send PASS command */
    code = FTP_Command(conn, password != NULL ? password : "labyrinth@", &response);
    if (response) free(response);

    return (code == FTP_LOGIN_OK);
}

/*
 * Change directory on FTP server
 * Returns TRUE on success, FALSE on failure
 */
BOOL FTP_ChangeDir(FTPConnection* conn, const char* path)
{
    char command[512];
    char* response;
    int code;

    if (conn == NULL || path == NULL) {
        return FALSE;
    }

    sprintf(command, "CWD %s", path);
    code = FTP_Command(conn, command, &response);
    if (response) free(response);

    return (code == FTP_FILE_ACTION_OK);
}

/*
 * Set transfer type (A for ASCII, I for binary)
 * Returns TRUE on success, FALSE on failure
 */
BOOL FTP_SetType(FTPConnection* conn, char type)
{
    char command[16];
    char* response;
    int code;

    if (conn == NULL) {
        return FALSE;
    }

    sprintf(command, "TYPE %c", type);
    code = FTP_Command(conn, command, &response);
    if (response) free(response);

    return (code == FTP_FILE_ACTION_OK);
}

/*
 * Enter passive mode
 * Returns data socket on success, INVALID_SOCKET on failure
 */
HTSocket FTP_Passive(FTPConnection* conn)
{
    char* response;
    int code;
    int h1, h2, h3, h4, p1, p2;
    char* p;
    struct sockaddr_in addr;
    HTSocket data_sock;

    if (conn == NULL) {
        return INVALID_SOCKET;
    }

    /* Send PASV command */
    code = FTP_Command(conn, "PASV", &response);
    if (code != FTP_PASSIVE_MODE || response == NULL) {
        if (response) free(response);
        return INVALID_SOCKET;
    }

    /* Parse response: 227 Entering Passive Mode (h1,h2,h3,h4,p1,p2) */
    p = strchr(response, '(');
    if (p == NULL) {
        free(response);
        return INVALID_SOCKET;
    }

    if (sscanf(p + 1, "%d,%d,%d,%d,%d,%d",
               &h1, &h2, &h3, &h4, &p1, &p2) != 6) {
        free(response);
        return INVALID_SOCKET;
    }

    free(response);

    /* Create data socket */
    data_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (data_sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }

    /* Setup address */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl((h1 << 24) | (h2 << 16) | (h3 << 8) | h4);
    addr.sin_port = htons((p1 << 8) | p2);

    /* Connect to data port */
    if (connect(data_sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        CLOSE_SOCKET(data_sock);
        return INVALID_SOCKET;
    }

    return data_sock;
}

/*
 * Retrieve file from FTP server
 * Returns HT_LOADED on success, HT_ERROR on failure
 */
int FTP_Retrieve(FTPConnection* conn, const char* filename,
                 char** data, int* length)
{
    char command[512];
    char* response;
    int code;
    HTSocket data_sock;
    int bytes_read;

    if (conn == NULL || filename == NULL || data == NULL || length == NULL) {
        return HT_ERROR;
    }

    /* Enter passive mode */
    data_sock = FTP_Passive(conn);
    if (data_sock == INVALID_SOCKET) {
        return HT_ERROR;
    }

    /* Send RETR command */
    sprintf(command, "RETR %s", filename);
    code = FTP_Command(conn, command, &response);
    if (response) free(response);

    if (code != 150 && code != 125) { /* 150 = About to open, 125 = Already open */
        CLOSE_SOCKET(data_sock);
        return HT_ERROR;
    }

    /* Read data */
    bytes_read = read_data(data_sock, data);
    CLOSE_SOCKET(data_sock);

    if (bytes_read < 0) {
        return HT_ERROR;
    }

    *length = bytes_read;

    /* Read completion response */
    response = read_response(conn->control, &code);
    if (response) free(response);

    return (code == FTP_TRANSFER_COMPLETE) ? HT_LOADED : HT_ERROR;
}

/*
 * Send FTP command and read response
 * Returns response code, -1 on error
 */
int FTP_Command(FTPConnection* conn, const char* command, char** response)
{
    char buffer[1024];
    int len;
    int code;

    if (conn == NULL || command == NULL) {
        return -1;
    }

    /* Format command with CRLF */
    len = sprintf(buffer, "%s\r\n", command);

    /* Send command */
    if (send(conn->control, buffer, len, 0) == SOCKET_ERROR) {
        return -1;
    }

    /* Read response */
    *response = read_response(conn->control, &code);

    return code;
}

/*
 * Close FTP connection
 */
void FTP_Close(FTPConnection* conn)
{
    char* response;

    if (conn == NULL) {
        return;
    }

    /* Send QUIT command */
    FTP_Command(conn, "QUIT", &response);
    if (response) free(response);

    /* Close sockets */
    if (conn->control != INVALID_SOCKET) {
        CLOSE_SOCKET(conn->control);
    }

    /* Free resources */
    if (conn->host) free(conn->host);
    if (conn->user) free(conn->user);
    if (conn->password) free(conn->password);

    free(conn);
}

/*
 * Read FTP response from control connection
 * Returns allocated response string, NULL on error
 * Sets code to response code
 */
static char* read_response(HTSocket sock, int* code)
{
    char buffer[1024];
    char* response = NULL;
    int response_len = 0;
    int i = 0;
    char c;

    *code = -1;

    /* Read response line */
    while (i < sizeof(buffer) - 1) {
        if (recv(sock, &c, 1, 0) <= 0) {
            return NULL;
        }

        if (c == '\n') {
            break;
        }

        if (c != '\r') {
            buffer[i++] = c;
        }
    }

    buffer[i] = '\0';

    /* Parse response code */
    if (i >= 3 && buffer[0] >= '0' && buffer[0] <= '9') {
        *code = atoi(buffer);
    }

    response = strdup(buffer);

    /* Handle multi-line responses */
    if (i >= 4 && buffer[3] == '-') {
        char continuation[16];
        sprintf(continuation, "%d ", *code);

        while (1) {
            i = 0;
            while (i < sizeof(buffer) - 1) {
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

            /* Check if this is the last line */
            if (strncmp(buffer, continuation, strlen(continuation)) == 0) {
                break;
            }
        }
    }

    return response;
}

/*
 * Read data from data connection
 * Returns number of bytes read, -1 on error
 */
static int read_data(HTSocket sock, char** data)
{
    char* buffer;
    int buffer_size = BUFFER_SIZE;
    int total_read = 0;
    int bytes_read;

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

    return total_read;
}
