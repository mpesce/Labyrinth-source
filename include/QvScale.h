/*
 * QvScale.h - Scale transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Non-uniform scaling in 3D space.
 */

#ifndef _QV_SCALE_H_
#define _QV_SCALE_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvScale - Scale transformation
 *
 * VRML Syntax:
 *   Scale {
 *     scaleFactor  1 1 1
 *   }
 */
class QvScale : public QvNode {
public:
    QvScale();
    virtual ~QvScale();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_SCALE; }
    virtual const char* getNodeName() const { return "Scale"; }

    // Fields
    QvSFVec3f scaleFactor;  // Scale factors (x,y,z), default: (1,1,1)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_SCALE_H_
