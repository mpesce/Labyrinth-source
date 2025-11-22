# Labyrinth VRML Browser (1995) - Reverse Engineering Analysis Report

**Target:** LABYRNTH.EXE
**Architecture:** x86 32-bit PE executable
**Build Date:** Saturday, March 4, 1995, 04:26:59
**Copyright:** (c) 1995, The Community Company
**Description:** First VRML WWW Browser

---

## EXECUTIVE SUMMARY

This report documents the reverse engineering analysis of Labyrinth, the first VRML browser from 1995. The analysis identifies major functional blocks, key entry points, and architectural components necessary for source code reconstruction.

### Key Findings:
- **Entry Point:** 0x004897d3 (line 162091 in disassembly)
- **Main Application:** 0x00401020 (line 22 in disassembly)
- **Code Size:** ~586KB (0x91e00 bytes)
- **Disassembly Lines:** 175,108 lines
- **Primary Components:** VRML Parser, 3D Rendering Engine, WWW/HTTP Client, GUI Framework

---

## 1. PROGRAM ENTRY POINTS

### 1.1 C Runtime Entry Point (0x004897d3)
**Location:** Line 162091 in labyrnth_disasm.txt
**Function Type:** Standard MSVC runtime initialization

**What it does:**
- Sets up Structured Exception Handling (SEH) using `%fs:0x0`
- Calls GetVersion (via 0x4c6338) to check OS version
- Calls GetCommandLineA (via 0x4c633c) to retrieve command-line arguments
- Calls GetStartupInfoA (via 0x4c6344) to get process startup information
- Parses command-line for .wrl file arguments
- Calls main application function at 0x424590
- Implements exception handling and cleanup

**Key String References:**
- None directly (C runtime code)

**Key API Calls:**
- GetVersion
- GetCommandLineA
- GetStartupInfoA
- ExitProcess

---

### 1.2 WinMain Entry Wrapper (0x00424590)
**Location:** Line 51875 in labyrnth_disasm.txt
**Function Type:** Windows application entry point wrapper

**What it does:**
- Receives hInstance parameter and stores at 0x4b99f0
- Calls main GUI initialization at 0x401020
- Returns exit code

**Key Operations:**
```assembly
424590:  mov    0x4(%esp),%eax
424594:  mov    0xc(%esp),%ecx
424598:  push   %ecx
424599:  mov    %eax,0x4b99f0      ; Store hInstance
42459e:  call   0x401020           ; Call main GUI function
```

---

### 1.3 Main Application Entry (0x00401020)
**Location:** Line 22 in labyrnth_disasm.txt
**Function Type:** Main GUI application initialization and message loop

**What it does:**
- Initializes application window and rendering context
- Creates main window object (allocated at 0x4b3000)
- Initializes 3D rendering system (calls 0x40e120)
- Loads configuration from registry/profile
- Parses command-line arguments for initial .wrl file
- Loads default or specified VRML world
- Enters Windows message loop (calls GetMessage, DispatchMessage)
- Implements main event loop for GUI interaction

**Key String References:**
- 0x4b30f4, 0x4b30f0, 0x4b30e8 - Configuration strings
- 0x4b30e0, 0x4b30dc, 0x4b30cc, 0x4b30c4 - Profile/registry keys
- 0x4b309c - Default home page string

**Key API Calls:**
- Memory allocation (0x487c36)
- Window creation/initialization
- Message loop functions

---

## 2. VRML PARSER FUNCTIONS

### 2.1 YACC/PCYACC Parser (Location: ~0x41xxxx-0x42xxxx range)
**Estimated Location:** Multiple functions in 0x41xxxx-0x42xxxx address range
**Function Type:** YACC-generated parser for VRML 1.0 format

**Evidence:**
- String "pcyacc internal stack overflow" (line 558 in strings)
- String "syntax error" (line 557 in strings)
- String "token: %s" (line 559 in strings)
- String "#VRML V1.0 ascii" (line 668 in strings) - VRML header check
- String "Reference VRML Parser 1.0" (line 679 in strings)

