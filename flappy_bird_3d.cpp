#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <cmath>

using namespace std;

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Lane properties
const int NUM_LANES = 5;
const float LANE_WIDTH = 2.0f;
const float LANE_SPACING = 2.5f;

// Pillar properties
const float PILLAR_WIDTH = 2.0f;
const float PILLAR_DEPTH = 0.5f;
const float PILLAR_SPEED = 0.15f;
const float SPAWN_DISTANCE = 15.0f;
const float GAP_SIZE = 2.5f;

// Bird properties
const float BIRD_SIZE = 0.4f;
const float GRAVITY = 0.015f;
const float JUMP_VELOCITY = 0.25f;
const float LANE_SWITCH_SPEED = 0.2f;

// Bird state
int currentLane = 2;
float targetLaneX = 0.0f;
float birdX = 0.0f;
float birdY = 2.0f;
float birdZ = 0.0f;
float birdVelocityY = 0.0f;

// Camera
float cameraZ = 8.0f;
float cameraY = 2.0f;

// Game state
bool gameOver = false;
int score = 0;

// Pillar gap types
enum GapType
{
    GAP_TOP,
    GAP_BOTTOM,
    GAP_NONE
};

// Pillar structure
struct Pillar
{
    int lane;
    float z;
    GapType gapType;
    float gapStart;
    bool passed;

    Pillar(int l, float zPos, GapType gt, float gs) : lane(l), z(zPos), gapType(gt), gapStart(gs), passed(false) {}
};

// Star structure
struct Star
{
    int lane;
    float y;
    float z;
    bool collected;

    Star(int l, float yPos, float zPos) : lane(l), y(yPos), z(zPos), collected(false) {}
};

vector<Pillar> pillars;
vector<Star> stars;
float lastPillarZ = -20.0f;

// Initialize OpenGL
void initGL()
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)WINDOW_WIDTH / (double)WINDOW_HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_pos[] = {0.0f, 5.0f, 10.0f, 1.0f};
    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    srand(time(NULL));
}

// Get lane X position
float getLaneX(int lane)
{
    return (lane - 2) * LANE_SPACING;
}

// Create pillar
void createPillar()
{
    int lane = rand() % NUM_LANES;

    int gapTypeRand = rand() % 10;
    GapType gapType;
    float gapStart;

    if (gapTypeRand < 3)
    {
        gapType = GAP_BOTTOM;
        gapStart = 0.0f;
    }
    else if (gapTypeRand < 6)
    {
        gapType = GAP_TOP;
        gapStart = 4.0f - GAP_SIZE;
    }
    else
    {
        gapType = GAP_NONE;
        gapStart = 0.0f;
    }

    pillars.push_back(Pillar(lane, lastPillarZ, gapType, gapStart));

    if (gapType == GAP_TOP)
    {
        stars.push_back(Star(lane, gapStart + GAP_SIZE / 2, lastPillarZ));
    }
    else if (gapType == GAP_BOTTOM)
    {
        stars.push_back(Star(lane, GAP_SIZE / 2, lastPillarZ));
    }
    else
    {
        stars.push_back(Star(lane, 4.5f, lastPillarZ));
    }

    lastPillarZ -= SPAWN_DISTANCE;
}

// Draw box
void drawBox(float x, float y, float z, float width, float height, float depth)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(width, height, depth);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Draw bird
void drawBird()
{
    glColor3f(1.0f, 0.84f, 0.0f);
    glPushMatrix();
    glTranslatef(birdX, birdY, birdZ);
    glutSolidSphere(BIRD_SIZE, 20, 20);
    glPopMatrix();
}

// Draw 3D star
void drawStar3D(float x, float y, float z, float size)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(45, 0, 1, 0);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 5; i++)
    {
        float angle1 = i * 72.0f * 3.14159f / 180.0f;
        float angle2 = (i + 1) * 72.0f * 3.14159f / 180.0f;

        glVertex3f(0, size, 0);
        glVertex3f(size * sin(angle1), 0, size * cos(angle1));
        glVertex3f(size * sin(angle2), 0, size * cos(angle2));
    }
    glEnd();

    glPopMatrix();
}

// Draw lanes
void drawLanes()
{
    glColor3f(0.3f, 0.3f, 0.3f);
    for (int i = 0; i < NUM_LANES; i++)
    {
        float x = getLaneX(i);
        drawBox(x, -0.1f, -10.0f, LANE_WIDTH, 0.2f, 40.0f);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < NUM_LANES + 1; i++)
    {
        float x = getLaneX(0) - LANE_WIDTH / 2 + i * LANE_SPACING;
        drawBox(x, 0.0f, -10.0f, 0.1f, 0.05f, 40.0f);
    }
}

// Draw pillars
void drawPillars()
{
    glColor3f(0.13f, 0.55f, 0.13f);

    for (size_t i = 0; i < pillars.size(); i++)
    {
        float x = getLaneX(pillars[i].lane);

        if (pillars[i].gapType == GAP_TOP)
        {
            float height = pillars[i].gapStart;
            drawBox(x, height / 2, pillars[i].z, PILLAR_WIDTH, height, PILLAR_DEPTH);
        }
        else if (pillars[i].gapType == GAP_BOTTOM)
        {
            float gapEnd = pillars[i].gapStart + GAP_SIZE;
            float height = 4.0f - gapEnd;
            drawBox(x, gapEnd + height / 2, pillars[i].z, PILLAR_WIDTH, height, PILLAR_DEPTH);
        }
        else
        {
            drawBox(x, 2.0f, pillars[i].z, PILLAR_WIDTH, 4.0f, PILLAR_DEPTH);
        }
    }
}

