/*
 * OpenGLRenderer.cpp - Modern OpenGL renderer implementation
 *
 * Part of Labyrinth VRML Browser - Modern Port
 * Full OpenGL 3.3+ implementation with GLFW3 and GLEW
 */

#include "../../include/renderer/OpenGLRenderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>

/* OpenGL headers */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Vertex shader source */
static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

/* Fragment shader source - supports up to 8 lights */
static const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

#define MAX_LIGHTS 8

struct Light {
    int type;          // 0=directional, 1=point, 2=spot
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    bool enabled;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;
uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;
uniform bool useTexture;
uniform sampler2D texSampler;

vec3 calculateDirectionalLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess * 128.0);
    vec3 specular = spec * light.color * light.intensity * specularColor;

    return diffuse + specular;
}

vec3 calculatePointLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);

    // Attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess * 128.0);
    vec3 specular = spec * light.color * light.intensity * specularColor;

    return (diffuse + specular) * attenuation;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient component
    vec3 result = ambientColor;

    // Add contribution from each light
    for(int i = 0; i < numLights && i < MAX_LIGHTS; i++) {
        if(!lights[i].enabled) continue;

        if(lights[i].type == 0) {
            result += calculateDirectionalLight(lights[i], norm, viewDir);
        } else if(lights[i].type == 1) {
            result += calculatePointLight(lights[i], norm, viewDir);
        }
        // TODO: Spotlight support (type 2)
    }

    // Multiply by material diffuse color or texture
    vec3 materialColor = diffuseColor;
    if(useTexture) {
        materialColor = texture(texSampler, TexCoord).rgb;
    }

    result *= materialColor;

    FragColor = vec4(result, 1.0);
}
)";

OpenGLRenderer::OpenGLRenderer()
{
    window = NULL;
    renderAction = NULL;

    /* Initialize camera */
    cameraPos = Vector3(0.0f, 0.0f, 5.0f);
    cameraTarget = Vector3(0.0f, 0.0f, 0.0f);
    cameraUp = Vector3(0.0f, 1.0f, 0.0f);
    cameraPitch = 0.0f;
    cameraYaw = -90.0f;
    cameraDistance = 5.0f;

    /* Initialize input state */
    lastX = 400.0f;
    lastY = 300.0f;
    firstMouse = true;
    mousePressed = false;

    windowWidth = 800;
    windowHeight = 600;
    shaderProgram = 0;
    vao = vbo = ebo = 0;

    /* Initialize scene state */
    currentModelMatrix.identity();
    currentColor[0] = 0.8f;
    currentColor[1] = 0.2f;
    currentColor[2] = 0.2f;

    /* Initialize lights */
    numLights = 0;
    for (int i = 0; i < 8; i++) {
        lights[i].type = 0;
        lights[i].position[0] = lights[i].position[1] = lights[i].position[2] = 0.0f;
        lights[i].direction[0] = 0.0f; lights[i].direction[1] = 0.0f; lights[i].direction[2] = -1.0f;
        lights[i].color[0] = lights[i].color[1] = lights[i].color[2] = 1.0f;
        lights[i].intensity = 1.0f;
        lights[i].enabled = false;
    }

    /* Initialize camera */
    vrmlCameraSet = false;
    vrmlCameraPos = Vector3(0, 0, 5);
    vrmlCameraTarget = Vector3(0, 0, 0);
    vrmlCameraFov = 0.785398f;  /* 45 degrees */

    /* Initialize material */
    currentAmbient[0] = currentAmbient[1] = currentAmbient[2] = 0.2f;
    currentDiffuse[0] = currentDiffuse[1] = currentDiffuse[2] = 0.8f;
    currentSpecular[0] = currentSpecular[1] = currentSpecular[2] = 0.0f;
    currentShininess = 0.2f;

    /* Initialize texture */
    currentTextureId = 0;
    currentTextureEnabled = false;
}

OpenGLRenderer::~OpenGLRenderer()
{
    shutdown();
}

