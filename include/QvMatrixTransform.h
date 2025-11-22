/*
 * QvMatrixTransform.h - Matrix transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Arbitrary 4x4 transformation matrix.
 */

#ifndef _QV_MATRIX_TRANSFORM_H_
#define _QV_MATRIX_TRANSFORM_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvMatrixTransform - 4x4 matrix transformation
 *
 * Allows arbitrary transformation using a 4x4 homogeneous matrix.
 *
 * VRML Syntax:
 *   MatrixTransform {
 *     matrix  1 0 0 0
 *             0 1 0 0
 *             0 0 1 0
 *             0 0 0 1
 *   }
 */
class QvMatrixTransform : public QvNode {
public:
    QvMatrixTransform();
    virtual ~QvMatrixTransform();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_MATRIX_TRANSFORM; }
    virtual const char* getNodeName() const { return "MatrixTransform"; }

    // Fields
    QvSFMatrix matrix;  // 4x4 transformation matrix, default: identity

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_MATRIX_TRANSFORM_H_
