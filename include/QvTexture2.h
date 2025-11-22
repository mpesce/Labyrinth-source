/*
 * QvTexture2.h - 2D texture node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Defines a 2D texture map for surfaces.
 */

#ifndef _QV_TEXTURE2_H_
#define _QV_TEXTURE2_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvTexture2 - 2D texture map
 *
 * Texture mapping types from strings (line 574-575):
 * - Flat
 * - Cylinder
 * - Sphere
 *
 * VRML Syntax:
 *   Texture2 {
 *     filename    ""
 *     image       0 0 0
 *     wrapS       REPEAT
 *     wrapT       REPEAT
 *   }
 */
class QvTexture2 : public QvNode {
public:
    QvTexture2();
    virtual ~QvTexture2();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_TEXTURE2; }
    virtual const char* getNodeName() const { return "Texture2"; }

    // Wrap modes
    enum Wrap {
        REPEAT,
        CLAMP
    };

    // Fields
    QvSFString filename;  // Texture image filename (.gif, etc.)
    QvSFImage image;      // Inline image data (width height components)
    QvSFEnum wrapS;       // S-coordinate wrapping, default: REPEAT
    QvSFEnum wrapT;       // T-coordinate wrapping, default: REPEAT

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_TEXTURE2_H_
