/*
 * QvTexture2Transform.h - 2D texture transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Transforms texture coordinates.
 */

#ifndef _QV_TEXTURE2_TRANSFORM_H_
#define _QV_TEXTURE2_TRANSFORM_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvTexture2Transform - 2D texture coordinate transformation
 *
 * Applies translation, rotation, and scale to texture coordinates.
 * Transform order: scale -> rotation -> translation
 * All transformations are around the center point.
 *
 * VRML Syntax:
 *   Texture2Transform {
 *     translation  0 0
 *     rotation     0
 *     scaleFactor  1 1
 *     center       0 0
 *   }
 */
class QvTexture2Transform : public QvNode {
public:
    QvTexture2Transform();
    virtual ~QvTexture2Transform();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_TEXTURE2_TRANSFORM; }
    virtual const char* getNodeName() const { return "Texture2Transform"; }

    // Fields
    QvSFVec2f translation;   // Translation (S,T), default: (0,0)
    QvSFFloat rotation;      // Rotation angle in radians, default: 0
    QvSFVec2f scaleFactor;   // Scale factors (S,T), default: (1,1)
    QvSFVec2f center;        // Center point for transforms, default: (0,0)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_TEXTURE2_TRANSFORM_H_
