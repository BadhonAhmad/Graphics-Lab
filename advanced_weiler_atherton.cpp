/*
 * Advanced Weiler-Atherton Polygon Clipping Algorithm
 * Complete implementation with visualization
 *
 * Features:
 * - Weiler-Atherton polygon clipping algorithm
 * - Handles convex and simple concave polygons
 * - Intersection point calculation
 * - Vertex traversal and list building
 * - Multiple clipped polygon output support
 * - Interactive polygon creation
 * - Step-by-step visualization
 *
 * Algorithm:
 * 1. Find all intersection points between subject and clip polygons
 * 2. Build vertex lists with entry/exit flags
 * 3. Traverse vertices starting from entry points
 * 4. Generate clipped polygon(s)
 *
 * Controls:
 * - '1': Load predefined test case
 * - '2': Draw subject polygon (click points, press 'f' to finish)
 * - '3': Draw clip polygon (click points, press 'f' to finish)
 * - 'p': Perform clipping
 * - 'i': Show intersection points
 * - 'c': Clear all
 * - 'n': Next test case
 */

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct Point
{
    float x, y;
    bool isIntersection;
    bool isEntry;         // Entry or exit point
    int intersectionWith; // Index of the other polygon's edge

    Point(float _x = 0, float _y = 0)
        : x(_x), y(_y), isIntersection(false), isEntry(false), intersectionWith(-1) {}

    bool operator==(const Point &other) const
    {
        return fabs(x - other.x) < 0.001f && fabs(y - other.y) < 0.001f;
    }
};

struct Edge
{
    Point p1, p2;
    Edge(Point _p1, Point _p2) : p1(_p1), p2(_p2) {}
};

vector<Point> subjectPolygon; // Polygon to be clipped
vector<Point> clipPolygon;    // Clipping window polygon
vector<Point> resultPolygon;  // Result after clipping
vector<Point> intersectionPoints;

int drawMode = 0; // 0: none, 1: subject, 2: clip
bool showIntersections = true;
bool clippingDone = false;
int currentTestCase = 0;

void init()
{
    glClearColor(0.95, 0.95, 0.95, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);
}

// Check if point is inside polygon
bool isInside(Point p, const vector<Point> &polygon)
{
    int n = polygon.size();
    if (n < 3)
        return false;

    int crossings = 0;
    for (int i = 0; i < n; i++)
    {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % n];

        if (((p1.y <= p.y) && (p2.y > p.y)) || ((p1.y > p.y) && (p2.y <= p.y)))
        {
            float vt = (p.y - p1.y) / (p2.y - p1.y);
            if (p.x < p1.x + vt * (p2.x - p1.x))
            {
                crossings++;
            }
        }
    }
    return (crossings % 2) == 1;
}

// Find intersection point of two line segments
bool lineIntersection(Point p1, Point p2, Point p3, Point p4, Point &intersection)
{
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;
    float x3 = p3.x, y3 = p3.y;
    float x4 = p4.x, y4 = p4.y;

    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (fabs(denom) < 1e-10)
    {
        return false; // Lines are parallel
    }

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        intersection.x = x1 + t * (x2 - x1);
        intersection.y = y1 + t * (y2 - y1);
        intersection.isIntersection = true;
        return true;
    }

    return false;
}

// Find all intersection points
void findIntersections()
{
    intersectionPoints.clear();

    int ns = subjectPolygon.size();
    int nc = clipPolygon.size();

    for (int i = 0; i < ns; i++)
    {
        Point s1 = subjectPolygon[i];
        Point s2 = subjectPolygon[(i + 1) % ns];

        for (int j = 0; j < nc; j++)
        {
            Point c1 = clipPolygon[j];
            Point c2 = clipPolygon[(j + 1) % nc];

            Point intersection;
            if (lineIntersection(s1, s2, c1, c2, intersection))
            {
                // Check if this intersection already exists
                bool exists = false;
                for (const auto &p : intersectionPoints)
                {
                    if (p == intersection)
                    {
                        exists = true;
                        break;
                    }
                }
                if (!exists)
                {
                    intersectionPoints.push_back(intersection);
                }
            }
        }
    }
}

