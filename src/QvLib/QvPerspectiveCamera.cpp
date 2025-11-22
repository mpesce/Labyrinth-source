/*
 * QvPerspectiveCamera.cpp - Perspective camera node
 */

#include "../../include/QvPerspectiveCamera.h"

QvFieldData* QvPerspectiveCamera::fieldData = NULL;

QvPerspectiveCamera::QvPerspectiveCamera()
{
    position.value.x = position.value.y = 0.0f;
    position.value.z = 1.0f;
    orientation.value.x = orientation.value.y = 0.0f;
    orientation.value.z = 1.0f;
    orientation.value.angle = 0.0f;
    focalDistance.value = 5.0f;
    heightAngle.value = 0.785398f;  // 45 degrees

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&position, "position");
        fieldData->addField(&orientation, "orientation");
        fieldData->addField(&focalDistance, "focalDistance");
        fieldData->addField(&heightAngle, "heightAngle");
    }
}

QvPerspectiveCamera::~QvPerspectiveCamera()
{
}

QvFieldData* QvPerspectiveCamera::getFieldData()
{
    return fieldData;
}

QvFieldData* QvPerspectiveCamera::getClassFieldData()
{
    return fieldData;
}

void QvPerspectiveCamera::traverse(void* action)
{
    // Set perspective projection camera
}
