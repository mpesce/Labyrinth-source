/*
 * QvInfo.cpp - Information/comment node
 */

#include "../../include/QvInfo.h"

QvFieldData* QvInfo::fieldData = NULL;

QvInfo::QvInfo()
{
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&string, "string");
    }
}

QvInfo::~QvInfo()
{
}

QvFieldData* QvInfo::getFieldData()
{
    return fieldData;
}

QvFieldData* QvInfo::getClassFieldData()
{
    return fieldData;
}

void QvInfo::traverse(void* action)
{
    // Info nodes have no visual representation
}
