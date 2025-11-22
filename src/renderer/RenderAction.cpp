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
#include "../../include/QvNormal.h"
#include "../../include/QvTextureCoordinate2.h"
#include "../../include/QvPerspectiveCamera.h"
#include "../../include/QvOrthographicCamera.h"
#include "../../include/QvDirectionalLight.h"
#include "../../include/QvPointLight.h"
#include "../../include/QvSpotLight.h"
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

    /* Geometry property nodes */
    currentCoordinates = NULL;
    currentNormals = NULL;
    currentTexCoords = NULL;
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

    /* Copy geometry property nodes (just the pointers, nodes are ref-counted) */
    newState->currentCoordinates = currentCoordinates;
    newState->currentNormals = currentNormals;
    newState->currentTexCoords = currentTexCoords;

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

    /* Initialize light and camera tracking */
    lightCount = 0;
    cameraSet = false;

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
    addLight = NULL;
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
    } else if (strcmp(nodeType, "Coordinate3") == 0) {
        /* Store current coordinates for IndexedFaceSet */
        currentState->currentCoordinates = (QvCoordinate3*)node;
    } else if (strcmp(nodeType, "Normal") == 0) {
        /* Store current normals for smooth shading */
        currentState->currentNormals = (QvNormal*)node;
    } else if (strcmp(nodeType, "TextureCoordinate2") == 0) {
        /* Store current texture coordinates */
        currentState->currentTexCoords = (QvTextureCoordinate2*)node;
    } else if (strcmp(nodeType, "PerspectiveCamera") == 0 ||
               strcmp(nodeType, "OrthographicCamera") == 0) {
        traverseCamera(node);
    } else if (strcmp(nodeType, "DirectionalLight") == 0 ||
               strcmp(nodeType, "PointLight") == 0 ||
               strcmp(nodeType, "SpotLight") == 0) {
        traverseLight(node);
    } else if (strcmp(nodeType, "Sphere") == 0 ||
               strcmp(nodeType, "Cube") == 0 ||
               strcmp(nodeType, "Cone") == 0 ||
               strcmp(nodeType, "Cylinder") == 0 ||
               strcmp(nodeType, "IndexedFaceSet") == 0) {
        traverseGeometry(node);
    }
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
    QvNodeType type = node->getNodeType();

    /* Apply transform to current matrix based on node type */
    if (type == QV_TRANSFORM) {
        QvTransform* transform = (QvTransform*)node;

        /* Build compound transform matrix */
        Matrix4 mat = Matrix4::makeIdentity();

        /* Translation */
        mat.translate(transform->translation.value.x,
                     transform->translation.value.y,
                     transform->translation.value.z);

        /* Rotation around center */
        if (transform->center.value.x != 0 || transform->center.value.y != 0 || transform->center.value.z != 0) {
            mat.translate(transform->center.value.x, transform->center.value.y, transform->center.value.z);
        }

        /* Scale orientation */
        if (transform->scaleOrientation.value.angle != 0) {
            mat.rotate(transform->scaleOrientation.value.angle,
                      transform->scaleOrientation.value.x,
                      transform->scaleOrientation.value.y,
                      transform->scaleOrientation.value.z);
        }

        /* Scale */
        mat.scale(transform->scaleFactor.value.x,
                 transform->scaleFactor.value.y,
                 transform->scaleFactor.value.z);

        /* Inverse scale orientation */
        if (transform->scaleOrientation.value.angle != 0) {
            mat.rotate(-transform->scaleOrientation.value.angle,
                      transform->scaleOrientation.value.x,
                      transform->scaleOrientation.value.y,
                      transform->scaleOrientation.value.z);
        }

        /* Rotation */
        mat.rotate(transform->rotation.value.angle,
                  transform->rotation.value.x,
                  transform->rotation.value.y,
                  transform->rotation.value.z);

        /* Inverse center */
        if (transform->center.value.x != 0 || transform->center.value.y != 0 || transform->center.value.z != 0) {
            mat.translate(-transform->center.value.x, -transform->center.value.y, -transform->center.value.z);
        }

        /* Apply to current matrix */
        currentState->modelMatrix->multiply(mat);

    } else if (type == QV_ROTATION) {
        QvRotation* rotation = (QvRotation*)node;
        currentState->modelMatrix->rotate(rotation->rotation.value.angle,
                                         rotation->rotation.value.x,
                                         rotation->rotation.value.y,
                                         rotation->rotation.value.z);

    } else if (type == QV_TRANSLATION) {
        QvTranslation* translation = (QvTranslation*)node;
        currentState->modelMatrix->translate(translation->translation.value.x,
                                            translation->translation.value.y,
                                            translation->translation.value.z);

    } else if (type == QV_SCALE) {
        QvScale* scale = (QvScale*)node;
        currentState->modelMatrix->scale(scale->scaleFactor.value.x,
                                        scale->scaleFactor.value.y,
                                        scale->scaleFactor.value.z);
    }

    /* Notify renderer of transform change */
    if (setTransform) {
        setTransform(currentState->modelMatrix, userData);
    }
}

