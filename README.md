# Labyrinth - The First VRML Browser (1995)

## Reconstructed Source Code

This repository contains the **reverse-engineered source code** for **Labyrinth**, the world's first VRML (Virtual Reality Modeling Language) browser.

- **Original Release**: March 4, 1995 (prerelease)
- **Copyright**: (c) 1995, The Community Company (Mark Pesce, Tony Parisi)
- **3D Engine**: RenderMorphics Reality Lab (later became Microsoft Direct3D)
- **Status**: Source code reconstructed from `LABYRNTH.EXE` executable

## Quick Facts

- 🏛️ **First VRML browser in history**
- 📅 **Demonstrated at WWW '94** conference in Chicago
- 🌐 **Foundation of VRML 1.0 specification**
- 💾 **726 KB** Windows 95/NT executable
- 🔧 **Reconstructed** from lost source code using reverse engineering

## Reconstruction Progress

- [x] Complete architectural analysis (175k lines of disassembly)
- [x] QvLib base class structure
- [x] VRML field type system
- [x] Initial node type headers
- [ ] Complete 30+ VRML node implementations
- [ ] YACC parser grammar extraction
- [ ] WWW library (HTTP/FTP/Gopher)
- [ ] RenderMorphics 3D rendering integration
- [ ] GUI framework
- [ ] Build system and tests

**Current Progress**: ~15% complete

## Repository Structure

```
├── LABYRNTH.EXE           # Original executable (March 4, 1995)
├── README.md              # This file
├── ANALYSIS.md            # Initial findings
├── ANALYSIS_REPORT.md     # Detailed reverse engineering analysis
├── RECONSTRUCTION.md      # Full reconstruction documentation
├── include/               # Reconstructed header files
│   ├── QvNode.h          # Base class for all VRML nodes
│   ├── QvFields.h        # VRML field type system
│   ├── QvSeparator.h     # Grouping node
│   ├── QvMaterial.h      # Material properties
│   └── QvIndexedFaceSet.h # Polygon mesh geometry
└── src/                   # Reconstructed source code
    ├── QvLib/            # VRML scene graph (30+ node types)
    ├── Parser/           # YACC-based VRML parser
    ├── WWW/              # HTTP/FTP networking (libwww-based)
    ├── VIEWER/           # Windows GUI framework
    └── Render/           # 3D rendering (Reality Lab)
```

## Technology Stack (1995)

- **Language**: C/C++
- **Compiler**: Microsoft Visual C++ 2.x
- **Platform**: Windows 95 / Windows NT 3.1+
- **Parser**: PC-YACC
- **3D Engine**: RenderMorphics Reality Lab
- **Networking**: Winsock 1.1 (HTTP, FTP, Gopher, NNTP)
- **GUI**: Win32 API (USER32, GDI32)

## Historical Significance

Labyrinth represents a pivotal moment in web history:

- First implementation of 3D graphics on the web
- Proved feasibility of real-time 3D in web browsers
- Established VRML as a standard (later influenced X3D, WebGL, WebXR)
- Demonstrated by Mark Pesce at TED5 (1995)
- Foundation for modern web 3D technologies

## Documentation

- **[RECONSTRUCTION.md](RECONSTRUCTION.md)** - Detailed reconstruction process and status
- **[ANALYSIS_REPORT.md](ANALYSIS_REPORT.md)** - Complete reverse engineering analysis
- **[ANALYSIS.md](ANALYSIS.md)** - Initial architectural findings

## Key Features

### VRML 1.0 Support
- ✅ 30+ node types (Separator, Transform, Material, IndexedFaceSet, etc.)
- ✅ DEF/USE reference system
- ✅ Field types (SF*/MF*)
- ✅ Scene graph traversal
- ✅ WWWAnchor and WWWInline for hyperlinks

### Network Protocols
- ✅ HTTP/1.0 client
- ✅ FTP (anonymous and authenticated)
- ✅ Gopher
- ✅ Local file access
- ✅ NNTP news

### 3D Rendering
- ✅ Reality Lab engine
- ✅ Multiple device support (software/hardware)
- ✅ Texture mapping (GIF loader)
- ✅ Lighting (directional, point, spot)
- ✅ Cameras (perspective, orthographic)

## Reverse Engineering Methodology

1. **Disassembly**: Full x86 disassembly (175,108 lines)
2. **String Analysis**: 1,421 strings extracted, including debug paths
3. **Import Analysis**: 160+ Windows API functions identified
4. **Pattern Recognition**: YACC parser, C++ class structures
5. **Architecture Mapping**: Component boundaries and interactions
6. **Header Reconstruction**: Class hierarchies from usage patterns
7. **Code Decompilation**: Assembly to readable C++

**Tools**: objdump, strings, manual analysis, Claude Code AI

## Default VRML Worlds

Historical VRML content referenced in the executable:
- `labyrnth.wrl` - Default startup world
- `home.wrl` - Home world
- http://www.eit.com/~mark/ushm/ushm.wrl - Universal Simulation Hypermedia Machine
- http://www.eit.com/~mark/soma/soma.wrl - SOMA world
- http://www.iuma.com/vrml/ - IUMA VRML

## Build Status

⚠️ **Not yet buildable** - Reconstruction in progress

Target build environments:
- **Historical**: Microsoft Visual C++ 2.0, Windows 95/NT, Reality Lab SDK
- **Modern** (planned): CMake, OpenGL, cross-platform (Windows/Linux/macOS)

## License

**Original**: (c) 1995, The Community Company
**3D Engine**: (c) 1993-1995, RenderMorphics, Ltd.
**Reconstruction**: 2025, for historical preservation and educational purposes

This reconstruction is for **historical preservation** and **education**.

## Credits

### Original Creators
- **Mark Pesce** - Co-creator of VRML, The Community Company
- **Tony Parisi** - Co-creator of VRML
- **RenderMorphics** - Reality Lab 3D engine (acquired by Microsoft, became Direct3D)

### Reconstruction
- **Reverse Engineering**: Claude Code AI-assisted analysis
- **Preservation**: 2025

## References

- [VRML 1.0 Specification](http://www.eit.com/vrml/vrmlspec.html) (historical)
- [VRML Repository](http://bug.village.virginia.edu/vrml/) (historical)
- [Mark Pesce's History of VRML](http://www.hyperreal.org/~mpesce/)

---

**Last Updated**: November 22, 2025
**Reconstruction Status**: Initial headers complete, implementation in progress
**Next Milestone**: Complete QvLib node implementations
