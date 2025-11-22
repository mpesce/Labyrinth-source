/*
 * QvDirectionalLight.cpp - Directional light source node
 */

#include "../../include/QvDirectionalLight.h"

QvFieldData* QvDirectionalLight::fieldData = NULL;

QvDirectionalLight::QvDirectionalLight()
{
    on.value = TRUE;
    intensity.value = 1.0f;
    color.value.x = color.value.y = color.value.z = 1.0f;
    direction.value.x = direction.value.y = 0.0f;
    direction.value.z = -1.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&on, "on");
        fieldData->addField(&intensity, "intensity");
        fieldData->addField(&color, "color");
        fieldData->addField(&direction, "direction");
    }
}

QvDirectionalLight::~QvDirectionalLight()
{
}

QvFieldData* QvDirectionalLight::getFieldData()
{
    return fieldData;
}

QvFieldData* QvDirectionalLight::getClassFieldData()
{
    return fieldData;
}

void QvDirectionalLight::traverse(void* action)
{
    // Add directional light to scene
}
