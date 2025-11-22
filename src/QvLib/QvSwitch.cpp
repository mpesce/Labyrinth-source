/*
 * QvSwitch.cpp - Switch grouping node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvSwitch.h"
#include <stdlib.h>

QvFieldData* QvSwitch::fieldData = NULL;

QvSwitch::QvSwitch()
{
    children = NULL;
    numChildren = 0;
    maxChildren = 0;
    whichChild.value = NONE;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&whichChild, "whichChild");
    }
}

QvSwitch::~QvSwitch()
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

QvFieldData* QvSwitch::getFieldData()
{
    return fieldData;
}

QvFieldData* QvSwitch::getClassFieldData()
{
    return fieldData;
}

void QvSwitch::addChild(QvNode* child)
{
    if (child == NULL) return;

    if (numChildren >= maxChildren) {
        maxChildren = (maxChildren == 0) ? 4 : maxChildren * 2;
        children = (QvNode**)realloc(children, maxChildren * sizeof(QvNode*));
    }

    children[numChildren++] = child;
    child->ref();
}

void QvSwitch::removeChild(int index)
{
    if (index < 0 || index >= numChildren) return;

    children[index]->unref();
    for (int i = index; i < numChildren - 1; i++) {
        children[i] = children[i + 1];
    }
    numChildren--;
}

QvNode* QvSwitch::getChild(int index)
{
    if (index < 0 || index >= numChildren) return NULL;
    return children[index];
}

int QvSwitch::getNumChildren()
{
    return numChildren;
}

void QvSwitch::traverse(void* action)
{
    // Traverse only the selected child (whichChild index)
    // NONE (-1) means traverse nothing
    // ALL (-3) means traverse all children
}