**What it does:**
- Parses VRML 1.0 ASCII format files
- Tokenizes input stream
- Builds scene graph from VRML node definitions
- Handles parser errors and syntax validation
- Processes VRML keywords and field values

**Key String References:**
- "VRML error in %s(): %s" (line 682)
- "VRML read error: %s" (line 742)
- "Compilation error: %s" (line 560)
- "At or near line %d" (line 561)
- "Expected '%c', saw '%c'" (line 721)
- "IDENTIFIER" (line 602)
- "Expected \"%s\", saw \"%s\"" (line 603)

**Parser State Management:**
- Implements state table-driven parsing (typical of YACC)
- Stack overflow protection
- Line number tracking for error reporting

---

### 2.2 VRML Field Readers (Location: ~0x42xxxx-0x43xxxx range)

**What they do:**
- Parse VRML field types: SFString, SFRotation, SFMatrix, SFBitMask
- Handle multi-value fields (MFString, MFFloat, MFInt32, etc.)
- Validate field values and types

**Key String References:**
- "SFString" (line 687)
- "SFRotation" (line 688)
- "SFMatrix" (line 689)
- "SFBitMask" (line 690)
- "Unknown field \"%s\"" (line 693)
- "Couldn't read value for field \"%s\"" (line 686)
- "Expected '%c' or '%c' but got '%c' while reading value %d" (line 691)

---

## 3. QVNODE CLASS METHODS (Scene Graph System)

### 3.1 QvNode Base Class
**Location:** ~0x41xxxx-0x43xxxx range
**Type:** C++ virtual class hierarchy

**Key Classes Identified:**

#### Node Types (from strings):
- **QvSeparator** (line 630) - Grouping node
- **QvSwitch** (line 631) - Switch between children
- **QvLevelOfDetail** (line 629) - LOD management
- **QvTransformSeparator** (line 632) - Transform isolation
- **QvWWWAnchor** (line 657) - Hyperlink node
- **QvWWWInline** (line 658) - External file reference
- **QvUnknownNode** (line 659) - Unknown node handler

#### Property Nodes:
- **QvCoordinate3** (line 633) - 3D coordinates
- **QvNormal** (line 636) - Normal vectors
- **QvMaterial** (line 634) - Material properties
- **QvMaterialBinding** (line 635) - Material binding mode
- **QvNormalBinding** (line 637) - Normal binding mode
- **QvShapeHints** (line 638) - Shape rendering hints
- **QvTexture2** (line 640) - 2D texture
- **QvTextureCoordinate2** (line 639) - Texture coordinates
- **QvTexture2Transform** (line 641) - Texture transformation

#### Light Nodes:
- **QvDirectionalLight** (line 642) - Directional light
- **QvPointLight** (line 643) - Point light source
- **QvSpotLight** (line 644) - Spotlight

#### Camera Nodes:
- **QvOrthographicCamera** (line 645) - Orthographic projection
- **QvPerspectiveCamera** (line 646) - Perspective projection

#### Transform Nodes:
- **QvTransform** (line 647) - General transformation
- **QvRotation** (line 648) - Rotation only
- **QvMatrixTransform** (line 649) - Matrix transformation
- **QvTranslation** (line 650) - Translation only

#### Geometry Nodes:
- **QvCube** (Implied, standard VRML primitive)
- **QvSphere** (line 656) - Sphere geometry
- **QvCylinder** (line 652) - Cylinder geometry
- **QvCone** (Implied, standard VRML primitive)
- **QvIndexedFaceSet** (line 653) - Polygonal mesh
- **QvIndexedLineSet** (line 654) - Line set
- **QvPointSet** (line 655) - Point cloud

**Key Methods:**
- Node creation/destruction
- Field parsing and validation
- Scene graph traversal
- Rendering callbacks
- Name dictionary management (see "QvNode::removeName" at line 716)

**Key String References:**
- "Unknown reference \"%s\"" (line 718)
- "Premature end of file after USE" (line 719)
- "Premature end of file after DEF" (line 724)
- "Unknown class \"%s\"" (line 725)
- "Invalid definition of %s" (line 722)
- "No name given after " (line 723)