// Draw stars
void drawStars()
{
    glColor3f(1.0f, 1.0f, 0.0f);
    for (size_t i = 0; i < stars.size(); i++)
    {
        if (!stars[i].collected)
        {
            float x = getLaneX(stars[i].lane);
            drawStar3D(x, stars[i].y, stars[i].z, 0.3f);
        }
    }
}

// Draw 2D text
void drawText(float x, float y, const char *text)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Check collision
bool checkCollision()
{
    for (size_t i = 0; i < pillars.size(); i++)
    {
        if (pillars[i].z > birdZ - PILLAR_DEPTH && pillars[i].z < birdZ + PILLAR_DEPTH)
        {
            float pillarX = getLaneX(pillars[i].lane);

            if (fabs(birdX - pillarX) < PILLAR_WIDTH / 2 + BIRD_SIZE)
            {
                if (pillars[i].gapType == GAP_TOP)
                {
                    if (birdY - BIRD_SIZE < pillars[i].gapStart)
                    {
                        return true;
                    }
                }
                else if (pillars[i].gapType == GAP_BOTTOM)
                {
                    float gapEnd = pillars[i].gapStart + GAP_SIZE;
                    if (birdY + BIRD_SIZE > gapEnd)
                    {
                        return true;
                    }
                }
                else
                {
                    return true;
                }
            }
        }
    }

    if (birdY - BIRD_SIZE < 0.0f || birdY + BIRD_SIZE > 4.0f)
    {
        return true;
    }

    return false;
}

// Check star collection
void checkStarCollection()
{
    for (size_t i = 0; i < stars.size(); i++)
    {
        if (!stars[i].collected)
        {
            float starX = getLaneX(stars[i].lane);

            float dx = birdX - starX;
            float dy = birdY - stars[i].y;
            float dz = birdZ - stars[i].z;
            float distance = sqrt(dx * dx + dy * dy + dz * dz);

            if (distance < BIRD_SIZE + 0.3f)
            {
                stars[i].collected = true;
                score += 10;
            }
        }
    }
}

// Update bird
void updateBird()
{
    if (gameOver)
        return;

    birdVelocityY -= GRAVITY;
    birdY += birdVelocityY;

    if (fabs(birdX - targetLaneX) > 0.01f)
    {
        float diff = targetLaneX - birdX;
        birdX += diff * LANE_SWITCH_SPEED;
    }
    else
    {
        birdX = targetLaneX;
    }

    if (checkCollision())
    {
        gameOver = true;
    }

    checkStarCollection();
}

// Update pillars
void updatePillars()
{
    if (gameOver)
        return;

    for (size_t i = 0; i < pillars.size(); i++)
    {
        pillars[i].z += PILLAR_SPEED;

        if (!pillars[i].passed && pillars[i].z > birdZ)
        {
            pillars[i].passed = true;
            score++;
        }
    }

    for (size_t i = 0; i < stars.size(); i++)
    {
        stars[i].z += PILLAR_SPEED;
    }

    while (!pillars.empty() && pillars[0].z > cameraZ + 2.0f)
    {
        pillars.erase(pillars.begin());
    }

    while (!stars.empty() && stars[0].z > cameraZ + 2.0f)
    {
        stars.erase(stars.begin());
    }

    if (pillars.empty() || pillars.back().z > lastPillarZ + SPAWN_DISTANCE)
    {
        createPillar();
    }
}

// Display
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, cameraY, cameraZ,
              0.0, cameraY, 0.0,
              0.0, 1.0, 0.0);

    drawLanes();
    drawPillars();
    drawStars();
    drawBird();

    glColor3f(1.0f, 1.0f, 1.0f);
    char scoreText[100];
    sprintf(scoreText, "Score: %d | Lane: %d", score, currentLane + 1);
    drawText(10, WINDOW_HEIGHT - 30, scoreText);

    glColor3f(1.0f, 1.0f, 0.0f);
    drawText(10, WINDOW_HEIGHT - 55, "UP/DOWN: Jump | LEFT/RIGHT: Switch Lane");

    if (gameOver)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawText(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 20, "GAME OVER!");
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 - 20, "Press 'R' to Restart");
    }

    glutSwapBuffers();
}

// Timer
void timer(int value)
{
    updateBird();
    updatePillars();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Special keys
void specialKeys(int key, int x, int y)
{
    if (gameOver)
        return;

    if (key == GLUT_KEY_UP)
    {
        birdVelocityY = JUMP_VELOCITY;
    }
    if (key == GLUT_KEY_DOWN)
    {
        birdVelocityY = -JUMP_VELOCITY;
    }
    if (key == GLUT_KEY_LEFT)
    {
        if (currentLane > 0)
        {
            currentLane--;
            targetLaneX = getLaneX(currentLane);
        }
    }
    if (key == GLUT_KEY_RIGHT)
    {
        if (currentLane < NUM_LANES - 1)
        {
            currentLane++;
            targetLaneX = getLaneX(currentLane);
        }
    }
}

// Keyboard
void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        exit(0);
    }
    if (key == 'r' || key == 'R')
    {
        pillars.clear();
        stars.clear();
        lastPillarZ = -20.0f;
        currentLane = 2;
        birdX = 0.0f;
        targetLaneX = 0.0f;
        birdY = 2.0f;
        birdVelocityY = 0;
        gameOver = false;
        score = 0;
        createPillar();
    }
    if (key == ' ' && !gameOver)
    {
        birdVelocityY = JUMP_VELOCITY;
    }
}

// Main
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Flappy Bird - 5 Lanes");

    initGL();

    targetLaneX = getLaneX(currentLane);
    createPillar();
    createPillar();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
