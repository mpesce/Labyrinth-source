/*
 * QvNormalBinding.h - Normal binding mode node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Specifies how normals are applied to geometry.
 */

#ifndef _QV_NORMAL_BINDING_H_
#define _QV_NORMAL_BINDING_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvNormalBinding - Normal application mode
 *
 * Binding types from strings (lines 715-710):
 * - PER_VERTEX
 * - PER_FACE
 * - PER_PART
 * - PER_VERTEX_INDEXED
 * - PER_FACE_INDEXED
 * - PER_PART_INDEXED
 *
 * VRML Syntax:
 *   NormalBinding {
 *     value  DEFAULT
 *   }
 */
class QvNormalBinding : public QvNode {
public:
    QvNormalBinding();
    virtual ~QvNormalBinding();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_NORMAL_BINDING; }
    virtual const char* getNodeName() const { return "NormalBinding"; }

    // Binding modes
    enum Binding {
        DEFAULT,
        OVERALL,
        PER_PART,
        PER_PART_INDEXED,
        PER_FACE,
        PER_FACE_INDEXED,
        PER_VERTEX,
        PER_VERTEX_INDEXED
    };

    // Fields
    QvSFEnum value;  // Binding mode, default: DEFAULT

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_NORMAL_BINDING_H_
