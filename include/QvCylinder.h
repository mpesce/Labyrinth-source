/*
 * QvCylinder.h - Cylinder geometry primitive
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Cylinder centered at origin, aligned with Y axis.
 */

#ifndef _QV_CYLINDER_H_
#define _QV_CYLINDER_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvCylinder - Cylinder geometry
 *
 * VRML Syntax:
 *   Cylinder {
 *     parts   ALL
 *     radius  1.0
 *     height  2.0
 *   }
 */
class QvCylinder : public QvNode {
public:
    QvCylinder();
    virtual ~QvCylinder();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_CYLINDER; }
    virtual const char* getNodeName() const { return "Cylinder"; }

    // Part flags
    enum Parts {
        SIDES  = 0x01,
        TOP    = 0x02,
        BOTTOM = 0x04,
        ALL    = 0x07
    };

    // Fields
    QvSFBitMask parts;   // Which parts to render (SIDES | TOP | BOTTOM)
    QvSFFloat radius;    // Radius, default: 1.0
    QvSFFloat height;    // Height along Y axis, default: 2.0

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_CYLINDER_H_
