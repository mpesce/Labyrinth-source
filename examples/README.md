# VRML 1.0 Example Files

This directory contains example VRML 1.0 files for testing the parser and demonstrating VRML features.

## Files

### simple.wrl
Basic VRML scene with:
- Directional light
- Perspective camera
- Red sphere
- Material properties

Perfect for testing basic parser functionality.

### cube.wrl
Demonstrates:
- Transform nodes (rotation)
- Cube geometry
- Material assignment

### defuse.wrl
Demonstrates DEF/USE mechanism:
- DEF to define reusable nodes
- USE to reference defined nodes
- Three spheres sharing geometry and material
- Demonstrates scene graph efficiency

### indexed.wrl
Advanced geometry with IndexedFaceSet:
- Coordinate3 vertex list
- IndexedFaceSet for pyramid
- Camera positioning and orientation

### www.wrl
WWW/hyperlink features:
- WWWAnchor for clickable objects
- WWWInline for loading external content
- Hyperlinks to VRML specification

## VRML 1.0 Format

All files follow the VRML 1.0 specification:
- Header: `#VRML V1.0 ascii`
- Comments start with `#`
- Nodes have the format: `NodeType { fields }`
- Hierarchical scene graph structure
- DEF/USE for node reuse

## Testing

Use these files to test the parser:

```cpp
#include "QvParser.h"

QvNode* root = QvParser_ReadFile("examples/simple.wrl");
if (root == NULL) {
    fprintf(stderr, "Error: %s\n", QvParser_GetError());
}
```

## Historical Context

These examples demonstrate features that were available in the world's first VRML browser (Labyrinth, March 1995):
- Basic geometry primitives
- Materials and lighting
- Cameras and viewpoints
- Hierarchical scene graphs
- Hyperlinked 3D content

## VRML 1.0 Specification

Full specification: http://www.eit.com/vrml/vrmlspec.html

The files in this directory are compatible with:
- Labyrinth (1995) - the first VRML browser
- WebSpace (1995)
- WorldView (1995)
- VRweb (1995)
- Any VRML 1.0 compliant viewer
