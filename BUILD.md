# Building Labyrinth VRML Browser

## Quick Start

```bash
# Build libraries and executable (without OpenGL)
make

# Run with example file
make run

# Or run directly
./bin/labyrinth examples/simple.wrl
```

## Build Requirements

### Required Dependencies

- **C++ Compiler**: g++ or clang++ with C++11 support
- **C Compiler**: gcc or clang
- **GNU Make**: Build system
- **Bison**: YACC-compatible parser generator
- **Flex**: LEX-compatible lexer generator

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential bison flex
```

**macOS:**
```bash
brew install bison flex
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc-c++ make bison flex
```

## Build Targets

### Libraries Only

Build just the libraries without the executable:

```bash
make libs
```

This creates:
- `lib/libQvLib.a` - VRML scene graph (37 node types)
- `lib/libWWW.a` - HTTP/FTP networking client
- `lib/libVRMLParser.a` - YACC/LEX VRML 1.0 parser
- `lib/libRenderer.a` - Rendering abstraction layer

### Full Application

Build everything including the executable:

```bash
make all
# or just
make
```

Creates: `bin/labyrinth`

### With OpenGL Support (Future)

**Note:** Full OpenGL support requires additional dependencies. Current version uses stub implementation.

To build with real OpenGL (when implemented):

```bash
# Install OpenGL dependencies first
sudo apt-get install libglfw3-dev libglew-dev

# Build with OpenGL enabled
make OPENGL=1
```

## Build Configuration

### Debug Build (default)

The default build includes debug symbols:

```bash
make
```

### Release Build

For optimized release build:

```bash
make CXXFLAGS="-O3 -DNDEBUG -I./include" CFLAGS="-O3 -DNDEBUG -I./include"
```

### Custom Compiler

Use a different compiler:

```bash
make CXX=clang++ CC=clang
```

## Cleaning

Remove all build artifacts:

```bash
make clean
```

Rebuild from scratch:

```bash
make rebuild
```

## Running

### Command Line Usage

```bash
./bin/labyrinth [options] [file.wrl]
```

**Options:**
- `-h, --help` - Show help message
- `-v, --version` - Show version information
- `-w, --width N` - Set window width (default: 800)
- `--height N` - Set window height (default: 600)

**Examples:**
```bash
# Load default world
./bin/labyrinth

# Load specific VRML file
./bin/labyrinth examples/cube.wrl

# Custom window size
./bin/labyrinth -w 1024 --height 768 examples/indexed.wrl
```

### Quick Run

Use the Makefile target:

```bash
make run
```

This runs the browser with `examples/simple.wrl`.

## Installation

Install to `/usr/local/bin`:

```bash
sudo make install
```

Then run from anywhere:

```bash
labyrinth scene.wrl
```

## Project Structure

```
Labyrinth-source/
├── bin/                    # Compiled executable
│   └── labyrinth
├── lib/                    # Static libraries
│   ├── libQvLib.a
│   ├── libWWW.a
│   ├── libVRMLParser.a
│   └── libRenderer.a
├── obj/                    # Object files
├── include/                # Header files
│   ├── Qv*.h              # QvLib node headers
│   ├── WWW/               # WWW library headers
│   └── renderer/          # Renderer headers
├── src/                    # Source files
│   ├── QvLib/             # Scene graph implementation
│   ├── WWW/               # Networking implementation
│   ├── parser/            # YACC/LEX parser
│   ├── renderer/          # Rendering layer
│   └── Main.cpp           # Application entry point
├── examples/              # Example VRML files
└── Makefile               # Build configuration
```

## Build Process Details

### 1. Parser Generation

The VRML parser is generated from YACC grammar and LEX lexer:

```bash
# Generate parser (automatically done by make)
cd src/parser
bison -y -d vrml.y      # Creates y.tab.c and y.tab.h
flex vrml.l             # Creates lex.yy.c
```

### 2. Compilation

Source files are compiled to object files:

```bash
# QvLib (C++)
g++ -Wall -g -I./include -c src/QvLib/QvNode.cpp -o obj/QvNode.o
# ... (37 node types)