---

### 3.2 Field Value Classes

**Field Types with String References:**
- Material fields: ambientColor, diffuseColor, specularColor, emissiveColor, shininess, transparency (lines 709-704)
- Binding types: PER_VERTEX, PER_FACE, PER_PART, PER_VERTEX_INDEXED, PER_FACE_INDEXED, PER_PART_INDEXED (lines 715-710)
- Shape hints: vertexOrdering (CLOCKWISE, COUNTERCLOCKWISE), faceType, shapeType (lines 761-766)
- Index fields: coordIndex, materialIndex, normalIndex, textureCoordIndex (lines 698-695)

---

## 4. VRML FILE READING FUNCTIONS

### 4.1 File I/O System (Location: ~0x40xxxx range)

**What it does:**
- Opens .wrl files from local filesystem or HTTP URLs
- Reads and buffers file contents
- Handles file errors and validation
- Validates VRML header "#VRML V1.0 ascii"

**Key String References:**
- "labyrnth.wrl" (line 102) - Default world file
- "home.wrl" (line 103) - Home world
- "VRML Files (*.wrl)" (line 781) - File dialog filter
- "All Files (*.*)" (line 782)
- "Open a World File" (line 780)
- "Save a World File" (line 783)
- "File does not have a valid header string" (line 701)
- "EOF reached before end of comment" (line 702)
- "Unexpected end of file within comment" (line 556)

**Default VRML Worlds Referenced:**
- http://www.butterfly.net/lab/ushm.wrl (line 486)
- http://www.eit.com/~mark/ushm/ushm.wrl (line 503)
- http://www.eit.com/~mark/soma/soma.wrl (line 504)
- http://www.iuma.com/vrml/ (line 505)

**File Operations:**
- CreateFileA, ReadFile, CloseHandle via Windows API
- File dialog support (GetOpenFileNameA, GetSaveFileNameA)
- Drag and drop support (DragAcceptFiles, DragQueryFileA from SHELL32.dll)

---

## 5. HTTP/WWW FUNCTIONS

### 5.1 HTTP Client Implementation (Source: E:\labsrc\WWW\HTTP.C)

**Location:** ~0x43xxxx-0x44xxxx range (estimated)
**File Reference:** "E:\labsrc\WWW\HTTP.C" (line 435 in strings)

**What it does:**
- Implements HTTP/1.0 protocol client
- Connects to HTTP servers
- Sends HTTP requests with headers
- Receives and parses HTTP responses
- Handles HTTP status codes and redirects

**Key String References:**
- "HTTP/1.0" (line 434)
- "Accept: %s%c%c" (line 432)
- "Accept: %s q=%.3f%c%c" (line 433)
- "User-Agent:  %s/%s  libwww/%s%c%c" (line 431)
- "HTTP Tx: %s" (line 430)
- "HTTP: read returned %d bytes." (line 427)
- "HTTP: Rx: %.70s" (line 426)
- "HTTP connected, socket %d" (line 437)
- "HTTP: Unable to connect to remote host for `%s' (errno = %d)." (line 438)
- "HTTPAccess: Direct access for %s" (line 439)
- "HTTP: close socket %d." (line 417)
- "HTTP: %s" (line 418)
- "HTTPAccess: Unable to connect to remote host for `%s'." (line 149)
- "HTTPAccess: Can't find internet node name `%s'." (line 402)

**HTTP Status Handling:**
- "Unknown status reply from server!" (line 423)
- "Redirection response from server is not handled by this client" (line 422)
- "HTTP 5xx status" (line 421)
- "HTTP server at %s replies:" (line 420)
- "Document address invalid or access not authorised" (line 425)

---

### 5.2 FTP Client Implementation (Source: E:\labsrc\WWW\HTFTP.C)

**Location:** ~0x43xxxx-0x44xxxx range (estimated)
**File Reference:** "E:\labsrc\WWW\HTFTP.C" (line 168 in strings)

