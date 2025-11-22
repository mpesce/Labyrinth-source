/*
 * QvFields.cpp - VRML field type implementations
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvFields.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*
 * QvField base class
 */
QvField::~QvField()
{
}

/*
 * QvFieldData - Container for node's fields
 */
QvFieldData::QvFieldData()
{
    fields = NULL;
    numFields = 0;
}

QvFieldData::~QvFieldData()
{
    if (fields != NULL) {
        free(fields);
    }
}

void QvFieldData::addField(QvField* field, const char* name)
{
    // Reallocate field array
    numFields++;
    fields = (FieldEntry*)realloc(fields, numFields * sizeof(FieldEntry));

    fields[numFields - 1].field = field;
    fields[numFields - 1].name = name;
}

QvField* QvFieldData::getField(const char* name)
{
    for (int i = 0; i < numFields; i++) {
        if (strcmp(fields[i].name, name) == 0) {
            return fields[i].field;
        }
    }
    return NULL;
}

QvField* QvFieldData::getField(int index, const char** name) const
{
    if (index < 0 || index >= numFields) {
        return NULL;
    }
    if (name != NULL) {
        *name = fields[index].name;
    }
    return fields[index].field;
}

const char* QvFieldData::getFieldName(int index) const
{
    if (index < 0 || index >= numFields) {
        return NULL;
    }
    return fields[index].name;
}

int QvFieldData::getNumFields() const
{
    return numFields;
}

/*
 * QvSFBool - Boolean field
 *
 * Error: "Unknown value (\"%s\") for QvSFBool" (line 692)
 * Error: "Illegal value for QvSFBool: %d (must be 0 or 1)" (line 694)
 */
BOOL QvSFBool::read(QvInput* in, const char* name)
{
    char token[256];

    if (!in->readIdentifier(token, sizeof(token))) {
        return FALSE;
    }

    // Check for TRUE/FALSE keywords
    if (strcmp(token, "TRUE") == 0 || strcmp(token, "1") == 0) {
        value = TRUE;
        return TRUE;
    } else if (strcmp(token, "FALSE") == 0 || strcmp(token, "0") == 0) {
        value = FALSE;
        return TRUE;
    }

    // Unknown value
    return FALSE;
}

/*
 * QvSFFloat - Single floating-point number
 */
BOOL QvSFFloat::read(QvInput* in, const char* name)
{
    return in->read(&value);
}

/*
 * QvSFInt32 - 32-bit integer
 */
BOOL QvSFInt32::read(QvInput* in, const char* name)
{
    int temp;
    if (!in->read(&temp)) {
        return FALSE;
    }
    value = (long)temp;
    return TRUE;
}

/*
 * QvSFString - Character string
 */
QvSFString::~QvSFString()
{
    if (value != NULL) {
        free(value);
    }
}

BOOL QvSFString::read(QvInput* in, const char* name)
{
    char buffer[4096];

    if (!in->read(buffer, sizeof(buffer))) {
        return FALSE;
    }

    if (value != NULL) {
        free(value);
    }

    value = (char*)malloc(strlen(buffer) + 1);
    strcpy(value, buffer);

    return TRUE;
}

/*
 * QvSFVec2f - 2D vector
 */
BOOL QvSFVec2f::read(QvInput* in, const char* name)
{
    return in->read(&value.x) && in->read(&value.y);
}

/*
 * QvSFVec3f - 3D vector
 */
BOOL QvSFVec3f::read(QvInput* in, const char* name)
{
    return in->read(&value.x) && in->read(&value.y) && in->read(&value.z);
}

/*
 * QvSFRotation - Rotation (axis + angle)
 */
BOOL QvSFRotation::read(QvInput* in, const char* name)
{
    return in->read(&value.x) &&
           in->read(&value.y) &&
           in->read(&value.z) &&
           in->read(&value.angle);
}

/*
 * QvSFMatrix - 4x4 transformation matrix
 */
