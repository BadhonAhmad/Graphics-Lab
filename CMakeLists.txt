cmake_minimum_required(VERSION 3.10)
project(OpenGL_Graphics_Course)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add executable
add_executable(circle_demo circle_demo.c)

# Link libraries
# On Windows, you'll need to link against freeglut or glut
# Adjust the paths based on your FreeGLUT installation
target_link_libraries(circle_demo ${OPENGL_LIBRARIES})

# If using FreeGLUT, uncomment and adjust paths:
# target_include_directories(circle_demo PRIVATE "C:/freeglut/include")
# target_link_directories(circle_demo PRIVATE "C:/freeglut/lib")
# target_link_libraries(circle_demo freeglut ${OPENGL_LIBRARIES})

# For Windows, also link against standard libraries
if(WIN32)
    target_link_libraries(circle_demo winmm gdi32)
endif()
