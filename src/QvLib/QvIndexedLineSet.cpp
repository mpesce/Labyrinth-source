/*
 * QvIndexedLineSet.cpp - Indexed line set geometry
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvIndexedLineSet.h"

QvFieldData* QvIndexedLineSet::fieldData = NULL;

QvIndexedLineSet::QvIndexedLineSet()
{
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&coordIndex, "coordIndex");
        fieldData->addField(&materialIndex, "materialIndex");
        fieldData->addField(&normalIndex, "normalIndex");
        fieldData->addField(&textureCoordIndex, "textureCoordIndex");
    }
}

QvIndexedLineSet::~QvIndexedLineSet()
{
}

QvFieldData* QvIndexedLineSet::getFieldData()
{
    return fieldData;
}

QvFieldData* QvIndexedLineSet::getClassFieldData()
{
    return fieldData;
}

void QvIndexedLineSet::traverse(void* action)
{
    // Generate polylines from current Coordinate3 using coordIndex
}