**What it does:**
- Implements FTP protocol client
- Handles FTP login and authentication
- Supports anonymous FTP
- File transfer operations
- Directory listing (NLST command)

**Key String References:**
- "FTP connected, socket %d" (line 166)
- "FTP: Unable to connect to remote host for `%s'." (line 167)
- "FTP: Looking for %s" (line 172)
- "FTP: Logged in." (line 159)
- "FTP: Login fail: %s" (line 160)
- "USER anonymous%c%c" (line 164)
- "USER %s%c%c" (line 165)
- "PASS %s%c%c" (line 163)
- "PASS %s@%s%c%c" (line 162)
- "ACCT noaccount%c%c" (line 161)
- "TYPE %s%c%c" (line 197)
- "RETR %s%c%c" (line 196)
- "CWD %s%c%c" (line 195)
- "NLST%c%c" (line 194)
- "PORT %d,%d,%d,%d,%d,%d%c%c" (line 184)

**FTP Connection Management:**
- "FTP: Existing connection is %d.%d.%d.%d" (line 170)
- "FTP: Already have connection for %d.%d.%d.%d." (line 171)
- "FTP: Closing control socket %d" (line 173)
- "FTP: Closing data socket %d" (line 192)
- "FTP: They close so we close socket %d" (line 174)
- "FTP: No control connection set up!!" (line 180)

---

### 5.3 TCP/IP Socket Layer (Source: E:\labsrc\WWW\*.C)

**What it does:**
- Low-level socket operations via WSOCK32.DLL
- Socket creation, binding, connecting
- Data transmission and reception
- Error handling

**Key String References:**
- "TCP: Master socket(), bind() and listen() all OK" (line 183)
- "TCP: Accepted new socket %d" (line 193)
- "TCP: Error %d in `errno' after call to %s() failed." (line 398)
- "TCP: Local host name is %s" (line 403)
- "TCP: Parsed address as port %d, IP address %d.%d.%d.%d" (line 401)
- "000 *** TCP read error on response" (line 175)

**Socket API (WSOCK32.DLL):**
- Referenced at line 491 in strings
- Provides standard Berkeley sockets API on Windows

---

### 5.4 WWW Library Components

**Additional Source Files Referenced:**
- "E:\labsrc\WWW\HTFILE.C" (line 111) - File protocol handler
- "E:\labsrc\WWW\HTACCESS.C" (line 269) - Access control and URL loading
- "E:\labsrc\WWW\HTPARSE.C" (line 200) - URL parsing
- "E:\labsrc\WWW\HTSTRING.C" (line 202) - String utilities
- "E:\labsrc\WWW\HTMIME.C" (line 211) - MIME type handling
- "E:\labsrc\WWW\HTFORMAT.C" (line 153) - Format conversion
- "E:\labsrc\WWW\HTGOPHER.C" (line 140) - Gopher protocol
- "E:\labsrc\WWW\HTML.C" (line 453) - HTML parsing
- "E:\labsrc\WWW\SGML.C" (line 363) - SGML parser
- "E:\labsrc\WWW\HTANCHOR.C" (line 412) - Anchor/link management
- "E:\labsrc\WWW\HTLIST.C" (line 372) - List data structure
- "E:\labsrc\WWW\HTATOM.C" (line 376) - Atom (interned string) management
- "E:\labsrc\WWW\HTBTREE.C" (line 287) - B-tree data structure
- "E:\labsrc\WWW\HTRULES.C" (line 281) - URL rewriting rules

**MIME Type Support:**
- "text/plain" (line 143)
- "text/html" (line 146)
- "image/gif" (line 145)
- "image/jpeg" (line 293)
- "audio/basic" (line 142)
- "video/mpeg" (line 300)
- "application/octet-stream" (line 118)
- Many more in lines 290-344

---

## 6. 3D RENDERING ENGINE

### 6.1 RenderMorphics Reality Lab Integration

