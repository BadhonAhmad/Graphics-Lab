#include <GL/gl.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;

// Global variable for rotation angle
float rotationAngle = 0.0f;
float rotationStep = 5.0f; // Degrees to rotate per key press

void drawSquare()
{
    glBegin(GL_QUADS);

    // Define square vertices (centered at origin)
    glVertex2f(-30.0f, -30.0f); // Bottom-left
    glVertex2f(30.0f, -30.0f);  // Bottom-right
    glVertex2f(30.0f, 30.0f);   // Top-right
    glVertex2f(-30.0f, 30.0f);  // Top-left

    glEnd();

    // Draw outline
    glColor3f(1.0, 1.0, 1.0); // White outline
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-30.0f, -30.0f);
    glVertex2f(30.0f, -30.0f);
    glVertex2f(30.0f, 30.0f);
    glVertex2f(-30.0f, 30.0f);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix(); // Save current matrix

    // Rotate the square
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f); // Rotate around Z-axis

    // Draw filled square (red)
    glColor3f(1.0, 0.0, 0.0);
    drawSquare();

    glPopMatrix(); // Restore matrix

    glFlush();
}

void specialKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        rotationAngle += rotationStep; // Rotate counter-clockwise
        cout << "Left arrow: Rotating counter-clockwise - Angle: " << rotationAngle << " degrees" << endl;
        break;
    case GLUT_KEY_RIGHT:
        rotationAngle -= rotationStep; // Rotate clockwise
        cout << "Right arrow: Rotating clockwise - Angle: " << rotationAngle << " degrees" << endl;
        break;
    }

    // Keep angle in 0-360 range for cleaner display
    if (rotationAngle >= 360.0f)
        rotationAngle -= 360.0f;
    if (rotationAngle < 0.0f)
        rotationAngle += 360.0f;

    glutPostRedisplay(); // Request redraw
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -100, 100);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Rotating Square - Use Arrow Keys");

    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKey); // Register arrow key handler

    cout << "Controls:" << endl;
    cout << "Left Arrow  - Rotate counter-clockwise" << endl;
    cout << "Right Arrow - Rotate clockwise" << endl;

    glutMainLoop();
    return 0;
}
