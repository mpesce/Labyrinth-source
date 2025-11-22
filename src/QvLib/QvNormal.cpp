/*
 * QvNormal.cpp - Normal vector array node
 */

#include "../../include/QvNormal.h"

QvFieldData* QvNormal::fieldData = NULL;

QvNormal::QvNormal()
{
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&vector, "vector");
    }
}

QvNormal::~QvNormal()
{
}

QvFieldData* QvNormal::getFieldData()
{
    return fieldData;
}

QvFieldData* QvNormal::getClassFieldData()
{
    return fieldData;
}

void QvNormal::traverse(void* action)
{
    // Set current normal array for subsequent geometry
}
