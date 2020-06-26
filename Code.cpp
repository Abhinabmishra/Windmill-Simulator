#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

int speed = -1;
int speed_diff = 5;
bool day = true;
float nx[15], ny[15];
float spacex, spacey;

void myInit()
{
    glClearColor(0.0, 0.0, 0.0, 0.0); //make background white. set to another color if needed
    glColor3f(0.0f, 0.0f, 0.0f);      //set initial drawing color
    glPointSize(20);
    glLineWidth(20);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-320.0, 320.0, -240.0, 240.0);
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius)
{
    int i;
    int triangleAmount = 50; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * 3.142;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}

void drawSpaceBody()
{
    // draw sun if day
    if (day)
    {
        if (!spacex)
        {
            spacex = 200;
            spacey = 150;
        }

        glColor3f(1.0, 1.0, 0.0);
        // drawFilledCircle(200, 150, 50);
        drawFilledCircle(spacex, spacey, 50);
    }

    // else draw moon
    else
    {
        if (!spacex)
        {
            spacex = -200;
            spacey = 150;
        }

        glColor3f(1.0, 1.0, 1.0);
        // drawFilledCircle(-200, 150, 30);
        drawFilledCircle(spacex, spacey, 30);
    }

    // reset point size
    // glPointSize(20);
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
    // change rotation direction
    if (key == 'c')
    {
        speed = speed * -1;
    }

    // increase speed
    if (key == 'f')
    {
        if (speed > 0)
            speed += speed_diff;
        else
            speed -= speed_diff;
    }

    // decrease speed
    if (key == 's')
    {
        if (speed > 0)
        {
            speed -= speed_diff;
            if (speed < 0)
                speed = 0;
        }

        else
        {
            speed += speed_diff;
            if (speed > 0)
                speed = 0;
        }
    }

    // change time to day
    if (key == 'd')
    {
        day = !day;
    }

    // change space object position
    if (key == 'm')
    {
        // if (x >= 320)
        //     spacex = x - 320;
        //     else
        //     spacex =
        spacex = (float)x - 320;
        spacey = 240 - (float)y;
    }
}

void myRotate(float a[], float b[])
{
    int i, j, k, h;
    for (h = 0; h < 4; h++)
    {
        float val = speed * (3.142 / 180);
        float p[3][3] = {cos(val), -sin(val), 0, sin(val), cos(val), 0, 0, 0, 1};
        float q[3][1] = {a[h], b[h], 1};
        float c[3][1] = {0, 0, 0};
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
            {
                c[i][j] = 0;
                for (k = 0; k < 3; k++)
                {
                    c[i][j] = c[i][j] + p[i][k] * q[k][j];
                }
            }
        }
        nx[h] = c[0][0];
        // std::cout << h << "   " << nx[h] << "\n";
        ny[h] = c[1][0];
        // std::cout << h << "   " << nx[h] << "\n";
    }
}

void afterrotate(int dummy)
{
    // set time of day
    if (day)
    {
        glClearColor(1.0, 1.0, 1.0, 0.0);
    }

    else
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
    }

    // clear color buffers
    glClear(GL_COLOR_BUFFER_BIT);

    // draw space body
    drawSpaceBody();

    // draw body of windmill
    glColor3f(0.3f, 0.6f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2d(-5, -320);
    glVertex2d(5, -320);
    glVertex2d(5, 0);
    glVertex2d(-5, 0);
    glEnd();

    // draw fan tether
    glColor3f(1.0f, 0.3f, 0.6f);
    glBegin(GL_POINTS);
    glVertex2d(0, 0);
    glEnd();

    // draw blades
    glColor3f(0.3f, 1.0f, 0.6f);
    glBegin(GL_LINES);
    glVertex2d(0, 0);
    glVertex2d(nx[0], ny[0]);
    glVertex2d(0, 0);
    glVertex2d(nx[1], ny[1]);
    glVertex2d(0, 0);
    glVertex2d(nx[2], ny[2]);
    glVertex2d(0, 0);
    glVertex2d(nx[3], ny[3]);
    glEnd();

    // send draw instructions
    glFlush();
    myRotate(nx, ny);
    glutTimerFunc(50, afterrotate, 1);
}

void myDisplay()
{
    nx[0] = -100;
    ny[0] = -100;
    nx[1] = -100;
    ny[1] = 100;
    nx[2] = 100;
    ny[2] = -100;
    nx[3] = 100;
    ny[3] = 100;
    afterrotate(1);
}

int main(int argc, char *argv[])
{
    std::cout << "Project Name: Windmill Simulation" << std::endl;
    std::cout << "Guide Name: Prof. Kiran Babu" << std::endl;
    std::cout << "Student Names: Abhinab Mishra and Krishna Keshan" << std::endl;
    std::cout << "Enter any key to start simulation: ";
    char ch;
    std::cin >> ch;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Windmill - 004 and 062");
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboardFunc);
    myInit();
    glutMainLoop();
    return 1;
}