/*
 * QvRotation.h - Rotation transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Rotation around arbitrary axis.
 */

#ifndef _QV_ROTATION_H_
#define _QV_ROTATION_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvRotation - Rotation transformation
 *
 * VRML Syntax:
 *   Rotation {
 *     rotation  0 0 1  0
 *   }
 *
 * Rotation is specified as axis (x,y,z) + angle in radians.
 */
class QvRotation : public QvNode {
public:
    QvRotation();
    virtual ~QvRotation();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_ROTATION; }
    virtual const char* getNodeName() const { return "Rotation"; }

    // Fields
    QvSFRotation rotation;  // Axis (x,y,z) + angle, default: (0,0,1,0)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_ROTATION_H_
