/*
 * QvTexture2.cpp - 2D texture node
 */

#include "../../include/QvTexture2.h"
#include <stdlib.h>

QvFieldData* QvTexture2::fieldData = NULL;

QvTexture2::QvTexture2()
{
    wrapS.value = REPEAT;
    wrapT.value = REPEAT;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&filename, "filename");
        fieldData->addField(&image, "image");
        fieldData->addField(&wrapS, "wrapS");
        fieldData->addField(&wrapT, "wrapT");
    }
}

QvTexture2::~QvTexture2()
{
}

QvFieldData* QvTexture2::getFieldData()
{
    return fieldData;
}

QvFieldData* QvTexture2::getClassFieldData()
{
    return fieldData;
}

void QvTexture2::traverse(void* action)
{
    // Load texture from filename (GIF format)
    // Set as current texture for subsequent geometry
}
