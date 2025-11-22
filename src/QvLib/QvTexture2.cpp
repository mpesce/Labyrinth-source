/*
 * QvTexture2.cpp - 2D texture node
 */

#include "../../include/QvTexture2.h"

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

QvSFImage::~QvSFImage()
{
    if (data != NULL) {
        free(data);
    }
}

BOOL QvSFImage::read(QvInput* in, const char* name)
{
    // Read: width height components [pixel data]
    if (!in->read(&width) || !in->read(&height) || !in->read(&components)) {
        return FALSE;
    }

    // Read pixel data
    int numPixels = width * height * components;
    if (data != NULL) {
        free(data);
    }
    data = (unsigned char*)malloc(numPixels);

    for (int i = 0; i < numPixels; i++) {
        int val;
        if (!in->read(&val)) {
            return FALSE;
        }
        data[i] = (unsigned char)val;
    }

    return TRUE;
}
