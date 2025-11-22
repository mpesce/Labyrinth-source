/*
 * SceneGraphDumper.cpp - Text-based scene graph renderer
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Modernized 2025
 *
 * Dumps the scene graph structure to stdout for debugging/testing
 */

#include "../../include/renderer/SceneGraphDumper.h"
#include "../../include/QvGroup.h"
#include "../../include/QvFields.h"
#include <stdio.h>
#include <string.h>

SceneGraphDumper::SceneGraphDumper()
    : indentLevel(0), showFields(true), showStats(true),
      nodeCount(0), groupCount(0), geometryCount(0),
      propertyCount(0), transformCount(0)
{
}

SceneGraphDumper::~SceneGraphDumper()
{
}

void SceneGraphDumper::dump(QvNode* root)
{
    if (root == NULL) {
        printf("Scene graph is empty (NULL root)\n");
        return;
    }

    // Reset statistics
    nodeCount = 0;
    groupCount = 0;
    geometryCount = 0;
    propertyCount = 0;
    transformCount = 0;

    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║             VRML Scene Graph Structure                          ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");
    printf("\n");

    dumpNode(root, 0);

    if (showStats) {
        printStats();
    }
}

void SceneGraphDumper::dumpNode(QvNode* node, int depth)
{
    if (node == NULL) {
        return;
    }

    nodeCount++;

    printIndent(depth);

    // Print node type
    const char* nodeName = node->getNodeName();
    printf("┌─ %s", nodeName);

    // Print node name if it has one (from DEF)
    const char* defName = node->getName();
    if (defName != NULL && strlen(defName) > 0) {
        printf(" (DEF \"%s\")", defName);
    }

    printf("\n");

    // Update statistics based on node type
    QvNodeType type = node->getNodeType();
    if (type == QV_SEPARATOR || type == QV_GROUP || type == QV_TRANSFORM_SEPARATOR ||
        type == QV_SWITCH || type == QV_LEVEL_OF_DETAIL) {
        groupCount++;
    } else if (type == QV_SPHERE || type == QV_CUBE || type == QV_CONE ||
               type == QV_CYLINDER || type == QV_INDEXED_FACE_SET ||
               type == QV_INDEXED_LINE_SET || type == QV_POINT_SET) {
        geometryCount++;
    } else if (type == QV_MATERIAL || type == QV_TEXTURE2 || type == QV_NORMAL ||
               type == QV_COORDINATE3) {
        propertyCount++;
    } else if (type == QV_TRANSFORM || type == QV_ROTATION || type == QV_TRANSLATION ||
               type == QV_SCALE || type == QV_MATRIX_TRANSFORM) {
        transformCount++;
    }

    // Print fields if enabled
    if (showFields) {
        dumpFields(node, depth);
    }

    // If this is a group node, dump children
    // Check node type instead of dynamic_cast (which may not work in C-style inheritance)
    if (type == QV_GROUP || type == QV_SEPARATOR || type == QV_TRANSFORM_SEPARATOR ||
        type == QV_SWITCH || type == QV_LEVEL_OF_DETAIL) {
        QvGroup* group = (QvGroup*)node;
        dumpChildren(group, depth);
    }
}

void SceneGraphDumper::dumpFields(QvNode* node, int depth)
{
    QvFieldData* fieldData = node->getFieldData();
    if (fieldData == NULL) {
        return;
    }

    int numFields = fieldData->getNumFields();
    if (numFields == 0) {
        return;
    }

    for (int i = 0; i < numFields; i++) {
        const char* fieldName = NULL;
        QvField* field = fieldData->getField(i, &fieldName);

        if (field != NULL && fieldName != NULL) {
            printField(fieldName, field, depth + 1);
        }
    }
}

