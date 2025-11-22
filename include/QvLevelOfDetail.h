/*
 * QvLevelOfDetail.h - Level-of-detail grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Selects child based on distance from camera.
 */

#ifndef _QV_LEVEL_OF_DETAIL_H_
#define _QV_LEVEL_OF_DETAIL_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvLevelOfDetail - Distance-based LOD selection
 *
 * Selects which child to traverse based on distance from camera.
 * Closer distances use earlier children (higher detail).
 *
 * VRML Syntax:
 *   LOD {
 *     range  [ 10, 20, 50 ]
 *     center 0 0 0
 *     <children>
 *   }
 *
 * If distance < range[0], use child 0
 * If range[0] <= distance < range[1], use child 1
 * etc.
 */
class QvLevelOfDetail : public QvNode {
public:
    QvLevelOfDetail();
    virtual ~QvLevelOfDetail();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_LEVEL_OF_DETAIL; }
    virtual const char* getNodeName() const { return "LOD"; }

    // Fields
    QvMFFloat range;    // Distance thresholds for LOD switching
    QvSFVec3f center;   // Center point for distance calculation, default: (0,0,0)

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

#endif // _QV_LEVEL_OF_DETAIL_H_
