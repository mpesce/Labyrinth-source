/*
 * QvSphere.h - Sphere geometry primitive
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Sphere centered at origin with specified radius.
 */

#ifndef _QV_SPHERE_H_
#define _QV_SPHERE_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvSphere - Sphere geometry
 *
 * VRML Syntax:
 *   Sphere {
 *     radius  1.0
 *   }
 */
class QvSphere : public QvNode {
public:
    QvSphere();
    virtual ~QvSphere();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_SPHERE; }
    virtual const char* getNodeName() const { return "Sphere"; }

    // Fields
    QvSFFloat radius;  // Default: 1.0

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_SPHERE_H_
