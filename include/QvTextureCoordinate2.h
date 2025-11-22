/*
 * QvTextureCoordinate2.h - 2D texture coordinate array node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Defines texture coordinates for geometry.
 */

#ifndef _QV_TEXTURE_COORDINATE2_H_
#define _QV_TEXTURE_COORDINATE2_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvTextureCoordinate2 - 2D texture coordinate array
 *
 * Provides texture coordinates (S,T) for vertices.
 * Typically used with IndexedFaceSet via textureCoordIndex.
 *
 * VRML Syntax:
 *   TextureCoordinate2 {
 *     point  [ 0 0, 1 0, 1 1, 0 1 ]
 *   }
 */
class QvTextureCoordinate2 : public QvNode {
public:
    QvTextureCoordinate2();
    virtual ~QvTextureCoordinate2();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_TEXTURE_COORDINATE2; }
    virtual const char* getNodeName() const { return "TextureCoordinate2"; }

    // Fields
    QvMFVec2f point;  // Array of 2D texture coordinates (S,T)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_TEXTURE_COORDINATE2_H_
