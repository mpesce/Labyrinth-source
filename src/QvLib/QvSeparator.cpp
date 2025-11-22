/*
 * QvSeparator.cpp - Separator grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Referenced in strings: "renderCulling" field
 */

#include "../../include/QvSeparator.h"
#include <stdlib.h>

// Static field data
QvFieldData* QvSeparator::fieldData = NULL;

/*
 * QvSeparator constructor
 */
QvSeparator::QvSeparator() : QvGroup()
{
    // Initialize renderCulling to AUTO (0)
    renderCulling.value = 0;  // AUTO

    // Set up field data on first instantiation
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&renderCulling, "renderCulling");
    }
}

/*
 * QvSeparator destructor
 */
QvSeparator::~QvSeparator()
{
    // Parent QvGroup destructor handles children cleanup
}

/*
 * Get field data for this node type
 */
QvFieldData* QvSeparator::getFieldData()
{
    return fieldData;
}

/*
 * Get class field data (static)
 */
QvFieldData* QvSeparator::getClassFieldData()
{
    return fieldData;
}

/*
 * Scene graph traversal
 *
 * Separator isolates property changes (materials, transforms, etc.)
 * Changes made within the separator don't affect nodes outside it.
 */
void QvSeparator::traverse(void* action)
{
    // TODO: Implement traversal with state isolation
    //
    // 1. Save current rendering state (push state stack)
    //    - Transformation matrix
    //    - Material properties
    //    - Texture properties
    //    - Lighting state
    //    - etc.
    //
    // 2. Traverse all children
    //    for (int i = 0; i < numChildren; i++) {
    //        if (children[i] != NULL) {
    //            children[i]->traverse(action);
    //        }
    //    }
    //
    // 3. Restore rendering state (pop state stack)
    //
    // This ensures that property changes inside the Separator
    // don't affect geometry outside it.
}
