/*
 * QvGroup.h - Basic grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Groups child nodes without state isolation.
 */

#ifndef _QV_GROUP_H_
#define _QV_GROUP_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvGroup - Basic grouping node
 *
 * Unlike Separator, Group does NOT isolate property changes.
 * Properties set inside a Group affect nodes outside it.
 *
 * VRML Syntax:
 *   Group {
 *     <children>
 *   }
 */
class QvGroup : public QvNode {
public:
    QvGroup();
    virtual ~QvGroup();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_GROUP; }
    virtual const char* getNodeName() const { return "Group"; }

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

#endif // _QV_GROUP_H_
