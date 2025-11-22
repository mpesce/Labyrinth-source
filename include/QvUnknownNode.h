/*
 * QvUnknownNode.h - Unknown node type handler
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Fallback node for unrecognized node types.
 */

#ifndef _QV_UNKNOWN_NODE_H_
#define _QV_UNKNOWN_NODE_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvUnknownNode - Fallback for unknown node types
 *
 * When the parser encounters a node type it doesn't recognize,
 * it creates an QvUnknownNode to preserve the scene structure.
 * Allows forward compatibility with future VRML extensions.
 *
 * Referenced in strings as "QvUnknownNode" (line 659)
 * Error: "Unknown class \"%s\"" (line 725)
 *
 * VRML Syntax:
 *   UnknownType {
 *     ... (fields are ignored but parsed) ...
 *   }
 */
class QvUnknownNode : public QvNode {
public:
    QvUnknownNode();
    virtual ~QvUnknownNode();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_UNKNOWN_NODE; }
    virtual const char* getNodeName() const { return className; }

    // Set the actual class name from VRML file
    void setClassName(const char* name);

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal (does nothing)
    virtual void traverse(void* action);

protected:
    char* className;  // Name of unknown node type

    static QvFieldData* fieldData;
};

#endif // _QV_UNKNOWN_NODE_H_
