/*
 * QvMaterial.h - Material properties node
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Defines surface material properties for subsequent geometry.
 */

#ifndef _QV_MATERIAL_H_
#define _QV_MATERIAL_H_

#include "QvNode.h"
#include "QvFields.h"

/*
 * QvMaterial - Surface material definition
 *
 * Field references from strings (lines 709-704):
 * - ambientColor
 * - diffuseColor
 * - specularColor
 * - emissiveColor
 * - shininess
 * - transparency
 *
 * VRML Syntax:
 *   Material {
 *     ambientColor   [ 0.2 0.2 0.2 ]
 *     diffuseColor   [ 0.8 0.8 0.8 ]
 *     specularColor  [ 0 0 0 ]
 *     emissiveColor  [ 0 0 0 ]
 *     shininess      [ 0.2 ]
 *     transparency   [ 0 ]
 *   }
 */
class QvMaterial : public QvNode {
public:
    QvMaterial();
    virtual ~QvMaterial();

    // Type identification
    virtual QvNodeType getNodeType() const { return QV_MATERIAL; }
    virtual const char* getNodeName() const { return "Material"; }

    // Fields - RGB color values (0.0 - 1.0)
    QvMFVec3f ambientColor;    // Ambient reflection color
    QvMFVec3f diffuseColor;    // Diffuse reflection color
    QvMFVec3f specularColor;   // Specular highlight color
    QvMFVec3f emissiveColor;   // Emissive (glow) color
    QvMFFloat shininess;       // Specular exponent (0.0 - 1.0)
    QvMFFloat transparency;    // Transparency (0.0=opaque, 1.0=transparent)

    // Field definitions
    virtual QvFieldData* getFieldData();
    static QvFieldData* getClassFieldData();

    // Scene graph traversal
    virtual void traverse(void* action);

protected:
    static QvFieldData* fieldData;
};

#endif // _QV_MATERIAL_H_
