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
```
OpenGL/
├── circle_demo.c                          # Circle drawing demo
├── scanline_fill.cpp                      # Scan line polygon fill algorithm
├── clipping.cpp                           # Cohen-Sutherland line clipping
├── polygon_clipping.cpp                   # Sutherland-Hodgman polygon clipping
├── flower.cpp                             # Flower pattern demo
├── flag.cpp                               # Flag drawing demo
├── bresenham_circle.cpp                   # Bresenham circle algorithm
├── rotating_square.cpp                    # Rotating square animation
├── advanced_composite_transformations.cpp # 🎓 2D transformations via matrix multiplication
├── advanced_cohen_sutherland.cpp          # 🎓 Cohen-Sutherland line clipping (complete)
├── advanced_weiler_atherton.cpp           # 🎓 Weiler-Atherton polygon clipping
├── Makefile                              # Build configuration for Make
├── CMakeLists.txt                        # Build configuration for CMake
└── README.md                             # This file
```

**🎓 = Advanced Graphics Lab Programs** (ready for challenging lab assignments!)


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

---

## 🎓 Advanced Graphics Lab Programs

### 4. Composite Matrix Transformations (`advanced_composite_transformations.cpp`)

**Advanced Implementation of 2D transformations using homogeneous coordinates and matrix multiplication.**

**Features:**
- Custom 3×3 transformation matrix implementation
- Homogeneous coordinate system (x, y, w)
- Composite transformations via matrix multiplication
- Demonstrates order dependency (T×R ≠ R×T)
- Interactive transformation controls
- Real-time matrix display
- Visual comparison of transformation orders

**Mathematical Concepts:**
- Translation matrix: T(tx, ty)
- Rotation matrix: R(θ)
- Scaling matrix: S(sx, sy)
- Composite: M = T × R × S (rightmost applied first)

**Controls:**
- `r/R`: Rotate counter-clockwise/clockwise (5° increments)
- Arrow keys: Translate object
- `+/-`: Scale up/down
- `1`: Apply Translation × Rotation (T × R)
- `2`: Apply Rotation × Translation (R × T)
- `3`: Apply full composite (T × R × S)
- `p`: Toggle transformation matrix display
- `x`: Reset all transformations
- `Space`: Toggle animation

**Build:**
```bash
g++ advanced_composite_transformations.cpp -o advanced_composite_transformations.exe -Ifreeglut/freeglut/include -Lfreeglut/freeglut/lib -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
./advanced_composite_transformations.exe
```

**Key Learning:**
Observe how the ORDER of matrix multiplication matters! Compare modes 1 and 2 to see how T×R produces different results than R×T.

---

### 5. Cohen-Sutherland Line Clipping (`advanced_cohen_sutherland.cpp`)

**Complete implementation of the Cohen-Sutherland line clipping algorithm with region codes.**

**Features:**
- 4-bit region code calculation (TBRL: Top, Bottom, Right, Left)
- Trivial acceptance test (both codes = 0000)
- Trivial rejection test (code1 & code2 ≠ 0)
- Iterative clipping against window boundaries
- Multiple test cases demonstrating all scenarios
- Interactive line drawing mode
- Region code visualization
- Step-by-step algorithm demonstration

**Algorithm Steps:**
1. Compute region codes for both endpoints
2. If both codes are 0000 → line completely inside (accept)
3. If (code1 & code2) ≠ 0 → line completely outside (trivial reject)
4. Otherwise, find intersection with boundary and clip
5. Repeat until line is accepted or rejected

**Region Codes (4-bit binary):**
```
Bit 3 (1000): TOP    - y > ymax
Bit 2 (0100): BOTTOM - y < ymin
Bit 1 (0010): RIGHT  - x > xmax
Bit 0 (0001): LEFT   - x < xmin
```

**Controls:**
- `1`: Load comprehensive test cases (10 different scenarios)
- `2`: Interactive drawing mode (click two points per line)
- `r`: Toggle region code display
- `+/-`: Adjust clipping window size
- `c`: Clear all lines

**Build:**
```bash
g++ advanced_cohen_sutherland.cpp -o advanced_cohen_sutherland.exe -Ifreeglut/freeglut/include -Lfreeglut/freeglut/lib -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
./advanced_cohen_sutherland.exe
```

**Visualization:**
- Red dashed: Lines completely outside (rejected)
- Orange dashed: Lines needing clipping
- Green dashed: Lines completely inside
- Dark green solid: Final clipped result

---

### 6. Weiler-Atherton Polygon Clipping (`advanced_weiler_atherton.cpp`)

**Advanced polygon clipping algorithm that can handle complex cases.**

**Features:**
- Complete Weiler-Atherton algorithm implementation
- Handles convex clip polygons
- Supports simple concave subject polygons
- Intersection point calculation and visualization
- Entry/exit point determination
- Vertex list traversal
- Can produce multiple clipped polygons
- Interactive polygon drawing
- Multiple test cases

**Algorithm Overview:**
1. Find all intersection points between subject and clip polygons
2. Insert intersection points into both vertex lists
3. Mark each intersection as "entry" or "exit" point
4. Start traversal from an entry point
5. Follow subject polygon edges until exit point
6. Switch to clip polygon and follow until next entry
7. Repeat until back to starting point
8. Result is the clipped polygon

**Advantages over Sutherland-Hodgman:**
- More general (can clip against concave clip polygons)
- Can produce multiple disjoint clipped regions
- Better suited for complex polygon operations

**Controls:**
- `1`: Load test case (cycles through predefined examples)
- `2`: Draw subject polygon (polygon to be clipped)
- `3`: Draw clip polygon (clipping window polygon)
- Click: Add vertices when in drawing mode
- `f`: Finish drawing current polygon
- `p`: Perform Weiler-Atherton clipping
- `i`: Toggle intersection points display
- `n`: Next test case
- `c`: Clear all polygons

**Build:**
```bash
g++ advanced_weiler_atherton.cpp -o advanced_weiler_atherton.exe -Ifreeglut/freeglut/include -Lfreeglut/freeglut/lib -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
./advanced_weiler_atherton.exe
```

**Visualization:**
- Orange: Subject polygon (to be clipped)
- Blue: Clip polygon (clipping window)
- Green: Clipped result polygon
- Magenta circles: Intersection points

---

## Build All Advanced Programs

```bash
# Composite Matrix Transformations
g++ advanced_composite_transformations.cpp -o advanced_composite_transformations.exe -Ifreeglut/freeglut/include -Lfreeglut/freeglut/lib -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32

# Cohen-Sutherland Line Clipping
g++ advanced_cohen_sutherland.cpp -o advanced_cohen_sutherland.exe -Ifreeglut/freeglut/include -Lfreeglut/freeglut/lib -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32

# Weiler-Atherton Polygon Clipping
g++ advanced_weiler_atherton.cpp -o advanced_weiler_atherton.exe -Ifreeglut/freeglut/include -Lfreeglut/freeglut/lib -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
```

---

## Next Steps

Try modifying the demos:
- Change the circle color by modifying `glColor3f()` parameters
- Change the circle radius
- Draw multiple circles
- Add animation using `glutTimerFunc()`
- Draw other shapes (ellipse, rectangle, polygon)
- Modify clipping window boundaries in clipping algorithms
- Experiment with different polygon shapes in fill and clipping algorithms
