/*
 * QvTransformSeparator.cpp - Transform isolation grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvTransformSeparator.h"
#include <stdlib.h>

QvFieldData* QvTransformSeparator::fieldData = NULL;

QvTransformSeparator::QvTransformSeparator()
{
    children = NULL;
    numChildren = 0;
    maxChildren = 0;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
    }
}

QvTransformSeparator::~QvTransformSeparator()
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

QvFieldData* QvTransformSeparator::getFieldData()
{
    return fieldData;
}

QvFieldData* QvTransformSeparator::getClassFieldData()
{
    return fieldData;
}

void QvTransformSeparator::addChild(QvNode* child)
{
    if (child == NULL) return;

    if (numChildren >= maxChildren) {
        maxChildren = (maxChildren == 0) ? 4 : maxChildren * 2;
        children = (QvNode**)realloc(children, maxChildren * sizeof(QvNode*));
    }

    children[numChildren++] = child;
    child->ref();
}

void QvTransformSeparator::removeChild(int index)
{
    if (index < 0 || index >= numChildren) return;

    children[index]->unref();
    for (int i = index; i < numChildren - 1; i++) {
        children[i] = children[i + 1];
    }
    numChildren--;
}

QvNode* QvTransformSeparator::getChild(int index)
{
    if (index < 0 || index >= numChildren) return NULL;
    return children[index];
}

int QvTransformSeparator::getNumChildren()
{
    return numChildren;
}

void QvTransformSeparator::traverse(void* action)
{
    // Save/restore transformation matrix only
    // Material, texture, and other properties pass through
}
