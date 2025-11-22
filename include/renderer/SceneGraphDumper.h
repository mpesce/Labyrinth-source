/*
 * SceneGraphDumper.h - Text-based scene graph renderer
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Modernized 2025
 *
 * Dumps the scene graph structure to stdout for debugging/testing
 */

#ifndef SCENE_GRAPH_DUMPER_H
#define SCENE_GRAPH_DUMPER_H

#include "../QvNode.h"

/* Forward declarations */
class QvGroup;
class QvField;

/*
 * SceneGraphDumper
 * Traverses scene graph and prints structure and field values
 */
class SceneGraphDumper {
public:
    SceneGraphDumper();
    ~SceneGraphDumper();

    /* Dump entire scene graph to stdout */
    void dump(QvNode* root);

    /* Set indentation level */
    void setIndent(int level) { indentLevel = level; }

    /* Enable/disable field value printing */
    void setShowFields(bool show) { showFields = show; }

    /* Enable/disable statistics */
    void setShowStats(bool show) { showStats = show; }

private:
    int indentLevel;
    bool showFields;
    bool showStats;

    /* Statistics */
    int nodeCount;
    int groupCount;
    int geometryCount;
    int propertyCount;
    int transformCount;

    /* Recursive dump */
    void dumpNode(QvNode* node, int depth);
    void dumpFields(QvNode* node, int depth);
    void dumpChildren(QvGroup* group, int depth);

    /* Utility */
    void printIndent(int depth);
    void printField(const char* name, QvField* field, int depth);
    void printStats();
};

#endif /* SCENE_GRAPH_DUMPER_H */