**Evidence:**
- "Portions Copyright (c) 1993 - 1995, RenderMorphics, Ltd." (line 100 in strings)
- Reality Lab was an early 3D rendering library (later became Direct3D)

**Key Components:**
- Frame management: "Error on RLFrameAddVisual." (line 515)
- Mesh operations: "Received error %d from RLMeshAddVertex" (line 516)
- Mesh normals: "Received error %d from RLMeshAddNormal" (line 517)
- Billboard support: "Made a billboard." (line 518)

**Rendering Devices (from strings):**
- "NTDevice" (line 788) - Windows NT rendering device
- "RampDevice" (line 819) - Software rasterizer
- "RGBDevice" (line 822) - RGB color device
- "RGBPalDevice" (line 821) - Paletted RGB device
- "MeshRenderer" (line 820) - Mesh rendering
- "GeneralDevice" (line 815) - Generic device
- "PaletteDevice" (line 814) - Palette-based device
- "Viewport" (line 816) - Viewport management

**Error Messages:**
- Error codes: "no error", "object expected in argument", "bad argument type passed", "out of memory", "face already used in a mesh", "object not found in specified place", "unimplemented", "file cannot be opened", "data file is corrupt", "device is not compatible with renderer", "bad argument value" (lines 809-799)
- Debug assertions from "api\object\objdes.c" (line 794)
- Additional assertions from "loadxof.c" (line 797), "tilemap.c" (line 818), "device.c" (line 811), "object.c" (line 813)

---

### 6.2 Texture Mapping

**What it does:**
- Loads and applies textures to 3D geometry
- Supports multiple texture mapping modes
- Texture coordinate generation

**Key String References:**
- "texture.ppm" (line 493) - Texture output file
- "Couldn't create texture map output file." (line 492)
- Texture mapping types: "Flat", "Cylinder", "Sphere" (line 574-575)
- "Internal error: null texture map type" (line 576)
- "Text map type must be Flat, Cylinder or Sphere" (line 574)

---

### 6.3 GIF Image Loader

**Location:** ~0x45xxxx-0x46xxxx range (estimated)

**What it does:**
- Reads GIF87a and GIF89a format images
- Decompresses LZW-compressed image data
- Handles interlaced GIFs
- Parses GIF extensions and comments

**Key String References:**
- "not a GIF file" (line 531)
- "bad version number, not '87a' or '89a'" (line 530)
- "failed to read screen descriptor" (line 529)
- "error reading global colormap" (line 528)
- "error reading magic number" (line 532)
- "bad colormap" (line 533)
- "reading %d by %d%s GIF image" (line 546)
- " interlaced" (line 547)
- "gif comment: %s" (line 536)
- "only %d image%s found in file" (line 526)
- "missing EOD in data stream (common occurence)" (line 543)
- "circular table entry BIG ERROR" (line 542)

---

## 7. GUI FRAMEWORK

### 7.1 Windows GUI Components

**Window Classes:**
- "LABWIN_DRAWING_WINDOW" (line 785) - Main drawing/rendering window
- "InterSpace" (line 778) - Main application window class

**Error Messages:**
- "Can't register drawing window class" (line 786)
- "Can't create drawing window" (line 784)
- "Can't register shell class" (line 779)
- "Can't create shell" (line 777)
- "RL Error" (line 787) - Reality Lab error dialog

**Window Management API (USER32.dll):**
- CreateWindowExA, RegisterClassA
- GetMessageA, PeekMessageA, DispatchMessageA, TranslateMessage
- BeginPaint, EndPaint
- SetCapture, ReleaseCapture, GetCapture
- SetCursor, LoadCursorA, LoadIconA
- ShowWindow, MoveWindow, DestroyWindow
- DialogBoxParamA, EndDialog
- LoadMenuA, GetSubMenu, CheckMenuItem, EnableMenuItem
- And many more (see lines 980-1078 in strings)

**GDI Operations (GDI32.dll):**
- CreatePalette, SelectPalette, RealizePalette
- SetDIBitsToDevice
- CreatePen, CreateSolidBrush
- SelectObject, DeleteObject
- TextOutA, SetBkMode
- (see lines 1020-1030 in strings)

