# VRML 1.0 Parser

This directory contains the YACC/LEX-based parser for VRML 1.0, reconstructed from LABYRNTH.EXE.

## Files

- `vrml.y` - YACC grammar for VRML 1.0 syntax
- `vrml.l` - LEX lexical analyzer for VRML tokenization
- `QvParser.cpp` - High-level parser interface
- `QvParser.h` - Parser public API (in `include/`)

## Grammar Overview

The VRML 1.0 grammar supports:

1. **Header**: `#VRML V1.0 ascii`
2. **Nodes**: Named blocks with fields
3. **DEF/USE**: Node naming and reuse
4. **Comments**: Lines starting with `#`
5. **Field Types**:
   - Integers
   - Floats
   - Strings (quoted)
   - Vectors (vec2f, vec3f)
   - Rotations (4 floats: axis + angle)
   - Arrays (bracketed lists)

## Supported Nodes (35 types)

### Grouping
- Separator, TransformSeparator, Group, Switch, LevelOfDetail

### Transforms
- Transform, Rotation, Translation, Scale, MatrixTransform

### Properties
- Material, MaterialBinding, NormalBinding, ShapeHints
- Coordinate3, Normal

### Textures
- Texture2, Texture2Transform, TextureCoordinate2

### Geometry
- Sphere, Cube, Cone, Cylinder
- IndexedFaceSet, IndexedLineSet, PointSet

### Lights
- DirectionalLight, PointLight, SpotLight

### Cameras
- OrthographicCamera, PerspectiveCamera

### WWW
- WWWAnchor, WWWInline

### Other
- Info

## Building

The parser is built using standard YACC and LEX tools:

```bash
# Generate parser from YACC grammar
yacc -d vrml.y          # or: bison -d vrml.y

# Generate lexer from LEX specification
lex vrml.l              # or: flex vrml.l

# Compile generated files
gcc -c lex.yy.c -o lex.yy.o
gcc -c y.tab.c -o y.tab.o
g++ -c QvParser.cpp -o QvParser.o

# Link into library
ar rcs libVRMLParser.a lex.yy.o y.tab.o QvParser.o
```

Or use the Makefile:

```bash
make parser
```

## Usage

```cpp
#include "QvParser.h"

// Parse VRML file
QvNode* root = QvParser_ReadFile("scene.wrl");
if (root == NULL) {
    fprintf(stderr, "Parse error: %s\n", QvParser_GetError());
    return 1;
}

// Use the scene graph
root->traverse(myAction);

// Clean up
root->unref();
```

## Example VRML

```vrml
#VRML V1.0 ascii

Separator {
  DirectionalLight {
    direction 0 0 -1
  }

  PerspectiveCamera {
    position 0 0 5
    orientation 0 0 1 0
    focalDistance 5
  }

  Material {
    diffuseColor 1 0 0
  }

  Sphere {
    radius 1
  }
}
```

## Error Handling

The parser reports errors with:

- **Syntax errors**: "VRML error at line N: unexpected token"
- **Unknown nodes**: "Unknown node type: NodeName"
- **USE errors**: "Unknown USE name: nodeName"
- **Field errors**: "Unknown field 'fieldName'"

Error messages match those found in LABYRNTH.EXE:
- "VRML error in %s(): %s"
- "VRML read error: %s"
- "Premature end of file after DEF"
- "Premature end of file after USE"

## Original Implementation

The original Labyrinth used **PC-YACC** (DOS/Windows YACC implementation) as evidenced by the string "pcyacc internal stack overflow" found in the executable.

This reconstruction uses standard YACC/Bison and LEX/Flex for portability while maintaining compatibility with the VRML 1.0 specification.

## References

- VRML 1.0 Specification: http://www.eit.com/vrml/vrmlspec.html
- String analysis from LABYRNTH.EXE (March 4, 1995)
- QvLib node implementation
