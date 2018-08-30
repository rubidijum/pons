#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include "help.h"
#define MAIN_FILE
#include "bridge.h"

#define INIT_WINDOW_WIDTH 700
#define INIT_WINDOW_HEIGHT 700
#define MAX_BEAMS 100

#define TIMER ID 0
#define TIMER_INTERVAL 20

#define HALF_GRID_SIZE 25

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse(int button, int state, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);

bool is_on_road();

static int animation_ongoing;
double animation_parameter = 0;

char purpose;
char purposes[2]  = {'r', 'b'};

unsigned gameOver = 0;

double rotateMe = 0;

void draw_point(float x, float y);
void draw_scene(void);
void draw_road(int Xp, int Yp, int Xr, int Yr);

void draw_car();

static float carX = 0, carY = 445; //HACK: vrati na 35

GLdouble camX = 0;
GLdouble camY = 0;
GLdouble camZ = 0.5;
int currentWidth = INIT_WINDOW_WIDTH;
int currentHeight = INIT_WINDOW_HEIGHT;

float Xp, Yp, Xr, Yr;

unsigned build_bridge_mode = 1;

Joint A, B;
Beam AB;
Bridge most;

int main(int argc, char** argv){
    
    //inicijalizacija broja suseda
    A.numOfNeighbours = 0;
    B.numOfNeighbours = 0;
    
    A.neighbours = malloc(sizeof(Joint)*5);
    B.neighbours = malloc(sizeof(Joint)*5);
    
    
    purpose = 'r';
    
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
    glClearDepth(10);
    
    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    
    return 0;
}

static void on_timer(int value){
    
    animation_parameter += 0.01;
    
    if(gameOver){
        animation_ongoing = 0;
    }
    
    if(animation_ongoing){
        glutTimerFunc(10, on_timer, 0);        
    }
    
    glutPostRedisplay();
    
}

static void on_display(void){
    //printf("%f %f %f\n", camX, camY, camZ);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if(build_bridge_mode){ //build bridge
        
        gluLookAt(0, 0, 1.8, 0, 0, 0, 0, 1, 0);
        draw_grid(HALF_GRID_SIZE); //25 => 50 celija * 50 celija
        draw_scene();
        draw_bridge(); 
        draw_car();
        coordsys();
        
    }
    else {//animation + draw bridge
        
        gluLookAt(0.5, 1.1, 3.7, 0, 0, 0, 0, 1, 0);
        //gluLookAt(0.5, 1.1, 0, 0, 0, 0, 0, 1, 0);
        coordsys();
        draw_scene();
        draw_bridge();
        draw_car();
    }
    glFlush();
    
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
                animation_ongoing = 0;
                glutPostRedisplay();
            }
            break;
        case 'u':
        case 'U':
            undo_add_beam();
            glutPostRedisplay();
            break;
        case 'g':
        case 'G':
            if(!build_bridge_mode){
                if(!animation_ongoing){
                    animation_ongoing = 1;
                    glutTimerFunc(10, on_timer, 0);
                    break;
                }
            }
        case 's':
        case 'S':
            animation_ongoing = 0;
            break;
        case 'r':
        case 'R':
            animation_parameter = 0;
            carX = 0;
            carY = 445;
            break;
        case 't':
        case 'T':
            purpose = purposes[0];
            printf("%c\n", purpose);
            break;
        case 'y':
        case 'Y':
            purpose = purposes[1];
            printf("%c\n", purpose);
            break;
       
           
            
    }
}



static void on_mouse(int button, int state, int x, int y){
    
       
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
       
        
        Xp = x;
        Yp = y;
        
        float snapX = 25*1.f;
        Xp = Xp * snapX;
        Xp = roundf(Xp);
        Xp = Xp / snapX;
         
        float snapY = 25*1.f;
        Yp = Yp * snapY;
        Yp = roundf(Yp);
        Yp = Yp / snapY;
        
        A.X = Xp;
        A.Y = Yp;
        
        
    }
    
    if(joint_exists(A)){
            printf("Postoji!\n");
        AB.begin = A;
        draw_point(Xp,Yp);
    
      if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        printf("\n=============\n\nPRESSED X:%d Y:%d\n", x, y);
          printf("Released X:%d Y:%d\n", x, y);
        //dodaj suseda zglobu A i B
        add_neighbour(&A, B);
        add_neighbour(&B, A);
          
        Xr = x;
        Yr = y;
        
        float snapX = 25*1.f;
        Xr = Xr * snapX;
        Xr = roundf(Xr);
        Xr = Xr / snapX;
         
        float snapY = 25*1.f;
        Yr = Yr * snapY;
        Yr = roundf(Yr);
        Yr = Yr / snapY;
        
        B.X = Xr;
        B.Y = Yr;
        
        AB.end = B;
        AB.purpose = purpose;
        printf("begin X:%d, begin Y:%d\nend X:%d, end Y:%d\n", AB.begin.X, AB.begin.Y, AB.end.X, AB.end.Y);
        //add_beam
         int i,j;
//          if(beamPointer >= 2){
//          for(i = 0; i <= beamPointer; i++){
//              for(j = 0; j < 5; j++)
//                  printf("neighbour %d %d\n", most.beams[0].begin.neighbours[0].X, most.beams[0].begin.neighbours[0].Y);
//          }
//          }
        add_beam_to_bridge();
        draw_point(Xr,Yr);
        draw_road(Xp, Yp, Xr, Yr);
      }
    
    
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

bool is_on_road(){
    
  int i;
  for(i = 0; i <= beamPointer ; i++){
  printf("CarX : %f, carY: %f, beam start %d beam end %d animation_parameter %f\n", carX, carY, most.beams[i].begin.X , most.beams[i].end.X,animation_parameter);
      if(most.beams[i].purpose == 'r'){
          if(carX + 5 >= most.beams[i].begin.X && carX <= most.beams[i].end.X){
              printf("On road\n");
            return true;
          }else if(carX <= most.beams[i].begin.X && carX >= most.beams[i].end.X){
              printf("On road from right\n");
        }
          }
      }
      return false;
    
}

void draw_car(){
    
    //is_on_road();
    
    if(round(carX) == 650)
        gameOver = 1;
    if(round(carX) == 380 && carY >= 450)
        gameOver = 1; //fail
    
    glColor3f(1,0,0);
    
    GLUquadricObj* Cylinder;
    Cylinder = gluNewQuadric();
    gluQuadricDrawStyle(Cylinder, GLU_LINE);
    gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
   
    glPushMatrix();
   
    //carX += animation_parameter/11;
    carX = 35 + 37*animation_parameter;
    glTranslatef(animation_parameter/10, 0, 0);
    if(!(is_on_road()) && carX >= 200 && carX <= 530){
        carY = 445 + animation_parameter; //FIXME: adjust
        printf("%f\n", animation_parameter);
        glTranslatef(0, -(animation_parameter-4.49)/3, 0);
        glRotatef(-(animation_parameter-4.46)*15, 0, 0, 1);
    }
   
    //FIXME: popravi redosled mozda
    glRotatef(90, 0, 1, 0);
    glScalef(0.1,0.1,0.1);
    glTranslatef(-1.8,-2.8,-9);
   
    //sin(animation_parameter*50)/1000
    
     glPushMatrix();
         glTranslatef(0, sin(animation_parameter*50)/50, 0);
         glScalef (1.0, 0.3, 1.8);
         glutSolidCube (1.0);
         glScalef(1,1,0.5);
         glTranslatef(0,0.9,0);
         glutSolidCube(1.0);
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
