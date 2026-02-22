# Makefile for OpenGL Circle Demo
# For Windows with MinGW

# Compiler
CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -Wall -O2
CXXFLAGS = -Wall -O2

# Libraries
# Adjust these paths if FreeGLUT is installed in a different location
LIBS = -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32

# Include directories (adjust if needed)
INCLUDES = -I"freeglut/freeglut/include"

# Library directories (adjust if needed)
LIBDIRS = -L"freeglut/freeglut/lib"

# Target executables
TARGET1 = circle_demo.exe
TARGET2 = flower.exe

# Source files
SOURCES1 = circle_demo.c
SOURCES2 = flower.cpp

# Default target
all: $(TARGET1) $(TARGET2)

# Build circle demo
$(TARGET1): $(SOURCES1)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES1) $(LIBDIRS) $(LIBS) -o $(TARGET1)

# Build flower pattern
$(TARGET2): $(SOURCES2)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES2) $(LIBDIRS) $(LIBS) -o $(TARGET2)

# Clean targs
run-circle: $(TARGET1)
	./$(TARGET1)

run-flower: $(TARGET2)
	./$(TARGET2)

run: run-flower

.PHONY: all clean run run-circle run-flower
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
