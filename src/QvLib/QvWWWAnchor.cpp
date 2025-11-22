/*
 * QvWWWAnchor.cpp - WWW hyperlink anchor node
 */

#include "../../include/QvWWWAnchor.h"
#include <stdlib.h>

QvFieldData* QvWWWAnchor::fieldData = NULL;

QvWWWAnchor::QvWWWAnchor()
{
    children = NULL;
    numChildren = 0;
    maxChildren = 0;
    map.value = NONE;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&name, "name");
        fieldData->addField(&description, "description");
        fieldData->addField(&map, "map");
    }
}

QvWWWAnchor::~QvWWWAnchor()
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

QvFieldData* QvWWWAnchor::getFieldData()
{
    return fieldData;
}

QvFieldData* QvWWWAnchor::getClassFieldData()
{
    return fieldData;
}

void QvWWWAnchor::addChild(QvNode* child)
{
    if (child == NULL) return;

    if (numChildren >= maxChildren) {
        maxChildren = (maxChildren == 0) ? 4 : maxChildren * 2;
        children = (QvNode**)realloc(children, maxChildren * sizeof(QvNode*));
    }

    children[numChildren++] = child;
    child->ref();
}

void QvWWWAnchor::removeChild(int index)
{
    if (index < 0 || index >= numChildren) return;

    children[index]->unref();
    for (int i = index; i < numChildren - 1; i++) {
        children[i] = children[i + 1];
    }
    numChildren--;
}

QvNode* QvWWWAnchor::getChild(int index)
{
    if (index < 0 || index >= numChildren) return NULL;
    return children[index];
}

int QvWWWAnchor::getNumChildren()
{
    return numChildren;
}

void QvWWWAnchor::traverse(void* action)
{
    // Render children as clickable link
}