QvSFMatrix::QvSFMatrix()
{
    // Initialize to identity matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            value.m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

BOOL QvSFMatrix::read(QvInput* in, const char* name)
{
    // Read 16 floats in row-major order
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!in->read(&value.m[i][j])) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/*
 * QvMFFloat - Array of floats
 *
 * Error: "Expected '%c' or '%c' but got '%c' while reading value %d" (line 691)
 */
QvMFFloat::~QvMFFloat()
{
    if (values != NULL) {
        free(values);
    }
}

BOOL QvMFFloat::read(QvInput* in, const char* name)
{
    char c;

    in->skipWhiteSpace();
    if (!in->read(&c)) {
        return FALSE;
    }

    // Check for opening bracket
    if (c != '[') {
        return FALSE;
    }

    // Read floats until closing bracket
    num = 0;
    values = NULL;

    while (TRUE) {
        in->skipWhiteSpace();

        // Check for closing bracket
        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;
        }

        // Put character back (TODO: implement pushback)

        // Read float value
        float val;
        if (!in->read(&val)) {
            return FALSE;
        }

        // Add to array
        num++;
        values = (float*)realloc(values, num * sizeof(float));
        values[num - 1] = val;

        // Check for comma separator
        in->skipWhiteSpace();
        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;  // End of array
        }

        if (c != ',') {
            // Expected comma or closing bracket
            return FALSE;
        }
    }

    return TRUE;
}

/*
 * QvMFInt32 - Array of integers
 */
QvMFInt32::~QvMFInt32()
{
    if (values != NULL) {
        free(values);
    }
}

BOOL QvMFInt32::read(QvInput* in, const char* name)
{
    char c;

    in->skipWhiteSpace();
    if (!in->read(&c)) {
        return FALSE;
    }

    if (c != '[') {
        return FALSE;
    }

    num = 0;
    values = NULL;

    while (TRUE) {
        in->skipWhiteSpace();

        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;
        }

        // Put character back (TODO)

        int val;
        if (!in->read(&val)) {
            return FALSE;
        }

        num++;
        values = (long*)realloc(values, num * sizeof(long));
        values[num - 1] = (long)val;

        in->skipWhiteSpace();
        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;
        }

        if (c != ',') {
            return FALSE;
        }
    }

    return TRUE;
}

/*
 * QvMFVec3f - Array of 3D vectors
 */
QvMFVec3f::~QvMFVec3f()
{
    if (values != NULL) {
        free(values);
    }
}

BOOL QvMFVec3f::read(QvInput* in, const char* name)
{
    char c;

    in->skipWhiteSpace();
    if (!in->read(&c)) {
        return FALSE;
    }

    if (c != '[') {
        return FALSE;
    }

    num = 0;
    values = NULL;

    while (TRUE) {
        in->skipWhiteSpace();

        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;
        }

        // Put character back (TODO)

        // Read 3 floats
        QvVec3f vec;
        if (!in->read(&vec.x) || !in->read(&vec.y) || !in->read(&vec.z)) {
            return FALSE;
        }

        num++;
        values = (QvVec3f*)realloc(values, num * sizeof(QvVec3f));
        values[num - 1] = vec;

        in->skipWhiteSpace();
        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;
        }

        if (c != ',') {
            return FALSE;
        }
    }

    return TRUE;
}

/*
 * QvInput - Input stream for parsing VRML
 */
QvInput::QvInput()
{
    buffer = NULL;
    buflen = 0;
    curpos = 0;
    lineNum = 1;
    fileHandle = NULL;
}

QvInput::~QvInput()
{
    if (fileHandle != NULL) {
        fclose(fileHandle);
    }
}

BOOL QvInput::openFile(const char* filename)
{
    /* Open file with standard C library (cross-platform) */
    fileHandle = fopen(filename, "rb");  /* Binary mode */

    if (fileHandle == NULL) {
        return FALSE;
    }

    /* Get file size */
    fseek(fileHandle, 0, SEEK_END);
    long fileSize = ftell(fileHandle);
    fseek(fileHandle, 0, SEEK_SET);

    /* Allocate buffer */
    char* tempBuf = (char*)malloc(fileSize + 1);
    if (tempBuf == NULL) {
        fclose(fileHandle);
        fileHandle = NULL;
        return FALSE;
    }

    /* Read file */
    size_t bytesRead = fread(tempBuf, 1, fileSize, fileHandle);
    tempBuf[bytesRead] = '\0';

    setBuffer(tempBuf, bytesRead);
    free(tempBuf);

    /* Note: fileHandle stays open for potential reuse */

    return TRUE;
}

