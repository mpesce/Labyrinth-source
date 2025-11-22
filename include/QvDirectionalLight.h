/*
 * QvDirectionalLight.h - Directional light source node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Distant light source with parallel rays (like sunlight).
 */

#ifndef _QV_DIRECTIONAL_LIGHT_H_
#define _QV_DIRECTIONAL_LIGHT_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvDirectionalLight - Directional (infinite) light source
 *
 * Light rays are parallel, coming from specified direction.
 * Position is ignored - only direction matters.
 *
 * VRML Syntax:
 *   DirectionalLight {
 *     on          TRUE
 *     intensity   1.0
 *     color       1 1 1
 *     direction   0 0 -1
 *   }
 */
class QvDirectionalLight : public QvNode {
public:
    QvDirectionalLight();
    virtual ~QvDirectionalLight();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_DIRECTIONAL_LIGHT; }
    virtual const char* getNodeName() const { return "DirectionalLight"; }

    // Fields
    QvSFBool on;           // Light enabled, default: TRUE
    QvSFFloat intensity;   // Light intensity (0.0-1.0), default: 1.0
    QvSFVec3f color;       // Light color (RGB 0.0-1.0), default: (1,1,1) white
    QvSFVec3f direction;   // Light direction vector, default: (0,0,-1)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_DIRECTIONAL_LIGHT_H_
