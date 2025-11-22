/*
 * QvOrthographicCamera.cpp - Orthographic camera node
 */

#include "../../include/QvOrthographicCamera.h"

QvFieldData* QvOrthographicCamera::fieldData = NULL;

QvOrthographicCamera::QvOrthographicCamera()
{
    position.value.x = position.value.y = 0.0f;
    position.value.z = 1.0f;
    orientation.value.x = orientation.value.y = 0.0f;
    orientation.value.z = 1.0f;
    orientation.value.angle = 0.0f;
    focalDistance.value = 5.0f;
    height.value = 2.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&position, "position");
        fieldData->addField(&orientation, "orientation");
        fieldData->addField(&focalDistance, "focalDistance");
        fieldData->addField(&height, "height");
    }
}

QvOrthographicCamera::~QvOrthographicCamera()
{
}

QvFieldData* QvOrthographicCamera::getFieldData()
{
    return fieldData;
}

QvFieldData* QvOrthographicCamera::getClassFieldData()
{
    return fieldData;
}

void QvOrthographicCamera::traverse(void* action)
{
    // Set orthographic projection camera
}