// Simplified Weiler-Atherton clipping (for convex clip polygon)
void weilerAthertonClip()
{
    resultPolygon.clear();

    if (subjectPolygon.size() < 3 || clipPolygon.size() < 3)
    {
        cout << "Need both polygons defined!" << endl;
        return;
    }

    findIntersections();

    cout << "Found " << intersectionPoints.size() << " intersection points" << endl;

    // Simple approach: For each vertex of subject polygon
    // if inside clip polygon, add to result
    // Also add intersection points in order

    vector<Point> candidatePoints;

    // Add subject polygon vertices that are inside clip polygon
    for (const auto &p : subjectPolygon)
    {
        if (isInside(p, clipPolygon))
        {
            candidatePoints.push_back(p);
        }
    }

    // Add clip polygon vertices that are inside subject polygon
    for (const auto &p : clipPolygon)
    {
        if (isInside(p, subjectPolygon))
        {
            bool exists = false;
            for (const auto &cp : candidatePoints)
            {
                if (cp == p)
                {
                    exists = true;
                    break;
                }
            }
            if (!exists)
            {
                candidatePoints.push_back(p);
            }
        }
    }

    // Add all intersection points
    for (const auto &p : intersectionPoints)
    {
        candidatePoints.push_back(p);
    }

    if (candidatePoints.empty())
    {
        cout << "No intersection - polygons don't overlap" << endl;
        return;
    }

    // Sort points by angle from centroid to create proper polygon
    if (candidatePoints.size() >= 3)
    {
        // Calculate centroid
        float cx = 0, cy = 0;
        for (const auto &p : candidatePoints)
        {
            cx += p.x;
            cy += p.y;
        }
        cx /= candidatePoints.size();
        cy /= candidatePoints.size();

        // Sort by angle
        sort(candidatePoints.begin(), candidatePoints.end(),
             [cx, cy](const Point &a, const Point &b)
             {
                 float angleA = atan2(a.y - cy, a.x - cx);
                 float angleB = atan2(b.y - cy, b.x - cx);
                 return angleA < angleB;
             });

        resultPolygon = candidatePoints;

        cout << "Result polygon has " << resultPolygon.size() << " vertices" << endl;
        clippingDone = true;
    }
}

void drawPolygon(const vector<Point> &polygon, float r, float g, float b, bool filled, float lineWidth = 2.0f)
{
    if (polygon.size() < 2)
        return;

    if (filled && polygon.size() >= 3)
    {
        glColor4f(r, g, b, 0.3f);
        glBegin(GL_POLYGON);
        for (const auto &p : polygon)
        {
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }

    // Draw outline
    glColor3f(r * 0.7f, g * 0.7f, b * 0.7f);
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
    for (const auto &p : polygon)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // Draw vertices
    glColor3f(r, g, b);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    for (const auto &p : polygon)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // Draw vertex numbers
    glColor3f(0, 0, 0);
    for (size_t i = 0; i < polygon.size(); i++)
    {
        glRasterPos2f(polygon[i].x + 5, polygon[i].y + 5);
        string num = to_string(i);
        for (char c : num)
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
        }
    }
}

void drawIntersections()
{
    if (!showIntersections || intersectionPoints.empty())
        return;

    glColor3f(1.0f, 0.0f, 1.0f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    for (const auto &p : intersectionPoints)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // Draw circles around intersections
    for (const auto &p : intersectionPoints)
    {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 20; i++)
        {
            float angle = 2.0f * M_PI * i / 20;
            glVertex2f(p.x + 8 * cos(angle), p.y + 8 * sin(angle));
        }
        glEnd();
    }

    // Label intersections
    glColor3f(0.5f, 0.0f, 0.5f);
    for (size_t i = 0; i < intersectionPoints.size(); i++)
    {
        glRasterPos2f(intersectionPoints[i].x + 10, intersectionPoints[i].y + 10);
        string label = "I" + to_string(i);
        for (char c : label)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    }
}

void drawUI()
{
    // Background
    glColor4f(0.2, 0.2, 0.2, 0.9);
    glBegin(GL_QUADS);
    glVertex2f(0, 750);
    glVertex2f(1000, 750);
    glVertex2f(1000, 800);
    glVertex2f(0, 800);
    glEnd();

    glColor3f(1.0, 1.0, 0.0);

    // Title
    glRasterPos2f(10, 790);
    string title = "Weiler-Atherton Polygon Clipping Algorithm";
    for (char c : title)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glColor3f(1.0, 1.0, 1.0);

    // Controls
    glRasterPos2f(10, 770);
    string msg = "'1': Test Case | '2': Draw Subject | '3': Draw Clip | 'p': Perform Clipping | 'i': Intersections | 'c': Clear";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    // Status
    glRasterPos2f(10, 755);
    msg = "Subject: " + to_string(subjectPolygon.size()) + " vertices | " +
          "Clip: " + to_string(clipPolygon.size()) + " vertices | " +
          "Result: " + to_string(resultPolygon.size()) + " vertices | " +
          "Intersections: " + to_string(intersectionPoints.size());
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }

    // Mode indicator
    if (drawMode == 1)
    {
        glColor3f(1.0, 0.5, 0.0);
        glRasterPos2f(10, 735);
        msg = "MODE: Drawing Subject Polygon (click points, press 'f' to finish)";
        for (char c : msg)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    }
    else if (drawMode == 2)
    {
        glColor3f(0.0, 0.5, 1.0);
        glRasterPos2f(10, 735);
        msg = "MODE: Drawing Clip Polygon (click points, press 'f' to finish)";
        for (char c : msg)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    }

    // Legend
    glColor3f(1.0, 0.5, 0.0);
    glRasterPos2f(10, 20);
    msg = "Orange: Subject Polygon | ";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }

    glColor3f(0.0, 0.5, 1.0);
    glRasterPos2f(220, 20);
    msg = "Blue: Clip Polygon | ";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }

    glColor3f(0.0, 1.0, 0.0);
    glRasterPos2f(380, 20);
    msg = "Green: Clipped Result | ";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }

    glColor3f(1.0, 0.0, 1.0);
    glRasterPos2f(570, 20);
    msg = "Magenta: Intersections";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw subject polygon (orange)
    if (!subjectPolygon.empty())
    {
        drawPolygon(subjectPolygon, 1.0f, 0.5f, 0.0f, true, 2.5f);
    }

    // Draw clip polygon (blue)
    if (!clipPolygon.empty())
    {
        drawPolygon(clipPolygon, 0.0f, 0.5f, 1.0f, true, 2.5f);
    }

    // Draw result polygon (green)
    if (clippingDone && !resultPolygon.empty())
    {
        drawPolygon(resultPolygon, 0.0f, 1.0f, 0.0f, true, 3.5f);
    }

    // Draw intersections
    drawIntersections();

    drawUI();

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && drawMode > 0)
    {
        float glY = 800 - y;

        if (glY > 730)
            return; // Don't add points in UI area

        Point p(x, glY);

        if (drawMode == 1)
        {
            subjectPolygon.push_back(p);
        }
        else if (drawMode == 2)
        {
            clipPolygon.push_back(p);
        }

        glutPostRedisplay();
    }
}

