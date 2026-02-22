// main.cpp  (OpenGL + GLUT)  — draws Bangladesh flag (green rectangle + red circle)

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <cmath>

using namespace std;

// cursor / seed (optional)
int x = -1000, y = -1000;

// 4-direction moves for flood fill
vector<pair<int, int>> mv = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

// visited sets for flood-fill
map<pair<int, int>, bool> visCircle;
map<pair<int, int>, bool> visRect;


// helper to draw a filled rectangle area using points
void helper(int xl, int xh, int yl, int yh)
{
    for (int i = xl; i <= xh; i++)
    {
        for (int j = yl; j <= yh; j++)
            glVertex2i(i, j);
    }
}

// inside green rectangle (strictly inside, not on border)
bool validRect(int px, int py)
{
    return (px > -20 && px < 30 && py > -15 && py < 15);
}

// flood fill for red circle (radius^2 = 36)
void fillCircle(int px, int py)
{
    if (px * px + py * py > 36)
        return;
    if (visCircle[{px, py}])
        return;

    visCircle[{px, py}] = true;
    glVertex2i(px, py);

    for (auto p : mv)
    {
        int dx = p.first;
        int dy = p.second;
        fillCircle(px + dx, py + dy);
    }
}

// flood fill for green rectangle
void fillRectangle(int px, int py)
{
    if (!validRect(px, py))
        return;
    if (visRect[{px, py}])
        return;

    visRect[{px, py}] = true;
    glVertex2i(px, py);

    for (auto p : mv)
    {
        int dx = p.first;
        int dy = p.second;
        fillRectangle(px + dx, py + dy);
    }
}

void drawRect(int x, int y) {

    for (int i = x; i <  20; i++) {
        glVertex2i(i, y);
        glVertex2i(i, y + 20);
    }

    for (int i = -10; i < 10; i++) {
        glVertex2i(i, sqrt(8 * 8 - i *i));
        glVertex2i(i, -sqrt(8 * 8 - i * i));
    }

    for (int i = -10; i < 10; i++) {
        glVertex2i(-20, i);
        glVertex2i(20, i);
    }

}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // -------- green rectangle --------
    glBegin(GL_POINTS);
    glPointSize(2);

    glColor3f(0.0f, 1.0f, 0.0f);

    drawRect(-20, -10);

    // visRect.clear();
    //fillRectangle(0, 0); // seed inside the rectangle (center is inside)

    glEnd();

    // -------- red circle --------
    glBegin(GL_POINTS);
    glPointSize(30);

    glColor3f(1.0f, 0.0f, 0.0f);
    visCircle.clear();
    //fillCircle(0, 0); // seed at circle center

    glEnd();

    glFlush();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
}

// optional: arrow keys just to print GLUT callback values (see your screenshot)
// (Note: GLUT passes mouse-x/mouse-y here, not "new position".)
void spe_key(int key, int mx, int my)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        cout << "mx = " << mx << ", my = " << my << endl;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Demo");

    init();
    glutDisplayFunc(display);
    glutSpecialFunc(spe_key);

    glutMainLoop();
    return 0;
}