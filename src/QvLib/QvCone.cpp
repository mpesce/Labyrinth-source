/*
 * QvCone.cpp - Cone geometry primitive
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvCone.h"

QvFieldData* QvCone::fieldData = NULL;

QvCone::QvCone()
{
    parts.value = ALL;
    bottomRadius.value = 1.0f;
    height.value = 2.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&parts, "parts");
        fieldData->addField(&bottomRadius, "bottomRadius");
        fieldData->addField(&height, "height");
    }
}

QvCone::~QvCone()
{
}

QvFieldData* QvCone::getFieldData()
{
    return fieldData;
}

QvFieldData* QvCone::getClassFieldData()
{
    return fieldData;
}

void QvCone::traverse(void* action)
{
    // Generate cone mesh with specified parts (SIDES and/or BOTTOM)
}
