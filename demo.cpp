#include <GL/gl.h>
#include <GL/freeglut.h>

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <cmath>

using namespace std;

vector<pair<int, int>> mv = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

map<pair<int, int>, bool> filledRect;
map<pair<int, int>, bool> filledCircle;

// Bangladesh flag dimensions: rectangle 10:6 ratio, circle radius ~20% of flag length
// Flag background: green, Circle: red
// Circle center is slightly left of center
void printPoint(int x, int y)
{
    glColor3f(1.0, 0.0, 0.0); // Red outline
    glVertex2i(x, y);
}
bool validRect(int px, int py)
{
    return (px >= -50 && px <= 50 && py >= -30 && py <= 30);
}

bool validCircle(int px, int py)
{
    // Circle center at (-5, 0) - slightly left of center
    int cx = px + 5;
    int cy = py;
    return (cx * cx + cy * cy <= 400); // radius = 20
}

// rectangle flood fill
void fillRect(int px, int py)
{
    if (!validRect(px, py))
        return;
    if (filledRect[{px, py}])
        return;

    filledRect[{px, py}] = true;
    glutPostRedisplay();

    for (auto p : mv)
        fillRect(px + p.first, py + p.second);
}

// circle flood fill
void fillCircle(int px, int py)
{
    if (!validCircle(px, py))
        return;
    if (filledCircle[{px, py}])
        return;

    filledCircle[{px, py}] = true;
    glutPostRedisplay();
    for (auto p : mv)
        fillCircle(px + p.first, py + p.second);
}

void drawRectBorder()
{
    // Draw the outline of the flag rectangle
    glColor3f(0.0, 1.0, 0.0); // Green outline
    // Top border
    for (int i = -50; i <= 50; i++)
        glVertex2i(i, 30);
    // Bottom border
    for (int i = -50; i <= 50; i++)
        glVertex2i(i, -30);
    // Left border
    for (int j = -30; j <= 30; j++)
        glVertex2i(-50, j);
    // Right border
    for (int j = -30; j <= 30; j++)
        glVertex2i(50, j);
}

void drawCircleBorder()
{
    // Draw the outline of the red circle (slightly left of center)
    for (int angle = 0; angle < 360; angle++)
    {
        float rad = angle * 3.14159f / 180.0f;
        int x = (int)(-5 + 20 * cos(rad)); // Center at (-5, 0), radius 20
        int y = (int)(0 + 20 * sin(rad));
        printPoint(x, y);
    }
}

void drawFilled()
{
    glPointSize(3);
    glBegin(GL_POINTS);

    // rectangle fill (green area)
    glColor3f(0.0, 0.5, 0.0); // Dark green fill for background
    for (auto &p : filledRect)
        glVertex2i(p.first.first, p.first.second);

    // circle fill (red area)
    glColor3f(0.8, 0.0, 0.0); // Dark red fill for circle
    for (auto &p : filledCircle)
        glVertex2i(p.first.first, p.first.second);

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(2);
    glBegin(GL_POINTS);

    // Draw Bangladesh flag background and circle
    drawRectBorder();
    drawCircleBorder();

    glEnd();

    // Draw filled points on top
    drawFilled();

    glFlush();
}

void mouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Convert window coordinates to OpenGL coordinates
        int ox = (mx - 300) / 3; // Scale to match our coordinate system
        int oy = (300 - my) / 3;

        cout << "Clicked at: (" << ox << ", " << oy << ")" << endl;

        if (validCircle(ox, oy))
        {
            cout << "Filling circle (red area)" << endl;
            fillCircle(ox, oy);
        }
        else if (validRect(ox, oy))
        {
            cout << "Filling rectangle (green area)" << endl;
            fillRect(ox, oy);
        }

        glutPostRedisplay();
    }
}

void specialKey(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
    {
        // Left arrow key fills the circle (red area)
        cout << "Left arrow pressed - Filling circle (red area)" << endl;
        fillCircle(-5, 0); // Start from center of circle
        glutPostRedisplay();
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        // Right arrow key fills the rectangle (green area)
        cout << "Right arrow pressed - Filling rectangle (green area)" << endl;
        fillRect(0, 0); // Start from a point in the rectangle
        glutPostRedisplay();
    }
}

void init()
{
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -100, 100);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bangladesh Flag - Click to Fill");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutSpecialFunc(specialKey);

    glutMainLoop();
    return 0;
}