BOOL QvInput::setBuffer(const char* buf, int len)
{
    buffer = buf;
    buflen = len;
    curpos = 0;
    lineNum = 1;
    return TRUE;
}

BOOL QvInput::read(char* c)
{
    if (curpos >= buflen) {
        return FALSE;
    }

    *c = buffer[curpos++];

    if (*c == '\n') {
        lineNum++;
    }

    return TRUE;
}

BOOL QvInput::read(int* i)
{
    skipWhiteSpace();

    char numStr[64];
    int numPos = 0;
    char c;

    // Read sign
    if (curpos < buflen && (buffer[curpos] == '-' || buffer[curpos] == '+')) {
        numStr[numPos++] = buffer[curpos++];
    }

    // Read digits
    while (curpos < buflen && isdigit(buffer[curpos])) {
        numStr[numPos++] = buffer[curpos++];
    }

    numStr[numPos] = '\0';

    if (numPos == 0 || (numPos == 1 && (numStr[0] == '-' || numStr[0] == '+'))) {
        return FALSE;
    }

    *i = atoi(numStr);
    return TRUE;
}

BOOL QvInput::read(float* f)
{
    skipWhiteSpace();

    char numStr[64];
    int numPos = 0;

    // Read sign
    if (curpos < buflen && (buffer[curpos] == '-' || buffer[curpos] == '+')) {
        numStr[numPos++] = buffer[curpos++];
    }

    // Read digits before decimal
    while (curpos < buflen && isdigit(buffer[curpos])) {
        numStr[numPos++] = buffer[curpos++];
    }

    // Read decimal point and digits after
    if (curpos < buflen && buffer[curpos] == '.') {
        numStr[numPos++] = buffer[curpos++];
        while (curpos < buflen && isdigit(buffer[curpos])) {
            numStr[numPos++] = buffer[curpos++];
        }
    }

    // Read exponent
    if (curpos < buflen && (buffer[curpos] == 'e' || buffer[curpos] == 'E')) {
        numStr[numPos++] = buffer[curpos++];
        if (curpos < buflen && (buffer[curpos] == '-' || buffer[curpos] == '+')) {
            numStr[numPos++] = buffer[curpos++];
        }
        while (curpos < buflen && isdigit(buffer[curpos])) {
            numStr[numPos++] = buffer[curpos++];
        }
    }

    numStr[numPos] = '\0';

    if (numPos == 0) {
        return FALSE;
    }

    *f = (float)atof(numStr);
    return TRUE;
}

BOOL QvInput::read(char* str, int maxlen)
{
    skipWhiteSpace();

    if (curpos >= buflen) {
        return FALSE;
    }

    // Strings are quoted
    if (buffer[curpos] != '"') {
        return FALSE;
    }
    curpos++;  // Skip opening quote

    int strPos = 0;
    while (curpos < buflen && buffer[curpos] != '"' && strPos < maxlen - 1) {
        if (buffer[curpos] == '\\') {
            // Escape sequence
            curpos++;
            if (curpos >= buflen) {
                return FALSE;
            }
            switch (buffer[curpos]) {
                case 'n': str[strPos++] = '\n'; break;
                case 't': str[strPos++] = '\t'; break;
                case 'r': str[strPos++] = '\r'; break;
                case '\\': str[strPos++] = '\\'; break;
                case '"': str[strPos++] = '"'; break;
                default: str[strPos++] = buffer[curpos]; break;
            }
        } else {
            str[strPos++] = buffer[curpos];
        }
        curpos++;
    }

    if (curpos >= buflen || buffer[curpos] != '"') {
        return FALSE;
    }
    curpos++;  // Skip closing quote

    str[strPos] = '\0';
    return TRUE;
}

