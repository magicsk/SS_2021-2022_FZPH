#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <GL/glew.h>
#include <GLUT/glut.h>

const float Lmax = 20;
const int jump = 50;
float pushXsq = 0.0;
int bounceSq = 0;
double ax = 0;
double ax0;
double t = 0;
double dt = 0.05;
double av;
double d = 60;
double bx = 0;
double bx0;
double bv;
FILE *file;

void drawRefresh(const int i)
{
    t += dt;
    ax = ax0 + av * t;
    bx = bx0 + bv * t;
    if (ax >= bx)
    {
        printf("x: %.2lf, time: %.2lf\n", ax, t);
    }
    // printf("ax: %.2lf, bx: %.2lf, time: %.2lf\n", ax, bx, t);
    // if (ax > 60 || bx > 60) exit(0);
    // if (x > 100) exit(0);
    // file = fopen("./ex02.dat", "a+");
    // fprintf(file, "%.2lf %.2lf\n", x, t);
    // fclose(file);
    glutTimerFunc(jump, drawRefresh, 0);
    glutPostRedisplay();
}

void drawReshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width == 0)
        width++;
    const float aspect_ratio = (float)height / width;
    float RMat[] = {
        2 / Lmax * aspect_ratio, 0, 0, 0,
        0, 2 / Lmax, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};
    glLoadMatrixf(RMat);
}

void drawSquare()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.15, 0.1, 0.75);
    glBegin(GL_POLYGON);
    glVertex2f((ax / 100) * Lmax, 0.1 * Lmax);
    glVertex2f(((ax / 100) + 0.1) * Lmax, 0.1 * Lmax);
    glVertex2f(((ax / 100) + 0.1) * Lmax, 0 * Lmax);
    glVertex2f((ax / 100) * Lmax, 0 * Lmax);
    glEnd();
}

void drawSquaree()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.75, 0.1, 0.15);
    glBegin(GL_POLYGON);
    glVertex2f((bx / 100) * Lmax, 0.3 * Lmax);
    glVertex2f(((bx / 100) + 0.1) * Lmax, 0.3 * Lmax);
    glVertex2f(((bx / 100) + 0.1) * Lmax, 0.2 * Lmax);
    glVertex2f((bx / 100) * Lmax, 0.2 * Lmax);
    glEnd();
}

void drawLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawSquare();
    drawSquaree();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    printf("Please enter initial cords A:");
    scanf("%lf", &ax0);
    printf("Please enter initial speed A:");
    scanf("%lf", &av);
    printf("Please enter initial cords B:");
    scanf("%lf", &bx0);
    printf("Please enter initial speed B:");
    scanf("%lf", &bv);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(200, 150);

    glutCreateWindow("The Window");

    glutReshapeFunc(drawReshape);
    glutDisplayFunc(drawLoop);
    glClearColor(0.9, 0.9, 0.9, 1);
    glutTimerFunc(jump, drawRefresh, 0);
    glutMainLoop();
    return 0;
}