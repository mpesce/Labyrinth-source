/*
 * QvMaterial.cpp - Material properties node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Field names from strings (lines 709-704):
 * - ambientColor
 * - diffuseColor
 * - specularColor
 * - emissiveColor
 * - shininess
 * - transparency
 */

#include "../../include/QvMaterial.h"

// Static field data
QvFieldData* QvMaterial::fieldData = NULL;

/*
 * QvMaterial constructor
 */
QvMaterial::QvMaterial()
{
    // Initialize to default material values
    // Default: gray diffuse material

    // Set up field data on first instantiation
    if (fieldData == NULL) {
        fieldData = new QvFieldData;
        fieldData->addField(&ambientColor, "ambientColor");
        fieldData->addField(&diffuseColor, "diffuseColor");
        fieldData->addField(&specularColor, "specularColor");
        fieldData->addField(&emissiveColor, "emissiveColor");
        fieldData->addField(&shininess, "shininess");
        fieldData->addField(&transparency, "transparency");
    }
}

/*
 * QvMaterial destructor
 */
QvMaterial::~QvMaterial()
{
}

/*
 * Get field data for this node type
 */
QvFieldData* QvMaterial::getFieldData()
{
    return fieldData;
}

/*
 * Get class field data (static)
 */
QvFieldData* QvMaterial::getClassFieldData()
{
    return fieldData;
}

/*
 * Scene graph traversal
 *
 * This sets the current material properties for subsequent geometry.
 */
void QvMaterial::traverse(void* action)
{
    // TODO: Set material properties in rendering state
    //
    // This would:
    // 1. Get current rendering state/action
    // 2. Set material properties:
    //    - Ambient color (reflected ambient light)
    //    - Diffuse color (main surface color)
    //    - Specular color (highlights)
    //    - Emissive color (self-illumination)
    //    - Shininess (specular exponent, 0-1)
    //    - Transparency (0=opaque, 1=transparent)
    //
    // RenderMorphics Reality Lab would use these for lighting calculations.
    //
    // Note: Material fields are multi-value (MFVec3f, MFFloat)
    // Multiple materials can be specified and bound to geometry
    // using MaterialBinding node (PER_VERTEX, PER_FACE, etc.)
}