BOOL QvInput::readIdentifier(char* id, int maxlen)
{
    skipWhiteSpace();

    if (curpos >= buflen) {
        return FALSE;
    }

    // Identifiers start with letter or underscore
    if (!isalpha(buffer[curpos]) && buffer[curpos] != '_') {
        return FALSE;
    }

    int idPos = 0;
    while (curpos < buflen && idPos < maxlen - 1) {
        char c = buffer[curpos];
        if (isalnum(c) || c == '_') {
            id[idPos++] = c;
            curpos++;
        } else {
            break;
        }
    }

    id[idPos] = '\0';
    return idPos > 0;
}

void QvInput::skipWhiteSpace()
{
    while (curpos < buflen) {
        char c = buffer[curpos];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (c == '\n') {
                lineNum++;
            }
            curpos++;
        } else if (c == '#') {
            // Comment - skip to end of line
            skipComment();
        } else {
            break;
        }
    }
}

void QvInput::skipComment()
{
    // Skip until newline
    // "EOF reached before end of comment" (line 702)
    // "Unexpected end of file within comment" (line 556)

    while (curpos < buflen && buffer[curpos] != '\n') {
        curpos++;
    }
    if (curpos < buflen) {
        lineNum++;
        curpos++;
    }
}

BOOL QvInput::eof() const
{
    return curpos >= buflen;
}

/*
 * QvSFBitMask - Bitmask field implementation
 */
BOOL QvSFBitMask::read(QvInput* in, const char* name)
{
    /* Read bitmask value (implementation stub) */
    int intval;
    if (!in->read(&intval)) {
        return FALSE;
    }
    value = (unsigned long)intval;
    return TRUE;
}

/*
 * QvSFEnum - Enumeration field implementation
 */
BOOL QvSFEnum::read(QvInput* in, const char* name)
{
    /* Read enum value (implementation stub) */
    if (!in->read(&value)) {
        return FALSE;
    }
    return TRUE;
}

/*
 * QvSFImage - Image field implementation
 */
QvSFImage::~QvSFImage()
{
    if (data != NULL) {
        free(data);
    }
}

BOOL QvSFImage::read(QvInput* in, const char* name)
{
    /* Read image: width height components [pixel data] */
    if (!in->read(&width) || !in->read(&height) || !in->read(&components)) {
        return FALSE;
    }

    /* Skip pixel data for now (stub implementation) */
    /* Full implementation would read hexadecimal pixel values */
    return TRUE;
}

/*
 * QvMFVec2f - Array of 2D vectors
 */
QvMFVec2f::~QvMFVec2f()
{
    if (values != NULL) {
        free(values);
    }
}

BOOL QvMFVec2f::read(QvInput* in, const char* name)
{
    char c;

    in->skipWhiteSpace();
    if (!in->read(&c)) {
        return FALSE;
    }

    if (c != '[') {
        return FALSE;
    }

    num = 0;
    values = NULL;

    while (TRUE) {
        in->skipWhiteSpace();

        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;
        }

        /* Read 2 floats */
        QvVec2f vec;
        if (!in->read(&vec.x) || !in->read(&vec.y)) {
            return FALSE;
        }

        num++;
        values = (QvVec2f*)realloc(values, num * sizeof(QvVec2f));
        values[num - 1] = vec;

        in->skipWhiteSpace();
        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;
        }

        if (c != ',') {
            return FALSE;
        }
    }

    return TRUE;
}

/*
 * QvMFString - Array of strings
 */
QvMFString::~QvMFString()
{
    if (values != NULL) {
        for (int i = 0; i < num; i++) {
            if (values[i] != NULL) {
                free(values[i]);
            }
        }
        free(values);
    }
}

BOOL QvMFString::read(QvInput* in, const char* name)
{
    char c;
    char str[256];

    in->skipWhiteSpace();
    if (!in->read(&c)) {
        return FALSE;
    }

    if (c != '[') {
        return FALSE;
    }

    num = 0;
    values = NULL;

    while (TRUE) {
        in->skipWhiteSpace();

        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;
        }

        /* Read string */
        if (!in->read(str, sizeof(str))) {
            return FALSE;
        }

        num++;
        values = (char**)realloc(values, num * sizeof(char*));
        values[num - 1] = (char*)malloc(strlen(str) + 1);
        strcpy(values[num - 1], str);

        in->skipWhiteSpace();
        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == ']') {
            break;
        }

        if (c != ',') {
            return FALSE;
        }
    }

    return TRUE;
}
