/*
 * QvFields.h - VRML field type definitions
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * VRML fields are the properties of nodes. Fields can be single-value (SF)
 * or multi-value (MF).
 */

#ifndef _QV_FIELDS_H_
#define _QV_FIELDS_H_

#include <windows.h>

// Forward declarations
class QvInput;
class QvString;

// Vector and matrix types
struct QvVec2f { float x, y; };
struct QvVec3f { float x, y, z; };
struct QvVec4f { float x, y, z, w; };
struct QvRotation { float x, y, z, angle; };  // Axis-angle rotation
struct QvMatrix {
    float m[4][4];  // 4x4 transformation matrix
};

/*
 * QvField - Base class for all field types
 */
class QvField {
public:
    virtual ~QvField();

    // Read field value from input stream
    virtual BOOL read(QvInput* in, const char* name) = 0;

    // Field type identification
    virtual const char* getTypeId() const = 0;
};

/*
 * QvFieldData - Container for node's fields
 */
class QvFieldData {
public:
    QvFieldData();
    ~QvFieldData();

    void addField(QvField* field, const char* name);
    QvField* getField(const char* name);
    int getNumFields() const;

private:
    struct FieldEntry {
        QvField* field;
        const char* name;
    };
    FieldEntry* fields;
    int numFields;
};

/*
 * Single-Value Fields (SF*)
 */

// QvSFBool - Boolean field (TRUE/FALSE, 0/1)
class QvSFBool : public QvField {
public:
    QvSFBool() : value(FALSE) {}
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFBool"; }

    BOOL value;
};

// QvSFBitMask - Bitmask field (combines multiple flags)
class QvSFBitMask : public QvField {
public:
    QvSFBitMask() : value(0) {}
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFBitMask"; }

    // Error messages from strings:
    // "Unknown QvSFBitMask bit mask value \"%s\""
    // "EOF reached before '%c' in QvSFBitMask value"

    unsigned long value;
};

// QvSFEnum - Enumeration field (one of several named values)
class QvSFEnum : public QvField {
public:
    QvSFEnum() : value(0) {}
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFEnum"; }

    // Error: "Unknown QvSFEnum enumeration value \"%s\""

    int value;
};

// QvSFFloat - Single floating-point number
class QvSFFloat : public QvField {
public:
    QvSFFloat() : value(0.0f) {}
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFFloat"; }

    float value;
};

// QvSFInt32 - 32-bit integer
class QvSFInt32 : public QvField {
public:
    QvSFInt32() : value(0) {}
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFInt32"; }

    long value;
};

// QvSFString - Character string
class QvSFString : public QvField {
public:
    QvSFString() : value(NULL) {}
    ~QvSFString();
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFString"; }

    char* value;
};

// QvSFVec2f - 2D vector
class QvSFVec2f : public QvField {
public:
    QvSFVec2f() { value.x = value.y = 0.0f; }
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFVec2f"; }

    QvVec2f value;
};

// QvSFVec3f - 3D vector
class QvSFVec3f : public QvField {
public:
    QvSFVec3f() { value.x = value.y = value.z = 0.0f; }
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFVec3f"; }

    QvVec3f value;
};

// QvSFRotation - Rotation (axis + angle)
class QvSFRotation : public QvField {
public:
    QvSFRotation() { value.x = value.y = value.z = value.angle = 0.0f; }
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFRotation"; }

    QvRotation value;
};

// QvSFMatrix - 4x4 transformation matrix
class QvSFMatrix : public QvField {
public:
    QvSFMatrix();
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "SFMatrix"; }

    QvMatrix value;
};

/*
 * Multi-Value Fields (MF*)
 * These can contain multiple values of the same type
 */

// QvMFFloat - Array of floats
class QvMFFloat : public QvField {
public:
    QvMFFloat() : values(NULL), num(0) {}
    ~QvMFFloat();
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "MFFloat"; }

    // Error: "Expected '%c' or '%c' but got '%c' while reading value %d"

    float* values;
    int num;
};

// QvMFInt32 - Array of integers
class QvMFInt32 : public QvField {
public:
    QvMFInt32() : values(NULL), num(0) {}
    ~QvMFInt32();
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "MFInt32"; }

    long* values;
    int num;
};

// QvMFString - Array of strings
class QvMFString : public QvField {
public:
    QvMFString() : values(NULL), num(0) {}
    ~QvMFString();
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "MFString"; }

    char** values;
    int num;
};

// QvMFVec2f - Array of 2D vectors
class QvMFVec2f : public QvField {
public:
    QvMFVec2f() : values(NULL), num(0) {}
    ~QvMFVec2f();
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "MFVec2f"; }

    QvVec2f* values;
    int num;
};

// QvMFVec3f - Array of 3D vectors
class QvMFVec3f : public QvField {
public:
    QvMFVec3f() : values(NULL), num(0) {}
    ~QvMFVec3f();
    virtual BOOL read(QvInput* in, const char* name);
    virtual const char* getTypeId() const { return "MFVec3f"; }

    QvVec3f* values;
    int num;
};

/*
 * QvInput - Input stream for parsing VRML
 */
class QvInput {
public:
    QvInput();
    ~QvInput();

    BOOL openFile(const char* filename);
    BOOL setBuffer(const char* buffer, int len);

    BOOL read(char* c);
    BOOL read(int* i);
    BOOL read(float* f);
    BOOL read(char* str, int maxlen);
    BOOL readIdentifier(char* id, int maxlen);

    void skipWhiteSpace();
    void skipComment();

    int getLineNumber() const { return lineNum; }
    BOOL eof() const;

private:
    const char* buffer;
    int buflen;
    int curpos;
    int lineNum;
    HANDLE fileHandle;
};

#endif // _QV_FIELDS_H_
