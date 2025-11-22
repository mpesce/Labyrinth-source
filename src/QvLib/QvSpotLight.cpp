/*
 * QvSpotLight.cpp - Spotlight source node
 */

#include "../../include/QvSpotLight.h"

QvFieldData* QvSpotLight::fieldData = NULL;

QvSpotLight::QvSpotLight()
{
    on.value = TRUE;
    intensity.value = 1.0f;
    color.value.x = color.value.y = color.value.z = 1.0f;
    location.value.x = location.value.y = 0.0f;
    location.value.z = 1.0f;
    direction.value.x = direction.value.y = 0.0f;
    direction.value.z = -1.0f;
    dropOffRate.value = 0.0f;
    cutOffAngle.value = 0.785398f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&on, "on");
        fieldData->addField(&intensity, "intensity");
        fieldData->addField(&color, "color");
        fieldData->addField(&location, "location");
        fieldData->addField(&direction, "direction");
        fieldData->addField(&dropOffRate, "dropOffRate");
        fieldData->addField(&cutOffAngle, "cutOffAngle");
    }
}

QvSpotLight::~QvSpotLight()
{
}

QvFieldData* QvSpotLight::getFieldData()
{
    return fieldData;
}

QvFieldData* QvSpotLight::getClassFieldData()
{
    return fieldData;
}

void QvSpotLight::traverse(void* action)
{
    // Add spotlight to scene
}