void RenderAction::traverseMaterial(QvNode* node)
{
    QvMaterial* material = (QvMaterial*)node;

    /* Update diffuse color (most important for rendering) */
    if (material->diffuseColor.num > 0) {
        currentState->diffuseColor->r = material->diffuseColor.values[0].x;
        currentState->diffuseColor->g = material->diffuseColor.values[0].y;
        currentState->diffuseColor->b = material->diffuseColor.values[0].z;
    }

    /* Update ambient color */
    if (material->ambientColor.num > 0) {
        currentState->ambientColor->r = material->ambientColor.values[0].x;
        currentState->ambientColor->g = material->ambientColor.values[0].y;
        currentState->ambientColor->b = material->ambientColor.values[0].z;
    }

    /* Update specular color */
    if (material->specularColor.num > 0) {
        currentState->specularColor->r = material->specularColor.values[0].x;
        currentState->specularColor->g = material->specularColor.values[0].y;
        currentState->specularColor->b = material->specularColor.values[0].z;
    }

    /* Update emissive color */
    if (material->emissiveColor.num > 0) {
        currentState->emissiveColor->r = material->emissiveColor.values[0].x;
        currentState->emissiveColor->g = material->emissiveColor.values[0].y;
        currentState->emissiveColor->b = material->emissiveColor.values[0].z;
    }

    /* Update shininess and transparency */
    if (material->shininess.num > 0) {
        currentState->shininess = material->shininess.values[0];
    }
    if (material->transparency.num > 0) {
        currentState->transparency = material->transparency.values[0];
    }

    /* Notify renderer of material change */
    if (setMaterial) {
        setMaterial(currentState, userData);
    }
}

void RenderAction::traverseGeometry(QvNode* node)
{
    QvNodeType type = node->getNodeType();

    if (type == QV_SPHERE) {
        QvSphere* sphere = (QvSphere*)node;
        if (drawSphere) {
            drawSphere(sphere->radius.value, userData);
        }
    } else if (type == QV_CUBE) {
        QvCube* cube = (QvCube*)node;
        if (drawCube) {
            drawCube(cube->width.value, cube->height.value, cube->depth.value, userData);
        }
    } else if (type == QV_CONE) {
        QvCone* cone = (QvCone*)node;
        if (drawCone) {
            drawCone(cone->bottomRadius.value, cone->height.value, userData);
        }
    } else if (type == QV_CYLINDER) {
        QvCylinder* cylinder = (QvCylinder*)node;
        if (drawCylinder) {
            drawCylinder(cylinder->radius.value, cylinder->height.value, userData);
        }
    } else if (type == QV_INDEXED_FACE_SET) {
        QvIndexedFaceSet* faceSet = (QvIndexedFaceSet*)node;
        if (drawIndexedFaceSet && faceSet->coordIndex.num > 0 &&
            currentState->currentCoordinates != NULL) {
            /* Get coordinate data from current Coordinate3 node */
            QvCoordinate3* coords = currentState->currentCoordinates;

            /* Get optional normals and texture coordinates */
            float* normals = NULL;
            int numNormals = 0;
            if (currentState->currentNormals != NULL) {
                normals = (float*)currentState->currentNormals->vector.values;
                numNormals = currentState->currentNormals->vector.num;
            }

            float* texCoords = NULL;
            int numTexCoords = 0;
            if (currentState->currentTexCoords != NULL) {
                texCoords = (float*)currentState->currentTexCoords->point.values;
                numTexCoords = currentState->currentTexCoords->point.num;
            }

            /* Pass coordIndex array and vertex data to renderer */
            /* Note: coordIndex.values is long*, callback expects int* */
            drawIndexedFaceSet((int*)faceSet->coordIndex.values,
                             faceSet->coordIndex.num,
                             (float*)coords->point.values,
                             coords->point.num,
                             normals,
                             numNormals,
                             texCoords,
                             numTexCoords,
                             userData);
        }
    }
}

