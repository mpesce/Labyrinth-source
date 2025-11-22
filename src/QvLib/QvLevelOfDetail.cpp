/*
 * QvLevelOfDetail.cpp - Level-of-detail grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvLevelOfDetail.h"
#include <stdlib.h>

QvFieldData* QvLevelOfDetail::fieldData = NULL;

QvLevelOfDetail::QvLevelOfDetail()
{
    children = NULL;
    numChildren = 0;
    maxChildren = 0;
    center.value.x = center.value.y = center.value.z = 0.0f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&range, "range");
        fieldData->addField(&center, "center");
    }
}

QvLevelOfDetail::~QvLevelOfDetail()
{
    for (int i = 0; i < numChildren; i++) {
        if (children[i] != NULL) {
            children[i]->unref();
        }
    }
    if (children != NULL) {
        free(children);
    }
}

QvFieldData* QvLevelOfDetail::getFieldData()
{
    return fieldData;
}

QvFieldData* QvLevelOfDetail::getClassFieldData()
{
    return fieldData;
}

void QvLevelOfDetail::addChild(QvNode* child)
{
    if (child == NULL) return;

    if (numChildren >= maxChildren) {
        maxChildren = (maxChildren == 0) ? 4 : maxChildren * 2;
        children = (QvNode**)realloc(children, maxChildren * sizeof(QvNode*));
    }

    children[numChildren++] = child;
    child->ref();
}

void QvLevelOfDetail::removeChild(int index)
{
    if (index < 0 || index >= numChildren) return;

    children[index]->unref();
    for (int i = index; i < numChildren - 1; i++) {
        children[i] = children[i + 1];
    }
    numChildren--;
}

QvNode* QvLevelOfDetail::getChild(int index)
{
    if (index < 0 || index >= numChildren) return NULL;
    return children[index];
}

int QvLevelOfDetail::getNumChildren()
{
    return numChildren;
}

void QvLevelOfDetail::traverse(void* action)
{
    // Calculate distance from camera to center point
    // Select child based on distance and range values
    // Closer = earlier children (higher detail)
}
