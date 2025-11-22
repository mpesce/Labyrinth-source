/*
 * QvIndexedFaceSet.h - Indexed polygonal mesh geometry
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Defines polygonal geometry using indexed vertices.
 */

#ifndef _QV_INDEXED_FACE_SET_H_
#define _QV_INDEXED_FACE_SET_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvIndexedFaceSet - Polygonal mesh using indexed vertices
 *
 * Index field references from strings (lines 698-695):
 * - coordIndex
 * - materialIndex
 * - normalIndex
 * - textureCoordIndex
 *
 * Uses current Coordinate3 for vertex positions,
 * current Material for colors,
 * current Normal for vertex normals,
 * current TextureCoordinate2 for texture coordinates.
 *
 * VRML Syntax:
 *   IndexedFaceSet {
 *     coordIndex        []
 *     materialIndex     []
 *     normalIndex       []
 *     textureCoordIndex []
 *   }
 *
 * Indices are separated by -1 to indicate end of polygon.
 */
class QvIndexedFaceSet : public QvNode {
public:
    QvIndexedFaceSet();
    virtual ~QvIndexedFaceSet();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_INDEXED_FACE_SET; }
    virtual const char* getNodeName() const { return "IndexedFaceSet"; }

    // Fields - all use -1 as polygon/line terminator
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

#endif // _QV_INDEXED_FACE_SET_H_
