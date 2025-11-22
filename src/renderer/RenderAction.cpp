/*
 * RenderAction.cpp - Scene graph traversal implementation
 *
 * Part of Labyrinth VRML Browser - Modern Port
 */

#include "../../include/renderer/RenderAction.h"
#include "../../include/QvSeparator.h"
#include "../../include/QvGroup.h"
#include "../../include/QvTransform.h"
#include "../../include/QvRotation.h"
#include "../../include/QvTranslation.h"
#include "../../include/QvScale.h"
#include "../../include/QvMaterial.h"
#include "../../include/QvSphere.h"
#include "../../include/QvCube.h"
#include "../../include/QvCone.h"
#include "../../include/QvCylinder.h"
#include "../../include/QvIndexedFaceSet.h"
#include "../../include/QvCoordinate3.h"
#include <cmath>
#include <cstring>

/* RenderState implementation */
RenderState::RenderState()
{
    modelMatrix = new Matrix4();
    modelMatrix->identity();

    diffuseColor = new Color(0.8f, 0.8f, 0.8f);
    ambientColor = new Color(0.2f, 0.2f, 0.2f);
    specularColor = new Color(0.0f, 0.0f, 0.0f);
    emissiveColor = new Color(0.0f, 0.0f, 0.0f);

    shininess = 0.2f;
    transparency = 0.0f;

    textureId = 0;
    textureEnabled = false;
    lightingEnabled = true;

    vertexOrdering = 0;  /* UNKNOWN */
    shapeType = 0;       /* UNKNOWN */
    faceType = 0;        /* UNKNOWN */
}

RenderState::~RenderState()
{
    delete modelMatrix;
    delete diffuseColor;
    delete ambientColor;
    delete specularColor;
    delete emissiveColor;
}

RenderState* RenderState::copy() const
{
    RenderState* newState = new RenderState();

    memcpy(newState->modelMatrix->m, modelMatrix->m, sizeof(float) * 16);
    *newState->diffuseColor = *diffuseColor;
    *newState->ambientColor = *ambientColor;
    *newState->specularColor = *specularColor;
    *newState->emissiveColor = *emissiveColor;

    newState->shininess = shininess;
    newState->transparency = transparency;
    newState->textureId = textureId;
    newState->textureEnabled = textureEnabled;
    newState->lightingEnabled = lightingEnabled;

    newState->vertexOrdering = vertexOrdering;
    newState->shapeType = shapeType;
    newState->faceType = faceType;

    return newState;
}

/* Matrix4 implementation */
Matrix4::Matrix4()
{
    identity();
}

void Matrix4::identity()
{
    for (int i = 0; i < 16; i++) {
        m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

void Matrix4::multiply(const Matrix4& other)
{
    float result[16];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i * 4 + j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i * 4 + j] += m[i * 4 + k] * other.m[k * 4 + j];
            }
        }
    }

    memcpy(m, result, sizeof(float) * 16);
}

void Matrix4::translate(float x, float y, float z)
{
    Matrix4 trans = makeTranslation(x, y, z);
    multiply(trans);
}

void Matrix4::rotate(float angle, float x, float y, float z)
{
    Matrix4 rot = makeRotation(angle, x, y, z);
    multiply(rot);
}

void Matrix4::scale(float x, float y, float z)
{
    Matrix4 scl = makeScale(x, y, z);
    multiply(scl);
}

Matrix4 Matrix4::makeIdentity()
{
    Matrix4 mat;
    mat.identity();
    return mat;
}

Matrix4 Matrix4::makeTranslation(float x, float y, float z)
{
    Matrix4 mat;
    mat.identity();
    mat.m[12] = x;
    mat.m[13] = y;
    mat.m[14] = z;
    return mat;
}

Matrix4 Matrix4::makeRotation(float angle, float x, float y, float z)
{
    Matrix4 mat;
    float c = cosf(angle);
    float s = sinf(angle);
    float t = 1.0f - c;

    /* Normalize axis */
    float len = sqrtf(x * x + y * y + z * z);
    if (len > 0.0f) {
        x /= len;
        y /= len;
        z /= len;
    }

    mat.m[0] = t * x * x + c;
    mat.m[1] = t * x * y + s * z;
    mat.m[2] = t * x * z - s * y;
    mat.m[3] = 0;

    mat.m[4] = t * x * y - s * z;
    mat.m[5] = t * y * y + c;
    mat.m[6] = t * y * z + s * x;
    mat.m[7] = 0;

    mat.m[8] = t * x * z + s * y;
    mat.m[9] = t * y * z - s * x;
    mat.m[10] = t * z * z + c;
    mat.m[11] = 0;

    mat.m[12] = 0;
    mat.m[13] = 0;
    mat.m[14] = 0;
    mat.m[15] = 1;

    return mat;
}

