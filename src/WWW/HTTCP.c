/*
 * HTTCP.c - TCP/IP socket utilities
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Source: E:\labsrc\WWW\HTTCP.C
 */

#include "../../include/WWW/HTTCP.h"
#include <stdio.h>
#include <string.h>

static BOOL winsock_initialized = FALSE;

/*
 * Initialize Winsock
 * Returns TRUE on success, FALSE on failure
 */
BOOL HTTCP_Init(void)
{
    WSADATA wsaData;
    int result;

    if (winsock_initialized) {
        return TRUE;
    }

    result = WSAStartup(MAKEWORD(1, 1), &wsaData);
    if (result != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", result);
        return FALSE;
    }

    winsock_initialized = TRUE;
    return TRUE;
}

/*
 * Cleanup Winsock
 */
void HTTCP_Cleanup(void)
{
    if (winsock_initialized) {
        WSACleanup();
        winsock_initialized = FALSE;
    }
}

/*
 * Create TCP socket
 * Returns socket on success, INVALID_SOCKET on failure
 */
HTSocket HTTCP_Socket(void)
{
    if (!winsock_initialized) {
        HTTCP_Init();
    }

    return socket(AF_INET, SOCK_STREAM, 0);
}

/*
 * Connect to remote host
 * Returns TRUE on success, FALSE on failure
 */
BOOL HTTCP_Connect(HTSocket sock, const char* host, int port)
{
    struct sockaddr_in addr;
    struct hostent* hp;

    if (sock == INVALID_SOCKET || host == NULL) {
        return FALSE;
    }

    /* Resolve hostname */
    hp = gethostbyname(host);
    if (hp == NULL) {
        return FALSE;
    }

    /* Setup address structure */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);

    /* Connect */
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        return FALSE;
    }

    return TRUE;
}

/*
 * Close socket
 */
void HTTCP_Close(HTSocket sock)
{
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }
}

/*
 * Send data on socket
 * Returns number of bytes sent, -1 on error
 */
int HTTCP_Send(HTSocket sock, const char* data, int length)
{
    if (sock == INVALID_SOCKET || data == NULL) {
        return -1;
    }

    return send(sock, data, length, 0);
}

/*
 * Receive data from socket
 * Returns number of bytes received, -1 on error
 */
int HTTCP_Recv(HTSocket sock, char* buffer, int length)
{
    if (sock == INVALID_SOCKET || buffer == NULL) {
        return -1;
    }

    return recv(sock, buffer, length, 0);
}

/*
 * Read a line from socket (up to CRLF)
 * Returns number of bytes read, -1 on error
 */
int HTTCP_ReadLine(HTSocket sock, char* buffer, int size)
{
    int i = 0;
    char c;
    int bytes_read;

    if (sock == INVALID_SOCKET || buffer == NULL) {
        return -1;
    }

    while (i < size - 1) {
        bytes_read = recv(sock, &c, 1, 0);
        if (bytes_read <= 0) {
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
    return i;
}

/*
 * Get host by name
 * Returns IP address in network byte order, 0 on error
 */
unsigned long HTTCP_GetHostByName(const char* host)
{
    struct hostent* hp;

    if (host == NULL) {
        return 0;
    }

    if (!winsock_initialized) {
        HTTCP_Init();
    }

    hp = gethostbyname(host);
    if (hp == NULL) {
        return 0;
    }

    return *(unsigned long*)hp->h_addr;
}

/*
 * Get error code from last socket operation
 */
int HTTCP_GetLastError(void)
{
    return WSAGetLastError();
}
