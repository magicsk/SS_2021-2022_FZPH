// create file with coordinates and time columns
// dt = 0,01 (sec)
// x = 0;  x = x0+v+t
// t = 0;  t = dt
// x(t) = x0 + v*t
// argv, argc input <x0> and <v>
// x and x0 € <-100,100> (Lmax)

#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

const float Lmax = 100;
const int jump = 10;
float pushXsq = 0.0;
int bounceSq = 0;
double x = 0;
double x0;
double dt = 0.01;
double t = 0;
double v;
FILE* file;

void drawRefresh(const int i) {
    t += dt;
    x = x0 + v * t;
    if (x > 100) exit(0);
    file = fopen("./ex02.dat", "a+");
    fprintf(file, "%.2lf %.2lf\n", x, t);
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
    glVertex2f((x / 200) * Lmax, 0.1 * Lmax);
    glVertex2f(((x / 200) + 0.1) * Lmax, 0.1 * Lmax);
    glVertex2f(((x / 200) + 0.1) * Lmax, 0 * Lmax);
    glVertex2f((x / 200) * Lmax, 0 * Lmax);
    glEnd();
}

void drawLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawSquare();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    printf("Please enter initial cords:");
    scanf("%lf", &x0);
    printf("Please enter initial speed:");
    scanf("%lf", &v);

    file = fopen("./ex02.dat", "w");
    fprintf(file, "# cords, time\n");
    fclose(file);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(200, 150);

    glutCreateWindow("The Window");

    glutReshapeFunc(drawReshape);
    glutDisplayFunc(drawLoop);
    glClearColor(0.9, 0.1, 0.15, 1);
    glutTimerFunc(jump, drawRefresh, 0);
    glutMainLoop();
    return 0;
}