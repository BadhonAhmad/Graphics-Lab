# OpenGL Graphics Course Project

This project contains OpenGL demos for graphics course assignments.

## Demo: Circle Drawing

The `circle_demo.c` program demonstrates how to draw a circle using OpenGL and GLUT.

## Prerequisites

### Windows Setup

1. **Install MinGW-w64** (C Compiler)
   - Download from: https://www.mingw-w64.org/downloads/
   - Or use MSYS2: https://www.msys2.org/
   - Make sure `gcc` is in your PATH

2. **Install FreeGLUT** (OpenGL Utility Toolkit)
   
   **Option A: Using MSYS2 (Recommended)**
   ```bash
   # Open MSYS2 terminal
   pacman -S mingw-w64-x86_64-freeglut
   pacman -S mingw-w64-x86_64-gcc
   ```

   **Option B: Manual Installation**
   - Download FreeGLUT from: https://www.transmissionzero.co.uk/software/freeglut-devel/
   - Extract to a location (e.g., `C:\freeglut`)
   - Update the Makefile or CMakeLists.txt with the correct paths

3. **Install CMake** (Optional, for CMake build)
   - Download from: https://cmake.org/download/

## Building the Project

### Method 1: Using Make (Simple)

```bash
# Build
make

# Run
make run

# Or run directly
./circle_demo.exe
```

### Method 2: Using CMake

```bash
# Create build directory
mkdir build
cd build

# Configure (adjust FreeGLUT path if needed)
cmake ..

# Build
cmake --build .

# Run
./circle_demo.exe
```

### Method 3: Direct Compilation

If FreeGLUT is in your system PATH:
```bash
gcc circle_demo.c -o circle_demo.exe -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
```

If FreeGLUT is not in PATH, specify include and library directories:
```bash
gcc circle_demo.c -o circle_demo.exe -I"C:/freeglut/include" -L"C:/freeglut/lib" -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
```

### Method 4: Using VS Code Tasks

Press `Ctrl+Shift+B` to build, or use the Terminal menu → Run Build Task.

## Expected Output

When you run the program, you should see:
- A window titled "Circle Drawing" (1366x768 pixels)
- A green circle drawn on a black background
- The circle has a radius of 200 pixels centered in the window

## Troubleshooting

### "Cannot find GL/glut.h"
- FreeGLUT is not installed or not in the include path
- Update the Makefile or CMakeLists.txt with correct paths
- Or install FreeGLUT via MSYS2

### "Cannot find -lfreeglut"
- FreeGLUT library is not in the library path
- Update the Makefile or CMakeLists.txt with correct library paths
- Make sure the DLL files are accessible (copy them to your project directory or add to PATH)

### "Missing DLL" error when running
- Copy `freeglut.dll` from FreeGLUT installation to your project directory
- Or add FreeGLUT bin directory to your system PATH

## Project Structure

```
OpenGL/
├── circle_demo.c          # Circle drawing demo
├── scanline_fill.cpp      # Scan line polygon fill algorithm
├── clipping.cpp           # Cohen-Sutherland line clipping algorithm
├── polygon_clipping.cpp   # Sutherland-Hodgman polygon clipping
├── flower.cpp             # Flower pattern demo
├── flag.cpp               # Flag drawing demo
├── bresenham_circle.cpp   # Bresenham circle algorithm
├── rotating_square.cpp    # Rotating square animation
├── Makefile              # Build configuration for Make
├── CMakeLists.txt        # Build configuration for CMake
└── README.md             # This file
```

## Learning Resources

- OpenGL Documentation: https://www.opengl.org/documentation/
- GLUT Tutorial: https://www.opengl.org/resources/libraries/glut/
- LearnOpenGL: https://learnopengl.com/

## Graphics Algorithms Demos

### 1. Scan Line Fill Algorithm (`scanline_fill.cpp`)

Demonstrates the scan line polygon fill algorithm.

**How to use:**
- Click to add polygon vertices
- Press 'f' to fill the polygon
- Press 'c' to clear and start over

**Build:**
```bash
g++ scanline_fill.cpp -o scanline_fill.exe -Ifreeglut/freeglut/include -Lfreeglut/freeglut/lib -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
./scanline_fill.exe
```

### 2. Cohen-Sutherland Line Clipping (`clipping.cpp`)

Demonstrates the Cohen-Sutherland line clipping algorithm against a rectangular window.

**How to use:**
- Press '1' to show predefined test lines
- Press '2' to draw your own lines (click two points per line)
- Press 'c' to clear

**Visualization:**
- Red dashed lines: Original lines
- Green solid lines: Clipped lines (parts inside clipping window)
- Black rectangle: Clipping window boundary

**Build:**
```bash
g++ clipping.cpp -o clipping.exe -Ifreeglut/freeglut/include -Lfreeglut/freeglut/lib -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
./clipping.exe
```

### 3. Sutherland-Hodgman Polygon Clipping (`polygon_clipping.cpp`)

Demonstrates the Sutherland-Hodgman polygon clipping algorithm.

**How to use:**
- Press '1' to show predefined polygon
- Press '2' to draw your own polygon (click to add vertices)
- Press 'f' to finish drawing and perform clipping
- Press 'c' to clear

**Visualization:**
- Red outlined polygon: Original polygon
- Green filled polygon: Clipped polygon (part inside clipping window)
- Black rectangle: Clipping window boundary

**Build:**
```bash
g++ polygon_clipping.cpp -o polygon_clipping.exe -Ifreeglut/freeglut/include -Lfreeglut/freeglut/lib -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
./polygon_clipping.exe
```

## Next Steps

Try modifying the demos:
- Change the circle color by modifying `glColor3f()` parameters
- Change the circle radius
- Draw multiple circles
- Add animation using `glutTimerFunc()`
- Draw other shapes (ellipse, rectangle, polygon)
- Modify clipping window boundaries in clipping algorithms
- Experiment with different polygon shapes in fill and clipping algorithms
