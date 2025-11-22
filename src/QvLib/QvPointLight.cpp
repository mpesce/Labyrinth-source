/*
 * QvPointLight.cpp - Point light source node
 */

#include "../../include/QvPointLight.h"

QvFieldData* QvPointLight::fieldData = NULL;

QvPointLight::QvPointLight()
{
    on.value = TRUE;
    intensity.value = 1.0f;
    color.value.x = color.value.y = color.value.z = 1.0f;
    location.value.x = location.value.y = location.value.z = 0.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&on, "on");
        fieldData->addField(&intensity, "intensity");
        fieldData->addField(&color, "color");
        fieldData->addField(&location, "location");
    }
}

QvPointLight::~QvPointLight()
{
}

QvFieldData* QvPointLight::getFieldData()
{
    return fieldData;
}

QvFieldData* QvPointLight::getClassFieldData()
{
    return fieldData;
}

void QvPointLight::traverse(void* action)
{
    // Add point light to scene
}
