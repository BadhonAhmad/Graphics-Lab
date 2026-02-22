/*
 * Polygon Clipping - Sutherland-Hodgman Algorithm
 * This program demonstrates the Sutherland-Hodgman polygon clipping algorithm
 * Press '1' for predefined polygon, '2' to draw your own, 'c' to clear
 */

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Clipping window boundaries
float xmin = 200, ymin = 150, xmax = 600, ymax = 450;

struct Point
{
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

vector<Point> originalPolygon;
vector<Point> clippedPolygon;
int mode = 0; // 0: waiting, 1: predefined, 2: user drawing

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
}

// Edge clipping functions
enum EdgeType
{
    LEFT_EDGE,
    RIGHT_EDGE,
    BOTTOM_EDGE,
    TOP_EDGE
};

bool inside(Point p, EdgeType edge)
{
    switch (edge)
    {
    case LEFT_EDGE:
        return p.x >= xmin;
    case RIGHT_EDGE:
        return p.x <= xmax;
    case BOTTOM_EDGE:
        return p.y >= ymin;
    case TOP_EDGE:
        return p.y <= ymax;
    }
    return false;
}

Point intersection(Point p1, Point p2, EdgeType edge)
{
    Point intersect;
    float m;

    if (p2.x != p1.x)
    {
        m = (p2.y - p1.y) / (p2.x - p1.x);
    }

    switch (edge)
    {
    case LEFT_EDGE:
        intersect.x = xmin;
        intersect.y = p1.y + m * (xmin - p1.x);
        break;
    case RIGHT_EDGE:
        intersect.x = xmax;
        intersect.y = p1.y + m * (xmax - p1.x);
        break;
    case BOTTOM_EDGE:
        intersect.y = ymin;
        if (p2.x != p1.x)
            intersect.x = p1.x + (ymin - p1.y) / m;
        else
            intersect.x = p1.x;
        break;
    case TOP_EDGE:
        intersect.y = ymax;
        if (p2.x != p1.x)
            intersect.x = p1.x + (ymax - p1.y) / m;
        else
            intersect.x = p1.x;
        break;
    }

    return intersect;
}

vector<Point> clipPolygonEdge(vector<Point> polygon, EdgeType edge)
{
    vector<Point> output;

    if (polygon.size() == 0)
        return output;

    Point S = polygon[polygon.size() - 1];

    for (const auto &E : polygon)
    {
        if (inside(E, edge))
        {
            if (!inside(S, edge))
            {
                output.push_back(intersection(S, E, edge));
            }
            output.push_back(E);
        }
        else if (inside(S, edge))
        {
            output.push_back(intersection(S, E, edge));
        }
        S = E;
    }

    return output;
}

void sutherlandHodgmanClip()
{
    clippedPolygon = originalPolygon;

    // Clip against each edge of the clipping window
    clippedPolygon = clipPolygonEdge(clippedPolygon, LEFT_EDGE);
    clippedPolygon = clipPolygonEdge(clippedPolygon, RIGHT_EDGE);
    clippedPolygon = clipPolygonEdge(clippedPolygon, BOTTOM_EDGE);
    clippedPolygon = clipPolygonEdge(clippedPolygon, TOP_EDGE);
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

void drawPolygons()
{
    // Draw original polygon in red (outlined)
    if (originalPolygon.size() > 0)
    {
        glColor3f(1.0, 0.8, 0.8);
        glBegin(GL_POLYGON);
        for (const auto &p : originalPolygon)
        {
            glVertex2f(p.x, p.y);
        }
        glEnd();

        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        for (const auto &p : originalPolygon)
        {
            glVertex2f(p.x, p.y);
        }
        glEnd();

        // Draw vertices
        glPointSize(5.0);
        glBegin(GL_POINTS);
        for (const auto &p : originalPolygon)
        {
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }

    // Draw clipped polygon in green (filled)
    if (clippedPolygon.size() > 0)
    {
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_POLYGON);
        for (const auto &p : clippedPolygon)
        {
            glVertex2f(p.x, p.y);
        }
        glEnd();

        glColor3f(0.0, 0.5, 0.0);
        glLineWidth(3.0);
        glBegin(GL_LINE_LOOP);
        for (const auto &p : clippedPolygon)
        {
            glVertex2f(p.x, p.y);
        }
        glEnd();

        // Draw vertices
        glPointSize(6.0);
        glBegin(GL_POINTS);
        for (const auto &p : clippedPolygon)
        {
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }

    glLineWidth(1.0);
    glPointSize(1.0);
}

void loadPredefinedPolygon()
{
    originalPolygon.clear();

    // Create a polygon that extends beyond the clipping window
    originalPolygon.push_back(Point(100, 300));
    originalPolygon.push_back(Point(300, 100));
    originalPolygon.push_back(Point(500, 200));
    originalPolygon.push_back(Point(700, 300));
    originalPolygon.push_back(Point(600, 500));
    originalPolygon.push_back(Point(300, 550));

    sutherlandHodgmanClip();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawClippingWindow();

    if (mode > 0)
    {
        drawPolygons();
    }

    // Instructions
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(10, 580);
    string msg = "Press '1': Predefined Polygon | '2': Draw Polygon | 'f': Finish & Clip | 'c': Clear | ESC: Exit";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    glRasterPos2f(10, 560);
    msg = "Red: Original Polygon | Green: Clipped Polygon";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    if (mode == 2 && clippedPolygon.empty())
    {
        glRasterPos2f(10, 540);
        msg = "Click to add vertices, press 'f' to finish and clip";
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

        originalPolygon.push_back(Point(glX, glY));
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        mode = 1;
        loadPredefinedPolygon();
        glutPostRedisplay();
        break;
    case '2':
        mode = 2;
        originalPolygon.clear();
        clippedPolygon.clear();
        glutPostRedisplay();
        break;
    case 'f':
    case 'F':
        if (mode == 2 && originalPolygon.size() >= 3)
        {
            sutherlandHodgmanClip();
            glutPostRedisplay();
        }
        break;
    case 'c':
    case 'C':
        originalPolygon.clear();
        clippedPolygon.clear();
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
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping Algorithm");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    cout << "Sutherland-Hodgman Polygon Clipping Algorithm\n";
    cout << "---------------------------------------------\n";
    cout << "Press '1' to show predefined polygon\n";
    cout << "Press '2' to draw your own polygon\n";
    cout << "Press 'f' to finish drawing and clip\n";
    cout << "Press 'c' to clear\n";
    cout << "Press ESC to exit\n";
    cout << "\nClipping Window: (" << xmin << "," << ymin << ") to ("
         << xmax << "," << ymax << ")\n";

    glutMainLoop();
    return 0;
}
