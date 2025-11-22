/*
 * QvSeparator.h - Separator grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Separator nodes group children and isolate property changes.
 * Properties set within a Separator do not affect nodes outside it.
 */

#ifndef _QV_SEPARATOR_H_
#define _QV_SEPARATOR_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvSeparator - Grouping node with state isolation
 *
 * VRML Syntax:
 *   Separator {
 *     renderCulling  AUTO | ON | OFF
 *     <children>
 *   }
 */
class QvSeparator : public QvNode {
public:
    QvSeparator();
    virtual ~QvSeparator();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_SEPARATOR; }
    virtual const char* getNodeName() const { return "Separator"; }

    // Fields
    QvSFEnum renderCulling;  // AUTO, ON, OFF

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

#endif // _QV_SEPARATOR_H_
