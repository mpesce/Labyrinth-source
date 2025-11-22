/*
 * WWWLib.h - Main header for WWW library
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Include this file to use the WWW library
 */

#ifndef WWWLIB_H
#define WWWLIB_H

/* Core utilities */
#include "HTUtils.h"

/* URL parsing */
#include "HTParse.h"

/* Protocol implementations */
#include "HTTP.h"
#include "HTFTP.h"

/* TCP/IP layer */
#include "HTTCP.h"

/* Version information */
#define WWWLIB_VERSION "2.17"
#define WWWLIB_NAME "libwww"

#endif /* WWWLIB_H */
