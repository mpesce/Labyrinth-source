/*
 * QvTextureCoordinate2.cpp - 2D texture coordinate array node
 */

#include "../../include/QvTextureCoordinate2.h"

QvFieldData* QvTextureCoordinate2::fieldData = NULL;

QvTextureCoordinate2::QvTextureCoordinate2()
{
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&point, "point");
    }
}

QvTextureCoordinate2::~QvTextureCoordinate2()
{
}

QvFieldData* QvTextureCoordinate2::getFieldData()
{
    return fieldData;
}

QvFieldData* QvTextureCoordinate2::getClassFieldData()
{
    return fieldData;
}

void QvTextureCoordinate2::traverse(void* action)
{
    // Set current texture coordinate array
}
