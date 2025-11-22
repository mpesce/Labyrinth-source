/*
 * QvTransformSeparator.h - Transform isolation grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Isolates transformation changes (but not other properties).
 */

#ifndef _QV_TRANSFORM_SEPARATOR_H_
#define _QV_TRANSFORM_SEPARATOR_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvTransformSeparator - Transform-only isolation
 *
 * Like Separator, but only isolates transformations.
 * Material, texture, and other properties pass through unchanged.
 *
 * VRML Syntax:
 *   TransformSeparator {
 *     <children>
 *   }
 */
class QvTransformSeparator : public QvNode {
public:
    QvTransformSeparator();
    virtual ~QvTransformSeparator();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_TRANSFORM_SEPARATOR; }
    virtual const char* getNodeName() const { return "TransformSeparator"; }

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Children management
    void addChild(QvNode* child);
    void removeChild(int index);
    QvNode* getChild(int index);
    int getNumChildren();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    QvNode** children;
    int numChildren;
    int maxChildren;

    static QvFieldData* fieldData;
};

#endif // _QV_TRANSFORM_SEPARATOR_H_
