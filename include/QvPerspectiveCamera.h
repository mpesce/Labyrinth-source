/*
 * QvPerspectiveCamera.h - Perspective camera node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Perspective (realistic) camera projection.
 */

#ifndef _QV_PERSPECTIVE_CAMERA_H_
#define _QV_PERSPECTIVE_CAMERA_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvPerspectiveCamera - Perspective projection camera
 *
 * Realistic projection - distant objects appear smaller.
 * Most common camera type for 3D scenes.
 *
 * VRML Syntax:
 *   PerspectiveCamera {
 *     position       0 0 1
 *     orientation    0 0 1  0
 *     focalDistance  5
 *     heightAngle    0.785398
 *   }
 */
class QvPerspectiveCamera : public QvNode {
public:
    QvPerspectiveCamera();
    virtual ~QvPerspectiveCamera();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_PERSPECTIVE_CAMERA; }
    virtual const char* getNodeName() const { return "PerspectiveCamera"; }

    // Fields
    QvSFVec3f position;        // Camera position in 3D space, default: (0,0,1)
    QvSFRotation orientation;  // Camera orientation, default: (0,0,1,0)
    QvSFFloat focalDistance;   // Distance to focal point, default: 5
    QvSFFloat heightAngle;     // Vertical field of view in radians, default: 0.785398 (45°)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_PERSPECTIVE_CAMERA_H_
