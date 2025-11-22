/*
 * Main.cpp - Labyrinth VRML Browser (Modern Port)
 *
 * Copyright (c) 1995, The Community Company
 * Modernized 2025
 *
 * The world's first VRML browser, reconstructed from LABYRNTH.EXE
 * Original: March 4, 1995
 * Modern port: January 2025
 *
 * This is a modern OpenGL port of the original browser that used
 * RenderMorphics Reality Lab (later became Microsoft Direct3D).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/QvParser.h"
#include "../include/QvNode.h"
#include "../include/renderer/OpenGLRenderer.h"

/* Default world if no file specified */
static const char* DEFAULT_WORLD = "examples/simple.wrl";

/* Print usage information */
void print_usage(const char* program_name)
{
    printf("Labyrinth VRML Browser - Modern Port\n");
    printf("Original (c) 1995, The Community Company\n");
    printf("Modernized 2025\n");
    printf("\n");
    printf("The world's first VRML browser, reconstructed from LABYRNTH.EXE\n");
    printf("Built: March 4, 1995 (original) | January 2025 (modern port)\n");
    printf("\n");
    printf("Usage: %s [options] [file.wrl]\n", program_name);
    printf("\n");
    printf("Options:\n");
    printf("  -h, --help       Show this help message\n");
    printf("  -v, --version    Show version information\n");
    printf("  -w, --width N    Set window width (default: 800)\n");
    printf("  -h, --height N   Set window height (default: 600)\n");
    printf("\n");
    printf("Controls:\n");
    printf("  Mouse drag:      Rotate view\n");
    printf("  Scroll:          Zoom in/out\n");
    printf("  W/S:             Move forward/backward\n");
    printf("  A/D:             Move left/right\n");
    printf("  ESC:             Exit\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s                      # Load default world\n", program_name);
    printf("  %s scene.wrl            # Load VRML file\n", program_name);
    printf("  %s -w 1024 -h 768       # Custom window size\n", program_name);
    printf("\n");
}

void print_version()
{
    printf("Labyrinth VRML Browser\n");
    printf("Original version: 1.0 (March 4, 1995)\n");
    printf("Modern port: 2.0 (January 2025)\n");
    printf("\n");
    printf("Components:\n");
    printf("  QvLib:          VRML 1.0 scene graph (37 node types)\n");
    printf("  Parser:         YACC/LEX VRML 1.0 parser\n");
    printf("  WWW Library:    HTTP/FTP client (libwww 2.17)\n");
    printf("  Renderer:       OpenGL 3.3+ (replaces Reality Lab)\n");
    printf("\n");
    printf("Original author: Mark Pesce, Tony Parisi\n");
    printf("Company: The Community Company\n");
    printf("Reconstruction: Claude Code Assistant (2025)\n");
    printf("\n");
}

/* Main application */
int main(int argc, char** argv)
{
    const char* vrml_file = NULL;
    int window_width = 800;
    int window_height = 600;
    QvNode* scene_root = NULL;
    OpenGLRenderer* renderer = NULL;
    int exit_code = 0;

    /* Parse command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) {
            if (i + 1 < argc) {
                window_width = atoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "--height") == 0) {
            if (i + 1 < argc) {
                window_height = atoi(argv[++i]);
            }
        } else if (argv[i][0] != '-') {
            /* File argument */
            vrml_file = argv[i];
        }
    }

    /* Use default world if no file specified */
    if (vrml_file == NULL) {
        vrml_file = DEFAULT_WORLD;
        printf("No VRML file specified, loading default world: %s\n", vrml_file);
    }

    /* Banner */
    printf("=============================================================\n");
    printf(" Labyrinth VRML Browser - Modern Port\n");
    printf(" Original (c) 1995, The Community Company\n");
    printf(" The world's first VRML browser\n");
    printf("=============================================================\n");
    printf("\n");

    /* Load VRML file */
    printf("Loading VRML file: %s\n", vrml_file);
    scene_root = QvParser_ReadFile(vrml_file);

    if (scene_root == NULL) {
        const char* error = QvParser_GetError();
        fprintf(stderr, "ERROR: Failed to parse VRML file\n");
        if (error) {
            fprintf(stderr, "  %s\n", error);
        }
        exit_code = 1;
        goto cleanup;
    }

    printf("Successfully parsed VRML file\n");
    printf("  Root node type: %s\n", scene_root->getNodeName());

    /* Initialize renderer */
    printf("\nInitializing OpenGL renderer...\n");
    printf("  Window size: %dx%d\n", window_width, window_height);

    renderer = new OpenGLRenderer();
    if (!renderer->initialize(window_width, window_height, "Labyrinth VRML Browser")) {
        fprintf(stderr, "ERROR: Failed to initialize renderer\n");
        exit_code = 1;
        goto cleanup;
    }

    printf("Renderer initialized\n");
    printf("\n");
    printf("NOTE: This is a stub implementation.\n");
    printf("To build with full OpenGL support:\n");
    printf("  1. Install GLFW3: sudo apt-get install libglfw3-dev\n");
    printf("  2. Install GLEW: sudo apt-get install libglew-dev\n");
    printf("  3. Uncomment OpenGL headers in OpenGLRenderer.cpp\n");
    printf("  4. Build with: make OPENGL=1\n");
    printf("  5. Run with: ./labyrinth scene.wrl\n");
    printf("\n");

    /* Render loop (stub) */
    printf("Starting render loop...\n");
    printf("(In full implementation, this would display the 3D scene)\n");
    printf("\n");

    /* In a real implementation:
     *
     * while (!renderer->shouldClose()) {
     *     renderer->beginFrame();
     *     renderer->clear();
     *     renderer->processInput(deltaTime);
     *     renderer->renderScene(scene_root);
     *     renderer->endFrame();
     *     renderer->pollEvents();
     * }
     */

    /* For now, just traverse and print */
    printf("Scene graph traversal:\n");
    renderer->renderScene(scene_root);

    printf("\nPress ENTER to exit...\n");
    getchar();

cleanup:
    /* Cleanup */
    if (renderer) {
        renderer->shutdown();
        delete renderer;
    }

    if (scene_root) {
        scene_root->unref();
    }

    printf("\nLabyrinth exiting. Goodbye!\n");
    return exit_code;
}
