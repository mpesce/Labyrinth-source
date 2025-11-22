/*
 * QvNormal.h - Normal vector array node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Defines normal vectors for lighting calculations.
 */

#ifndef _QV_NORMAL_H_
#define _QV_NORMAL_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvNormal - Normal vector array
 *
 * Provides surface normal vectors for smooth shading.
 *
 * VRML Syntax:
 *   Normal {
 *     vector  [ 0 0 1, 0 0 1, 0 0 1, 0 0 1 ]
 *   }
 */
class QvNormal : public QvNode {
public:
    QvNormal();
    virtual ~QvNormal();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_NORMAL; }
    virtual const char* getNodeName() const { return "Normal"; }

    // Fields
    QvMFVec3f vector;  // Array of normal vectors (should be unit length)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_NORMAL_H_
