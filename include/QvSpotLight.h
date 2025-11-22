/*
 * QvSpotLight.h - Spotlight source node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Cone-shaped light source.
 */

#ifndef _QV_SPOT_LIGHT_H_
#define _QV_SPOT_LIGHT_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvSpotLight - Spotlight (cone-shaped) light source
 *
 * Light radiates from a point in a cone shape.
 * Like a flashlight or stage spotlight.
 *
 * VRML Syntax:
 *   SpotLight {
 *     on           TRUE
 *     intensity    1.0
 *     color        1 1 1
 *     location     0 0 1
 *     direction    0 0 -1
 *     dropOffRate  0
 *     cutOffAngle  0.785398
 *   }
 */
class QvSpotLight : public QvNode {
public:
    QvSpotLight();
    virtual ~QvSpotLight();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_SPOT_LIGHT; }
    virtual const char* getNodeName() const { return "SpotLight"; }

    // Fields
    QvSFBool on;            // Light enabled, default: TRUE
    QvSFFloat intensity;    // Light intensity (0.0-1.0), default: 1.0
    QvSFVec3f color;        // Light color (RGB 0.0-1.0), default: (1,1,1) white
    QvSFVec3f location;     // Light position in 3D space, default: (0,0,1)
    QvSFVec3f direction;    // Light direction vector, default: (0,0,-1)
    QvSFFloat dropOffRate;  // Intensity falloff (0.0-1.0), default: 0
    QvSFFloat cutOffAngle;  // Cone half-angle in radians, default: 0.785398 (45°)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_SPOT_LIGHT_H_
