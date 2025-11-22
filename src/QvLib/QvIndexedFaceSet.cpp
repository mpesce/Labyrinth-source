/*
 * QvIndexedFaceSet.cpp - Indexed polygonal mesh geometry
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvIndexedFaceSet.h"

QvFieldData* QvIndexedFaceSet::fieldData = NULL;

QvIndexedFaceSet::QvIndexedFaceSet()
{
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&coordIndex, "coordIndex");
        fieldData->addField(&materialIndex, "materialIndex");
        fieldData->addField(&normalIndex, "normalIndex");
        fieldData->addField(&textureCoordIndex, "textureCoordIndex");
    }
}

QvIndexedFaceSet::~QvIndexedFaceSet()
{
}

QvFieldData* QvIndexedFaceSet::getFieldData()
{
    return fieldData;
}

QvFieldData* QvIndexedFaceSet::getClassFieldData()
{
    return fieldData;
}

void QvIndexedFaceSet::traverse(void* action)
{
    // Generate mesh from current Coordinate3 using coordIndex
    // Apply materials/normals/texcoords based on binding and indices
}
