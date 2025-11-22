/*
 * RenderAction.h - Scene graph traversal for rendering
 *
 * Part of Labyrinth VRML Browser - Modern Port
 * Copyright (c) 1995, The Community Company
 * Modernized 2025
 *
 * Rendering abstraction layer - traverses QvNode scene graph
 * and generates rendering commands
 */

#ifndef RENDER_ACTION_H
#define RENDER_ACTION_H

#include "QvNode.h"
#include <stack>

/* Forward declarations */
class Matrix4;
class Vector3;
class Color;

/* Forward declarations */
class QvCoordinate3;
class QvNormal;
class QvTextureCoordinate2;

/*
 * Rendering state stack
 * Maintains current transform, material, texture, etc.
 */
class RenderState {
public:
    /* Transform state */
    Matrix4* modelMatrix;

    /* Material state */
    Color* diffuseColor;
    Color* ambientColor;
    Color* specularColor;
    Color* emissiveColor;
    float shininess;
    float transparency;

    /* Texture state */
    int textureId;
    bool textureEnabled;

    /* Lighting state */
    bool lightingEnabled;

    /* Shape hints */
    int vertexOrdering;    /* UNKNOWN, CLOCKWISE, COUNTERCLOCKWISE */
    int shapeType;         /* UNKNOWN, SOLID */
    int faceType;          /* UNKNOWN, CONVEX */

    /* Geometry property nodes */
    QvCoordinate3* currentCoordinates;          /* For IndexedFaceSet/IndexedLineSet */
    QvNormal* currentNormals;                   /* For smooth shading */
    QvTextureCoordinate2* currentTexCoords;     /* For texture mapping */

    RenderState();
    ~RenderState();
    RenderState* copy() const;
};

/*
 * Render action class
 * Traverses scene graph and calls renderer
 */
class RenderAction {
public:
    RenderAction();
    ~RenderAction();

    /* Apply to scene graph */
    void apply(QvNode* root);

    /* State management */
    void pushState();
    void popState();
    RenderState* getCurrentState();

    /* Callbacks for renderer (set by OpenGL renderer) */
    void (*beginSeparator)(void* userData);
    void (*endSeparator)(void* userData);
    void (*setTransform)(Matrix4* matrix, void* userData);
    void (*setMaterial)(RenderState* state, void* userData);
    void (*drawSphere)(float radius, void* userData);
    void (*drawCube)(float width, float height, float depth, void* userData);
    void (*drawCone)(float bottomRadius, float height, void* userData);
    void (*drawCylinder)(float radius, float height, void* userData);
    void (*drawIndexedFaceSet)(int* coordIndex, int numIndices,
                               float* coords, int numCoords,
                               float* normals, int numNormals,
                               float* texCoords, int numTexCoords,
                               void* userData);
    void (*drawIndexedLineSet)(int* coordIndex, int numIndices,
                               float* coords, int numCoords,
                               void* userData);
    void (*addLight)(int lightIndex, int type, float* position, float* direction,
                     float* color, float intensity, bool on, void* userData);
    void (*setCamera)(int type, float* position, float* orientation,
                      float fov, float aspectRatio, void* userData);

    void* userData;

private:
    std::stack<RenderState*> stateStack;
    RenderState* currentState;

    /* Track lights and cameras during traversal */
    int lightCount;
    bool cameraSet;

    /* Node-specific traversal methods */
    void traverseNode(QvNode* node);
    void traverseSeparator(QvNode* node);
    void traverseGroup(QvNode* node);
    void traverseTransform(QvNode* node);
    void traverseMaterial(QvNode* node);
    void traverseGeometry(QvNode* node);
    void traverseCamera(QvNode* node);
    void traverseLight(QvNode* node);
};

/* Math helper classes */
class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Color {
public:
    float r, g, b, a;

    Color() : r(1), g(1), b(1), a(1) {}
    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
};

class Matrix4 {
public:
    float m[16];  /* Column-major order for OpenGL */

    Matrix4();
    void identity();
    void multiply(const Matrix4& other);
    void translate(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    void scale(float x, float y, float z);

    static Matrix4 makeIdentity();
    static Matrix4 makeTranslation(float x, float y, float z);
    static Matrix4 makeRotation(float angle, float x, float y, float z);
    static Matrix4 makeScale(float x, float y, float z);
};

#endif /* RENDER_ACTION_H */