---

### 7.2 Configuration and Settings

**Profile/Registry Access:**
- Uses GetPrivateProfileStringA (line 977)
- Stores settings in INI-style format
- Default style file: "default.style" (line 496)

**Application Info:**
- Title: "Labyrinth VRML WWW Browser" (line 105)
- Window title: "Labyrinth" (line 488)
- Autoload home option: "Autoload Home" (line 104)
- PLAINTEXT and BLOCKQUOTE HTML modes (lines 106-107)

---

## 8. ADDITIONAL COMPONENTS

### 8.1 HTML/SGML Parsing

**Source Files:**
- "E:\labsrc\WWW\SGML.C" (line 363)
- "E:\labsrc\WWW\HTML.C" (line 453)
- "E:\labsrc\WWW\HTMLGEN.C" (line 369)
- "E:\labsrc\VIEWER\GT.C" (line 476) - GridText viewer

**What it does:**
- Parses HTML documents (for help/info pages)
- SGML entity handling
- HTML text display in viewer window

**Key String References:**
- "SGML: Found </%s> when expecting </%s>..." (lines 359-361)
- "SGML: Unknown entity %s" (line 357)
- "SGML: Unknown attribute %s for tag %s" (line 355)
- "SGML: Unknown element %s" (line 353)
- Various HTML elements: "Heading1"-"Heading7", "Glossary", "BlockQuote", "Preformatted" (lines 452-442)

---

### 8.2 Gopher Protocol Support

**Source File:** "E:\labsrc\WWW\HTGOPHER.C" (line 140)

**What it does:**
- Implements Gopher protocol client
- Menu parsing and navigation
- CSO directory search
- Telnet/TN3270 URL generation

**Key String References:**
- "This is a searchable Gopher index." (line 136)
- "CSO Search Results" (line 134)
- "Select one of:" (line 133)
- "HTGopher: Menu item: %s" (line 132)
- "telnet://%s/" (line 130)
- "tn3270://%s/" (line 128)
- "gopher://error.host:1/0" (line 126)

---

### 8.3 News (NNTP) Protocol Support

**Source:** Implied from strings

**What it does:**
- Connects to NNTP news servers
- Retrieves newsgroup articles
- Displays threaded discussions

**Key String References:**
- "NNTPSERVER" (line 220) - Environment variable
- "HTNews: NNTPSERVER defined as `%s'" (line 219)
- "Please define your NNTP server" (line 216)
- "NNTP command to be sent: %s" (line 224)
- "NNTP Response: %s" (line 222)
- "HTNews: Connected to news host %s." (line 252)
- "Connecting to NewsHost ..." (line 256)
- "ARTICLE " (line 257)
- "HEAD %d%c%c" (line 239)
- "Newsgroups:" (line 227)
- "References:" (line 226)

---

## 9. HELP AND DOCUMENTATION URLS

**Built-in Help URLs:**
- http://www.eit.com/vrml/vrmlspec.html (line 499) - VRML specification
- http://www.net.org/~tcc/labyrinth/howto.html (line 500) - How-to guide
- http://www.net.org/~tcc/labyrinth/navigate.html (line 501) - Navigation help
- http://bug.village.virginia.edu/vrml/ (line 502) - VRML resources
- http://info.cern.ch:8001/ (line 267) - CERN HTTP gateway

---

## 10. ERROR HANDLING AND DEBUGGING

### 10.1 Debug Output

**Debug File References:**
- All source files from "E:\labsrc\*" paths are embedded as debug strings
- Allows correlation of errors to specific source files

**Error Dialog:**
- "WWW Alert:  %s" (line 404)
- Format: "%s %s: out of memory." (line 109)
- "Program aborted." (line 110)

### 10.2 Assertion System

**Assert Format:**
- "Assertion failed: %s, file %s, line %d" (line 948)
- Multiple assertions in Reality Lab code (lines 792-810)

---

## 11. MEMORY MANAGEMENT

