#include <GL/gl.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

using namespace std;

// Global variables for circle center position
int centerX = 0;
int centerY = 0;
int stepSize = 5;      // Movement step size
int playerRadius = 20; // Player circle radius

// Global variables for bouncing ball
float ballX = 30.0f;    // Ball position X
float ballY = 30.0f;    // Ball position Y
float velocityX = 1.5f; // Ball velocity X
float velocityY = 2.0f; // Ball velocity Y
int ballRadius = 15;    // Ball radius
int boundaryLeft = -100;
int boundaryRight = 100;
int boundaryBottom = -100;
int boundaryTop = 100;

// Game state
bool gameOver = false;

// Bresenham's Circle Drawing Algorithm (Midpoint Circle Algorithm)
void setPixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
}

void drawCircleBresenham(int xc, int yc, int radius)
{
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius; // Initial decision parameter

    // Plot initial point
    setPixel(xc + x, yc + y);

    while (x <= y)
    {
        // Plot 8 symmetric points
        setPixel(xc + x, yc + y);
        setPixel(xc - x, yc + y);
        setPixel(xc + x, yc - y);
        setPixel(xc - x, yc - y);
        setPixel(xc + y, yc + x);
        setPixel(xc - y, yc + x);
        setPixel(xc + y, yc - x);
        setPixel(xc - y, yc - x);

        x++;

        if (d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
    }

    glEnd();
}

// Check collision between two circles
bool checkCollision(float x1, float y1, int r1, float x2, float y2, int r2)
{
    // Calculate distance between centers
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);

    // Collision occurs if distance is less than sum of radii
    return (distance <= (r1 + r2));
}

void updateBall(int value)
{
    // Stop updating if game is over
    if (gameOver)
    {
        return;
    }

    // Update ball position
    ballX += velocityX;
    ballY += velocityY;

    // Check collision with player circle
    if (checkCollision(centerX, centerY, playerRadius, ballX, ballY, ballRadius))
    {
        gameOver = true;
        cout << "\n========================================" << endl;
        cout << "COLLISION DETECTED!" << endl;
        cout << "GAME OVER!" << endl;
        cout << "========================================\n"
             << endl;
        glutPostRedisplay();
        return; // Stop the game
    }

    // Check collision with left/right boundaries
    if (ballX - ballRadius <= boundaryLeft)
    {
        ballX = boundaryLeft + ballRadius; // Prevent going out of bounds
        velocityX = -velocityX;            // Reverse X direction (mirror)
        cout << "Ball hit left boundary - reflecting" << endl;
    }
    else if (ballX + ballRadius >= boundaryRight)
    {
        ballX = boundaryRight - ballRadius;
        velocityX = -velocityX; // Reverse X direction (mirror)
        cout << "Ball hit right boundary - reflecting" << endl;
    }

    // Check collision with top/bottom boundaries
    if (ballY - ballRadius <= boundaryBottom)
    {
        ballY = boundaryBottom + ballRadius;
        velocityY = -velocityY; // Reverse Y direction (mirror)
        cout << "Ball hit bottom boundary - reflecting" << endl;
    }
    else if (ballY + ballRadius >= boundaryTop)
    {
        ballY = boundaryTop - ballRadius;
        velocityY = -velocityY; // Reverse Y direction (mirror)
        cout << "Ball hit top boundary - reflecting" << endl;
    }

    glutPostRedisplay();              // Request redraw
    glutTimerFunc(16, updateBall, 0); // Call again after ~16ms (60 FPS)
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(2);

    if (gameOver)
    {
        // Draw both circles in red to show collision
        glColor3f(1.0, 0.0, 0.0);
        drawCircleBresenham(centerX, centerY, playerRadius);
        drawCircleBresenham((int)ballX, (int)ballY, ballRadius);

        // Display game over text would require additional library
        // For now, circles turn red and animation stops
    }
    else
    {
        // Draw controllable circle (white)
        glColor3f(1.0, 1.0, 1.0);
        drawCircleBresenham(centerX, centerY, playerRadius);

        // Draw bouncing ball (red)
        glColor3f(1.0, 0.0, 0.0);
        drawCircleBresenham((int)ballX, (int)ballY, ballRadius);
    }

    glFlush();
}

void specialKey(int key, int x, int y)
{
    // Don't allow movement if game is over
    if (gameOver)
    {
        return;
    }

    switch (key)
    {
    case GLUT_KEY_LEFT:
        centerX -= stepSize;
        cout << "Left arrow: Moving circle to (" << centerX << ", " << centerY << ")" << endl;
        break;
    case GLUT_KEY_RIGHT:
        centerX += stepSize;
        cout << "Right arrow: Moving circle to (" << centerX << ", " << centerY << ")" << endl;
        break;
    case GLUT_KEY_UP:
        centerY += stepSize;
        cout << "Up arrow: Moving circle to (" << centerX << ", " << centerY << ")" << endl;
        break;
    case GLUT_KEY_DOWN:
        centerY -= stepSize;
        cout << "Down arrow: Moving circle to (" << centerX << ", " << centerY << ")" << endl;
        break;
    }
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
    glutCreateWindow("Bresenham Circle Drawing Algorithm");

    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKey);     // Register arrow key handler
    glutTimerFunc(0, updateBall, 0); // Start ball animation

    glutMainLoop();
    return 0;
}
