/*
 * QvCube.cpp - Cube geometry primitive
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvCube.h"

// Static field data
QvFieldData* QvCube::fieldData = NULL;

/*
 * QvCube constructor
 */
QvCube::QvCube()
{
    // Initialize fields to default values
    width.value = 2.0f;
    height.value = 2.0f;
    depth.value = 2.0f;

    // Set up field data on first instantiation
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&width, "width");
        fieldData->addField(&height, "height");
        fieldData->addField(&depth, "depth");
    }
}

/*
 * QvCube destructor
 */
QvCube::~QvCube()
{
}

/*
 * Get field data for this node type
 */
QvFieldData* QvCube::getFieldData()
{
    return fieldData;
}

/*
 * Get class field data (static)
 */
QvFieldData* QvCube::getClassFieldData()
{
    return fieldData;
}

/*
 * Scene graph traversal
 *
 * This is called during rendering to generate the cube geometry.
 */
void QvCube::traverse(void* action)
{
    // TODO: Generate cube mesh
    //
    // Cube has 8 vertices, 12 triangles (2 per face)
    // Vertices at (+/-width/2, +/-height/2, +/-depth/2)
    //
    // Would interface with RenderMorphics Reality Lab:
    // - Create mesh
    // - Add 8 vertices
    // - Add 12 triangles (6 faces * 2 triangles each)
    // - Set normals for flat shading
}