**Allocation Functions:**
- Custom allocator calls at 0x487c36, 0x487c23, 0x488014, 0x487ef9, etc.
- Standard C runtime malloc/free wrappers
- Out of memory handling: "out of memory" (line 806)

**Memory Errors:**
- "- not enough space for arguments" (line 956)
- "- not enough space for environment" (line 957)
- "- not enough space for thread data" (line 959)
- "- not enough space for _onexit/atexit table" (line 963)

---

## 12. RUNTIME LIBRARY ERRORS

**C Runtime Errors:**
- "abnormal program termination" (line 958)
- "- unexpected multithread lock error" (line 960)
- "- unexpected heap error" (line 961)
- "- unable to open console device" (line 962)
- "- pure virtual function call" (line 964)
- "runtime error " (line 968)
- "DOMAIN error" (line 965)
- "SING error" (line 966)
- "TLOSS error" (line 967)

---

## 13. FUNCTION ADDRESS MAP (Key Functions)

| Address    | Line (approx) | Function Name/Description |
|------------|---------------|---------------------------|
| 0x004897d3 | 162091 | **Entry Point** - C Runtime initialization |
| 0x00424590 | 51875  | **WinMain Wrapper** - Windows entry |
| 0x00401020 | 22     | **Main Application** - GUI init and message loop |
| 0x0040e120 | ~      | 3D rendering context initialization |
| 0x004228e0 | ~      | Object creation/allocation |
| 0x00401000 | 7      | DLL loading/initialization |
| 0x004245e0 | 51898  | Command-line parsing |
| 0x00487c36 | ~      | Memory allocation |
| 0x00487c23 | ~      | String/memory operation |
| 0x00487ef9 | ~      | Printf-style formatting |
| 0x00487fac | ~      | String operation |
| 0x00488014 | ~      | Memory allocation (variant) |
| 0x00487e38 | ~      | Exit/cleanup |
| 0x0048b227 | ~      | Initialization routine |
| 0x0048cb88 | ~      | Initialization routine |
| 0x0048e810 | ~      | Library initialization |
| 0x0048e384 | ~      | Environment setup |
| 0x0048e2b9 | ~      | Setup routine |
| 0x00487e08 | ~      | Initialization |

---

## 14. IMPORT TABLE SUMMARY

### DLL Dependencies:
1. **KERNEL32.dll** - Core Windows API (96+ functions)
2. **USER32.dll** - Window management and GUI (45+ functions)
3. **GDI32.dll** - Graphics Device Interface (18+ functions)
4. **comdlg32.dll** - Common dialogs (2 functions: GetOpenFileNameA, GetSaveFileNameA)
5. **SHELL32.dll** - Shell integration (drag-drop support)
6. **WSOCK32.dll** - Winsock 1.1 networking

---

## 15. DATA SECTION ANALYSIS

### String Data Ranges:
- **0x4b30xx** - Configuration, profile strings
- **0x4b39xx** - Error messages, file paths
- **0x4b3axx** - More error messages
- **0x4b3bxx** - WWW/HTTP strings
- **0x4b3cxx** - Additional WWW strings
- **0x4b3dxx** - VRML parser strings
- **0x4b3exx** - Parser and error strings
- **0x4b3fxx** - Field and node type strings
- **0x4b40xx** - QvNode type strings
- **0x4b41xx** - More node type strings
- **0x4b42xx** - Rendering and texture strings
- **0x4b43xx** - HTTP/FTP protocol strings
- **0x4b44xx** - Additional protocol strings

---

## 16. CODE ORGANIZATION (Estimated)

Based on function call patterns and string references:

| Address Range | Primary Functionality |
|---------------|----------------------|
| 0x401000-0x404FFF | **GUI and application framework** |
| 0x405000-0x40FFFF | **VRML scene graph (QvNode classes)** |
| 0x410000-0x420FFF | **VRML parser and field readers** |
| 0x421000-0x424FFF | **3D rendering interface** |
| 0x425000-0x42FFFF | **Application logic and utilities** |
| 0x430000-0x445FFF | **WWW library (HTTP, FTP, Gopher, etc.)** |
| 0x445000-0x460FFF | **Image loaders (GIF, etc.)** |
| 0x461000-0x480FFF | **3D rendering engine (Reality Lab)** |
| 0x481000-0x491FFF | **C Runtime library functions** |

