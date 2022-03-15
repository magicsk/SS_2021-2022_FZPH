// free fall 
// y0 -> load from io
// y = y0 - 1/2 * pow(gt, 2)
// y=0 -> ground, exit program
//
// write to dat file, test with gnuplot
// # height y0=$y0
// # t[s] y[m] vy[m/s]

#include<stdio.h>
#include<math.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

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
    if (ay < 0) {
        // printf("x: %.2lf, time: %.2lf\n", ay, t);
        dt = 0;
        t = 0;
        // exit(0);
    } else {
        t += dt;
        ay = ay0 - 0.5* g * pow(t, 2);
        ayv = 0 - g * t;

        ax = ax0 + av * t;
    }
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
    printf("Please enter initial cords A:");
    scanf("%lf", &ay0);

    file = fopen("./ex03.dat", "w");
    fprintf(file, "# height y0=%.2lf\n# t[s] y[m] vy[m/s]\n", ay0);
    fclose(file);

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