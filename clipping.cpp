/*
 * Line Clipping Algorithms - Cohen-Sutherland Algorithm
 * This program demonstrates the Cohen-Sutherland line clipping algorithm
 * Press '1' for predefined lines, '2' to draw your own, 'c' to clear
 */

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Clipping window boundaries
float xmin = 200, ymin = 150, xmax = 600, ymax = 450;

struct Point
{
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

struct Line
{
    Point p1, p2;
    Line(Point _p1, Point _p2) : p1(_p1), p2(_p2) {}
};

vector<Line> originalLines;
vector<Line> clippedLines;
Point tempPoint;
bool drawingLine = false;
int mode = 0; // 0: waiting, 1: predefined, 2: user drawing

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
}

// Compute region code for a point
int computeCode(float x, float y)
{
    int code = INSIDE;

    if (x < xmin)
        code |= LEFT;
    else if (x > xmax)
        code |= RIGHT;
    if (y < ymin)
        code |= BOTTOM;
    else if (y > ymax)
        code |= TOP;

    return code;
}

// Cohen-Sutherland line clipping algorithm
bool cohenSutherlandClip(Point &p1, Point &p2)
{
    int code1 = computeCode(p1.x, p1.y);
    int code2 = computeCode(p2.x, p2.y);
    bool accept = false;

    while (true)
    {
        if ((code1 == 0) && (code2 == 0))
        {
            // Both points inside
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // Both points in same outside region
            break;
        }
        else
        {
            // Line needs clipping
            int codeOut;
            float x, y;

            // Pick point outside clipping window
            if (code1 != 0)
                codeOut = code1;
            else
                codeOut = code2;

            // Find intersection point
            if (codeOut & TOP)
            {
                x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / (p2.y - p1.y);
                y = ymax;
            }
            else if (codeOut & BOTTOM)
            {
                x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / (p2.y - p1.y);
                y = ymin;
            }
            else if (codeOut & RIGHT)
            {
                y = p1.y + (p2.y - p1.y) * (xmax - p1.x) / (p2.x - p1.x);
                x = xmax;
            }
            else if (codeOut & LEFT)
            {
                y = p1.y + (p2.y - p1.y) * (xmin - p1.x) / (p2.x - p1.x);
                x = xmin;
            }

            // Replace point outside clipping window
            if (codeOut == code1)
            {
                p1.x = x;
                p1.y = y;
                code1 = computeCode(p1.x, p1.y);
            }
            else
            {
                p2.x = x;
                p2.y = y;
                code2 = computeCode(p2.x, p2.y);
            }
        }
    }

    return accept;
}

void performClipping()
{
    clippedLines.clear();

    for (const auto &line : originalLines)
    {
        Point p1 = line.p1;
        Point p2 = line.p2;

        if (cohenSutherlandClip(p1, p2))
        {
            clippedLines.push_back(Line(p1, p2));
        }
    }
}

void drawClippingWindow()
{
    // Draw clipping window
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();
    glLineWidth(1.0);

    // Label
    glRasterPos2f(xmin + 5, ymax - 15);
    string msg = "Clipping Window";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void drawLines()
{
    // Draw original lines in red (dashed effect)
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(1.0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);

    for (const auto &line : originalLines)
    {
        glBegin(GL_LINES);
        glVertex2f(line.p1.x, line.p1.y);
        glVertex2f(line.p2.x, line.p2.y);
        glEnd();
    }

    glDisable(GL_LINE_STIPPLE);

    // Draw clipped lines in green (solid)
    glColor3f(0.0, 0.8, 0.0);
    glLineWidth(3.0);

    for (const auto &line : clippedLines)
    {
        glBegin(GL_LINES);
        glVertex2f(line.p1.x, line.p1.y);
        glVertex2f(line.p2.x, line.p2.y);
        glEnd();
    }

    glLineWidth(1.0);
}

void loadPredefinedLines()
{
    originalLines.clear();

    // Lines demonstrating different clipping cases
    originalLines.push_back(Line(Point(100, 300), Point(700, 300))); // Horizontal through window
    originalLines.push_back(Line(Point(400, 100), Point(400, 500))); // Vertical through window
    originalLines.push_back(Line(Point(150, 200), Point(650, 400))); // Diagonal through window
    originalLines.push_back(Line(Point(100, 100), Point(200, 200))); // Outside left-bottom
    originalLines.push_back(Line(Point(650, 500), Point(750, 550))); // Outside right-top
    originalLines.push_back(Line(Point(300, 250), Point(500, 350))); // Completely inside
    originalLines.push_back(Line(Point(100, 500), Point(700, 100))); // Diagonal across
    originalLines.push_back(Line(Point(250, 100), Point(550, 500))); // Vertical diagonal

    performClipping();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawClippingWindow();

    if (mode > 0)
    {
        drawLines();
    }

    // Draw temporary line while drawing
    if (drawingLine)
    {
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_LINES);
        glVertex2f(tempPoint.x, tempPoint.y);
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glEnd();
    }

    // Instructions
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(10, 580);
    string msg = "Press '1': Predefined Lines | '2': Draw Lines | 'c': Clear | ESC: Exit";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    glRasterPos2f(10, 560);
    msg = "Red (dashed): Original | Green (solid): Clipped";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    if (mode == 2 && !drawingLine)
    {
        glRasterPos2f(10, 540);
        msg = "Click two points to draw a line";
        for (char c : msg)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    if (mode != 2)
        return;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float glX = x;
        float glY = 600 - y;

        if (!drawingLine)
        {
            tempPoint = Point(glX, glY);
            drawingLine = true;
        }
        else
        {
            originalLines.push_back(Line(tempPoint, Point(glX, glY)));
            performClipping();
            drawingLine = false;
        }

        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        mode = 1;
        loadPredefinedLines();
        drawingLine = false;
        glutPostRedisplay();
        break;
    case '2':
        mode = 2;
        originalLines.clear();
        clippedLines.clear();
        drawingLine = false;
        glutPostRedisplay();
        break;
    case 'c':
    case 'C':
        originalLines.clear();
        clippedLines.clear();
        drawingLine = false;
        mode = 0;
        glutPostRedisplay();
        break;
    case 27: // ESC
        exit(0);
        break;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen-Sutherland Line Clipping Algorithm");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    cout << "Cohen-Sutherland Line Clipping Algorithm\n";
    cout << "----------------------------------------\n";
    cout << "Press '1' to show predefined test lines\n";
    cout << "Press '2' to draw your own lines\n";
    cout << "Press 'c' to clear\n";
    cout << "Press ESC to exit\n";
    cout << "\nClipping Window: (" << xmin << "," << ymin << ") to ("
         << xmax << "," << ymax << ")\n";

    glutMainLoop();
    return 0;
}
