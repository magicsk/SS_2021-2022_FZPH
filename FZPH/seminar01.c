#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

const float Lmax = 4.0;
const int jump = 25;
float pushX = 0.0;
void drawRefresh(const int i) {
    pushX += 0.05;
    glutTimerFunc(jump, drawRefresh, 0);
    glutPostRedisplay();
}

void drawResize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width == 0) width++;
    const float aspect_ratio = (float)width / height;
    gluOrtho2D(-0.5 * Lmax, 0.5 * Lmax, -0.5 * Lmax * aspect_ratio, 0.5 * Lmax * aspect_ratio);
}

void drawLoop() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void drawTriangle() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.15, 0.75, 0.1);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.8 + pushX, -0.8);
    glVertex2f(0.8 + pushX, -0.8);
    glVertex2f(0 + pushX, 0.8);
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(200, 150);

    glutCreateWindow("The Window");

    glutReshapeFunc(drawResize);
    glutDisplayFunc(drawTriangle);
    glClearColor(0.9, 0.1, 0.15, 1);
    glutTimerFunc(jump, drawRefresh, 0);
    glutMainLoop();
    return 0;
}
