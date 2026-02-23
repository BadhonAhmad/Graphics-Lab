/*
 * Advanced Composite Matrix Transformations
 * Demonstrates 2D transformations using matrix multiplication
 *
 * Features:
 * - Custom 3x3 homogeneous transformation matrices
 * - Rotation, Translation, Scaling using matrix multiplication
 * - Composite transformations (combining multiple operations)
 * - Order of transformation demonstration
 * - Interactive transformation controls
 *
 * Controls:
 * - 'r'/'R': Rotate clockwise/counter-clockwise
 * - Arrow keys: Translate object
 * - '+'/'-': Scale up/down
 * - '1': Apply Translation then Rotation
 * - '2': Apply Rotation then Translation
 * - '3': Apply Scale -> Rotate -> Translate
 * - 'x': Reset transformations
 * - 'p': Toggle showing transformation matrix
 * - Space: Toggle animation
 */

#include <GL/glut.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

// 3x3 Matrix for 2D homogeneous transformations
struct Matrix3x3
{
    float m[3][3];

    // Initialize as identity matrix
    Matrix3x3()
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    // Matrix multiplication
    Matrix3x3 operator*(const Matrix3x3 &other) const
    {
        Matrix3x3 result;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                result.m[i][j] = 0;
                for (int k = 0; k < 3; k++)
                {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    // Display matrix
    void print() const
    {
        cout << "Transformation Matrix:" << endl;
        for (int i = 0; i < 3; i++)
        {
            cout << "[ ";
            for (int j = 0; j < 3; j++)
            {
                cout << setw(8) << fixed << setprecision(3) << m[i][j] << " ";
            }
            cout << "]" << endl;
        }
        cout << endl;
    }
};

// 2D Point with homogeneous coordinates
struct Point2D
{
    float x, y, w;

    Point2D(float _x = 0, float _y = 0, float _w = 1) : x(_x), y(_y), w(_w) {}

    // Transform point by matrix
    Point2D transform(const Matrix3x3 &mat) const
    {
        Point2D result;
        result.x = mat.m[0][0] * x + mat.m[0][1] * y + mat.m[0][2] * w;
        result.y = mat.m[1][0] * x + mat.m[1][1] * y + mat.m[1][2] * w;
        result.w = mat.m[2][0] * x + mat.m[2][1] * y + mat.m[2][2] * w;

        // Normalize if w != 1
        if (result.w != 0 && result.w != 1)
        {
            result.x /= result.w;
            result.y /= result.w;
            result.w = 1;
        }
        return result;
    }
};

// Create translation matrix
Matrix3x3 createTranslationMatrix(float tx, float ty)
{
    Matrix3x3 mat;
    mat.m[0][2] = tx;
    mat.m[1][2] = ty;
    return mat;
}

// Create rotation matrix (angle in degrees)
Matrix3x3 createRotationMatrix(float angleDeg)
{
    Matrix3x3 mat;
    float angleRad = angleDeg * M_PI / 180.0f;
    mat.m[0][0] = cos(angleRad);
    mat.m[0][1] = -sin(angleRad);
    mat.m[1][0] = sin(angleRad);
    mat.m[1][1] = cos(angleRad);
    return mat;
}

// Create scaling matrix
Matrix3x3 createScalingMatrix(float sx, float sy)
{
    Matrix3x3 mat;
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    return mat;
}

// Create rotation matrix around a point
Matrix3x3 createRotationAroundPoint(float angleDeg, float px, float py)
{
    // T^-1 * R * T (Translate to origin, rotate, translate back)
    Matrix3x3 T1 = createTranslationMatrix(-px, -py);
    Matrix3x3 R = createRotationMatrix(angleDeg);
    Matrix3x3 T2 = createTranslationMatrix(px, py);
    return T2 * R * T1;
}

// Create scaling matrix around a point
Matrix3x3 createScalingAroundPoint(float sx, float sy, float px, float py)
{
    Matrix3x3 T1 = createTranslationMatrix(-px, -py);
    Matrix3x3 S = createScalingMatrix(sx, sy);
    Matrix3x3 T2 = createTranslationMatrix(px, py);
    return T2 * S * T1;
}

// Original object vertices (a simple house shape)
vector<Point2D> originalVertices = {
    Point2D(0, 0),    // 0: Bottom left
    Point2D(100, 0),  // 1: Bottom right
    Point2D(100, 60), // 2: Wall top right
    Point2D(50, 100), // 3: Roof peak
    Point2D(0, 60),   // 4: Wall top left
    Point2D(20, 15),  // 5: Door bottom left
    Point2D(45, 15),  // 6: Door bottom right
    Point2D(45, 45),  // 7: Door top right
    Point2D(20, 45),  // 8: Door top left
    Point2D(65, 25),  // 9: Window bottom left
    Point2D(85, 25),  // 10: Window bottom right
    Point2D(85, 45),  // 11: Window top right
    Point2D(65, 45)   // 12: Window top left
};

// Transformation parameters
float translateX = 300, translateY = 200;
float rotationAngle = 0;
float scaleX = 1.5f, scaleY = 1.5f;
int transformMode = 0; // 0: manual, 1: T*R, 2: R*T, 3: S*R*T
bool showMatrix = true;
bool animate = false;
float animSpeed = 2.0f;

void init()
{
    glClearColor(0.9, 0.93, 0.95, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
}

Matrix3x3 getCurrentTransformationMatrix()
{
    Matrix3x3 composite;

    switch (transformMode)
    {
    case 0: // Manual individual transformations
        composite = createTranslationMatrix(translateX, translateY) *
                    createRotationMatrix(rotationAngle) *
                    createScalingMatrix(scaleX, scaleY);
        break;
    case 1: // Translation then Rotation (T * R)
        composite = createTranslationMatrix(translateX, translateY) *
                    createRotationMatrix(rotationAngle);
        break;
    case 2: // Rotation then Translation (R * T)
        composite = createRotationMatrix(rotationAngle) *
                    createTranslationMatrix(translateX, translateY);
        break;
    case 3: // Scale, Rotate, Translate (T * R * S)
        composite = createTranslationMatrix(translateX, translateY) *
                    createRotationMatrix(rotationAngle) *
                    createScalingMatrix(scaleX, scaleY);
        break;
    }

    return composite;
}

void drawHouse(const Matrix3x3 &transform, float r, float g, float b, bool filled)
{
    vector<Point2D> transformed;
    for (const auto &v : originalVertices)
    {
        transformed.push_back(v.transform(transform));
    }

    if (filled)
    {
        // Draw filled house body
        glColor3f(r * 0.9f, g * 0.9f, b * 0.9f);
        glBegin(GL_POLYGON);
        glVertex2f(transformed[0].x, transformed[0].y);
        glVertex2f(transformed[1].x, transformed[1].y);
        glVertex2f(transformed[2].x, transformed[2].y);
        glVertex2f(transformed[4].x, transformed[4].y);
        glEnd();

        // Draw roof
        glColor3f(r * 0.7f, g * 0.3f, b * 0.3f);
        glBegin(GL_TRIANGLES);
        glVertex2f(transformed[4].x, transformed[4].y);
        glVertex2f(transformed[2].x, transformed[2].y);
        glVertex2f(transformed[3].x, transformed[3].y);
        glEnd();

        // Draw door
        glColor3f(0.4f, 0.2f, 0.1f);
        glBegin(GL_POLYGON);
        glVertex2f(transformed[5].x, transformed[5].y);
        glVertex2f(transformed[6].x, transformed[6].y);
        glVertex2f(transformed[7].x, transformed[7].y);
        glVertex2f(transformed[8].x, transformed[8].y);
        glEnd();

        // Draw window
        glColor3f(0.6f, 0.8f, 1.0f);
        glBegin(GL_POLYGON);
        glVertex2f(transformed[9].x, transformed[9].y);
        glVertex2f(transformed[10].x, transformed[10].y);
        glVertex2f(transformed[11].x, transformed[11].y);
        glVertex2f(transformed[12].x, transformed[12].y);
        glEnd();
    }

    // Draw outline
    glColor3f(r * 0.3f, g * 0.3f, b * 0.3f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(transformed[0].x, transformed[0].y);
    glVertex2f(transformed[1].x, transformed[1].y);
    glVertex2f(transformed[2].x, transformed[2].y);
    glVertex2f(transformed[3].x, transformed[3].y);
    glVertex2f(transformed[4].x, transformed[4].y);
    glEnd();

    // Door outline
    glBegin(GL_LINE_LOOP);
    for (int i = 5; i <= 8; i++)
    {
        glVertex2f(transformed[i].x, transformed[i].y);
    }
    glEnd();

    // Window outline
    glBegin(GL_LINE_LOOP);
    for (int i = 9; i <= 12; i++)
    {
        glVertex2f(transformed[i].x, transformed[i].y);
    }
    glEnd();

    // Window cross
    glBegin(GL_LINES);
    glVertex2f((transformed[9].x + transformed[11].x) / 2, transformed[9].y);
    glVertex2f((transformed[9].x + transformed[11].x) / 2, transformed[11].y);
    glVertex2f(transformed[9].x, (transformed[9].y + transformed[11].y) / 2);
    glVertex2f(transformed[11].x, (transformed[9].y + transformed[11].y) / 2);
    glEnd();
}

void drawAxes()
{
    glLineWidth(1.0f);
    glColor3f(0.7, 0.7, 0.7);

    // X-axis
    glBegin(GL_LINES);
    glVertex2f(0, 300);
    glVertex2f(800, 300);
    glEnd();

    // Y-axis
    glBegin(GL_LINES);
    glVertex2f(400, 0);
    glVertex2f(400, 600);
    glEnd();
}

void drawUI()
{
    glColor3f(0.1, 0.1, 0.1);

    // Title
    glRasterPos2f(10, 580);
    string title = "Advanced Composite Matrix Transformations";
    for (char c : title)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Controls
    glRasterPos2f(10, 560);
    string msg = "r/R: Rotate | Arrows: Translate | +/-: Scale | 1: T*R | 2: R*T | 3: S*R*T | x: Reset | p: Matrix | Space: Animate";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    // Current values
    glRasterPos2f(10, 540);
    string mode;
    switch (transformMode)
    {
    case 0:
        mode = "Manual (T*R*S)";
        break;
    case 1:
        mode = "Translation * Rotation";
        break;
    case 2:
        mode = "Rotation * Translation";
        break;
    case 3:
        mode = "Scale * Rotate * Translate";
        break;
    }
    msg = "Mode: " + mode + " | Angle: " + to_string((int)rotationAngle) + "° | " +
          "Pos: (" + to_string((int)translateX) + ", " + to_string((int)translateY) + ") | " +
          "Scale: (" + to_string(scaleX).substr(0, 4) + ", " + to_string(scaleY).substr(0, 4) + ")";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    // Matrix display
    if (showMatrix)
    {
        Matrix3x3 mat = getCurrentTransformationMatrix();
        glRasterPos2f(10, 110);
        string matTitle = "Composite Transformation Matrix:";
        for (char c : matTitle)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }

        for (int i = 0; i < 3; i++)
        {
            glRasterPos2f(10, 90 - i * 20);
            char buffer[100];
            sprintf(buffer, "[ %7.3f  %7.3f  %7.3f ]", mat.m[i][0], mat.m[i][1], mat.m[i][2]);
            string row = buffer;
            for (char c : row)
            {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
            }
        }
    }

    // Legend
    glColor3f(0.5, 0.5, 0.5);
    glRasterPos2f(10, 20);
    msg = "Red: Original | Blue: Transformed | Note: Order of transformations matters!";
    for (char c : msg)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    // Draw original object in red (semi-transparent)
    Matrix3x3 originalTransform = createTranslationMatrix(100, 100);
    drawHouse(originalTransform, 1.0f, 0.3f, 0.3f, true);

    // Draw transformed object in blue
    Matrix3x3 transform = getCurrentTransformationMatrix();
    drawHouse(transform, 0.3f, 0.5f, 1.0f, true);

    drawUI();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        rotationAngle -= 5.0f;
        break;
    case 'R':
        rotationAngle += 5.0f;
        break;
    case '+':
    case '=':
        scaleX += 0.1f;
        scaleY += 0.1f;
        break;
    case '-':
    case '_':
        scaleX = max(0.1f, scaleX - 0.1f);
        scaleY = max(0.1f, scaleY - 0.1f);
        break;
    case '1':
        transformMode = 1;
        cout << "Mode: Translation * Rotation (T * R)" << endl;
        break;
    case '2':
        transformMode = 2;
        cout << "Mode: Rotation * Translation (R * T)" << endl;
        break;
    case '3':
        transformMode = 3;
        cout << "Mode: Scale * Rotate * Translate (T * R * S)" << endl;
        break;
    case 'x':
    case 'X':
        translateX = 300;
        translateY = 200;
        rotationAngle = 0;
        scaleX = scaleY = 1.5f;
        transformMode = 0;
        animate = false;
        cout << "Transformations reset" << endl;
        break;
    case 'p':
    case 'P':
        showMatrix = !showMatrix;
        getCurrentTransformationMatrix().print();
        break;
    case ' ':
        animate = !animate;
        break;
    case 27: // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        translateY += 10;
        break;
    case GLUT_KEY_DOWN:
        translateY -= 10;
        break;
    case GLUT_KEY_LEFT:
        translateX -= 10;
        break;
    case GLUT_KEY_RIGHT:
        translateX += 10;
        break;
    }
    glutPostRedisplay();
}

void timer(int value)
{
    if (animate)
    {
        rotationAngle += animSpeed;
        if (rotationAngle >= 360)
            rotationAngle -= 360;
        glutPostRedisplay();
    }
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Advanced Composite Matrix Transformations");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);

    cout << "Advanced Composite Matrix Transformations\n";
    cout << "==========================================\n";
    cout << "Demonstrates 2D transformations using homogeneous coordinates\n";
    cout << "and 3x3 transformation matrices.\n\n";
    cout << "Key Concepts:\n";
    cout << "- Homogeneous coordinates (x, y, w)\n";
    cout << "- 3x3 transformation matrices\n";
    cout << "- Matrix multiplication for composite transformations\n";
    cout << "- Order dependency (T*R != R*T)\n\n";
    cout << "Controls:\n";
    cout << "r/R: Rotate counter-clockwise/clockwise\n";
    cout << "Arrow keys: Translate object\n";
    cout << "+/-: Scale up/down\n";
    cout << "1: Apply Translation then Rotation (T * R)\n";
    cout << "2: Apply Rotation then Translation (R * T)\n";
    cout << "3: Apply full composite (T * R * S)\n";
    cout << "p: Toggle matrix display\n";
    cout << "x: Reset all transformations\n";
    cout << "Space: Toggle animation\n\n";
    cout << "Note: Observe how changing the order of transformations\n";
    cout << "produces different results!\n";

    glutMainLoop();
    return 0;
}