---

## 17. RECONSTRUCTION RECOMMENDATIONS

### Priority 1 - Core Infrastructure:
1. **Entry Point & Initialization** (0x4897d3, 0x401020)
   - Recreate WinMain and startup code
   - Initialize rendering system
   - Set up message loop

2. **QvNode Class Hierarchy** (~0x405000-0x40FFFF)
   - Define base QvNode class
   - Implement all 30+ node types
   - Virtual method tables for polymorphism

3. **VRML Parser** (~0x410000-0x420FFF)
   - YACC grammar for VRML 1.0
   - Lexer/tokenizer
   - Field value parsers
   - Error handling

### Priority 2 - Networking:
4. **WWW Library** (~0x430000-0x445FFF)
   - HTTP/1.0 client
   - FTP client
   - URL parsing
   - MIME type handling
   - Socket layer (Winsock 1.1)

### Priority 3 - Rendering:
5. **3D Rendering** (~0x461000-0x480FFF)
   - Reality Lab API integration
   - Mesh generation from VRML geometry
   - Texture mapping
   - Lighting
   - Camera control

6. **Image Loading** (~0x445000-0x460FFF)
   - GIF loader (87a/89a)
   - Texture format conversion

### Priority 4 - UI:
7. **GUI Framework** (~0x401000-0x404FFF)
   - Window classes
   - Menu handling
   - File dialogs
   - Mouse/keyboard input
   - Drag-drop support

---

## 18. KNOWN ISSUES AND GAPS

1. **No Direct Symbol Information:** The executable is stripped of symbols, making exact function identification challenging.

2. **Inlined Code:** Many C++ methods may be inlined, making class boundaries unclear.

3. **Virtual Tables:** C++ virtual method tables are present but not directly identifiable without deeper analysis.

4. **Data Structures:** Exact structure layouts for QvNode and related classes need to be inferred from usage patterns.

5. **Reality Lab API:** The exact Reality Lab API version and header files would be needed for accurate rendering code reconstruction.

---

## 19. NEXT STEPS

1. **Detailed Function Analysis:**
   - Trace execution flow from entry point
   - Identify vtable structures
   - Map out class hierarchies

2. **Data Structure Recovery:**
   - Analyze memory allocation patterns
   - Identify structure field accesses
   - Reconstruct class layouts

3. **YACC Grammar Recreation:**
   - Extract parser state tables
   - Reconstruct VRML 1.0 grammar
   - Regenerate parser code

4. **API Header Reconstruction:**
   - Create header files for QvNode classes
   - Define VRML field types
   - Document public interfaces

5. **Test Case Development:**
   - Create sample .wrl files
   - Test parser with various VRML constructs
   - Validate rendering output

---

## CONCLUSION

Labyrinth represents a sophisticated early attempt at 3D web browsing, combining:
- A complete VRML 1.0 parser
- Object-oriented scene graph (QvNode hierarchy)
- HTTP/FTP networking stack
- 3D rendering via Reality Lab
- Windows GUI framework

The codebase is well-structured with clear separation between:
- Networking (WWW library)
- Parsing (YACC-based VRML parser)
- Scene representation (QvNode classes)
- Rendering (Reality Lab integration)
- User interface (Windows GUI)

This modular architecture should facilitate source code reconstruction by allowing each component to be rebuilt independently and integrated progressively.

**Total estimated source files:** 30-40 C/C++ files
**Estimated lines of code:** 30,000-50,000 LOC
**Development period:** 1993-1995
**Compiler:** Microsoft Visual C++ 2.x (based on linker version 2.50)

---

**Report Generated:** 2025-11-22
**Analysis Time:** ~2 hours
**Analyst:** Claude Code Assistant
**Status:** Initial architectural mapping complete
