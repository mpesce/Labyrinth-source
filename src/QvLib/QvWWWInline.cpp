/*
 * QvWWWInline.cpp - WWW inline external file node
 */

#include "../../include/QvWWWInline.h"

QvFieldData* QvWWWInline::fieldData = NULL;

QvWWWInline::QvWWWInline()
{
    inlineRoot = NULL;
    bboxSize.value.x = bboxSize.value.y = bboxSize.value.z = 0.0f;
    bboxCenter.value.x = bboxCenter.value.y = bboxCenter.value.z = 0.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&name, "name");
        fieldData->addField(&bboxSize, "bboxSize");
        fieldData->addField(&bboxCenter, "bboxCenter");
    }
}

QvWWWInline::~QvWWWInline()
{
    unload();
}

QvFieldData* QvWWWInline::getFieldData()
{
    return fieldData;
}

QvFieldData* QvWWWInline::getClassFieldData()
{
    return fieldData;
}

void QvWWWInline::traverse(void* action)
{
    // Load and traverse inline content if not already loaded
    if (inlineRoot == NULL) {
        load();
    }
    if (inlineRoot != NULL) {
        inlineRoot->traverse(action);
    }
}

QvNode* QvWWWInline::getInlineRoot()
{
    return inlineRoot;
}

BOOL QvWWWInline::load()
{
    // TODO: Load VRML file from URL
    // Use WWW library (HTTP/FTP) to fetch file
    // Parse VRML and set inlineRoot
    return FALSE;
}

void QvWWWInline::unload()
{
    if (inlineRoot != NULL) {
        inlineRoot->unref();
        inlineRoot = NULL;
    }
}
