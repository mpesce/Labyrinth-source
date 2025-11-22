/*
 * QvTranslation.cpp - Translation transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvTranslation.h"

// Static field data
QvFieldData* QvTranslation::fieldData = NULL;

/*
 * QvTranslation constructor
 */
QvTranslation::QvTranslation()
{
    // Initialize to default (no translation)
    translation.value.x = 0.0f;
    translation.value.y = 0.0f;
    translation.value.z = 0.0f;

    // Set up field data on first instantiation
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&translation, "translation");
    }
}

/*
 * QvTranslation destructor
 */
QvTranslation::~QvTranslation()
{
}

/*
 * Get field data for this node type
 */
QvFieldData* QvTranslation::getFieldData()
{
    return fieldData;
}

/*
 * Get class field data (static)
 */
QvFieldData* QvTranslation::getClassFieldData()
{
    return fieldData;
}

/*
 * Scene graph traversal
 *
 * This modifies the current transformation matrix.
 */
void QvTranslation::traverse(void* action)
{
    // TODO: Apply translation to current transformation matrix
    //
    // This would:
    // 1. Get current transformation matrix from rendering state
    // 2. Create translation matrix:
    //    [ 1  0  0  tx ]
    //    [ 0  1  0  ty ]
    //    [ 0  0  1  tz ]
    //    [ 0  0  0  1  ]
    // 3. Multiply: newMatrix = currentMatrix * translationMatrix
    // 4. Update rendering state
}
