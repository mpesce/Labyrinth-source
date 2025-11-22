/*
 * QvMatrixTransform.cpp - Matrix transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvMatrixTransform.h"

QvFieldData* QvMatrixTransform::fieldData = NULL;

QvMatrixTransform::QvMatrixTransform()
{
    // Matrix initialized to identity by QvSFMatrix constructor

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&matrix, "matrix");
    }
}

QvMatrixTransform::~QvMatrixTransform()
{
}

QvFieldData* QvMatrixTransform::getFieldData()
{
    return fieldData;
}

QvFieldData* QvMatrixTransform::getClassFieldData()
{
    return fieldData;
}

void QvMatrixTransform::traverse(void* action)
{
    // Apply arbitrary 4x4 transformation matrix
}