bool OpenGLRenderer::initialize(int width, int height, const char* title)
{
    windowWidth = width;
    windowHeight = height;

    /* Initialize GLFW */
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    /* Set OpenGL version 3.3 core profile */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create window */
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    /* Initialize GLEW */
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    /* Set viewport */
    glViewport(0, 0, width, height);

    /* Set up callbacks */
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* Enable depth testing */
    glEnable(GL_DEPTH_TEST);

    /* Compile shaders */
    if (!compileShaders()) {
        fprintf(stderr, "Failed to compile shaders\n");
        shutdown();
        return false;
    }

    /* Create render action */
    renderAction = new RenderAction();

    /* Set up render callbacks */
    renderAction->beginSeparator = cb_beginSeparator;
    renderAction->endSeparator = cb_endSeparator;
    renderAction->setTransform = cb_setTransform;
    renderAction->setMaterial = cb_setMaterial;
    renderAction->drawSphere = cb_drawSphere;
    renderAction->drawCube = cb_drawCube;
    renderAction->drawCone = cb_drawCone;
    renderAction->drawCylinder = cb_drawCylinder;
    renderAction->drawIndexedFaceSet = cb_drawIndexedFaceSet;
    renderAction->addLight = cb_addLight;
    renderAction->setCamera = cb_setCamera;
    renderAction->loadTexture = cb_loadTexture;
    renderAction->userData = this;

    printf("OpenGL Renderer initialized successfully\n");
    printf("  OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("  GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("  Renderer: %s\n", glGetString(GL_RENDERER));

    return true;
}

void OpenGLRenderer::shutdown()
{
    if (renderAction) {
        delete renderAction;
        renderAction = NULL;
    }

    if (shaderProgram) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }

    if (vao) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

    if (vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    if (ebo) {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }

    /* Delete all loaded textures */
    for (std::map<std::string, unsigned int>::iterator it = textureCache.begin();
         it != textureCache.end(); ++it) {
        glDeleteTextures(1, &it->second);
    }
    textureCache.clear();

    if (window) {
        glfwDestroyWindow(window);
        window = NULL;
    }

    glfwTerminate();
}

void OpenGLRenderer::beginFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::endFrame()
{
    glfwSwapBuffers(window);
}

void OpenGLRenderer::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void OpenGLRenderer::renderScene(QvNode* root)
{
    if (root == NULL || renderAction == NULL) {
        return;
    }

    /* Update camera matrices */
    updateCamera();

    /* Traverse scene graph */
    renderAction->apply(root);
}

bool OpenGLRenderer::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void OpenGLRenderer::pollEvents()
{
    glfwPollEvents();
}

void OpenGLRenderer::getWindowSize(int* width, int* height)
{
    *width = windowWidth;
    *height = windowHeight;
}

void OpenGLRenderer::setCameraPosition(float x, float y, float z)
{
    cameraPos = Vector3(x, y, z);
}

void OpenGLRenderer::setCameraRotation(float pitch, float yaw)
{
    cameraPitch = pitch;
    cameraYaw = yaw;
}

void OpenGLRenderer::updateCamera()
{
    if (!shaderProgram) return;

    /* Calculate camera front vector from pitch/yaw */
    Vector3 front;
    front.x = cosf(cameraPitch) * cosf(cameraYaw);
    front.y = sinf(cameraPitch);
    front.z = cosf(cameraPitch) * sinf(cameraYaw);

    /* Normalize */
    float len = sqrtf(front.x * front.x + front.y * front.y + front.z * front.z);
    if (len > 0.0f) {
        front.x /= len;
        front.y /= len;
        front.z /= len;
    }

    cameraTarget.x = cameraPos.x + front.x;
    cameraTarget.y = cameraPos.y + front.y;
    cameraTarget.z = cameraPos.z + front.z;

    /* Use shader program */
    glUseProgram(shaderProgram);

    /* Set up view matrix (simplified lookAt) */
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    if (viewLoc != -1) {
        /* Simple view matrix - identity for now */
        float viewMatrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -cameraPos.x, -cameraPos.y, -cameraPos.z, 1.0f
        };
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
    }

    /* Set up projection matrix (perspective) */
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    if (projLoc != -1) {
        float aspect = (float)windowWidth / (float)windowHeight;
        float fov = 45.0f * M_PI / 180.0f;
        float near = 0.1f;
        float far = 100.0f;

        float f = 1.0f / tanf(fov / 2.0f);
        float projMatrix[16] = {
            f / aspect, 0.0f, 0.0f, 0.0f,
            0.0f, f, 0.0f, 0.0f,
            0.0f, 0.0f, (far + near) / (near - far), -1.0f,
            0.0f, 0.0f, (2.0f * far * near) / (near - far), 0.0f
        };
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projMatrix);
    }

    /* Set up model matrix (from current transform) */
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    if (modelLoc != -1) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, currentModelMatrix.m);
    }

    /* Set up lighting uniforms */
    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    if (lightPosLoc != -1) {
        glUniform3f(lightPosLoc, 5.0f, 5.0f, 5.0f);
    }

    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    if (viewPosLoc != -1) {
        glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
    }

    GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    if (lightColorLoc != -1) {
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
    }

    GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    if (objectColorLoc != -1) {
        glUniform3f(objectColorLoc, currentColor[0], currentColor[1], currentColor[2]);
    }
}

