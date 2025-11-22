/*
 * QvOrthographicCamera.h - Orthographic camera node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Orthographic (parallel projection) camera.
 */

#ifndef _QV_ORTHOGRAPHIC_CAMERA_H_
#define _QV_ORTHOGRAPHIC_CAMERA_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvOrthographicCamera - Orthographic projection camera
 *
 * Parallel projection - objects don't get smaller with distance.
 * Used for technical drawings, CAD, etc.
 *
 * VRML Syntax:
 *   OrthographicCamera {
 *     position       0 0 1
 *     orientation    0 0 1  0
 *     focalDistance  5
 *     height         2
 *   }
 */
class QvOrthographicCamera : public QvNode {
public:
    QvOrthographicCamera();
    virtual ~QvOrthographicCamera();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_ORTHOGRAPHIC_CAMERA; }
    virtual const char* getNodeName() const { return "OrthographicCamera"; }

    // Fields
    QvSFVec3f position;        // Camera position in 3D space, default: (0,0,1)
    QvSFRotation orientation;  // Camera orientation, default: (0,0,1,0)
    QvSFFloat focalDistance;   // Distance to focal point, default: 5
    QvSFFloat height;          // Height of view volume, default: 2

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_ORTHOGRAPHIC_CAMERA_H_
