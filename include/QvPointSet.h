/*
 * QvPointSet.h - Point cloud geometry
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Set of 3D points in space.
 */

#ifndef _QV_POINT_SET_H_
#define _QV_POINT_SET_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvPointSet - Point cloud geometry
 *
 * Uses current Coordinate3 for point positions,
 * current Material for colors.
 *
 * VRML Syntax:
 *   PointSet {
 *     startIndex  0
 *     numPoints  -1
 *   }
 *
 * Renders points from Coordinate3 array starting at startIndex.
 * numPoints of -1 means render all remaining points.
 */
class QvPointSet : public QvNode {
public:
    QvPointSet();
    virtual ~QvPointSet();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_POINT_SET; }
    virtual const char* getNodeName() const { return "PointSet"; }

    // Fields
    QvSFInt32 startIndex;  // First point to render, default: 0
    QvSFInt32 numPoints;   // Number of points (-1 = all), default: -1

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_POINT_SET_H_
