/*
 * HTTCP.h - TCP/IP socket utilities
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Source: E:\labsrc\WWW\HTTCP.C
 */

#ifndef HTTCP_H
#define HTTCP_H

#include "HTUtils.h"

/*
 * Initialize networking (Winsock on Windows, no-op on Unix)
 * Returns TRUE on success, FALSE on failure
 */
BOOL HTTCP_Init(void);

/*
 * Cleanup networking (Winsock on Windows, no-op on Unix)
 */
void HTTCP_Cleanup(void);

/*
 * Create TCP socket
 * Returns socket on success, INVALID_SOCKET on failure
 */
HTSocket HTTCP_Socket(void);

/*
 * Connect to remote host
 * Returns TRUE on success, FALSE on failure
 */
BOOL HTTCP_Connect(HTSocket sock, const char* host, int port);

/*
 * Close socket
 */
void HTTCP_Close(HTSocket sock);

/*
 * Send data on socket
 * Returns number of bytes sent, -1 on error
 */
int HTTCP_Send(HTSocket sock, const char* data, int length);

/*
 * Receive data from socket
 * Returns number of bytes received, -1 on error
 */
int HTTCP_Recv(HTSocket sock, char* buffer, int length);

/*
 * Read a line from socket (up to CRLF)
 * Returns number of bytes read, -1 on error
 */
int HTTCP_ReadLine(HTSocket sock, char* buffer, int size);

/*
 * Get host by name
 * Returns IP address in network byte order, 0 on error
 */
unsigned long HTTCP_GetHostByName(const char* host);

/*
 * Get error code from last socket operation
 */
int HTTCP_GetLastError(void);

#endif /* HTTCP_H */
