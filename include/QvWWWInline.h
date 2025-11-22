/*
 * QvWWWInline.h - WWW inline external file node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Loads and displays content from an external VRML file.
 */

#ifndef _QV_WWW_INLINE_H_
#define _QV_WWW_INLINE_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvWWWInline - External file reference
 *
 * Loads a VRML scene from a URL and includes it in the current scene.
 * Allows building complex worlds from multiple files.
 *
 * VRML Syntax:
 *   WWWInline {
 *     name        ""
 *     bboxSize    0 0 0
 *     bboxCenter  0 0 0
 *   }
 */
class QvWWWInline : public QvNode {
public:
    QvWWWInline();
    virtual ~QvWWWInline();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_WWW_INLINE; }
    virtual const char* getNodeName() const { return "WWWInline"; }

    // Fields
    QvSFString name;        // URL of VRML file to load
    QvSFVec3f bboxSize;     // Bounding box size hint, default: (0,0,0)
    QvSFVec3f bboxCenter;   // Bounding box center hint, default: (0,0,0)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

    // Inline content management
    QvNode* getInlineRoot();  // Returns loaded scene root
    BOOL load();              // Load file from URL
    void unload();            // Unload inline content

protected:
    QvNode* inlineRoot;  // Root of loaded scene

    static QvFieldData* fieldData;
};

#endif // _QV_WWW_INLINE_H_
