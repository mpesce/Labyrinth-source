/*
 * QvShapeHints.h - Shape rendering hints node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Provides hints for optimizing rendering of shapes.
 */

#ifndef _QV_SHAPE_HINTS_H_
#define _QV_SHAPE_HINTS_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvShapeHints - Rendering optimization hints
 *
 * Shape hints from strings (lines 761-766):
 * - vertexOrdering: CLOCKWISE, COUNTERCLOCKWISE
 * - faceType
 * - shapeType
 *
 * VRML Syntax:
 *   ShapeHints {
 *     vertexOrdering  UNKNOWN_ORDERING
 *     shapeType       UNKNOWN_SHAPE_TYPE
 *     faceType        CONVEX
 *     creaseAngle     0.5
 *   }
 */
class QvShapeHints : public QvNode {
public:
    QvShapeHints();
    virtual ~QvShapeHints();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_SHAPE_HINTS; }
    virtual const char* getNodeName() const { return "ShapeHints"; }

    // Vertex ordering
    enum VertexOrdering {
        UNKNOWN_ORDERING,
        CLOCKWISE,
        COUNTERCLOCKWISE
    };

    // Shape type
    enum ShapeType {
        UNKNOWN_SHAPE_TYPE,
        SOLID
    };

    // Face type
    enum FaceType {
        UNKNOWN_FACE_TYPE,
        CONVEX
    };

    // Fields
    QvSFEnum vertexOrdering;  // Vertex winding order, default: UNKNOWN_ORDERING
    QvSFEnum shapeType;       // Shape topology, default: UNKNOWN_SHAPE_TYPE
    QvSFEnum faceType;        // Face convexity, default: CONVEX
    QvSFFloat creaseAngle;    // Crease angle for smooth shading, default: 0.5

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_SHAPE_HINTS_H_
