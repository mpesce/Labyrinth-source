/*
 * QvSwitch.h - Switch grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Selects one child to traverse based on whichChild field.
 */

#ifndef _QV_SWITCH_H_
#define _QV_SWITCH_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvSwitch - Selective child rendering
 *
 * Only one child is traversed, selected by whichChild index.
 * whichChild of -1 means render none.
 *
 * VRML Syntax:
 *   Switch {
 *     whichChild  -1
 *     <children>
 *   }
 */
class QvSwitch : public QvNode {
public:
    QvSwitch();
    virtual ~QvSwitch();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_SWITCH; }
    virtual const char* getNodeName() const { return "Switch"; }

    // Special values for whichChild
    enum {
        NONE = -1,   // Don't traverse any child
        ALL  = -3    // Traverse all children (extension)
    };

    // Fields
    QvSFInt32 whichChild;  // Which child to traverse (-1 = none), default: -1

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

#endif // _QV_SWITCH_H_
