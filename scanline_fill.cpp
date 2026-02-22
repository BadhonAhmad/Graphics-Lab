/*
 * Scan Line Polygon Fill Algorithm
 * This program demonstrates the scan line fill algorithm for filling polygons
 * Click to add polygon vertices, press 'f' to fill, 'c' to clear
 */

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Point
{
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

struct Edge
{
    float ymax, xmin, slope_inv;
    Edge(float ym, float xm, float si) : ymax(ym), xmin(xm), slope_inv(si) {}
};

vector<Point> polygon;
bool fillMode = false;

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
}

void drawPolygon()
{
    if (polygon.size() < 2)
        return;

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (const auto &p : polygon)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // Draw vertices
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (const auto &p : polygon)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void scanLineFill()
{
    if (polygon.size() < 3)
        return;

    // Find ymin and ymax
    float ymin = polygon[0].y, ymax = polygon[0].y;
    for (const auto &p : polygon)
    {
        if (p.y < ymin)
            ymin = p.y;
        if (p.y > ymax)
            ymax = p.y;
    }

    // Build edge table
    vector<Edge> edgeTable;
    int n = polygon.size();

    for (int i = 0; i < n; i++)
    {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % n];

        // Skip horizontal edges
        if (p1.y == p2.y)
            continue;

        // Ensure p1 is the lower point
        if (p1.y > p2.y)
            swap(p1, p2);

        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float slope_inv = dx / dy;

        edgeTable.push_back(Edge(p2.y, p1.x, slope_inv));
    }

    // Scan line algorithm
    glColor3f(0.5, 0.7, 1.0); // Light blue fill

    for (float y = ymin; y <= ymax; y += 1.0)
    {
        vector<float> intersections;

        // Find intersections with active edges
        for (auto &edge : edgeTable)
        {
            if (y < edge.ymax)
            {
                // Calculate x at current y
                float x = edge.xmin;
                // Adjust x for current scanline
                float dy_from_start = y - (edge.ymax - (edge.ymax - ymin));

                // Recalculate proper intersection
                // For each edge, we need to track current x value
                intersections.push_back(x);
            }
        }

        // Recalculate intersections properly
        intersections.clear();
        for (int i = 0; i < n; i++)
        {
            Point p1 = polygon[i];
            Point p2 = polygon[(i + 1) % n];

            if (p1.y > p2.y)
                swap(p1, p2);

            // Check if scanline intersects this edge
            if (y >= p1.y && y < p2.y)
            {
                if (p2.y != p1.y)
                {
                    float x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                    intersections.push_back(x);
                }
            }
        }

        // Sort intersections
        sort(intersections.begin(), intersections.end());

        // Fill between pairs of intersections
        glBegin(GL_LINES);
        for (size_t i = 0; i + 1 < intersections.size(); i += 2)
        {
            glVertex2f(intersections[i], y);
            glVertex2f(intersections[i + 1], y);
        }
        glEnd();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (fillMode)
    {
        scanLineFill();
    }

    drawPolygon();

    // Draw instructions
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(10, 580);
    string msg = "Click to add vertices | Press 'f' to fill | Press 'c' to clear | Press 'ESC' to exit";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Convert window coordinates to OpenGL coordinates
        float glX = x;
        float glY = 600 - y;

        polygon.push_back(Point(glX, glY));
        fillMode = false;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'f':
    case 'F':
        if (polygon.size() >= 3)
        {
            fillMode = true;
            glutPostRedisplay();
        }
        break;
    case 'c':
    case 'C':
        polygon.clear();
        fillMode = false;
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
    glutCreateWindow("Scan Line Fill Algorithm");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    cout << "Scan Line Fill Algorithm Demo\n";
    cout << "------------------------------\n";
    cout << "Click to add polygon vertices\n";
    cout << "Press 'f' to fill the polygon\n";
    cout << "Press 'c' to clear\n";
    cout << "Press ESC to exit\n";

    glutMainLoop();
    return 0;
}
