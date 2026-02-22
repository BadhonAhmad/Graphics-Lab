#include <GL/freeglut.h>
#include <iostream>

using namespace std;

// struct Point{
//     float x,y,z=1;
// };
//
// Point p1,p2,p3;

int Points[3][3] = {{0, 10, 15}, {0, 0, 10}, {1, 1, 1}}; // column major
//(0,0), (10,0), (15,10)

void Translate(int a, int b)
{
    int t[3][3] = {{1, 0, a}, {0, 1, b}, {0, 0, 1}};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int sum = 0;
            for (int k = 0; k < 3; k++)
            {
                sum += t[i][k] * Points[k][j];
            }
            Points[i][j] = sum;
            cout << Points[i][j] << " ";
        }
        cout << endl;
    }

    return;
}

void display(void)
{
    /* clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT);
    /* draw white polygon (rectangle) with corners at
     * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
     */
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_TRIANGLES); // Begin quadrilateral coordinates

    // Trapezoid
    glVertex2f(Points[0][0], Points[1][0]);
    glVertex2f(Points[0][1], Points[1][1]);
    glVertex2f(Points[0][2], Points[1][2]);
    // Printed as column major

    glEnd(); // End quadrilateral coordinates

    //        glBegin(GL_TRIANGLES); //Begin triangle coordinates
    //
    //
    //        //Pentagon
    //        glColor3f (0.0, 1.0, 0.0);
    //        glVertex3f(0.5f, 0.05f, 0.0f);
    //        glVertex3f(0.75f, 0.05f, 0.0f);
    //        glVertex3f(0.5f, 0.35f, 0.0f);
    //
    //
    //        /*glVertex3f(0.5f, 0.35f, 0.0f);
    //        glVertex3f(0.75f, 0.05f, 0.0f);
    //        glVertex3f(0.75f, 0.35f, 0.0f);*/
    //    glColor3f (0.0, 0.0, 1.0);
    //        glVertex3f(0.5f, 0.35f, 0.0f);
    //        glVertex3f(0.75f, 0.35f, 0.0f);
    //        glVertex3f(0.63f, 0.50f, 0.0f);
    //
    //
    //        //Triangle
    //        /*glVertex3f(0.30f, 0.65f, 0.0f);
    //        glVertex3f(0.60f, 0.65f, 0.0f);
    //        glVertex3f(0.45, 0.85f, 0.0f);*/

    //        glEnd();//End triangle coordinates
    /* don't wait!
     * start processing buffered OpenGL routines
     */
    glFlush();
}
void init(void)
{
    /* select clearing (background) color */
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -10.0, 10.0);
    //    Points[0][1] = 0;
    //    p1.y = 0;
    //    p2.x = 10;
    //    p2.y = 0;
    //    p3.x = 15;
    //    p3.y = 10;
    //    Points[3][3] = {{p1.x,p1.y,1},{p2.x,p2.y,1},{p3.x,p3.y,1}};
}

void spe_key(int key, int x, int y)
{
    switch (key)
    {

    case GLUT_KEY_LEFT:
        Translate(-1, 0);
        glutPostRedisplay();
        break;

        // spinDisplay_left();
        // break;

    case GLUT_KEY_RIGHT:
        Translate(1, 0);
        glutPostRedisplay();
        break;
    case GLUT_KEY_UP:
        Translate(0, 1);
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:
        Translate(0, -1);
        glutPostRedisplay();
        break;
        // spinDisplay_right();
        // break;

    default:
        break;
    }
}
/*
 * Declare initial window size, position, and display mode
 * (single buffer and RGBA). Open window with "hello"
 * in its title bar. Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Interactive");
    init();

    glutDisplayFunc(display);
    //	glutKeyboardFunc(my_keyboard);
    glutSpecialFunc(spe_key);
    //
    //	glutMouseFunc(my_mouse);
    glutMainLoop();
    return 0;
}