Matrix4 Matrix4::makeScale(float x, float y, float z)
{
    Matrix4 mat;
    mat.identity();
    mat.m[0] = x;
    mat.m[5] = y;
    mat.m[10] = z;
    return mat;
}

/* RenderAction implementation */
RenderAction::RenderAction()
{
    currentState = new RenderState();

    /* Initialize callbacks to NULL */
    beginSeparator = NULL;
    endSeparator = NULL;
    setTransform = NULL;
    setMaterial = NULL;
    drawSphere = NULL;
    drawCube = NULL;
    drawCone = NULL;
    drawCylinder = NULL;
    drawIndexedFaceSet = NULL;
    drawIndexedLineSet = NULL;
    setLight = NULL;
    setCamera = NULL;
    userData = NULL;
}

RenderAction::~RenderAction()
{
    delete currentState;

    /* Clean up state stack */
    while (!stateStack.empty()) {
        delete stateStack.top();
        stateStack.pop();
    }
}

void RenderAction::apply(QvNode* root)
{
    if (root == NULL) {
        return;
    }

    traverseNode(root);
}

void RenderAction::pushState()
{
    stateStack.push(currentState->copy());
}

void RenderAction::popState()
{
    if (!stateStack.empty()) {
        delete currentState;
        currentState = stateStack.top();
        stateStack.pop();
    }
}

RenderState* RenderAction::getCurrentState()
{
    return currentState;
}

void RenderAction::traverseNode(QvNode* node)
{
    if (node == NULL) {
        return;
    }

    /* Dispatch based on node type */
    const char* nodeType = node->getNodeName();

    if (strcmp(nodeType, "Separator") == 0 ||
        strcmp(nodeType, "TransformSeparator") == 0) {
        traverseSeparator(node);
    } else if (strcmp(nodeType, "Group") == 0 ||
               strcmp(nodeType, "Switch") == 0) {
        traverseGroup(node);
    } else if (strcmp(nodeType, "Transform") == 0 ||
               strcmp(nodeType, "Rotation") == 0 ||
               strcmp(nodeType, "Translation") == 0 ||
               strcmp(nodeType, "Scale") == 0) {
        traverseTransform(node);
    } else if (strcmp(nodeType, "Material") == 0) {
        traverseMaterial(node);
    } else if (strcmp(nodeType, "Sphere") == 0 ||
               strcmp(nodeType, "Cube") == 0 ||
               strcmp(nodeType, "Cone") == 0 ||
               strcmp(nodeType, "Cylinder") == 0 ||
               strcmp(nodeType, "IndexedFaceSet") == 0) {
        traverseGeometry(node);
    }
    /* TODO: Handle other node types (lights, cameras, etc.) */
}

void RenderAction::traverseSeparator(QvNode* node)
{
    /* Push state */
    pushState();

    if (beginSeparator) {
        beginSeparator(userData);
    }

    /* Traverse children */
    QvGroup* group = (QvGroup*)node;
    for (int i = 0; i < group->getNumChildren(); i++) {
        traverseNode(group->getChild(i));
    }

    if (endSeparator) {
        endSeparator(userData);
    }

    /* Pop state */
    popState();
}

void RenderAction::traverseGroup(QvNode* node)
{
    /* Groups don't isolate state, just traverse children */
    QvGroup* group = (QvGroup*)node;
    for (int i = 0; i < group->getNumChildren(); i++) {
        traverseNode(group->getChild(i));
    }
}

void RenderAction::traverseTransform(QvNode* node)
{
    const char* nodeType = node->getNodeName();

    /* Apply transform to current matrix */
    /* Note: This is simplified - real implementation would read fields */

    if (setTransform) {
        setTransform(currentState->modelMatrix, userData);
    }
}

void RenderAction::traverseMaterial(QvNode* node)
{
    /* Update material state */
    /* Note: This is simplified - real implementation would read fields */

    if (setMaterial) {
        setMaterial(currentState, userData);
    }
}

void RenderAction::traverseGeometry(QvNode* node)
{
    const char* nodeType = node->getNodeName();

    if (strcmp(nodeType, "Sphere") == 0) {
        if (drawSphere) {
            drawSphere(1.0f, userData);  /* Default radius */
        }
    } else if (strcmp(nodeType, "Cube") == 0) {
        if (drawCube) {
            drawCube(2.0f, 2.0f, 2.0f, userData);  /* Default size */
        }
    } else if (strcmp(nodeType, "Cone") == 0) {
        if (drawCone) {
            drawCone(1.0f, 2.0f, userData);  /* Default size */
        }
    } else if (strcmp(nodeType, "Cylinder") == 0) {
        if (drawCylinder) {
            drawCylinder(1.0f, 2.0f, userData);  /* Default size */
        }
    } else if (strcmp(nodeType, "IndexedFaceSet") == 0) {
        /* TODO: Extract coordinate and index data from fields */
        if (drawIndexedFaceSet) {
            /* drawIndexedFaceSet(...) */
        }
    }
}