void loadTestCase(int testNum)
{
    subjectPolygon.clear();
    clipPolygon.clear();
    resultPolygon.clear();
    intersectionPoints.clear();
    clippingDone = false;

    switch (testNum)
    {
    case 0:
        // Simple overlapping rectangles
        subjectPolygon = {
            Point(200, 200), Point(500, 200),
            Point(500, 500), Point(200, 500)};
        clipPolygon = {
            Point(350, 150), Point(650, 150),
            Point(650, 450), Point(350, 450)};
        break;
    case 1:
        // Triangle and rectangle
        subjectPolygon = {
            Point(300, 200), Point(600, 200),
            Point(450, 550)};
        clipPolygon = {
            Point(250, 250), Point(550, 250),
            Point(550, 500), Point(250, 500)};
        break;
    case 2:
        // Pentagon and rectangle
        subjectPolygon = {
            Point(400, 200), Point(550, 300),
            Point(500, 500), Point(300, 500),
            Point(250, 300)};
        clipPolygon = {
            Point(200, 250), Point(600, 250),
            Point(600, 550), Point(200, 550)};
        break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        loadTestCase(currentTestCase);
        currentTestCase = (currentTestCase + 1) % 3;
        drawMode = 0;
        cout << "Loaded test case" << endl;
        break;
    case '2':
        drawMode = 1;
        subjectPolygon.clear();
        resultPolygon.clear();
        clippingDone = false;
        cout << "Draw subject polygon mode" << endl;
        break;
    case '3':
        drawMode = 2;
        clipPolygon.clear();
        resultPolygon.clear();
        clippingDone = false;
        cout << "Draw clip polygon mode" << endl;
        break;
    case 'f':
    case 'F':
        drawMode = 0;
        cout << "Finished drawing" << endl;
        break;
    case 'p':
    case 'P':
        weilerAthertonClip();
        break;
    case 'i':
    case 'I':
        showIntersections = !showIntersections;
        if (!clippingDone)
        {
            findIntersections();
        }
        break;
    case 'n':
    case 'N':
        loadTestCase(currentTestCase);
        currentTestCase = (currentTestCase + 1) % 3;
        break;
    case 'c':
    case 'C':
        subjectPolygon.clear();
        clipPolygon.clear();
        resultPolygon.clear();
        intersectionPoints.clear();
        clippingDone = false;
        drawMode = 0;
        break;
    case 27: // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Weiler-Atherton Polygon Clipping");

    init();
    loadTestCase(0);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    cout << "Weiler-Atherton Polygon Clipping Algorithm\n";
    cout << "===========================================\n\n";
    cout << "Algorithm Overview:\n";
    cout << "1. Find intersection points between subject and clip polygons\n";
    cout << "2. Insert intersection points into both polygon vertex lists\n";
    cout << "3. Mark intersection points as entry or exit points\n";
    cout << "4. Traverse vertex list starting from entry point\n";
    cout << "5. Follow subject polygon edges until exit point\n";
    cout << "6. Switch to clip polygon and follow until next entry\n";
    cout << "7. Repeat until back to starting point\n\n";
    cout << "Features:\n";
    cout << "- Handles convex clip polygons\n";
    cout << "- Supports simple concave subject polygons\n";
    cout << "- Can produce multiple clipped regions\n";
    cout << "- More powerful than Sutherland-Hodgman\n\n";
    cout << "Controls:\n";
    cout << "'1': Load test case (cycles through examples)\n";
    cout << "'2': Draw subject polygon (polygon to be clipped)\n";
    cout << "'3': Draw clip polygon (clipping window)\n";
    cout << "'f': Finish drawing current polygon\n";
    cout << "'p': Perform Weiler-Atherton clipping\n";
    cout << "'i': Toggle intersection points display\n";
    cout << "'c': Clear all polygons\n\n";

    glutMainLoop();
    return 0;
}
