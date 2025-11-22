/*
 * QvInfo.h - Information/comment node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Stores text information/comments in the scene graph.
 */

#ifndef _QV_INFO_H_
#define _QV_INFO_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvInfo - Information/metadata node
 *
 * Stores arbitrary text information in the scene.
 * Has no visual representation - purely for documentation.
 *
 * VRML Syntax:
 *   Info {
 *     string  "<text>"
 *   }
 */
class QvInfo : public QvNode {
public:
    QvInfo();
    virtual ~QvInfo();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_INFO; }
    virtual const char* getNodeName() const { return "Info"; }

    // Fields
    QvSFString string;  // Information text

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_INFO_H_
