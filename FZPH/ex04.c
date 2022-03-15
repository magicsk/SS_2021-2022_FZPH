//
//  ex04.c
//  FZPH
//
//  Created by Rastislav Brna on 15/03/2022.
//
//  Compiled with (macos):
//  gcc -Wall -fdiagnostics-color=always -g ex04.c -o ex04 -lm -lglew -framework opengl -framework glut

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#define FILE_NAME "./ex04.dat"
//#define FILE_NAME "./t_x_y_vx_vy_v.dat"
#define GRAVI_S 9.80665 //m/s^2

const float Lmax = 50;
const int jump = 10; //ms
double t = 0;
double dt = jump / 1000.0; //s

FILE* file;

double h0 = 10; // height of cannon
double persistantH0 = 10; // height of cannon
double v0 = 0; // initial speed
double x0 = -34; // initial x
double alpha0 = 0; // initial angle
double v, vx, vy;
double x; //position of x
double y; //position of y

bool bounce = false;

void drawRefresh(const int i) {
    t += dt;
    
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

void drawTower(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.35, 0.3, 0.35);
    glBegin(GL_POLYGON);
    glVertex2f(-38, 16);
    glVertex2f(-36, 18);
    glVertex2f(-34, 16);
    glVertex2f(-34, 0);
    glVertex2f(-38, 0);
    glEnd();
}

void drawGround(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.15, 0.15, 0.15);
    glBegin(GL_POLYGON);
    glVertex2f(-50, -0.2);
    glVertex2f(50, -0.2);
    glVertex2f(50, 0);
    glVertex2f(-50, 0);
    glEnd();
}

void drawCannon(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_POLYGON);
    glVertex2f(-35, persistantH0 + 1);
    glVertex2f(-34, persistantH0 + 1);
    glVertex2f(-34, persistantH0 - 1);
    glVertex2f(-35, persistantH0 - 1);
    glEnd();
}

void drawBall(void){
    // number to degree calculation
    double alpha = M_PI/(180/alpha0);
    
    // vx(t)
    vx = v0*cos(alpha);
    // vy(t)
    vy = v0 - GRAVI_S*t;
    
    v = vx + vy;
    
    // X
    if (x < 43.8) x = x0 + vx*cos(alpha)*t;
    
    // Y
    // bounce (unrealistic)
    if (y <= 0.3) {
        if (t > 0.1 && bounce) {
            x0 = x;
            h0 = y;
            v0 = v0/1.2;
            t = 0;
        }
    }
    
    double calcY = h0 + vy*sin(alpha)*t - 0.5*GRAVI_S*pow(t, 2);
    if (!(y <= 0.3 && calcY < y)) y = calcY;
    
    int quality = 1000;
    double radius = 0.2;
    double twoPi = 2 * M_PI;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0, 0, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= quality; i++)   {
        glVertex2f ((x + (radius * cos(i * twoPi / quality))), (y + (radius * sin(i * twoPi / quality))));
    }
    glEnd();
    
    file = fopen(FILE_NAME, "a+");
    fprintf(file, "%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n", t, x, y, vx, vy, v);
    fclose(file);
}

void drawLoop(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawTower();
    drawGround();
    drawCannon();
    drawBall();
    glutSwapBuffers();
}

int main(int argc, char** argv) {

    for (int i = 0; i < argc; i++) {
        if (argc <= 1) {
            printf("Usage: ./ex04 -v 25 -a 45 -b\n\n-v  speed in m/s from <1, 50>\n-a  angle of shot in degrees from min.: 1° max: 90°>\n-b  enable bounce (experimental)\n");
            exit(0);
        }
        if (strcmp(argv[i], "-v") == 0 && argc > i+1) {
            sscanf(argv[i+1], "%lf", &v0);
            printf("Speed: %.lfm/s\n", v0);
        }
        if (strcmp(argv[i], "-a") == 0 && argc > i+1) {
            sscanf(argv[i+1], "%lf", &alpha0);
            printf("Angle: %.lf°\n", alpha0);
        }
        if (strcmp(argv[i], "-b") == 0) {
            bounce = true;
            printf("Bounce enabled (experimental)\n");
        }
    }
    
    if (v0 == 0 || alpha0 == 0 || v0 > 50 || v0 < 1 || v0 > 50 || alpha0 < 1 || alpha0 > 90) {
        printf("Usage: ./ex04 -v 25 -a 45 -b\n\n-v  speed in m/s from <1, 50>\n-a  angle of shot in degrees from min.: 1° max: 90°>\n-b  enable bounce (experimental)\n");
        exit(0);
    }
    
    file = fopen(FILE_NAME, "w");
    fprintf(file, "# h0=10 x0=-34 v0=%.lf alpha0=%.lf \n# t[s] x[m] y[m] vx[m/s] vy[m/s] v[m/s]\n", v0, alpha0);
    fprintf(file, "%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n", t, x, y, vx, vy, v);
    fclose(file);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1600, 900);
    glutInitWindowPosition(200, 150);

    glutCreateWindow("The Window");

    glutReshapeFunc(drawReshape);
    glutDisplayFunc(drawLoop);
    glClearColor(0.9, 0.9, 0.9, 1);
    glutTimerFunc(jump, drawRefresh, 0);
    glutMainLoop();
    return 0;
}
