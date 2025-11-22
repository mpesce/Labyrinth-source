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
    fprintf(stderr, "DEBUG: QvGroup::addChild start, child=%p\n", child);
    fflush(stderr);
    if (child == NULL) return;

    fprintf(stderr, "DEBUG: numChildren=%d, maxChildren=%d\n", numChildren, maxChildren);
    fflush(stderr);

    if (numChildren >= maxChildren) {
        fprintf(stderr, "DEBUG: reallocating children array, children=%p\n", children);
        fflush(stderr);
        maxChildren = (maxChildren == 0) ? 4 : maxChildren * 2;
        if (children == NULL) {
            fprintf(stderr, "DEBUG: using malloc (children was NULL)\n");
            fflush(stderr);
            children = (QvNode**)malloc(maxChildren * sizeof(QvNode*));
        } else {
            fprintf(stderr, "DEBUG: using realloc\n");
            fflush(stderr);
            children = (QvNode**)realloc(children, maxChildren * sizeof(QvNode*));
        }
        fprintf(stderr, "DEBUG: allocation done, new maxChildren=%d, children=%p\n", maxChildren, children);
        fflush(stderr);
    }

    fprintf(stderr, "DEBUG: adding child to array\n");
    fflush(stderr);
    children[numChildren++] = child;
    fprintf(stderr, "DEBUG: calling child->ref()\n");
    fflush(stderr);
    child->ref();
    fprintf(stderr, "DEBUG: QvGroup::addChild end\n");
    fflush(stderr);
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
