# Test Textures

This directory contains simple test texture images in PPM (Portable Pixmap) format.

## Available Textures

- **checker.ppm** - 64x64 black and white checkered pattern
- **gradient.ppm** - 64x64 color gradient (red to blue)
- **grid.ppm** - 64x64 grid pattern with black lines
- **red.ppm** - 64x64 solid red
- **green.ppm** - 64x64 solid green
- **blue.ppm** - 64x64 solid blue

## PPM Format

PPM (Portable Pixmap) is a very simple uncompressed image format:
- No external libraries required
- Plain text header followed by binary RGB data
- Easy to generate and parse
- Supported by most image viewers

## Regenerating Textures

To recreate or modify these textures, run:

```bash
python3 create_test_images.py
```

The script is located in the root directory and will regenerate all test images.

## Usage in VRML

Reference these textures in your VRML files like:

```vrml
Texture2 {
    filename "examples/textures/checker.ppm"
}
```

The path is relative to where you run the labyrinth executable from.