void SceneGraphDumper::printField(const char* name, QvField* field, int depth)
{
    if (field == NULL || name == NULL) {
        return;
    }

    printIndent(depth);
    printf("│  %s: ", name);

    const char* typeId = field->getTypeId();

    // Print field value based on type
    if (strcmp(typeId, "SFFloat") == 0) {
        QvSFFloat* f = (QvSFFloat*)field;
        printf("%.6g", f->value);
    }
    else if (strcmp(typeId, "SFInt32") == 0) {
        QvSFInt32* f = (QvSFInt32*)field;
        printf("%ld", f->value);
    }
    else if (strcmp(typeId, "SFBool") == 0) {
        QvSFBool* f = (QvSFBool*)field;
        printf("%s", f->value ? "TRUE" : "FALSE");
    }
    else if (strcmp(typeId, "SFString") == 0) {
        QvSFString* f = (QvSFString*)field;
        printf("\"%s\"", f->value ? f->value : "");
    }
    else if (strcmp(typeId, "SFVec2f") == 0) {
        QvSFVec2f* f = (QvSFVec2f*)field;
        printf("[ %.6g, %.6g ]", f->value.x, f->value.y);
    }
    else if (strcmp(typeId, "SFVec3f") == 0) {
        QvSFVec3f* f = (QvSFVec3f*)field;
        printf("[ %.6g, %.6g, %.6g ]", f->value.x, f->value.y, f->value.z);
    }
    else if (strcmp(typeId, "SFRotation") == 0) {
        QvSFRotation* f = (QvSFRotation*)field;
        printf("[ %.6g, %.6g, %.6g, %.6g ]",
               f->value.x, f->value.y, f->value.z, f->value.angle);
    }
    else if (strcmp(typeId, "MFFloat") == 0) {
        QvMFFloat* f = (QvMFFloat*)field;
        printf("[ ");
        for (int i = 0; i < f->num; i++) {
            printf("%.6g", f->values[i]);
            if (i < f->num - 1) printf(", ");
        }
        printf(" ] (%d values)", f->num);
    }
    else if (strcmp(typeId, "MFInt32") == 0) {
        QvMFInt32* f = (QvMFInt32*)field;
        printf("[ ");
        for (int i = 0; i < f->num && i < 10; i++) {
            printf("%ld", f->values[i]);
            if (i < f->num - 1) printf(", ");
        }
        if (f->num > 10) printf("... ");
        printf(" ] (%d values)", f->num);
    }
    else if (strcmp(typeId, "MFVec2f") == 0) {
        QvMFVec2f* f = (QvMFVec2f*)field;
        printf("[ ");
        for (int i = 0; i < f->num && i < 5; i++) {
            printf("(%.6g, %.6g)", f->values[i].x, f->values[i].y);
            if (i < f->num - 1) printf(", ");
        }
        if (f->num > 5) printf("... ");
        printf(" ] (%d vectors)", f->num);
    }
    else if (strcmp(typeId, "MFVec3f") == 0) {
        QvMFVec3f* f = (QvMFVec3f*)field;
        printf("[ ");
        for (int i = 0; i < f->num && i < 5; i++) {
            printf("(%.6g, %.6g, %.6g)",
                   f->values[i].x, f->values[i].y, f->values[i].z);
            if (i < f->num - 1) printf(", ");
        }
        if (f->num > 5) printf("... ");
        printf(" ] (%d vectors)", f->num);
    }
    else if (strcmp(typeId, "MFString") == 0) {
        QvMFString* f = (QvMFString*)field;
        printf("[ ");
        for (int i = 0; i < f->num && i < 3; i++) {
            printf("\"%s\"", f->values[i] ? f->values[i] : "");
            if (i < f->num - 1) printf(", ");
        }
        if (f->num > 3) printf("... ");
        printf(" ] (%d strings)", f->num);
    }
    else {
        printf("<%s>", typeId);
    }

    printf("\n");
}

void SceneGraphDumper::dumpChildren(QvGroup* group, int depth)
{
    int numChildren = group->getNumChildren();

    if (numChildren > 0) {
        printIndent(depth);
        printf("│  Children: %d\n", numChildren);

        for (int i = 0; i < numChildren; i++) {
            QvNode* child = group->getChild(i);
            if (child != NULL) {
                dumpNode(child, depth + 1);
            }
        }
    }
}

void SceneGraphDumper::printIndent(int depth)
{
    for (int i = 0; i < depth; i++) {
        printf("│   ");
    }
}

void SceneGraphDumper::printStats()
{
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                   Scene Graph Statistics                        ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  Total nodes:      %d\n", nodeCount);
    printf("  Group nodes:      %d\n", groupCount);
    printf("  Geometry nodes:   %d\n", geometryCount);
    printf("  Property nodes:   %d\n", propertyCount);
    printf("  Transform nodes:  %d\n", transformCount);
    printf("\n");
}
