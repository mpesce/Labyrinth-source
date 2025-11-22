/*
 * QvMaterialBinding.cpp - Material binding mode node
 */

#include "../../include/QvMaterialBinding.h"

QvFieldData* QvMaterialBinding::fieldData = NULL;

QvMaterialBinding::QvMaterialBinding()
{
    value.value = DEFAULT;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&value, "value");
    }
}

QvMaterialBinding::~QvMaterialBinding()
{
}

QvFieldData* QvMaterialBinding::getFieldData()
{
    return fieldData;
}

QvFieldData* QvMaterialBinding::getClassFieldData()
{
    return fieldData;
}

void QvMaterialBinding::traverse(void* action)
{
    // Set material binding mode for subsequent geometry
}
