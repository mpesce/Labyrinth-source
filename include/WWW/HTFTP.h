/*
 * HTFTP.h - FTP protocol client
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Source: E:\labsrc\WWW\HTFTP.C
 */

#ifndef HTFTP_H
#define HTFTP_H

#include "HTUtils.h"

/* FTP response codes */
#define FTP_READY           220
#define FTP_TRANSFER_COMPLETE 226
#define FTP_PASSIVE_MODE    227
#define FTP_LOGIN_OK        230
#define FTP_FILE_ACTION_OK  250
#define FTP_PATHNAME        257
#define FTP_NEED_PASSWORD   331
#define FTP_FILE_UNAVAILABLE 550

/* FTP connection structure */
typedef struct {
    HTSocket control;       /* Control connection socket */
    HTSocket data;          /* Data connection socket */
    char* host;            /* Server hostname */
    int port;              /* Server port (default 21) */
    char* user;            /* Username */
    char* password;        /* Password */
    BOOL passive;          /* Use passive mode */
} FTPConnection;

/*
 * Load a file via FTP
 * Returns HT_LOADED on success, HT_ERROR on failure
 */
int FTP_Get(const char* url, char** data, int* length);

/*
 * Connect to FTP server
 * Returns connection structure, NULL on failure
 */
FTPConnection* FTP_Connect(const char* host, int port);

/*
 * Login to FTP server
 * Returns TRUE on success, FALSE on failure
 */
BOOL FTP_Login(FTPConnection* conn, const char* user, const char* password);

/*
 * Change directory on FTP server
 * Returns TRUE on success, FALSE on failure
 */
BOOL FTP_ChangeDir(FTPConnection* conn, const char* path);

/*
 * Set transfer type (A for ASCII, I for binary)
 * Returns TRUE on success, FALSE on failure
 */
BOOL FTP_SetType(FTPConnection* conn, char type);

/*
 * Enter passive mode
 * Returns data socket on success, INVALID_SOCKET on failure
 */
HTSocket FTP_Passive(FTPConnection* conn);

/*
 * Retrieve file from FTP server
 * Returns HT_LOADED on success, HT_ERROR on failure
 */
int FTP_Retrieve(FTPConnection* conn, const char* filename,
                 char** data, int* length);

/*
 * Send FTP command and read response
 * Returns response code, -1 on error
 */
int FTP_Command(FTPConnection* conn, const char* command, char** response);

/*
 * Close FTP connection
 */
void FTP_Close(FTPConnection* conn);

#endif /* HTFTP_H */
