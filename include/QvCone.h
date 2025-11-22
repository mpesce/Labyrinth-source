/*
 * QvCone.h - Cone geometry primitive
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Cone centered at origin, aligned with Y axis.
 */

#ifndef _QV_CONE_H_
#define _QV_CONE_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvCone - Cone geometry
 *
 * VRML Syntax:
 *   Cone {
 *     parts       ALL
 *     bottomRadius 1.0
 *     height      2.0
 *   }
 */
class QvCone : public QvNode {
public:
    QvCone();
    virtual ~QvCone();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_CONE; }
    virtual const char* getNodeName() const { return "Cone"; }

    // Part flags
    enum Parts {
        SIDES  = 0x01,
        BOTTOM = 0x02,
        ALL    = 0x03
    };

    // Fields
    QvSFBitMask parts;         // Which parts to render (SIDES | BOTTOM)
    QvSFFloat bottomRadius;    // Radius at base, default: 1.0
    QvSFFloat height;          // Height along Y axis, default: 2.0

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_CONE_H_
