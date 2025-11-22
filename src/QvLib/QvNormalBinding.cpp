/*
 * QvNormalBinding.cpp - Normal binding mode node
 */

#include "../../include/QvNormalBinding.h"

QvFieldData* QvNormalBinding::fieldData = NULL;

QvNormalBinding::QvNormalBinding()
{
    value.value = DEFAULT;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&value, "value");
    }
}

QvNormalBinding::~QvNormalBinding()
{
}

QvFieldData* QvNormalBinding::getFieldData()
{
    return fieldData;
}

QvFieldData* QvNormalBinding::getClassFieldData()
{
    return fieldData;
}

void QvNormalBinding::traverse(void* action)
{
    // Set normal binding mode for subsequent geometry
}
