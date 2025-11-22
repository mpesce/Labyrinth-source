/*
 * QvIndexedLineSet.h - Indexed line set geometry
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Polyline geometry using indexed vertices.
 */

#ifndef _QV_INDEXED_LINE_SET_H_
#define _QV_INDEXED_LINE_SET_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvIndexedLineSet - Polylines using indexed vertices
 *
 * Uses current Coordinate3 for vertex positions,
 * current Material for colors (if materialBinding per vertex/line).
 *
 * VRML Syntax:
 *   IndexedLineSet {
 *     coordIndex     []
 *     materialIndex  []
 *     normalIndex    []
 *     textureCoordIndex []
 *   }
 *
 * Indices are separated by -1 to indicate end of polyline.
 */
class QvIndexedLineSet : public QvNode {
public:
    QvIndexedLineSet();
    virtual ~QvIndexedLineSet();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_INDEXED_LINE_SET; }
    virtual const char* getNodeName() const { return "IndexedLineSet"; }

    // Fields - all use -1 as line terminator
    QvMFInt32 coordIndex;         // Indices into current Coordinate3
    QvMFInt32 materialIndex;      // Indices into current Material
    QvMFInt32 normalIndex;        // Indices into current Normal
    QvMFInt32 textureCoordIndex;  // Indices into current TextureCoordinate2

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_INDEXED_LINE_SET_H_
