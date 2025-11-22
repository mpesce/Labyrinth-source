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

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

/* Fragment shader source */
static const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
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
    if (!renderer || !state || !state->diffuseColor) return;

    /* Store current material color */
    renderer->currentColor[0] = state->diffuseColor->r;
    renderer->currentColor[1] = state->diffuseColor->g;
    renderer->currentColor[2] = state->diffuseColor->b;
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
    std::vector<float> normals;

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

                        /* Calculate face normal using cross product */
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
                            normals.push_back(normal[0]);
                            normals.push_back(normal[1]);
                            normals.push_back(normal[2]);
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

        /* Interleave position and normal data */
        std::vector<float> interleavedData;
        for (size_t i = 0; i < vertices.size() / 3; i++) {
            /* Position */
            interleavedData.push_back(vertices[i * 3 + 0]);
            interleavedData.push_back(vertices[i * 3 + 1]);
            interleavedData.push_back(vertices[i * 3 + 2]);
            /* Normal */
            interleavedData.push_back(normals[i * 3 + 0]);
            interleavedData.push_back(normals[i * 3 + 1]);
            interleavedData.push_back(normals[i * 3 + 2]);
        }

        glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(float),
                     interleavedData.data(), GL_STATIC_DRAW);

        /* Position attribute */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        /* Normal attribute */
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                            (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

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

    /* Set object color */
    GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    if (objectColorLoc != -1) {
        glUniform3f(objectColorLoc, currentColor[0], currentColor[1], currentColor[2]);
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
