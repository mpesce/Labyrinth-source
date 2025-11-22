/*
 * QvTranslation.h - Translation transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Translation in 3D space.
 */

#ifndef _QV_TRANSLATION_H_
#define _QV_TRANSLATION_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvTranslation - Translation transformation
 *
 * VRML Syntax:
 *   Translation {
 *     translation  0 0 0
 *   }
 */
class QvTranslation : public QvNode {
public:
    QvTranslation();
    virtual ~QvTranslation();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_TRANSLATION; }
    virtual const char* getNodeName() const { return "Translation"; }

    // Fields
    QvSFVec3f translation;  // Translation vector (x,y,z), default: (0,0,0)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_TRANSLATION_H_
