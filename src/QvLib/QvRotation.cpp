/*
 * QvRotation.cpp - Rotation transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvRotation.h"

QvFieldData* QvRotation::fieldData = NULL;

QvRotation::QvRotation()
{
    rotation.value.x = 0.0f;
    rotation.value.y = 0.0f;
    rotation.value.z = 1.0f;
    rotation.value.angle = 0.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&rotation, "rotation");
    }
}

QvRotation::~QvRotation()
{
}

QvFieldData* QvRotation::getFieldData()
{
    return fieldData;
}

QvFieldData* QvRotation::getClassFieldData()
{
    return fieldData;
}

void QvRotation::traverse(void* action)
{
    // Apply rotation around axis (x,y,z) by angle (radians)
}