void OpenGLRenderer::processInput(float deltaTime)
{
    if (window == NULL) return;

    float cameraSpeed = 2.5f * deltaTime;

    /* Calculate front vector */
    Vector3 front;
    front.x = cosf(cameraPitch) * cosf(cameraYaw);
    front.y = sinf(cameraPitch);
    front.z = cosf(cameraPitch) * sinf(cameraYaw);

    /* Normalize */
    float len = sqrtf(front.x * front.x + front.y * front.y + front.z * front.z);
    if (len > 0.0f) {
        front.x /= len;
        front.y /= len;
        front.z /= len;
    }

    /* Calculate right vector (cross product of front and up) */
    Vector3 right;
    right.x = front.y * cameraUp.z - front.z * cameraUp.y;
    right.y = front.z * cameraUp.x - front.x * cameraUp.z;
    right.z = front.x * cameraUp.y - front.y * cameraUp.x;

    /* Normalize right */
    len = sqrtf(right.x * right.x + right.y * right.y + right.z * right.z);
    if (len > 0.0f) {
        right.x /= len;
        right.y /= len;
        right.z /= len;
    }

    /* WASD movement */
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos.x += front.x * cameraSpeed;
        cameraPos.y += front.y * cameraSpeed;
        cameraPos.z += front.z * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos.x -= front.x * cameraSpeed;
        cameraPos.y -= front.y * cameraSpeed;
        cameraPos.z -= front.z * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos.x -= right.x * cameraSpeed;
        cameraPos.y -= right.y * cameraSpeed;
        cameraPos.z -= right.z * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos.x += right.x * cameraSpeed;
        cameraPos.y += right.y * cameraSpeed;
        cameraPos.z += right.z * cameraSpeed;
    }

    /* ESC to close */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

/* GLFW callback function implementations */
void OpenGLRenderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)glfwGetWindowUserPointer(window);
    if (renderer) {
        renderer->windowWidth = width;
        renderer->windowHeight = height;
        glViewport(0, 0, width, height);
    }
}

void OpenGLRenderer::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)glfwGetWindowUserPointer(window);
    if (renderer == NULL || !renderer->mousePressed) return;

    if (renderer->firstMouse) {
        renderer->lastX = xpos;
        renderer->lastY = ypos;
        renderer->firstMouse = false;
    }

    float xoffset = xpos - renderer->lastX;
    float yoffset = renderer->lastY - ypos;  /* Reversed: y goes from bottom to top */

    renderer->lastX = xpos;
    renderer->lastY = ypos;

    float sensitivity = 0.005f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    renderer->cameraYaw += xoffset;
    renderer->cameraPitch += yoffset;

    /* Constrain pitch */
    if (renderer->cameraPitch > 1.5f) renderer->cameraPitch = 1.5f;
    if (renderer->cameraPitch < -1.5f) renderer->cameraPitch = -1.5f;
}

void OpenGLRenderer::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)glfwGetWindowUserPointer(window);
    if (renderer == NULL) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            renderer->mousePressed = true;
            renderer->firstMouse = true;
        } else if (action == GLFW_RELEASE) {
            renderer->mousePressed = false;
        }
    }
}

void OpenGLRenderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)glfwGetWindowUserPointer(window);
    if (renderer == NULL) return;

    renderer->cameraDistance -= yoffset * 0.5f;
    if (renderer->cameraDistance < 1.0f) renderer->cameraDistance = 1.0f;
    if (renderer->cameraDistance > 50.0f) renderer->cameraDistance = 50.0f;
}

/* Callback implementations */
void OpenGLRenderer::cb_beginSeparator(void* userData)
{
    /* Push matrix stack */
}

void OpenGLRenderer::cb_endSeparator(void* userData)
{
    /* Pop matrix stack */
}

void OpenGLRenderer::cb_setTransform(Matrix4* matrix, void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (!renderer || !matrix) return;

    /* Store the current model matrix */
    for (int i = 0; i < 16; i++) {
        renderer->currentModelMatrix.m[i] = matrix->m[i];
    }
}

void OpenGLRenderer::cb_setMaterial(RenderState* state, void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (!renderer || !state) return;

    /* Store all material properties */
    if (state->ambientColor) {
        renderer->currentAmbient[0] = state->ambientColor->r;
        renderer->currentAmbient[1] = state->ambientColor->g;
        renderer->currentAmbient[2] = state->ambientColor->b;
    }

    if (state->diffuseColor) {
        renderer->currentDiffuse[0] = state->diffuseColor->r;
        renderer->currentDiffuse[1] = state->diffuseColor->g;
        renderer->currentDiffuse[2] = state->diffuseColor->b;
        /* Keep currentColor for compatibility */
        renderer->currentColor[0] = state->diffuseColor->r;
        renderer->currentColor[1] = state->diffuseColor->g;
        renderer->currentColor[2] = state->diffuseColor->b;
    }

    if (state->specularColor) {
        renderer->currentSpecular[0] = state->specularColor->r;
        renderer->currentSpecular[1] = state->specularColor->g;
        renderer->currentSpecular[2] = state->specularColor->b;
    }

    renderer->currentShininess = state->shininess;
}

void OpenGLRenderer::cb_drawSphere(float radius, void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (renderer && renderer->shaderProgram) {
        renderer->generateSphere(radius, 32);
    }
}

void OpenGLRenderer::cb_drawCube(float width, float height, float depth, void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (renderer && renderer->shaderProgram) {
        renderer->generateCube(width, height, depth);
    }
}

void OpenGLRenderer::cb_drawCone(float bottomRadius, float height, void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (renderer && renderer->shaderProgram) {
        renderer->generateCone(bottomRadius, height, 32);
    }
}

void OpenGLRenderer::cb_drawCylinder(float radius, float height, void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (renderer && renderer->shaderProgram) {
        renderer->generateCylinder(radius, height, 32);
    }
}