void RenderAction::traverseCamera(QvNode* node)
{
    if (!setCamera || cameraSet) return;  /* Only use first camera */

    QvNodeType type = node->getNodeType();
    float position[3];
    float orientation[4];  /* axis-angle: x, y, z, angle */
    float fov = 0.785398f;  /* Default 45 degrees */
    float aspectRatio = 1.0f;

    if (type == QV_PERSPECTIVE_CAMERA) {
        QvPerspectiveCamera* camera = (QvPerspectiveCamera*)node;
        position[0] = camera->position.value.x;
        position[1] = camera->position.value.y;
        position[2] = camera->position.value.z;
        orientation[0] = camera->orientation.value.x;
        orientation[1] = camera->orientation.value.y;
        orientation[2] = camera->orientation.value.z;
        orientation[3] = camera->orientation.value.angle;
        fov = camera->heightAngle.value;

        setCamera(0, position, orientation, fov, aspectRatio, userData);  /* 0 = perspective */
        cameraSet = true;

    } else if (type == QV_ORTHOGRAPHIC_CAMERA) {
        QvOrthographicCamera* camera = (QvOrthographicCamera*)node;
        position[0] = camera->position.value.x;
        position[1] = camera->position.value.y;
        position[2] = camera->position.value.z;
        orientation[0] = camera->orientation.value.x;
        orientation[1] = camera->orientation.value.y;
        orientation[2] = camera->orientation.value.z;
        orientation[3] = camera->orientation.value.angle;
        fov = camera->height.value;  /* Orthographic uses height instead of angle */

        setCamera(1, position, orientation, fov, aspectRatio, userData);  /* 1 = orthographic */
        cameraSet = true;
    }
}

void RenderAction::traverseLight(QvNode* node)
{
    if (!addLight) return;

    QvNodeType type = node->getNodeType();
    float position[3] = {0, 0, 0};
    float direction[3] = {0, 0, -1};
    float color[3] = {1, 1, 1};
    float intensity = 1.0f;
    bool on = true;
    int lightType = 0;  /* 0=directional, 1=point, 2=spot */

    if (type == QV_DIRECTIONAL_LIGHT) {
        QvDirectionalLight* light = (QvDirectionalLight*)node;
        direction[0] = light->direction.value.x;
        direction[1] = light->direction.value.y;
        direction[2] = light->direction.value.z;
        color[0] = light->color.value.x;
        color[1] = light->color.value.y;
        color[2] = light->color.value.z;
        intensity = light->intensity.value;
        on = light->on.value;
        lightType = 0;

    } else if (type == QV_POINT_LIGHT) {
        QvPointLight* light = (QvPointLight*)node;
        position[0] = light->location.value.x;
        position[1] = light->location.value.y;
        position[2] = light->location.value.z;
        color[0] = light->color.value.x;
        color[1] = light->color.value.y;
        color[2] = light->color.value.z;
        intensity = light->intensity.value;
        on = light->on.value;
        lightType = 1;

    } else if (type == QV_SPOT_LIGHT) {
        QvSpotLight* light = (QvSpotLight*)node;
        position[0] = light->location.value.x;
        position[1] = light->location.value.y;
        position[2] = light->location.value.z;
        direction[0] = light->direction.value.x;
        direction[1] = light->direction.value.y;
        direction[2] = light->direction.value.z;
        color[0] = light->color.value.x;
        color[1] = light->color.value.y;
        color[2] = light->color.value.z;
        intensity = light->intensity.value;
        on = light->on.value;
        lightType = 2;
    }

    /* Pass light to renderer */
    addLight(lightCount, lightType, position, direction, color, intensity, on, userData);
    lightCount++;
}
