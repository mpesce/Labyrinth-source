/*
 * QvTransform.h - General transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Complete transformation with translation, rotation, scale, and center.
 */

#ifndef _QV_TRANSFORM_H_
#define _QV_TRANSFORM_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvTransform - General transformation
 *
 * Combines translation, rotation, scale, and scale orientation.
 * Transform order: scale -> scaleOrientation -> rotation -> translation
 * Center point is used for rotation and scaling.
 *
 * VRML Syntax:
 *   Transform {
 *     translation      0 0 0
 *     rotation         0 0 1  0
 *     scaleFactor      1 1 1
 *     scaleOrientation 0 0 1  0
 *     center           0 0 0
 *   }
 */
class QvTransform : public QvNode {
public:
    QvTransform();
    virtual ~QvTransform();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_TRANSFORM; }
    virtual const char* getNodeName() const { return "Transform"; }

    // Fields
    QvSFVec3f translation;        // Translation vector, default: (0,0,0)
    QvSFRotation rotation;        // Rotation (axis + angle), default: (0,0,1,0)
    QvSFVec3f scaleFactor;        // Scale factors, default: (1,1,1)
    QvSFRotation scaleOrientation; // Rotation before scale, default: (0,0,1,0)
    QvSFVec3f center;             // Center point for rotation/scale, default: (0,0,0)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_TRANSFORM_H_
