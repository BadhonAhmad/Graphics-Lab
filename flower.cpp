// C++ program to draw a flower pattern using Bresenham's Line Algorithm
// Creates a flower pattern using lines
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

// Window dimensions
const int WINDOW_WIDTH = 1366;
const int WINDOW_HEIGHT = 768;

// Flower parameters
const int NUM_PETALS = 8;
const int PETAL_LENGTH = 200;

// Flower parameters
const int NUM_PETALS = 8;
const int PETAL_LENGTH = 200;

// Function to set a pixel
void setPixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham's Line Drawing Algorithm (for lines with slope 0 < m < 1)
void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int x = x1, y = y1;

    while (true)
    {
        setPixel(x, y);

        if (x == x2 && y == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}
}
}

// Function to draw a petal using lines
void drawPetal(int cx, int cy, double angle, int length)
{
    // Calculate petal tip
    int tipX = cx + (int)(length * cos(angle));
    int tipY = cy + (int)(length * sin(angle));

    // Calculate petal base points (two points on either side)
    double baseAngle1 = angle - 0.3; // Left base point
    double baseAngle2 = angle + 0.3; // Right base point
    int baseLength = length / 3;

    int base1X = cx + (int)(baseLength * cos(baseAngle1));
    int base1Y = cy + (int)(baseLength * sin(baseAngle1));

    int base2X = cx + (int)(baseLength * cos(baseAngle2));
    int base2Y = cy + (int)(baseLength * sin(baseAngle2));

    // Draw petal outline
    drawLine(cx, cy, tipX, tipY);             // Center to tip
    drawLine(base1X, base1Y, tipX, tipY);     // Left base to tip
    drawLine(base2X, base2Y, tipX, tipY);     // Right base to tip
    drawLine(base1X, base1Y, base2X, base2Y); // Connect base points
}

// Function to draw a circle using lines (for center)
void drawCircleWithLines(int cx, int cy, int radius)
{
    int numSegments = 50;
    for (int i = 0; i < numSegments; i++)
    {
        double angle1 = 2.0 * M_PI * i / numSegments;
        double angle2 = 2.0 * M_PI * (i + 1) / numSegments;

        int x1 = cx + (int)(radius * cos(angle1));
        int y1 = cy + (int)(radius * sin(angle1));
        int x2 = cx + (int)(radius * cos(angle2));
        int y2 = cy + (int)(radius * sin(angle2));

        drawLine(x1, y1, x2, y2);
    }
}

// Initialize OpenGL settings
void myInit(void)
{
    // Black background
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Set point size
    glPointSize(2.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set coordinate system
    gluOrtho2D(-WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2);
}

// Display callback function
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Center coordinates
    int cx = 0;
    int cy = 0;

    // Set color to white for entire flower
    glColor3f(1.0, 1.0, 1.0);

    // Draw all petals
    for (int i = 0; i < NUM_PETALS; i++)
    {
        double angle = 2.0 * M_PI * i / NUM_PETALS;
        drawPetal(cx, cy, angle, PETAL_LENGTH);
    }

    // Draw center circle
    drawCircleWithLines(cx, cy, 30);

    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Set window size
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);

    // Create window
    glutCreateWindow("Flower Pattern - Bresenham's Line Algorithm");

    myInit();

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