void OpenGLRenderer::cb_drawIndexedFaceSet(int* coordIndex, int numIndices,
                                           float* coords, int numCoords,
                                           float* normals, int numNormals,
                                           float* texCoords, int numTexCoords,
                                           void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (!renderer || !renderer->shaderProgram || !coordIndex || !coords) {
        return;
    }

    /* Apply current transform and material state */
    renderer->applyCurrentState();

    /* Parse coordIndex array and build triangles */
    std::vector<float> vertices;
    std::vector<float> generatedNormals;
    std::vector<float> generatedTexCoords;

    /* Current polygon being built */
    std::vector<int> polygon;

    for (int i = 0; i < numIndices; i++) {
        int idx = coordIndex[i];

        if (idx == -1) {
            /* End of polygon - triangulate it */
            if (polygon.size() >= 3) {
                /* For simplicity, do fan triangulation from first vertex */
                /* This works for convex polygons */
                for (size_t j = 1; j + 1 < polygon.size(); j++) {
                    int i0 = polygon[0];
                    int i1 = polygon[j];
                    int i2 = polygon[j + 1];

                    /* Validate indices */
                    if (i0 >= 0 && i0 < numCoords &&
                        i1 >= 0 && i1 < numCoords &&
                        i2 >= 0 && i2 < numCoords) {

                        /* Add triangle vertices */
                        /* Each coordinate is 3 floats (x, y, z) */
                        for (int k = 0; k < 3; k++) {
                            vertices.push_back(coords[i0 * 3 + k]);
                        }
                        for (int k = 0; k < 3; k++) {
                            vertices.push_back(coords[i1 * 3 + k]);
                        }
                        for (int k = 0; k < 3; k++) {
                            vertices.push_back(coords[i2 * 3 + k]);
                        }

                        /* Add normals - use provided normals if available, else generate */
                        if (normals != NULL && numNormals > 0) {
                            /* Use provided vertex normals (smooth shading) */
                            /* Assume one normal per vertex, indexed by coordIndex */
                            if (i0 < numNormals && i1 < numNormals && i2 < numNormals) {
                                /* Normal for vertex 0 */
                                generatedNormals.push_back(normals[i0 * 3 + 0]);
                                generatedNormals.push_back(normals[i0 * 3 + 1]);
                                generatedNormals.push_back(normals[i0 * 3 + 2]);
                                /* Normal for vertex 1 */
                                generatedNormals.push_back(normals[i1 * 3 + 0]);
                                generatedNormals.push_back(normals[i1 * 3 + 1]);
                                generatedNormals.push_back(normals[i1 * 3 + 2]);
                                /* Normal for vertex 2 */
                                generatedNormals.push_back(normals[i2 * 3 + 0]);
                                generatedNormals.push_back(normals[i2 * 3 + 1]);
                                generatedNormals.push_back(normals[i2 * 3 + 2]);
                            }
                        } else {
                            /* Generate face normal using cross product (flat shading) */
                            float v0[3] = {
                                coords[i0 * 3 + 0], coords[i0 * 3 + 1], coords[i0 * 3 + 2]
                            };
                            float v1[3] = {
                                coords[i1 * 3 + 0], coords[i1 * 3 + 1], coords[i1 * 3 + 2]
                            };
                            float v2[3] = {
                                coords[i2 * 3 + 0], coords[i2 * 3 + 1], coords[i2 * 3 + 2]
                            };

                            /* Edge vectors */
                            float e1[3] = { v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2] };
                            float e2[3] = { v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2] };

                            /* Cross product */
                            float normal[3] = {
                                e1[1] * e2[2] - e1[2] * e2[1],
                                e1[2] * e2[0] - e1[0] * e2[2],
                                e1[0] * e2[1] - e1[1] * e2[0]
                            };

                            /* Normalize */
                            float length = sqrtf(normal[0] * normal[0] +
                                               normal[1] * normal[1] +
                                               normal[2] * normal[2]);
                            if (length > 0.0001f) {
                                normal[0] /= length;
                                normal[1] /= length;
                                normal[2] /= length;
                            }

                            /* Add same normal for all 3 vertices (flat shading) */
                            for (int v = 0; v < 3; v++) {
                                generatedNormals.push_back(normal[0]);
                                generatedNormals.push_back(normal[1]);
                                generatedNormals.push_back(normal[2]);
                            }
                        }

                        /* Add texture coordinates - use provided coords if available, else generate defaults */
                        if (texCoords != NULL && numTexCoords > 0) {
                            /* Use provided texture coordinates */
                            /* Assume one texCoord per vertex, indexed by coordIndex */
                            if (i0 < numTexCoords && i1 < numTexCoords && i2 < numTexCoords) {
                                /* TexCoord for vertex 0 (S, T) */
                                generatedTexCoords.push_back(texCoords[i0 * 2 + 0]);
                                generatedTexCoords.push_back(texCoords[i0 * 2 + 1]);
                                /* TexCoord for vertex 1 */
                                generatedTexCoords.push_back(texCoords[i1 * 2 + 0]);
                                generatedTexCoords.push_back(texCoords[i1 * 2 + 1]);
                                /* TexCoord for vertex 2 */
                                generatedTexCoords.push_back(texCoords[i2 * 2 + 0]);
                                generatedTexCoords.push_back(texCoords[i2 * 2 + 1]);
                            }
                        } else {
                            /* Generate default texture coordinates (0,0) */
                            for (int v = 0; v < 3; v++) {
                                generatedTexCoords.push_back(0.0f);
                                generatedTexCoords.push_back(0.0f);
                            }
                        }
                    }
                }
            }

            /* Clear polygon for next face */
            polygon.clear();
        } else {
            /* Add vertex index to current polygon */
            polygon.push_back(idx);
        }
    }

    /* If we have vertices to render, create temporary VAO/VBO and draw */
    if (vertices.size() > 0) {
        GLuint tempVAO, tempVBO;
        glGenVertexArrays(1, &tempVAO);
        glGenBuffers(1, &tempVBO);

        glBindVertexArray(tempVAO);
        glBindBuffer(GL_ARRAY_BUFFER, tempVBO);

        /* Interleave position, normal, and texCoord data */
        std::vector<float> interleavedData;
        for (size_t i = 0; i < vertices.size() / 3; i++) {
            /* Position */
            interleavedData.push_back(vertices[i * 3 + 0]);
            interleavedData.push_back(vertices[i * 3 + 1]);
            interleavedData.push_back(vertices[i * 3 + 2]);
            /* Normal */
            interleavedData.push_back(generatedNormals[i * 3 + 0]);
            interleavedData.push_back(generatedNormals[i * 3 + 1]);
            interleavedData.push_back(generatedNormals[i * 3 + 2]);
            /* TexCoord */
            interleavedData.push_back(generatedTexCoords[i * 2 + 0]);
            interleavedData.push_back(generatedTexCoords[i * 2 + 1]);
        }

        glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(float),
                     interleavedData.data(), GL_STATIC_DRAW);

        /* Position attribute */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        /* Normal attribute */
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                            (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        /* TexCoord attribute */
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                            (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        /* Draw triangles */
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

        /* Cleanup */
        glBindVertexArray(0);
        glDeleteBuffers(1, &tempVBO);
        glDeleteVertexArrays(1, &tempVAO);
    }
}

