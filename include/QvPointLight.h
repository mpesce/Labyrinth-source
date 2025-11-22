/*
 * QvPointLight.h - Point light source node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Omnidirectional light source at a specific location.
 */

#ifndef _QV_POINT_LIGHT_H_
#define _QV_POINT_LIGHT_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvPointLight - Omnidirectional point light source
 *
 * Light radiates equally in all directions from a point.
 * Like a light bulb.
 *
 * VRML Syntax:
 *   PointLight {
 *     on          TRUE
 *     intensity   1.0
 *     color       1 1 1
 *     location    0 0 0
 *   }
 */
class QvPointLight : public QvNode {
public:
    QvPointLight();
    virtual ~QvPointLight();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_POINT_LIGHT; }
    virtual const char* getNodeName() const { return "PointLight"; }

    // Fields
    QvSFBool on;           // Light enabled, default: TRUE
    QvSFFloat intensity;   // Light intensity (0.0-1.0), default: 1.0
    QvSFVec3f color;       // Light color (RGB 0.0-1.0), default: (1,1,1) white
    QvSFVec3f location;    // Light position in 3D space, default: (0,0,0)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_POINT_LIGHT_H_
