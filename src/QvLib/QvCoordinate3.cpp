/*
 * QvCoordinate3.cpp - 3D coordinate array node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvCoordinate3.h"

QvFieldData* QvCoordinate3::fieldData = NULL;

QvCoordinate3::QvCoordinate3()
{
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&point, "point");
    }
}

QvCoordinate3::~QvCoordinate3()
{
}

QvFieldData* QvCoordinate3::getFieldData()
{
    return fieldData;
}

QvFieldData* QvCoordinate3::getClassFieldData()
{
    return fieldData;
}

void QvCoordinate3::traverse(void* action)
{
    // Set current coordinate array for subsequent geometry nodes
}
