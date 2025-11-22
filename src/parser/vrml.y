/*
 * vrml.y - YACC grammar for VRML 1.0
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 *
 * Source: E:\labsrc\parser\VRML.Y
 * Parser: PC-YACC (referenced by "pcyacc internal stack overflow" string)
 *
 * VRML 1.0 Specification Grammar
 * See: http://www.eit.com/vrml/vrmlspec.html
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/QvNode.h"
#include "../include/QvSeparator.h"
#include "../include/QvGroup.h"
#include "../include/QvSwitch.h"
#include "../include/QvLevelOfDetail.h"
#include "../include/QvTransformSeparator.h"
#include "../include/QvTransform.h"
#include "../include/QvRotation.h"
#include "../include/QvTranslation.h"
#include "../include/QvScale.h"
#include "../include/QvMatrixTransform.h"
#include "../include/QvMaterial.h"
#include "../include/QvMaterialBinding.h"
#include "../include/QvNormalBinding.h"
#include "../include/QvShapeHints.h"
#include "../include/QvTexture2.h"
#include "../include/QvTexture2Transform.h"
#include "../include/QvTextureCoordinate2.h"
#include "../include/QvCoordinate3.h"
#include "../include/QvNormal.h"
#include "../include/QvSphere.h"
#include "../include/QvCube.h"
#include "../include/QvCone.h"
#include "../include/QvCylinder.h"
#include "../include/QvIndexedFaceSet.h"
#include "../include/QvIndexedLineSet.h"
#include "../include/QvPointSet.h"
#include "../include/QvDirectionalLight.h"
#include "../include/QvPointLight.h"
#include "../include/QvSpotLight.h"
#include "../include/QvOrthographicCamera.h"
#include "../include/QvPerspectiveCamera.h"
#include "../include/QvWWWAnchor.h"
#include "../include/QvWWWInline.h"
#include "../include/QvInfo.h"
#include "../include/QvUnknownNode.h"

/* Global root node */
static QvNode* vrml_root = NULL;

/* Name dictionary for DEF/USE */
static void* name_dict = NULL;

/* Error messages from strings analysis */
void yyerror(const char* s);
int yylex(void);

/* Forward declarations */
QvNode* create_node(const char* type);
void add_child_to_group(QvNode* parent, QvNode* child);
%}

%union {
    int             intval;
    float           floatval;
    char*           stringval;
    QvNode*         node;
}

/* Tokens from lexer */
%token HEADER           /* #VRML V1.0 ascii */
%token DEF USE
%token LBRACE RBRACE
%token LBRACKET RBRACKET
%token COMMA

/* Node type keywords */
%token SEPARATOR TRANSFORMSEPARATOR GROUP SWITCH LEVELOFDETAIL
%token TRANSFORM ROTATION TRANSLATION SCALE MATRIXTRANSFORM
%token MATERIAL MATERIALBINDING NORMALBINDING SHAPEHINTS
%token COORDINATE3 NORMAL
%token TEXTURE2 TEXTURE2TRANSFORM TEXTURECOORDINATE2
%token SPHERE CUBE CONE CYLINDER
%token INDEXEDFACESET INDEXEDLINESET POINTSET
%token DIRECTIONALLIGHT POINTLIGHT SPOTLIGHT
%token ORTHOGRAPHICCAMERA PERSPECTIVECAMERA
%token WWWANCHOR WWWINLINE
%token INFO

/* Field types */
%token <stringval> IDENTIFIER
%token <stringval> STRING
%token <intval>    INTEGER
%token <floatval>  FLOAT

/* Non-terminals */
%type <node>        vrmlScene
%type <node>        node
%type <node>        nodeGuts
%type <node>        definedNode
%type <node>        usedNode
%type <stringval>   nodeName

%%

/* Top-level VRML file */
vrmlFile:
    HEADER vrmlScene
    {
        vrml_root = $2;
    }
    ;

vrmlScene:
    /* empty */
    {
        $$ = NULL;
    }
    | vrmlScene node
    {
        if ($1 == NULL) {
            /* Create implicit root Separator */
            $$ = new QvSeparator;
        } else {
            $$ = $1;
        }
        if ($2 != NULL) {
            add_child_to_group($$, $2);
        }
    }
    ;

