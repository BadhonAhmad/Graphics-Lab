/*
 * Advanced Cohen-Sutherland Line Clipping Algorithm
 * Complete implementation with visualization and step-by-step demonstration
 *
 * Features:
 * - Cohen-Sutherland line clipping against rectangular window
 * - Region code calculation and visualization
 * - Step-by-step clipping process display
 * - Multiple test cases demonstrating all scenarios
 * - Interactive line drawing
 * - Before/after comparison
 *
 * Controls:
 * - '1': Show all test cases
 * - '2': Interactive drawing mode (click 2 points)
 * - 's': Step through clipping algorithm
 * - 'n': Next test case
 * - 'r': Show region codes
 * - 'c': Clear all
 * - '+/-': Adjust window size
 */

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// Region codes for Cohen-Sutherland
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Clipping window
float xmin = 200, ymin = 150, xmax = 600, ymax = 450;

struct Point
{
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

struct Line
{
    Point p1, p2;
    int code1, code2;
    bool visible;
    string status;

    Line(Point _p1, Point _p2) : p1(_p1), p2(_p2), visible(false), status("Unprocessed")
    {
        code1 = computeCode(p1);
        code2 = computeCode(p2);
    }

    int computeCode(Point p)
    {
        int code = INSIDE;
        if (p.x < xmin)
            code |= LEFT;
        else if (p.x > xmax)
            code |= RIGHT;
        if (p.y < ymin)
            code |= BOTTOM;
        else if (p.y > ymax)
            code |= TOP;
        return code;
    }
};

vector<Line> lines;
vector<Line> clippedLines;
int currentStep = 0;
int currentLineIndex = 0;
bool showRegionCodes = true;
bool stepMode = false;
bool interactiveMode = false;
Point tempPoint;
bool hasFirstPoint = false;

void init()
{
    glClearColor(0.95, 0.95, 0.95, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
}

int computeRegionCode(float x, float y)
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

string regionCodeToString(int code)
{
    string result = "";
    result += (code & TOP) ? "T" : "-";
    result += (code & BOTTOM) ? "B" : "-";
    result += (code & RIGHT) ? "R" : "-";
    result += (code & LEFT) ? "L" : "-";
    return result;
}

Point computeIntersection(Point p1, Point p2, int regionCode)
{
    Point p;
    float m;

    if (p2.x != p1.x)
    {
        m = (p2.y - p1.y) / (p2.x - p1.x);
    }

    if (regionCode & TOP)
    {
        p.y = ymax;
        p.x = p1.x + (ymax - p1.y) / m;
    }
    else if (regionCode & BOTTOM)
    {
        p.y = ymin;
        p.x = p1.x + (ymin - p1.y) / m;
    }
    else if (regionCode & RIGHT)
    {
        p.x = xmax;
        p.y = p1.y + m * (xmax - p1.x);
    }
    else if (regionCode & LEFT)
    {
        p.x = xmin;
        p.y = p1.y + m * (xmin - p1.x);
    }

    return p;
}

bool cohenSutherlandClip(Line &line)
{
    Point p1 = line.p1;
    Point p2 = line.p2;
    int code1 = computeRegionCode(p1.x, p1.y);
    int code2 = computeRegionCode(p2.x, p2.y);
    bool accept = false;

    int iteration = 0;
    const int MAX_ITERATIONS = 10;

    while (iteration < MAX_ITERATIONS)
    {
        iteration++;

        if ((code1 == 0) && (code2 == 0))
        {
            // Both endpoints inside
            accept = true;
            line.status = "Completely Inside";
            break;
        }
        else if (code1 & code2)
        {
            // Both endpoints in same outside region
            line.status = "Completely Outside (Trivial Reject)";
            break;
        }
        else
        {
            // Line needs clipping
            int codeOut = code1 ? code1 : code2;
            Point p = computeIntersection(p1, p2, codeOut);

            if (codeOut == code1)
            {
                p1 = p;
                code1 = computeRegionCode(p1.x, p1.y);
            }
            else
            {
                p2 = p;
                code2 = computeRegionCode(p2.x, p2.y);
            }

            line.status = "Partially Inside (Needs Clipping)";
        }
    }

    if (accept)
    {
        line.p1 = p1;
        line.p2 = p2;
        line.visible = true;
    }

    return accept;
}

void loadTestCases()
{
    lines.clear();
    clippedLines.clear();

    // Test case 1: Completely inside
    lines.push_back(Line(Point(300, 250), Point(500, 350)));

    // Test case 2: Completely outside (left)
    lines.push_back(Line(Point(50, 200), Point(150, 400)));

    // Test case 3: Completely outside (top)
    lines.push_back(Line(Point(300, 500), Point(500, 550)));

    // Test case 4: Crosses left boundary
    lines.push_back(Line(Point(100, 300), Point(400, 300)));

    // Test case 5: Crosses right boundary
    lines.push_back(Line(Point(400, 250), Point(700, 350)));

    // Test case 6: Crosses top boundary
    lines.push_back(Line(Point(300, 350), Point(500, 550)));

    // Test case 7: Crosses bottom boundary
    lines.push_back(Line(Point(250, 50), Point(450, 250)));

    // Test case 8: Diagonal crossing two boundaries
    lines.push_back(Line(Point(100, 100), Point(700, 500)));

    // Test case 9: Crosses all four boundaries
    lines.push_back(Line(Point(50, 50), Point(750, 550)));

    // Test case 10: Corner cases
    lines.push_back(Line(Point(150, 100), Point(250, 500)));

    // Perform clipping
    for (auto line : lines)
    {
        Line clipped = line;
        if (cohenSutherlandClip(clipped))
        {
            clippedLines.push_back(clipped);
        }
    }
}

void drawClippingWindow()
{
    // Draw semi-transparent clipping region
    glColor4f(0.9, 0.95, 1.0, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();

    // Draw clipping window border
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(3.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();
    glLineWidth(1.0);

    // Label
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(xmin + 10, ymax - 20);
    string label = "Clipping Window";
    for (char c : label)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    // Window coordinates
    glRasterPos2f(xmin + 10, ymax - 35);
    char coords[100];
    sprintf(coords, "(%.0f,%.0f) to (%.0f,%.0f)", xmin, ymin, xmax, ymax);
    for (char *c = coords; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

void drawRegionGrid()
{
    if (!showRegionCodes)
        return;

    glColor3f(0.7, 0.7, 0.7);
    glLineWidth(1.0);

    // Draw extended region boundaries
    float margin = 100;

    // Vertical lines
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);
    glBegin(GL_LINES);
    glVertex2f(xmin, 0);
    glVertex2f(xmin, 600);
    glVertex2f(xmax, 0);
    glVertex2f(xmax, 600);
    glEnd();

    // Horizontal lines
    glBegin(GL_LINES);
    glVertex2f(0, ymin);
    glVertex2f(800, ymin);
    glVertex2f(0, ymax);
    glVertex2f(800, ymax);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    // Region code labels
    glColor3f(0.4, 0.4, 0.4);

    struct RegionLabel
    {
        float x, y;
        string code;
    };

    vector<RegionLabel> regions = {
        {100, 500, "1001"}, {400, 500, "1000"}, {700, 500, "1010"}, // Top row
        {100, 300, "0001"},
        {400, 300, "0000"},
        {700, 300, "0010"}, // Middle row
        {100, 100, "0101"},
        {400, 100, "0100"},
        {700, 100, "0110"} // Bottom row
    };

    for (const auto &r : regions)
    {
        glRasterPos2f(r.x, r.y);
        for (char c : r.code)
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
        }
    }
}

void drawLines()
{
    // Draw original lines (dashed red)
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(3, 0xAAAA);
    glLineWidth(2.0);

    for (size_t i = 0; i < lines.size(); i++)
    {
        const auto &line = lines[i];

        // Color based on status
        if (line.code1 == 0 && line.code2 == 0)
        {
            glColor3f(0.0, 0.7, 0.0); // Green - completely inside
        }
        else if (line.code1 & line.code2)
        {
            glColor3f(0.8, 0.0, 0.0); // Red - trivially rejected
        }
        else
        {
            glColor3f(1.0, 0.5, 0.0); // Orange - needs clipping
        }

        glBegin(GL_LINES);
        glVertex2f(line.p1.x, line.p1.y);
        glVertex2f(line.p2.x, line.p2.y);
        glEnd();

        // Draw endpoints
        glPointSize(6.0);
        glBegin(GL_POINTS);
        glVertex2f(line.p1.x, line.p1.y);
        glVertex2f(line.p2.x, line.p2.y);
        glEnd();

        // Draw region codes at endpoints
        if (showRegionCodes)
        {
            glColor3f(0.0, 0.0, 0.0);
            glRasterPos2f(line.p1.x + 5, line.p1.y + 5);
            string code = regionCodeToString(line.code1);
            for (char c : code)
            {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
            }

            glRasterPos2f(line.p2.x + 5, line.p2.y + 5);
            code = regionCodeToString(line.code2);
            for (char c : code)
            {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
            }
        }
    }

    glDisable(GL_LINE_STIPPLE);

    // Draw clipped lines (solid green)
    glColor3f(0.0, 0.6, 0.0);
    glLineWidth(4.0);
    for (const auto &line : clippedLines)
    {
        glBegin(GL_LINES);
        glVertex2f(line.p1.x, line.p1.y);
        glVertex2f(line.p2.x, line.p2.y);
        glEnd();
    }

    glLineWidth(1.0);
}

void drawUI()
{
    // Background for UI
    glColor4f(0.2, 0.2, 0.2, 0.9);
    glBegin(GL_QUADS);
    glVertex2f(0, 560);
    glVertex2f(800, 560);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();

    glColor3f(1.0, 1.0, 0.0);

    // Title
    glRasterPos2f(10, 590);
    string title = "Cohen-Sutherland Line Clipping Algorithm";
    for (char c : title)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glColor3f(1.0, 1.0, 1.0);

    // Controls
    glRasterPos2f(10, 572);
    string msg = "'1': Test Cases | '2': Interactive | 'r': Region Codes | 'c': Clear | '+/-': Window Size";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    // Statistics
    glRasterPos2f(10, 555);
    msg = "Lines: " + to_string(lines.size()) + " | Clipped: " + to_string(clippedLines.size()) +
          " | Rejected: " + to_string(lines.size() - clippedLines.size());
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }

    // Legend
    glColor3f(0.0, 0.7, 0.0);
    glRasterPos2f(10, 20);
    msg = "Green: Inside | ";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }

    glColor3f(0.8, 0.0, 0.0);
    glRasterPos2f(120, 20);
    msg = "Red: Rejected | ";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }

    glColor3f(1.0, 0.5, 0.0);
    glRasterPos2f(240, 20);
    msg = "Orange: Needs Clipping | ";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }

    glColor3f(0.0, 0.6, 0.0);
    glRasterPos2f(420, 20);
    msg = "Dark Green: Clipped Result";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawRegionGrid();
    drawClippingWindow();
    drawLines();
    drawUI();

    // Draw temporary line in interactive mode
    if (interactiveMode && hasFirstPoint)
    {
        glColor3f(0.5, 0.5, 0.5);
        glLineWidth(2.0);
        glBegin(GL_LINES);
        glVertex2f(tempPoint.x, tempPoint.y);
        // Line to cursor would require motion callback
        glEnd();
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    if (!interactiveMode)
        return;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float glY = 600 - y;

        if (!hasFirstPoint)
        {
            tempPoint = Point(x, glY);
            hasFirstPoint = true;
        }
        else
        {
            Point p2(x, glY);
            lines.push_back(Line(tempPoint, p2));

            Line clipped = lines.back();
            if (cohenSutherlandClip(clipped))
            {
                clippedLines.push_back(clipped);
            }

            hasFirstPoint = false;
        }

        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        interactiveMode = false;
        loadTestCases();
        cout << "Loaded " << lines.size() << " test cases" << endl;
        break;
    case '2':
        interactiveMode = true;
        lines.clear();
        clippedLines.clear();
        hasFirstPoint = false;
        cout << "Interactive mode - Click two points to draw a line" << endl;
        break;
    case 'r':
    case 'R':
        showRegionCodes = !showRegionCodes;
        break;
    case 'c':
    case 'C':
        lines.clear();
        clippedLines.clear();
        hasFirstPoint = false;
        break;
    case '+':
    case '=':
        xmin += 10;
        ymin += 10;
        xmax -= 10;
        ymax -= 10;
        if (xmax <= xmin + 50)
        {
            xmin -= 10;
            xmax += 10;
        }
        if (ymax <= ymin + 50)
        {
            ymin -= 10;
            ymax += 10;
        }
        if (!lines.empty())
        {
            clippedLines.clear();
            for (auto line : lines)
            {
                if (cohenSutherlandClip(line))
                {
                    clippedLines.push_back(line);
                }
            }
        }
        break;
    case '-':
    case '_':
        xmin -= 10;
        ymin -= 10;
        xmax += 10;
        ymax += 10;
        xmin = max(50.0f, xmin);
        ymin = max(50.0f, ymin);
        xmax = min(750.0f, xmax);
        ymax = min(550.0f, ymax);
        if (!lines.empty())
        {
            clippedLines.clear();
            for (auto line : lines)
            {
                if (cohenSutherlandClip(line))
                {
                    clippedLines.push_back(line);
                }
            }
        }
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
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen-Sutherland Line Clipping Algorithm");

    init();
    loadTestCases();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    cout << "Cohen-Sutherland Line Clipping Algorithm\n";
    cout << "=========================================\n\n";
    cout << "Algorithm Steps:\n";
    cout << "1. Compute region codes for both endpoints\n";
    cout << "2. If both codes are 0000 -> line completely inside\n";
    cout << "3. If (code1 & code2) != 0 -> line completely outside (trivial reject)\n";
    cout << "4. Otherwise, clip line against window boundaries\n";
    cout << "5. Repeat until line is accepted or rejected\n\n";
    cout << "Region Codes (4-bit):\n";
    cout << "Bit 0 (1): LEFT   | Bit 1 (2): RIGHT\n";
    cout << "Bit 2 (4): BOTTOM | Bit 3 (8): TOP\n\n";
    cout << "Controls:\n";
    cout << "'1': Load test cases\n";
    cout << "'2': Interactive drawing mode\n";
    cout << "'r': Toggle region codes display\n";
    cout << "'+/-': Adjust clipping window size\n";
    cout << "'c': Clear all lines\n\n";
    cout << "Loaded " << lines.size() << " test cases\n";
    cout << "Clipped lines: " << clippedLines.size() << "\n";
    cout << "Rejected lines: " << (lines.size() - clippedLines.size()) << "\n";

    glutMainLoop();
    return 0;
}
