// movement withoud glTranslate

#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

const float Lmax = 4.0;
const int jump = 25;
float pushXsq = 0.0;
float pushXtr = 0.0;
int bounceSq = 0;
int bounceTr = 0;

void drawRefresh(const int i) {
    if (pushXsq > 1.2) bounceSq = 1;
    if (pushXsq < -1.2) bounceSq = 0;
    if (bounceSq) pushXsq -= 0.05; else pushXsq += 0.05;
    if (pushXtr > 1.2) bounceTr = 1;
    if (pushXtr < -1.2) bounceTr = 0;
    if (bounceTr) pushXtr -= 0.05; else pushXtr += 0.05;
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

void drawTriangle() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.15, 0.75, 0.1);
    glTranslatef(pushXtr, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.1 * Lmax, -0.1 * Lmax);
    glVertex2f(0.1 * Lmax, -0.1 * Lmax);
    glVertex2f(0 * Lmax, 0.1 * Lmax);
    glEnd();
}

void drawSquare() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.15, 0.1, 0.75);
    float push[] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        pushXsq,0,0,1
    };
    glLoadMatrixf(push);
    // glTranslatef(pushXsq, 0, 0);
    glScalef(Lmax, Lmax, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-0.1, 0.4);
    glVertex2f(0.1, 0.4);
    glVertex2f(0.1, 0.2);
    glVertex2f(-0.1, 0.2);
    glEnd();
}

void drawLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawTriangle();
    drawSquare();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
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