void OpenGLRenderer::cb_drawIndexedLineSet(int* coordIndex, int numIndices,
                                           float* coords, int numCoords,
                                           void* userData)
{
    fprintf(stderr, "Drawing indexed line set: %d indices, %d coords\n",
            numIndices, numCoords);
}

/* Helper to apply current state before drawing */
void OpenGLRenderer::applyCurrentState()
{
    if (!shaderProgram) return;

    glUseProgram(shaderProgram);

    /* Set model matrix */
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    if (modelLoc != -1) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, currentModelMatrix.m);
    }

    /* Set material colors */
    glUniform3fv(glGetUniformLocation(shaderProgram, "ambientColor"), 1, currentAmbient);
    glUniform3fv(glGetUniformLocation(shaderProgram, "diffuseColor"), 1, currentDiffuse);
    glUniform3fv(glGetUniformLocation(shaderProgram, "specularColor"), 1, currentSpecular);
    glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), currentShininess);

    /* Set camera position for specular calculations */
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"),
                cameraPos.x, cameraPos.y, cameraPos.z);

    /* Set number of lights */
    glUniform1i(glGetUniformLocation(shaderProgram, "numLights"), numLights);

    /* Set each light's parameters */
    for (int i = 0; i < numLights && i < 8; i++) {
        char uniformName[64];

        sprintf(uniformName, "lights[%d].type", i);
        glUniform1i(glGetUniformLocation(shaderProgram, uniformName), lights[i].type);

        sprintf(uniformName, "lights[%d].position", i);
        glUniform3fv(glGetUniformLocation(shaderProgram, uniformName), 1, lights[i].position);

        sprintf(uniformName, "lights[%d].direction", i);
        glUniform3fv(glGetUniformLocation(shaderProgram, uniformName), 1, lights[i].direction);

        sprintf(uniformName, "lights[%d].color", i);
        glUniform3fv(glGetUniformLocation(shaderProgram, uniformName), 1, lights[i].color);

        sprintf(uniformName, "lights[%d].intensity", i);
        glUniform1f(glGetUniformLocation(shaderProgram, uniformName), lights[i].intensity);

        sprintf(uniformName, "lights[%d].enabled", i);
        glUniform1i(glGetUniformLocation(shaderProgram, uniformName), lights[i].enabled ? 1 : 0);
    }

    /* Set texture */
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), currentTextureEnabled ? 1 : 0);
    if (currentTextureEnabled && currentTextureId != 0) {
        printf("DEBUG: applyCurrentState - Binding texture ID=%u to unit 0\n", currentTextureId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, currentTextureId);
        glUniform1i(glGetUniformLocation(shaderProgram, "texSampler"), 0);
    } else {
        printf("DEBUG: applyCurrentState - No texture (enabled=%d, ID=%u)\n", currentTextureEnabled, currentTextureId);
    }
}

