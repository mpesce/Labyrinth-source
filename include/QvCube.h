/*
 * QvCube.h - Cube geometry primitive
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Axis-aligned cube centered at origin.
 */

#ifndef _QV_CUBE_H_
#define _QV_CUBE_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvCube - Cube/Box geometry
 *
 * VRML Syntax:
 *   Cube {
 *     width   2.0
 *     height  2.0
 *     depth   2.0
 *   }
 */
class QvCube : public QvNode {
public:
    QvCube();
    virtual ~QvCube();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_CUBE; }
    virtual const char* getNodeName() const { return "Cube"; }

    // Fields
    QvSFFloat width;   // X dimension, default: 2.0
    QvSFFloat height;  // Y dimension, default: 2.0
    QvSFFloat depth;   // Z dimension, default: 2.0

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_CUBE_H_
