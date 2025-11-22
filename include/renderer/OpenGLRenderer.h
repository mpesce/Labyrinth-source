/*
 * OpenGLRenderer.h - Modern OpenGL 3.3+ renderer
 *
 * Part of Labyrinth VRML Browser - Modern Port
 * Copyright (c) 1995, The Community Company
 * Modernized 2025
 *
 * Replaces RenderMorphics Reality Lab with OpenGL
 */

#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "RenderAction.h"

/* Forward declaration for GLFW */
struct GLFWwindow;

/*
 * OpenGL Renderer
 * Implements rendering callbacks for RenderAction
 */
class OpenGLRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    /* Initialize renderer */
    bool initialize(int width, int height, const char* title);
    void shutdown();

    /* Rendering */
    void beginFrame();
    void endFrame();
    void clear();

    /* Scene rendering */
    void renderScene(QvNode* root);

    /* Window management */
    bool shouldClose();
    void pollEvents();
    void getWindowSize(int* width, int* height);

    /* Camera control */
    void setCameraPosition(float x, float y, float z);
    void setCameraRotation(float pitch, float yaw);
    void updateCamera();

    /* Input handling */
    void processInput(float deltaTime);

private:
    GLFWwindow* window;
    RenderAction* renderAction;

    /* Camera state */
    Vector3 cameraPos;
    Vector3 cameraTarget;
    Vector3 cameraUp;
    float cameraPitch;
    float cameraYaw;
    float cameraDistance;

    /* Input state */
    float lastX, lastY;
    bool firstMouse;
    bool mousePressed;

    /* Rendering state */
    int windowWidth;
    int windowHeight;
    unsigned int shaderProgram;
    unsigned int vao, vbo, ebo;

    /* Shader compilation */
    bool compileShaders();
    unsigned int compileShader(const char* source, unsigned int type);
    unsigned int linkProgram(unsigned int vertex, unsigned int fragment);

    /* Geometry generation */
    void generateSphere(float radius, int segments);
    void generateCube(float width, float height, float depth);
    void generateCone(float radius, float height, int segments);
    void generateCylinder(float radius, float height, int segments);

    /* Static callbacks for RenderAction */
    static void cb_beginSeparator(void* userData);
    static void cb_endSeparator(void* userData);
    static void cb_setTransform(Matrix4* matrix, void* userData);
    static void cb_setMaterial(RenderState* state, void* userData);
    static void cb_drawSphere(float radius, void* userData);
    static void cb_drawCube(float width, float height, float depth, void* userData);
    static void cb_drawCone(float bottomRadius, float height, void* userData);
    static void cb_drawCylinder(float radius, float height, void* userData);
    static void cb_drawIndexedFaceSet(int* coordIndex, int numIndices,
                                      float* coords, int numCoords, void* userData);
    static void cb_drawIndexedLineSet(int* coordIndex, int numIndices,
                                      float* coords, int numCoords, void* userData);

    /* GLFW callbacks */
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif /* OPENGL_RENDERER_H */
