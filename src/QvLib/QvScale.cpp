/*
 * QvScale.cpp - Scale transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvScale.h"

QvFieldData* QvScale::fieldData = NULL;

QvScale::QvScale()
{
    scaleFactor.value.x = 1.0f;
    scaleFactor.value.y = 1.0f;
    scaleFactor.value.z = 1.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&scaleFactor, "scaleFactor");
    }
}

QvScale::~QvScale()
{
}

QvFieldData* QvScale::getFieldData()
{
    return fieldData;
}

QvFieldData* QvScale::getClassFieldData()
{
    return fieldData;
}

void QvScale::traverse(void* action)
{
    // Apply non-uniform scale transformation
}
