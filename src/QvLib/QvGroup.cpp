/*
 * QvGroup.cpp - Basic grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvGroup.h"
#include <stdlib.h>

QvFieldData* QvGroup::fieldData = NULL;

QvGroup::QvGroup()
{
    children = NULL;
    numChildren = 0;
    maxChildren = 0;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
    }
}

QvGroup::~QvGroup()
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

QvFieldData* QvGroup::getFieldData()
{
    return fieldData;
}

QvFieldData* QvGroup::getClassFieldData()
{
    return fieldData;
}

void QvGroup::addChild(QvNode* child)
{
    if (child == NULL) return;

    if (numChildren >= maxChildren) {
        maxChildren = (maxChildren == 0) ? 4 : maxChildren * 2;
        children = (QvNode**)realloc(children, maxChildren * sizeof(QvNode*));
    }

    children[numChildren++] = child;
    child->ref();
}

void QvGroup::removeChild(int index)
{
    if (index < 0 || index >= numChildren) return;

    children[index]->unref();
    for (int i = index; i < numChildren - 1; i++) {
        children[i] = children[i + 1];
    }
    numChildren--;
}

QvNode* QvGroup::getChild(int index)
{
    if (index < 0 || index >= numChildren) return NULL;
    return children[index];
}

int QvGroup::getNumChildren()
{
    return numChildren;
}

void QvGroup::traverse(void* action)
{
    // Traverse all children without state isolation
    // Properties set inside this Group affect nodes outside
}