/* Geometry generation implementations */
void OpenGLRenderer::generateSphere(float radius, int segments)
{
    const int rings = segments;
    const int sectors = segments * 2;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    /* Generate vertices */
    for (int ring = 0; ring <= rings; ring++) {
        float phi = M_PI * float(ring) / float(rings);
        float y = radius * cosf(phi);
        float ringRadius = radius * sinf(phi);

        for (int sector = 0; sector <= sectors; sector++) {
            float theta = 2.0f * M_PI * float(sector) / float(sectors);
            float x = ringRadius * cosf(theta);
            float z = ringRadius * sinf(theta);

            /* Position */
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            /* Normal (normalized position for a sphere centered at origin) */
            vertices.push_back(x / radius);
            vertices.push_back(y / radius);
            vertices.push_back(z / radius);
        }
    }

    /* Generate indices */
    for (int ring = 0; ring < rings; ring++) {
        for (int sector = 0; sector < sectors; sector++) {
            int current = ring * (sectors + 1) + sector;
            int next = current + sectors + 1;

            /* Two triangles per quad */
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    /* Set up VAO/VBO/EBO */
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    /* Position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Normal attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Apply current transform and material */
    applyCurrentState();

    /* Draw */
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    /* Cleanup */
    glBindVertexArray(0);
}

void OpenGLRenderer::generateCube(float width, float height, float depth)
{
    float w = width / 2.0f;
    float h = height / 2.0f;
    float d = depth / 2.0f;

    /* Cube vertices with normals (6 faces, 4 vertices each, position + normal) */
    float vertices[] = {
        /* Front face (z = +d) */
        -w, -h,  d,  0.0f,  0.0f,  1.0f,
         w, -h,  d,  0.0f,  0.0f,  1.0f,
         w,  h,  d,  0.0f,  0.0f,  1.0f,
        -w,  h,  d,  0.0f,  0.0f,  1.0f,
        /* Back face (z = -d) */
        -w, -h, -d,  0.0f,  0.0f, -1.0f,
        -w,  h, -d,  0.0f,  0.0f, -1.0f,
         w,  h, -d,  0.0f,  0.0f, -1.0f,
         w, -h, -d,  0.0f,  0.0f, -1.0f,
        /* Top face (y = +h) */
        -w,  h, -d,  0.0f,  1.0f,  0.0f,
        -w,  h,  d,  0.0f,  1.0f,  0.0f,
         w,  h,  d,  0.0f,  1.0f,  0.0f,
         w,  h, -d,  0.0f,  1.0f,  0.0f,
        /* Bottom face (y = -h) */
        -w, -h, -d,  0.0f, -1.0f,  0.0f,
         w, -h, -d,  0.0f, -1.0f,  0.0f,
         w, -h,  d,  0.0f, -1.0f,  0.0f,
        -w, -h,  d,  0.0f, -1.0f,  0.0f,
        /* Right face (x = +w) */
         w, -h, -d,  1.0f,  0.0f,  0.0f,
         w,  h, -d,  1.0f,  0.0f,  0.0f,
         w,  h,  d,  1.0f,  0.0f,  0.0f,
         w, -h,  d,  1.0f,  0.0f,  0.0f,
        /* Left face (x = -w) */
        -w, -h, -d, -1.0f,  0.0f,  0.0f,
        -w, -h,  d, -1.0f,  0.0f,  0.0f,
        -w,  h,  d, -1.0f,  0.0f,  0.0f,
        -w,  h, -d, -1.0f,  0.0f,  0.0f,
    };

    unsigned int indices[] = {
        0,  1,  2,  2,  3,  0,   /* Front */
        4,  5,  6,  6,  7,  4,   /* Back */
        8,  9, 10, 10, 11,  8,   /* Top */
        12, 13, 14, 14, 15, 12,  /* Bottom */
        16, 17, 18, 18, 19, 16,  /* Right */
        20, 21, 22, 22, 23, 20   /* Left */
    };

    /* Set up VAO/VBO/EBO */
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Normal attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Apply current transform and material */
    applyCurrentState();

    /* Draw */
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    /* Cleanup */
    glBindVertexArray(0);
}

void OpenGLRenderer::generateCone(float radius, float height, int segments)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    /* Apex of cone */
    vertices.push_back(0.0f);
    vertices.push_back(height / 2.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);  /* Normal (will be averaged) */
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    /* Base circle vertices */
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);

        /* Side vertex */
        vertices.push_back(x);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(z);

        /* Normal (approximate for cone side) */
        float nx = x / radius;
        float ny = 0.5f;
        float nz = z / radius;
        float len = sqrtf(nx * nx + ny * ny + nz * nz);
        vertices.push_back(nx / len);
        vertices.push_back(ny / len);
        vertices.push_back(nz / len);
    }

    /* Side triangles */
    for (int i = 0; i < segments; i++) {
        indices.push_back(0);           /* Apex */
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    /* Base center */
    int baseCenter = vertices.size() / 6;
    vertices.push_back(0.0f);
    vertices.push_back(-height / 2.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    /* Base circle for bottom */
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);

        vertices.push_back(x);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(z);
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);
    }

    /* Base triangles */
    for (int i = 0; i < segments; i++) {
        indices.push_back(baseCenter);
        indices.push_back(baseCenter + i + 2);
        indices.push_back(baseCenter + i + 1);
    }

    /* Set up and draw */
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    applyCurrentState();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void OpenGLRenderer::generateCylinder(float radius, float height, int segments)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    /* Side vertices (top and bottom rings) */
    for (int ring = 0; ring < 2; ring++) {
        float y = (ring == 0) ? -height / 2.0f : height / 2.0f;

        for (int i = 0; i <= segments; i++) {
            float theta = 2.0f * M_PI * float(i) / float(segments);
            float x = radius * cosf(theta);
            float z = radius * sinf(theta);

            /* Position */
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            /* Normal (pointing outward) */
            vertices.push_back(x / radius);
            vertices.push_back(0.0f);
            vertices.push_back(z / radius);
        }
    }

    /* Side triangles */
    for (int i = 0; i < segments; i++) {
        int bottomStart = 0;
        int topStart = segments + 1;

        indices.push_back(bottomStart + i);
        indices.push_back(topStart + i);
        indices.push_back(bottomStart + i + 1);

        indices.push_back(bottomStart + i + 1);
        indices.push_back(topStart + i);
        indices.push_back(topStart + i + 1);
    }

    /* Top cap center */
    int topCenter = vertices.size() / 6;
    vertices.push_back(0.0f);
    vertices.push_back(height / 2.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    /* Top cap circle */
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);

        vertices.push_back(x);
        vertices.push_back(height / 2.0f);
        vertices.push_back(z);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);
    }

    /* Top cap triangles */
    for (int i = 0; i < segments; i++) {
        indices.push_back(topCenter);
        indices.push_back(topCenter + i + 1);
        indices.push_back(topCenter + i + 2);
    }

    /* Bottom cap center */
    int bottomCenter = vertices.size() / 6;
    vertices.push_back(0.0f);
    vertices.push_back(-height / 2.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    /* Bottom cap circle */
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);

        vertices.push_back(x);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(z);
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);
    }

    /* Bottom cap triangles */
    for (int i = 0; i < segments; i++) {
        indices.push_back(bottomCenter);
        indices.push_back(bottomCenter + i + 2);
        indices.push_back(bottomCenter + i + 1);
    }

    /* Set up and draw */
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    applyCurrentState();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

