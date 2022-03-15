
// base speed of car (not changin)
// that stopped after 35m (s) after stepping on break in 6.5ms (tr) from seeing barrier (in t0)
// tv = 0.8s
// t' when v=0
// 
// Sr = v0 * Tr
// s = Sr + Sb = n0 * Tr - 1/2 *a * pow(Tb, 2)
// Sb = v0 * Tb - 1/2 * a * pow(Tb, 2)
// s = v0 * Tr + pow(v0, 2)/a - 1/2 * a * pow((v0/a),2 )

#include<stdio.h>
#include<math.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
x
const float Lmax = 20;
const int jump = 10; //ms
float pushXsq = 0.0;
int bounceSq = 0;
double ax = 0;
double ax0 = 0;
double t = 0;
double dt = jump / 1000.0;
double av = 0;
double ay = 0;
double ay0;
double d = 60;
const double g = 9.80665;
double gt;
double ayv;
FILE* file;

void drawRefresh(const int i) {
    t += dt;
    gt = g * t;
    ayv = 0.5 * pow(gt, 2);
    ay = ay0 - ayv;
    ax = ax0 + av * t;
    // if (ay == 0 || ay < 0) {
    //     printf("x: %.2lf, time: %.2lf\n", ay, t);
    //     exit(0);
    // }
    file = fopen("./ex03.dat", "a+");
    fprintf(file, "%.2lf %.2lf %.2lf\n", t, ay, ayv);
    fclose(file);
    glutTimerFunc(jump, drawRefresh, 0);
    glutPostRedisplay();
}

void drawReshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width == 0) width++;
    const float aspect_ratio = (float)height / width;
    float RMat[] = {
        2 / Lmax * aspect_ratio, 0, 0, 0,
            0, 2 / Lmax, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    glLoadMatrixf(RMat);
}

void drawSquare() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.15, 0.1, 0.75);
    glBegin(GL_POLYGON);
    glVertex2f((ax / 100) * Lmax, ((ay / 100) + 0.1) * Lmax);
    glVertex2f(((ax / 100) + 0.1) * Lmax, ((ay / 100) + 0.1) * Lmax);
    glVertex2f(((ax / 100) + 0.1) * Lmax, (ay / 100) * Lmax);
    glVertex2f((ax / 100) * Lmax, (ay / 100) * Lmax);
    glEnd();
}

void drawLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawSquare();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    // printf("Please enter initial cords A:");
    // scanf("%lf", &ay0);

    // file = fopen("./ex03.1.dat", "w");
    // fprintf(file, "# height y0=%.2lf\n# t[s] y[m] vy[m/s]\n", ay0);
    // fclose(file);
    
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