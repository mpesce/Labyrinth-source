#!/usr/bin/env python3
"""
Create simple test images for VRML texture testing
Generates PPM (Portable Pixmap) format images
"""

def create_checkered(filename, width=64, height=64, square_size=8):
    """Create a checkered pattern image"""
    with open(filename, 'wb') as f:
        # PPM header
        f.write(f"P6\n{width} {height}\n255\n".encode())

        # Pixel data (RGB)
        for y in range(height):
            for x in range(width):
                # Checkered pattern
                is_white = ((x // square_size) % 2) == ((y // square_size) % 2)
                color = 255 if is_white else 128
                f.write(bytes([color, color, color]))

    print(f"Created {filename}: {width}x{height} checkered pattern")

def create_gradient(filename, width=64, height=64):
    """Create a color gradient image"""
    with open(filename, 'wb') as f:
        # PPM header
        f.write(f"P6\n{width} {height}\n255\n".encode())

        # Pixel data - horizontal gradient from red to blue
        for y in range(height):
            for x in range(width):
                r = int(255 * (1 - x / width))
                g = int(128 * y / height)
                b = int(255 * x / width)
                f.write(bytes([r, g, b]))

    print(f"Created {filename}: {width}x{height} color gradient")

def create_grid(filename, width=64, height=64, grid_size=8):
    """Create a grid pattern"""
    with open(filename, 'wb') as f:
        # PPM header
        f.write(f"P6\n{width} {height}\n255\n".encode())

        # Pixel data
        for y in range(height):
            for x in range(width):
                # Grid lines
                is_grid = (x % grid_size == 0) or (y % grid_size == 0)
                if is_grid:
                    f.write(bytes([0, 0, 0]))  # Black lines
                else:
                    f.write(bytes([200, 200, 200]))  # Light gray

    print(f"Created {filename}: {width}x{height} grid pattern")

def create_solid_color(filename, r, g, b, width=64, height=64):
    """Create a solid color image"""
    with open(filename, 'wb') as f:
        # PPM header
        f.write(f"P6\n{width} {height}\n255\n".encode())

        # Pixel data
        for _ in range(width * height):
            f.write(bytes([r, g, b]))

    print(f"Created {filename}: {width}x{height} solid color ({r},{g},{b})")

if __name__ == "__main__":
    import os
    os.makedirs("examples/textures", exist_ok=True)

    # Create test images
    create_checkered("examples/textures/checker.ppm")
    create_gradient("examples/textures/gradient.ppm")
    create_grid("examples/textures/grid.ppm")
    create_solid_color("examples/textures/red.ppm", 255, 0, 0)
    create_solid_color("examples/textures/green.ppm", 0, 255, 0)
    create_solid_color("examples/textures/blue.ppm", 0, 0, 255)

    print("\nAll test images created successfully!")
    print("Note: These are PPM format. You can view them with most image viewers.")
    print("To use in VRML, reference them like: filename 'textures/checker.ppm'")
