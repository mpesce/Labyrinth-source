/*
 * QvPointSet.cpp - Point cloud geometry
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvPointSet.h"

QvFieldData* QvPointSet::fieldData = NULL;

QvPointSet::QvPointSet()
{
    startIndex.value = 0;
    numPoints.value = -1;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&startIndex, "startIndex");
        fieldData->addField(&numPoints, "numPoints");
    }
}

QvPointSet::~QvPointSet()
{
}

QvFieldData* QvPointSet::getFieldData()
{
    return fieldData;
}

QvFieldData* QvPointSet::getClassFieldData()
{
    return fieldData;
}

void QvPointSet::traverse(void* action)
{
    // Render points from Coordinate3 array starting at startIndex
}