node:
    nodeGuts
    {
        $$ = $1;
    }
    | definedNode
    {
        $$ = $1;
    }
    | usedNode
    {
        $$ = $1;
    }
    ;

definedNode:
    DEF IDENTIFIER node
    {
        /* DEF name node - assign name and add to dictionary */
        $$ = $3;
        if ($$ != NULL) {
            $$->setName($2);
        }
        free($2);
    }
    ;

usedNode:
    USE IDENTIFIER
    {
        /* USE name - look up node in dictionary */
        /* Error: "Premature end of file after USE" */
        QvNode* existing = QvNode::findByName($2);
        if (existing != NULL) {
            existing->ref();
            $$ = existing;
        } else {
            fprintf(stderr, "Unknown USE name: %s\n", $2);
            $$ = NULL;
        }
        free($2);
    }
    ;

nodeGuts:
    nodeName LBRACE nodeBody RBRACE
    {
        $$ = create_node($1);
        free($1);
    }
    ;

nodeName:
    SEPARATOR           { $$ = strdup("Separator"); }
    | TRANSFORMSEPARATOR { $$ = strdup("TransformSeparator"); }
    | GROUP             { $$ = strdup("Group"); }
    | SWITCH            { $$ = strdup("Switch"); }
    | LEVELOFDETAIL     { $$ = strdup("LevelOfDetail"); }
    | TRANSFORM         { $$ = strdup("Transform"); }
    | ROTATION          { $$ = strdup("Rotation"); }
    | TRANSLATION       { $$ = strdup("Translation"); }
    | SCALE             { $$ = strdup("Scale"); }
    | MATRIXTRANSFORM   { $$ = strdup("MatrixTransform"); }
    | MATERIAL          { $$ = strdup("Material"); }
    | MATERIALBINDING   { $$ = strdup("MaterialBinding"); }
    | NORMALBINDING     { $$ = strdup("NormalBinding"); }
    | SHAPEHINTS        { $$ = strdup("ShapeHints"); }
    | COORDINATE3       { $$ = strdup("Coordinate3"); }
    | NORMAL            { $$ = strdup("Normal"); }
    | TEXTURE2          { $$ = strdup("Texture2"); }
    | TEXTURE2TRANSFORM { $$ = strdup("Texture2Transform"); }
    | TEXTURECOORDINATE2 { $$ = strdup("TextureCoordinate2"); }
    | SPHERE            { $$ = strdup("Sphere"); }
    | CUBE              { $$ = strdup("Cube"); }
    | CONE              { $$ = strdup("Cone"); }
    | CYLINDER          { $$ = strdup("Cylinder"); }
    | INDEXEDFACESET    { $$ = strdup("IndexedFaceSet"); }
    | INDEXEDLINESET    { $$ = strdup("IndexedLineSet"); }
    | POINTSET          { $$ = strdup("PointSet"); }
    | DIRECTIONALLIGHT  { $$ = strdup("DirectionalLight"); }
    | POINTLIGHT        { $$ = strdup("PointLight"); }
    | SPOTLIGHT         { $$ = strdup("SpotLight"); }
    | ORTHOGRAPHICCAMERA { $$ = strdup("OrthographicCamera"); }
    | PERSPECTIVECAMERA { $$ = strdup("PerspectiveCamera"); }
    | WWWANCHOR         { $$ = strdup("WWWAnchor"); }
    | WWWINLINE         { $$ = strdup("WWWInline"); }
    | INFO              { $$ = strdup("Info"); }
    | IDENTIFIER        { $$ = $1; /* Unknown node type */ }
    ;

nodeBody:
    /* empty */
    | nodeBody fieldDeclaration
    | nodeBody node
    ;

fieldDeclaration:
    IDENTIFIER fieldValue
    {
        /* Field assignments handled in QvNode::readInstance() */
        /* Error strings: "Unknown field", "Couldn't read value for field" */
        free($1);
    }
    ;

fieldValue:
    INTEGER
    | FLOAT
    | STRING
    | vec2f
    | vec3f
    | rotation
    | fieldArray
    ;

