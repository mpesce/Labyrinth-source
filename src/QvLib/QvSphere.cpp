/*
 * QvSphere.cpp - Sphere geometry primitive
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvSphere.h"

// Static field data
QvFieldData* QvSphere::fieldData = NULL;

/*
 * QvSphere constructor
 */
QvSphere::QvSphere()
{
    // Initialize fields to default values
    radius.value = 1.0f;

    // Set up field data on first instantiation
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&radius, "radius");
    }
}

/*
 * QvSphere destructor
 */
QvSphere::~QvSphere()
{
}

/*
 * Get field data for this node type
 */
QvFieldData* QvSphere::getFieldData()
{
    return fieldData;
}

/*
 * Get class field data (static)
 */
QvFieldData* QvSphere::getClassFieldData()
{
    return fieldData;
}

/*
 * Scene graph traversal
 *
 * This is called during rendering to generate the sphere geometry.
 * The actual mesh generation would interface with RenderMorphics Reality Lab.
 */
void QvSphere::traverse(void* action)
{
    // TODO: Generate sphere mesh and pass to rendering system
    // This would call RenderMorphics Reality Lab API functions
    // to create a sphere with the specified radius.
    //
    // From analysis: "Received error %d from RLMeshAddVertex" (line 516)
    // From analysis: "Received error %d from RLMeshAddNormal" (line 517)
    //
    // Typical sphere generation:
    // - Create mesh object
    // - Generate vertices in spherical coordinates
    // - Generate triangle indices
    // - Add to scene graph
}
