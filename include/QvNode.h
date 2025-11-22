/*
 * QvNode.h - Base class for all VRML nodes
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#ifndef _QV_NODE_H_
#define _QV_NODE_H_

#include <windows.h>

// Forward declarations
class QvField;
class QvFieldData;
class QvInput;
class QvOutput;
class QvString;

// Node types enumeration
typedef enum {
    QV_NODE,
    QV_GROUP,
    QV_SEPARATOR,
    QV_SWITCH,
    QV_TRANSFORM_SEPARATOR,
    QV_LEVEL_OF_DETAIL,
    QV_COORDINATE3,
    QV_MATERIAL,
    QV_MATERIAL_BINDING,
    QV_NORMAL,
    QV_NORMAL_BINDING,
    QV_SHAPE_HINTS,
    QV_TEXTURE_COORDINATE2,
    QV_TEXTURE2,
    QV_TEXTURE2_TRANSFORM,
    QV_DIRECTIONAL_LIGHT,
    QV_POINT_LIGHT,
    QV_SPOT_LIGHT,
    QV_ORTHOGRAPHIC_CAMERA,
    QV_PERSPECTIVE_CAMERA,
    QV_TRANSFORM,
    QV_ROTATION,
    QV_MATRIX_TRANSFORM,
    QV_TRANSLATION,
    QV_SCALE,
    QV_CONE,
    QV_CUBE,
    QV_CYLINDER,
    QV_SPHERE,
    QV_INDEXED_FACE_SET,
    QV_INDEXED_LINE_SET,
    QV_POINT_SET,
    QV_WWW_ANCHOR,
    QV_WWW_INLINE,
    QV_INFO,
    QV_UNKNOWN_NODE
} QvNodeType;

/*
 * QvNode - Base class for all VRML scene graph nodes
 *
 * This is the foundational class for the QvLib library, which implements
 * the VRML 1.0 scene graph. All node types inherit from this base class.
 */
class QvNode {
public:
    // Constructor/Destructor
    QvNode();
    virtual ~QvNode();

    // Type identification
    virtual QvNodeType getNodeType() const = 0;
    virtual const char* getNodeName() const = 0;

    // Name dictionary management
    // Nodes can be named with DEF in VRML and referenced with USE
    void setName(const char* name);
    const char* getName() const;
    static QvNode* findByName(const char* name);
    void removeName();  // Referenced in strings as "QvNode::removeName"

    // Field access
    virtual QvFieldData* getFieldData() = 0;
    QvField* getField(const char* fieldName);
    int getNumFields();

    // Reference counting for shared nodes (USE)
    void ref();
    void unref();
    int getRefCount() const;

    // Scene graph traversal
    virtual BOOL readInstance(QvInput* in);
    virtual void traverse(void* action) = 0;

protected:
    char* name;          // Node name (from DEF)
    int refCount;        // Reference count

    // Global name dictionary
    static void* nameDict;  // B-tree for DEF/USE name lookup

private:
    QvNode(const QvNode&);              // Disable copy constructor
    QvNode& operator=(const QvNode&);   // Disable assignment
};

#endif // _QV_NODE_H_
