# Labyrinth VRML Browser - Reverse Engineering Analysis

## Executable Information
- **File**: LABYRNTH.EXE
- **Size**: 726,016 bytes (709 KB)
- **Format**: PE32 (32-bit Windows executable)
- **Build Date**: Saturday, March 4, 1995 at 04:26:59
- **Platform**: Intel 80386, Windows GUI
- **Subsystem**: Windows GUI (version 3.10 - Windows NT 3.1/Windows 95)
- **Linker**: Microsoft Linker v2.50
- **Status**: PRERELEASE VERSION - DO NOT REDISTRIBUTE!

## Copyright & Attribution
- **Copyright (c) 1995, The Community Company** (Mark Pesce)
- **Portions Copyright (c) 1993 - 1995, RenderMorphics, Ltd.** (Reality Lab 3D engine)

## Architecture Overview

### QvLib - VRML Node Library
The codebase uses QvLib (Qv = "VRML") for scene graph representation.

#### Grouping Nodes
- `QvGroup` - Basic grouping node
- `QvSeparator` - Grouping with property isolation
- `QvSwitch` - Selectable children
- `QvTransformSeparator` - Transform isolation
- `QvLevelOfDetail` - LOD selection

#### Property Nodes
- `QvCoordinate3` - 3D coordinates
- `QvMaterial` - Material properties
- `QvMaterialBinding` - Material binding mode
- `QvNormal` - Normal vectors
- `QvNormalBinding` - Normal binding mode
- `QvShapeHints` - Rendering hints
- `QvTextureCoordinate2` - 2D texture coordinates
- `QvTexture2` - 2D texture
- `QvTexture2Transform` - Texture transformation

#### Light Nodes
- `QvDirectionalLight` - Directional light source
- `QvPointLight` - Point light source
- `QvSpotLight` - Spotlight source

#### Camera Nodes
- `QvOrthographicCamera` - Orthographic projection
- `QvPerspectiveCamera` - Perspective projection

#### Transform Nodes
- `QvTransform` - General transformation
- `QvRotation` - Rotation only
- `QvTranslation` - Translation only
- `QvScale` - Scaling only
- `QvMatrixTransform` - Matrix transformation

#### Geometry Nodes
- `QvCone` - Cone primitive
- `QvCube` - Cube primitive
- `QvCylinder` - Cylinder primitive
- `QvSphere` - Sphere primitive
- `QvIndexedFaceSet` - Indexed polygon mesh
- `QvIndexedLineSet` - Indexed line set
- `QvPointSet` - Point cloud

#### WWW/Hyperlink Nodes
- `QvWWWAnchor` - Hyperlink anchor
- `QvWWWInline` - Inline external file

#### Other Nodes
- `QvInfo` - Metadata/comments
- `QvUnknownNode` - Fallback for unknown nodes

### Parser Architecture
- **Parser Generator**: PC-YACC (indicated by "pcyacc internal stack overflow" message)
- **VRML Version**: Pre-VRML 1.0 / "Reference VRML Parser 1.0"
- **Format**: "#VRML V1.0 ascii"

### WWW/HTTP Integration
Multiple parser components for web integration:
- `HTParse` - HTTP URL parsing (from libwww)
- `MIMEParser` - MIME type handling
- `SGMLParser` - HTML/SGML parsing
- `WSRCParser` - Custom source file parser

### 3D Rendering
- **Engine**: RenderMorphics Reality Lab
- **Scene Management**: WcBeginScene(), WcEndScene()
- **File Format**: XOF (DirectX Object Format) support via loadxof.c

### Source File References
Embedded debug paths reveal source structure:
- `E:\labsrc\WWW\HTPARSE.C` - HTTP parser
- `api\object\objdes.c` - Object description
- `math.c` - Math utilities
- `loadxof.c` - XOF file loader
- `device.c` - Device management
- `object.c` - Object handling
- `tilemap.c` - Texture tile mapping

### Field Types (QvLib)
- `QvSFBool` - Boolean field
- `QvSFBitMask` - Bitmask field
- `QvSFEnum` - Enumeration field
- Additional field types implied

### Default VRML Files
- `labyrnth.wrl` - Main scene
- `home.wrl` - Home scene
- `http://www.butterfly.net/lab/ushm.wrl`
- `http://www.eit.com/~mark/ushm/ushm.wrl`
- `http://www.eit.com/~mark/soma/soma.wrl`
- `http://www.iuma.com/vrml/`

### External Links
- VRML Spec: `http://www.eit.com/vrml/vrmlspec.html`
- VRML Repository: `http://bug.village.virginia.edu/vrml/`

## Next Steps
1. Identify class vtables and method signatures
2. Reconstruct QvNode base class and inheritance hierarchy
3. Locate and decompile parser (YACC-generated code)
4. Decompile rendering integration (RenderMorphics)
5. Extract HTTP/WWW client code
6. Reconstruct header files
7. Create buildable source tree
