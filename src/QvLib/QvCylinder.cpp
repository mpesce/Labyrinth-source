/*
 * QvCylinder.cpp - Cylinder geometry primitive
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvCylinder.h"

QvFieldData* QvCylinder::fieldData = NULL;

QvCylinder::QvCylinder()
{
    parts.value = ALL;
    radius.value = 1.0f;
    height.value = 2.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&parts, "parts");
        fieldData->addField(&radius, "radius");
        fieldData->addField(&height, "height");
    }
}

QvCylinder::~QvCylinder()
{
}

QvFieldData* QvCylinder::getFieldData()
{
    return fieldData;
}

QvFieldData* QvCylinder::getClassFieldData()
{
    return fieldData;
}

void QvCylinder::traverse(void* action)
{
    // Generate cylinder mesh with specified parts (SIDES, TOP, BOTTOM)
}
