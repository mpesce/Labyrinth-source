/*
 * QvTexture2Transform.cpp - 2D texture transformation node
 */

#include "../../include/QvTexture2Transform.h"

QvFieldData* QvTexture2Transform::fieldData = NULL;

QvTexture2Transform::QvTexture2Transform()
{
    translation.value.x = translation.value.y = 0.0f;
    rotation.value = 0.0f;
    scaleFactor.value.x = scaleFactor.value.y = 1.0f;
    center.value.x = center.value.y = 0.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&translation, "translation");
        fieldData->addField(&rotation, "rotation");
        fieldData->addField(&scaleFactor, "scaleFactor");
        fieldData->addField(&center, "center");
    }
}

QvTexture2Transform::~QvTexture2Transform()
{
}

QvFieldData* QvTexture2Transform::getFieldData()
{
    return fieldData;
}

QvFieldData* QvTexture2Transform::getClassFieldData()
{
    return fieldData;
}

void QvTexture2Transform::traverse(void* action)
{
    // Apply 2D transformation to texture coordinates
}