# WWW Library (C)
gcc -Wall -g -I./include -c src/WWW/HTTP.c -o obj/WWW/HTTP.o
# ... (4 files)

# Parser (C/C++)
gcc -c src/parser/y.tab.c -o obj/parser/y.tab.o
gcc -c src/parser/lex.yy.c -o obj/parser/lex.yy.o
g++ -c src/parser/QvParser.cpp -o obj/parser/QvParser.o

# Renderer (C++)
g++ -c src/renderer/RenderAction.cpp -o obj/renderer/RenderAction.o
g++ -c src/renderer/OpenGLRenderer.cpp -o obj/renderer/OpenGLRenderer.o
```

### 3. Linking

Object files are archived into static libraries:

```bash
ar rcs lib/libQvLib.a obj/*.o
ar rcs lib/libWWW.a obj/WWW/*.o
ar rcs lib/libVRMLParser.a obj/parser/*.o
ar rcs lib/libRenderer.a obj/renderer/*.o
```

### 4. Executable

The main application is linked with all libraries:

```bash
g++ -o bin/labyrinth obj/Main.o \
    lib/libRenderer.a \
    lib/libVRMLParser.a \
    lib/libQvLib.a \
    lib/libWWW.a
```

## Troubleshooting

### Bison/Flex Not Found

```
Error: bison: command not found
```

**Solution:** Install bison and flex (see Dependencies above)

### Parser Warnings

You may see warnings from generated parser code. These are normal:

```
warning: unused function 'yyunput'
warning: unused function 'input'
```

These are suppressed with `-Wno-unused-function` in the Makefile.

### Missing Headers

```
fatal error: QvNode.h: No such file or directory
```

**Solution:** Ensure you're building from the project root directory

### Undefined References

```
undefined reference to `QvNode::QvNode()'
```

**Solution:** Rebuild from scratch:
```bash
make clean
make
```

## Development

### Adding New Node Types

1. Create header in `include/Qv*.h`
2. Create implementation in `src/QvLib/Qv*.cpp`
3. Add to `QVLIB_SRCS` in Makefile
4. Add to parser grammar in `src/parser/vrml.y`
5. Add to lexer in `src/parser/vrml.l`

### Testing

Test with example VRML files:

```bash
./bin/labyrinth examples/simple.wrl    # Basic sphere
./bin/labyrinth examples/cube.wrl      # Transforms
./bin/labyrinth examples/defuse.wrl    # DEF/USE
./bin/labyrinth examples/indexed.wrl   # IndexedFaceSet
./bin/labyrinth examples/www.wrl       # WWW nodes
```

## Current Status

### ✅ Fully Implemented

- QvLib scene graph (37 VRML node types)
- WWW library (HTTP, FTP, URL parsing)
- VRML 1.0 parser (YACC + LEX)
- Rendering abstraction layer
- Scene graph traversal
- Build system

### 🚧 Stub Implementation

- OpenGL renderer (prints traversal but doesn't render)
- Window management (no GLFW integration yet)
- Camera controls (interface defined but inactive)

### 📋 To Be Implemented

- Full OpenGL 3.3+ rendering
- GLFW window and input handling
- Geometry generation (sphere, cube, cone, cylinder)
- Texture loading (GIF support)
- Material and lighting calculations
- Interactive navigation controls

## Next Steps

To complete the modern port:

1. **Install OpenGL dependencies:**
   ```bash
   sudo apt-get install libglfw3-dev libglew-dev
   ```

2. **Uncomment OpenGL headers** in `src/renderer/OpenGLRenderer.cpp`

3. **Implement geometry generation** (sphere, cube, etc.)

4. **Add GLFW window creation** and event handling

5. **Build with OpenGL:**
   ```bash
   make OPENGL=1
   ```

6. **Test with VRML files:**
   ```bash
   ./bin/labyrinth examples/simple.wrl
   ```

## License

Original (c) 1995, The Community Company
Modern port (c) 2025

This is a historical reconstruction of the world's first VRML browser.
