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
#include <map>
#include <string>

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

    /* Current scene state (from RenderAction) */
    Matrix4 currentModelMatrix;
    float currentColor[3];  /* RGB diffuse color */

    /* Light tracking (up to 8 lights) */
    struct LightData {
        int type;           /* 0=directional, 1=point, 2=spot */
        float position[3];
        float direction[3];
        float color[3];
        float intensity;
        bool enabled;
    };
    LightData lights[8];
    int numLights;

    /* Camera from VRML scene */
    bool vrmlCameraSet;
    Vector3 vrmlCameraPos;
    Vector3 vrmlCameraTarget;
    float vrmlCameraFov;

    /* Material properties */
    float currentAmbient[3];
    float currentDiffuse[3];
    float currentSpecular[3];
    float currentShininess;

    /* Texture tracking */
    unsigned int currentTextureId;
    bool currentTextureEnabled;
    std::map<std::string, unsigned int> textureCache;  /* filename -> texture ID */

    /* Shader compilation */
    bool compileShaders();
    unsigned int compileShader(const char* source, unsigned int type);
    unsigned int linkProgram(unsigned int vertex, unsigned int fragment);

    /* Helper to apply current transform and material */
    void applyCurrentState();

    /* Texture loading */
    unsigned int loadTextureFromFile(const char* filename, int wrapS, int wrapT);

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
                                      float* coords, int numCoords,
                                      float* normals, int numNormals,
                                      float* texCoords, int numTexCoords,
                                      void* userData);
    static void cb_drawIndexedLineSet(int* coordIndex, int numIndices,
                                      float* coords, int numCoords, void* userData);
    static void cb_addLight(int lightIndex, int type, float* position, float* direction,
                            float* color, float intensity, bool on, void* userData);
    static void cb_setCamera(int type, float* position, float* orientation,
                             float fov, float aspectRatio, void* userData);
    static void cb_loadTexture(const char* filename, int wrapS, int wrapT, void* userData);

    /* GLFW callbacks */
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif /* OPENGL_RENDERER_H */
