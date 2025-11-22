/*
 * QvTransform.cpp - General transformation node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvTransform.h"

QvFieldData* QvTransform::fieldData = NULL;

QvTransform::QvTransform()
{
    translation.value.x = translation.value.y = translation.value.z = 0.0f;
    rotation.value.x = rotation.value.y = 0.0f;
    rotation.value.z = 1.0f;
    rotation.value.angle = 0.0f;
    scaleFactor.value.x = scaleFactor.value.y = scaleFactor.value.z = 1.0f;
    scaleOrientation.value.x = scaleOrientation.value.y = 0.0f;
    scaleOrientation.value.z = 1.0f;
    scaleOrientation.value.angle = 0.0f;
    center.value.x = center.value.y = center.value.z = 0.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&translation, "translation");
        fieldData->addField(&rotation, "rotation");
        fieldData->addField(&scaleFactor, "scaleFactor");
        fieldData->addField(&scaleOrientation, "scaleOrientation");
        fieldData->addField(&center, "center");
    }
}

QvTransform::~QvTransform()
{
}

QvFieldData* QvTransform::getFieldData()
{
    return fieldData;
}

QvFieldData* QvTransform::getClassFieldData()
{
    return fieldData;
}

void QvTransform::traverse(void* action)
{
    // Apply full transformation: scale -> scaleOrientation -> rotation -> translation
    // All around center point
}
