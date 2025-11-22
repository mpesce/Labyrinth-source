/*
 * QvWWWAnchor.h - WWW hyperlink anchor node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Makes children clickable with a hyperlink to another URL.
 */

#ifndef _QV_WWW_ANCHOR_H_
#define _QV_WWW_ANCHOR_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvWWWAnchor - Hyperlink anchor
 *
 * Creates a clickable link that navigates to another VRML world
 * or web page when user clicks on the children geometry.
 *
 * VRML Syntax:
 *   WWWAnchor {
 *     name        ""
 *     description ""
 *     map         NONE
 *     <children>
 *   }
 */
class QvWWWAnchor : public QvNode {
public:
    QvWWWAnchor();
    virtual ~QvWWWAnchor();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_WWW_ANCHOR; }
    virtual const char* getNodeName() const { return "WWWAnchor"; }

    // Map types
    enum Map {
        NONE,
        POINT
    };

    // Fields
    QvSFString name;         // URL to navigate to when clicked
    QvSFString description;  // User-visible description of link
    QvSFEnum map;            // Mapping type, default: NONE

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

#endif // _QV_WWW_ANCHOR_H_
