/*
 * QvShapeHints.cpp - Shape rendering hints node
 */

#include "../../include/QvShapeHints.h"

QvFieldData* QvShapeHints::fieldData = NULL;

QvShapeHints::QvShapeHints()
{
    vertexOrdering.value = UNKNOWN_ORDERING;
    shapeType.value = UNKNOWN_SHAPE_TYPE;
    faceType.value = CONVEX;
    creaseAngle.value = 0.5f;

    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&vertexOrdering, "vertexOrdering");
        fieldData->addField(&shapeType, "shapeType");
        fieldData->addField(&faceType, "faceType");
        fieldData->addField(&creaseAngle, "creaseAngle");
    }
}

QvShapeHints::~QvShapeHints()
{
}

QvFieldData* QvShapeHints::getFieldData()
{
    return fieldData;
}

QvFieldData* QvShapeHints::getClassFieldData()
{
    return fieldData;
}

void QvShapeHints::traverse(void* action)
{
    // Set shape hints for rendering optimization
}
