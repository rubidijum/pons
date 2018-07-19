#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "help.h"
#define MAIN_FILE
#include "bridge.h"

#define INIT_WINDOW_WIDTH 700
#define INIT_WINDOW_HEIGHT 700
#define MAX_BEAMS 100

#define HALF_GRID_SIZE 25

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse(int button, int state, int x, int y);
static void on_reshape(int width, int height);
void draw_point(float x, float y);
void draw_scene(void);
void draw_road(int Xp, int Yp, int Xr, int Yr);

void draw_car();

GLdouble camX = 0;
GLdouble camY = 0;
GLdouble camZ = 0.5;

int currentWidth = INIT_WINDOW_WIDTH;
int currentHeight = INIT_WINDOW_HEIGHT;

float Xp, Yp, Xr, Yr;

static unsigned build_bridge_mode = 1;

Joint A, B;
Beam AB;
Bridge most;


int main(int argc, char** argv){
    
    //inicijalizacija gluta
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    
    glutInitWindowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutMouseFunc(on_mouse);
    glutReshapeFunc(on_reshape);
    
    glClearColor(0,0,0, 1);
    
    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    
    return 0;
}

static void on_display(void){
    
    
    
    printf("%f %f %f\n", camX, camY, camZ);
        
    if(build_bridge_mode){ //build bridge
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(0, 0, 1.8, 0, 0, 0, 0, 1, 0);
        draw_grid(HALF_GRID_SIZE); //25 => 50 celija * 50 celija
        draw_scene();
        draw_bridge();
        draw_car();
        coordsys();
    }
    else{ //animation + draw bridge
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(0.5, 1.1, 3.7, 0, 0, 0, 0, 1, 0);
        coordsys();
        draw_scene();
        draw_car();
        draw_bridge();
    }
    
    //dugme za pokretanje
    //dugme za reset animacije
    
    glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27:
            exit(0);
            break;
        case 'v':
        case 'V':
            if(build_bridge_mode){
                build_bridge_mode = 0;
                printf("build_bridge_mode: %d\n", build_bridge_mode);
                glutPostRedisplay();
            }
            break;
        case 'b':
        case 'B':
            if(!build_bridge_mode){
                build_bridge_mode = 1;
                printf("build_bridge_mode: %d\n", build_bridge_mode);
                glutPostRedisplay();
            }
            break;
        case 'u':
        case 'U':
            undo_add_beam();
            glutPostRedisplay();
            break;
        case 'C': //NOTE: testiranje iscrtavanja auta
        case 'c':
            draw_car();
            glutPostRedisplay();
            break;
        case 'g':
        case 'G':
            break;
       
           
            
    }
}



static void on_mouse(int button, int state, int x, int y){
    
       
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        Xp = x;
        Yp = y;
        A.X = Xp;
        A.Y = Yp;
        AB.begin = A;
        draw_point(Xp,Yp);
        
    }
      if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        printf("\n=============\n\nPRESSED X:%d Y:%d\n", x, y);
          printf("Released X:%d Y:%d\n", x, y);
        
        Xr = x;
        Yr = y;
        B.X = Xr;
        B.Y = Yr;
        
        AB.end = B;
        printf("begin X:%d, begin Y:%d\nend X:%d, end Y:%d\n", AB.begin.X, AB.begin.Y, AB.end.X, AB.end.Y);
        //add_beam
        add_beam_to_bridge();
        draw_point(Xr,Yr);
        draw_road(Xp, Yp, Xr, Yr);
      }
    
    
    
}

static void on_reshape(int width, int height){
    
    currentHeight = height;
    currentWidth = width;
    
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(build_bridge_mode){
        printf("gluOrtho\n\n");
        glOrtho(-1.0, 1.0, -1.0, 1.0, -5.5, 5.5); 
    }
    else if(!build_bridge_mode){
        printf("gluPerspective\n\n");
        gluPerspective(30, (float) width / height, 0.1, 100);
    }
    
    glMatrixMode(GL_MODELVIEW);
       
}

void draw_car(){
    
    GLUquadricObj* Cylinder;
   Cylinder = gluNewQuadric();
   gluQuadricDrawStyle(Cylinder, GLU_FILL);
   gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
   
   glPushMatrix();
   
   glRotatef(90, 0, 1, 0);
   glScalef(0.1,0.1,0.1);
   glTranslatef(-1.8,-2.8,-9);
   
   glPushMatrix();
    glScalef (1.0, 0.3, 1.8);      /* modeling transformation */ 
    glutWireCube (1.0);
    glScalef(1,1,0.5);
    glTranslatef(0,0.9,0);
    glutWireCube(1.0);
   glPopMatrix();
   
   glPushMatrix();
    glTranslatef(0.5,-0.1,-0.6);
    glRotatef(90,0,1,0);
    gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
   glPopMatrix();
   
   glPushMatrix();
    glTranslatef(0.5,-0.1,0.6);
    glRotatef(90,0,1,0);
    gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
   glPopMatrix();
   
   
    glPushMatrix();
     glTranslatef(-0.6,-0.1,-0.6);
     glRotatef(90,0,1,0);
     gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
    glPopMatrix();
   
   
   glPushMatrix();
    glTranslatef(-0.6,-0.1,0.6);
    glRotatef(90,0,1,0);
    gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
   glPopMatrix();
   
   glPopMatrix();
}