vec2f:
    FLOAT FLOAT
    ;

vec3f:
    FLOAT FLOAT FLOAT
    ;

rotation:
    FLOAT FLOAT FLOAT FLOAT
    ;

fieldArray:
    LBRACKET fieldValueList RBRACKET
    ;

fieldValueList:
    /* empty */
    | fieldValue
    | fieldValueList COMMA fieldValue
    | fieldValueList fieldValue
    ;

%%

/*
 * Error handler
 * Errors from strings analysis:
 * - "VRML error in %s(): %s"
 * - "VRML read error: %s"
 */
void yyerror(const char* s)
{
    fprintf(stderr, "VRML error: %s\n", s);
}

/*
 * Create node by type name
 * Unknown nodes become QvUnknownNode
 */
QvNode* create_node(const char* type)
{
    if (strcmp(type, "Separator") == 0) return new QvSeparator;
    if (strcmp(type, "TransformSeparator") == 0) return new QvTransformSeparator;
    if (strcmp(type, "Group") == 0) return new QvGroup;
    if (strcmp(type, "Switch") == 0) return new QvSwitch;
    if (strcmp(type, "LevelOfDetail") == 0) return new QvLevelOfDetail;
    if (strcmp(type, "Transform") == 0) return new QvTransform;
    if (strcmp(type, "Rotation") == 0) return new QvRotation;
    if (strcmp(type, "Translation") == 0) return new QvTranslation;
    if (strcmp(type, "Scale") == 0) return new QvScale;
    if (strcmp(type, "MatrixTransform") == 0) return new QvMatrixTransform;
    if (strcmp(type, "Material") == 0) return new QvMaterial;
    if (strcmp(type, "MaterialBinding") == 0) return new QvMaterialBinding;
    if (strcmp(type, "NormalBinding") == 0) return new QvNormalBinding;
    if (strcmp(type, "ShapeHints") == 0) return new QvShapeHints;
    if (strcmp(type, "Coordinate3") == 0) return new QvCoordinate3;
    if (strcmp(type, "Normal") == 0) return new QvNormal;
    if (strcmp(type, "Texture2") == 0) return new QvTexture2;
    if (strcmp(type, "Texture2Transform") == 0) return new QvTexture2Transform;
    if (strcmp(type, "TextureCoordinate2") == 0) return new QvTextureCoordinate2;
    if (strcmp(type, "Sphere") == 0) return new QvSphere;
    if (strcmp(type, "Cube") == 0) return new QvCube;
    if (strcmp(type, "Cone") == 0) return new QvCone;
    if (strcmp(type, "Cylinder") == 0) return new QvCylinder;
    if (strcmp(type, "IndexedFaceSet") == 0) return new QvIndexedFaceSet;
    if (strcmp(type, "IndexedLineSet") == 0) return new QvIndexedLineSet;
    if (strcmp(type, "PointSet") == 0) return new QvPointSet;
    if (strcmp(type, "DirectionalLight") == 0) return new QvDirectionalLight;
    if (strcmp(type, "PointLight") == 0) return new QvPointLight;
    if (strcmp(type, "SpotLight") == 0) return new QvSpotLight;
    if (strcmp(type, "OrthographicCamera") == 0) return new QvOrthographicCamera;
    if (strcmp(type, "PerspectiveCamera") == 0) return new QvPerspectiveCamera;
    if (strcmp(type, "WWWAnchor") == 0) return new QvWWWAnchor;
    if (strcmp(type, "WWWInline") == 0) return new QvWWWInline;
    if (strcmp(type, "Info") == 0) return new QvInfo;

    /* Unknown node */
    fprintf(stderr, "Unknown node type: %s\n", type);
    return new QvUnknownNode;
}

/*
 * Add child to group node
 */
void add_child_to_group(QvNode* parent, QvNode* child)
{
    if (parent == NULL || child == NULL) {
        return;
    }

    /* Use dynamic_cast or check node type */
    QvGroup* group = (QvGroup*)parent;
    if (group != NULL) {
        group->addChild(child);
    }
}

/*
 * Get parsed scene root
 */
QvNode* vrml_get_root(void)
{
    return vrml_root;
}
