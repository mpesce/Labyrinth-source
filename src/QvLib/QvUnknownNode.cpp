/*
 * QvUnknownNode.cpp - Unknown node type handler
 */

#include "../../include/QvUnknownNode.h"
#include <stdlib.h>
#include <string.h>

QvFieldData* QvUnknownNode::fieldData = NULL;

QvUnknownNode::QvUnknownNode()
{
    className = NULL;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
    }
}

QvUnknownNode::~QvUnknownNode()
{
    if (className != NULL) {
        free(className);
    }
}

QvFieldData* QvUnknownNode::getFieldData()
{
    return fieldData;
}

QvFieldData* QvUnknownNode::getClassFieldData()
{
    return fieldData;
}

void QvUnknownNode::setClassName(const char* name)
{
    if (className != NULL) {
        free(className);
    }

    if (name != NULL) {
        className = (char*)malloc(strlen(name) + 1);
        strcpy(className, name);
    } else {
        className = NULL;
    }
}

void QvUnknownNode::traverse(void* action)
{
    // Unknown nodes are ignored during traversal
}