bool OpenGLRenderer::compileShaders()
{
    /* Compile vertex shader */
    unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        return false;
    }

    /* Compile fragment shader */
    unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return false;
    }

    /* Link shader program */
    shaderProgram = linkProgram(vertexShader, fragmentShader);
    if (shaderProgram == 0) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    /* Clean up individual shaders (no longer needed after linking) */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    printf("Shaders compiled and linked successfully\n");
    return true;
}

unsigned int OpenGLRenderer::compileShader(const char* source, unsigned int type)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    /* Check for compilation errors */
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        const char* shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        fprintf(stderr, "Shader compilation failed (%s):\n%s\n", shaderType, infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

unsigned int OpenGLRenderer::linkProgram(unsigned int vertex, unsigned int fragment)
{
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    /* Check for linking errors */
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "Shader linking failed:\n%s\n", infoLog);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void OpenGLRenderer::cb_addLight(int lightIndex, int type, float* position, float* direction,
                                 float* color, float intensity, bool on, void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (!renderer || lightIndex < 0 || lightIndex >= 8) return;

    /* Store light data */
    renderer->lights[lightIndex].type = type;
    renderer->lights[lightIndex].position[0] = position[0];
    renderer->lights[lightIndex].position[1] = position[1];
    renderer->lights[lightIndex].position[2] = position[2];
    renderer->lights[lightIndex].direction[0] = direction[0];
    renderer->lights[lightIndex].direction[1] = direction[1];
    renderer->lights[lightIndex].direction[2] = direction[2];
    renderer->lights[lightIndex].color[0] = color[0];
    renderer->lights[lightIndex].color[1] = color[1];
    renderer->lights[lightIndex].color[2] = color[2];
    renderer->lights[lightIndex].intensity = intensity;
    renderer->lights[lightIndex].enabled = on;

    if (lightIndex >= renderer->numLights) {
        renderer->numLights = lightIndex + 1;
    }
}

void OpenGLRenderer::cb_setCamera(int type, float* position, float* orientation,
                                   float fov, float aspectRatio, void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (!renderer) return;

    /* Convert axis-angle orientation to look direction */
    /* For simplicity, just use position and orientation to set camera */
    renderer->vrmlCameraSet = true;
    renderer->vrmlCameraPos = Vector3(position[0], position[1], position[2]);
    renderer->vrmlCameraFov = fov;

    /* Convert axis-angle orientation to target point */
    /* orientation is [x, y, z, angle] - axis and rotation angle */
    /* For now, use simple forward direction based on rotation */
    /* TODO: Proper axis-angle to direction conversion */
    float angle = orientation[3];
    Vector3 axis(orientation[0], orientation[1], orientation[2]);

    /* Simple approximation: rotate default forward vector (0, 0, -1) */
    Vector3 forward(0, 0, -1);
    renderer->vrmlCameraTarget = Vector3(
        position[0] + forward.x,
        position[1] + forward.y,
        position[2] + forward.z
    );
}

void OpenGLRenderer::cb_loadTexture(const char* filename, int wrapS, int wrapT, void* userData)
{
    OpenGLRenderer* renderer = (OpenGLRenderer*)userData;
    if (!renderer || !filename) return;

    printf("DEBUG: cb_loadTexture called: filename='%s', wrapS=%d, wrapT=%d\n", filename, wrapS, wrapT);

    /* Check if texture is already loaded */
    std::string key(filename);
    if (renderer->textureCache.find(key) != renderer->textureCache.end()) {
        /* Texture already loaded, reuse it */
        renderer->currentTextureId = renderer->textureCache[key];
        renderer->currentTextureEnabled = true;
        printf("DEBUG: Texture cached, ID=%u\n", renderer->currentTextureId);
        return;
    }

    /* Load new texture */
    unsigned int textureId = renderer->loadTextureFromFile(filename, wrapS, wrapT);
    if (textureId != 0) {
        renderer->textureCache[key] = textureId;
        renderer->currentTextureId = textureId;
        renderer->currentTextureEnabled = true;
        printf("DEBUG: Texture loaded successfully, ID=%u, enabled=%d\n", textureId, renderer->currentTextureEnabled);
    } else {
        renderer->currentTextureEnabled = false;
        printf("DEBUG: Texture loading FAILED\n");
    }
}

unsigned int OpenGLRenderer::loadTextureFromFile(const char* filename, int wrapS, int wrapT)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    printf("DEBUG: loadTextureFromFile - Generated texture ID=%u for '%s'\n", textureID, filename);

    /* Try to load PPM file first */
    FILE* file = fopen(filename, "rb");
    if (file) {
        printf("DEBUG: File opened successfully\n");
        char magic[3];
        int width, height, maxval;

        /* Read PPM header */
        int scanResult = fscanf(file, "%2s\n%d %d\n%d\n", magic, &width, &height, &maxval);
        printf("DEBUG: PPM header scan result=%d, magic='%c%c', w=%d, h=%d, maxval=%d\n",
               scanResult, magic[0], magic[1], width, height, maxval);

        if (scanResult == 4 && magic[0] == 'P' && magic[1] == '6' && maxval == 255) {
            printf("DEBUG: Valid PPM header detected\n");

            /* Read RGB data */
            int size = width * height * 3;
            unsigned char* data = (unsigned char*)malloc(size);

            size_t bytesRead = fread(data, 1, size, file);
            printf("DEBUG: Attempted to read %d bytes, actually read %zu bytes\n", size, bytesRead);

            if (data && bytesRead == (size_t)size) {
                printf("DEBUG: Successfully read image data, creating OpenGL texture\n");
                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

                GLenum err = glGetError();
                if (err != GL_NO_ERROR) {
                    printf("DEBUG: glTexImage2D ERROR: 0x%x\n", err);
                }

                glGenerateMipmap(GL_TEXTURE_2D);

                err = glGetError();
                if (err != GL_NO_ERROR) {
                    printf("DEBUG: glGenerateMipmap ERROR: 0x%x\n", err);
                }

                /* Set wrap mode */
                GLint glWrapS = (wrapS == 0) ? GL_REPEAT : GL_CLAMP_TO_EDGE;
                GLint glWrapT = (wrapT == 0) ? GL_REPEAT : GL_CLAMP_TO_EDGE;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapS);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapT);

                /* Set filtering */
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                free(data);
                fclose(file);

                printf("Loaded texture from file: %s (%dx%d)\n", filename, width, height);
                return textureID;
            } else {
                printf("DEBUG: Failed to read complete image data\n");
            }

            if (data) free(data);
        } else {
            printf("DEBUG: Invalid PPM header, using fallback\n");
        }

        fclose(file);
    } else {
        printf("DEBUG: Failed to open file '%s'\n", filename);
    }

    /* Fallback: create procedural checkered pattern */
    const int width = 64;
    const int height = 64;
    unsigned char data[width * height * 3];

    /* Generate checkered pattern */
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 3;
            bool isWhite = ((x / 8) % 2) == ((y / 8) % 2);
            unsigned char color = isWhite ? 255 : 128;
            data[idx + 0] = color;
            data[idx + 1] = color;
            data[idx + 2] = color;
        }
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    /* Set wrap mode */
    GLint glWrapS = (wrapS == 0) ? GL_REPEAT : GL_CLAMP_TO_EDGE;  /* 0=REPEAT, 1=CLAMP */
    GLint glWrapT = (wrapT == 0) ? GL_REPEAT : GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapT);

    /* Set filtering */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    printf("Loaded texture (procedural fallback): %s\n", filename);
    return textureID;
}
