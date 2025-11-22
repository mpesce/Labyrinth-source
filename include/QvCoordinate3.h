/*
 * QvCoordinate3.h - 3D coordinate array node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Defines 3D vertex positions for geometry.
 */

#ifndef _QV_COORDINATE3_H_
#define _QV_COORDINATE3_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvCoordinate3 - 3D vertex coordinate array
 *
 * Provides vertex positions for IndexedFaceSet, IndexedLineSet, PointSet.
 *
 * VRML Syntax:
 *   Coordinate3 {
 *     point  [ 0 0 0, 1 0 0, 1 1 0, 0 1 0 ]
 *   }
 */
class QvCoordinate3 : public QvNode {
public:
    QvCoordinate3();
    virtual ~QvCoordinate3();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_COORDINATE3; }
    virtual const char* getNodeName() const { return "Coordinate3"; }

    // Fields
    QvMFVec3f point;  // Array of 3D coordinates

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_COORDINATE3_H_
