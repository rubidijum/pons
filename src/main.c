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

//TODO: move to callbacks.h
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse(int button, int state, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);

bool is_on_road();

/*static*/ int animation_ongoing;
double animation_parameter = 0;

// Indicates if load is calculated for the bridge
unsigned bridge_loaded = 0;

// Specifies beam's purpose
char purpose;
char purposes[2]  = {'r', 'b'};

// Indicates wheter game is over or not
unsigned gameOver = 0;

// Indicates if car is falling
unsigned freeFall = 0;
double freeFall_parameter = 0;

// Remembers index of a road beam where car is positioned
int roadIndex;

//TODO: move to other file
void draw_car();

// Car coordinates
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
    
    purpose = 'r';
    
    //inicijalizacija gluta
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    
    glutInitWindowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutMouseFunc(on_mouse);
    glutReshapeFunc(on_reshape);
    
    
    glClearColor(0,0,0,1);
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
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if(build_bridge_mode){ //mode for constructing bridge
        
        gluLookAt(0, 0, 1.8, 0, 0, 0, 0, 1, 0);
        draw_grid(HALF_GRID_SIZE); //25 => 50 cells * 50 cells
        draw_scene();
        draw_bridge(); 
        draw_car();
        coordsys();
        
    }
    else { //mode for testing bridge
        
        gluLookAt(0.5, 1.1, 3.7, 0, 0, 0, 0, 1, 0);
        draw_scene();
        draw_bridge();
        draw_car();
    }
    
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
                bridge_loaded = 0;
                reset_bridge_load();
                build_bridge_mode = 1;
                printf("build_bridge_mode: %d\n", build_bridge_mode);
                animation_ongoing = 0;
                glutPostRedisplay();
            }
            break;
        case 'u':
        case 'U':
            undo_add_beam();
            bridge_loaded = 0;
            reset_bridge_load();
            glutPostRedisplay();
            break;
        case 'g':
        case 'G':
            if(!build_bridge_mode){
                if(!bridge_loaded){
                    printf("loading bridge...\n");
                    load_bridge();
                }
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
            freeFall = 0;
            freeFall_parameter = 0;
            gameOver = 0;
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
//         case 'l':
//             load_bridge();
//             break;
       
           
            
    }
}



static void on_mouse(int button, int state, int x, int y){
    /*
     * Left click pressed creates joint A
     * Left click released creates joint B
     * Beam AB is added to the bridge
    */
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
        A.broken = 0;
        
    }
    
    // Only allow existing nodes to be clicked
    if(joint_exists(&A)){
            
            
        AB.begin = A;
        draw_point(Xp,Yp);
    
      if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
//         printf("\n=============\n\nPRESSED X:%d Y:%d\n", x, y);
//           printf("Released X:%d Y:%d\n", x, y);
          
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
        B.broken = 0;
        
        joint_exists(&B); //HACK: updates joint B to have same coordinates like an existing joint
        
        AB.end = B;
        AB.purpose = purpose;
        printf("begin X:%d, begin Y:%d\nend X:%d, end Y:%d\n", AB.begin.X, AB.begin.Y, AB.end.X, AB.end.Y);

         
        // Add current beam to the bridge
        add_beam_to_bridge();
        draw_point(Xr,Yr);
        switch(purpose){
        case 'r':
            glColor3f(vaporBlue);
            break;
        case 'b':
            glColor3f(vaporPink);
            break;
        }
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

/*
 * For each road beam in the bridge, check if the car is currently between begin and end point of the beam
 *          carX
 *           | 
 * o_________v_____________o <- Y = 445
 * 
 * If car started falling, it's Y coordinate will be greater than 445
*/
bool is_on_road(){
    
  int i;
  for(i = 0; i <= beamPointer ; i++){
  //printf("CarX : %f, carY: %f, beam start %d beam end %d animation_parameter %f\n", carX, carY, most.beams[i].begin.X , most.beams[i].end.X,animation_parameter);
      if(most.beams[i].purpose == 'r'){ 
          if(carX + 5 >= most.beams[i].begin.X && carX <= most.beams[i].end.X && carY <= 445){ 
            roadIndex = i;
            return true;
          }else if(carX <= most.beams[i].begin.X && carX >= most.beams[i].end.X  && carY <= 445){
              roadIndex = i;
              return true;
        }
          }
      }
    return false;
    
}

void draw_car(){
    
    // When car 'steps' on a beam, rotate that beam if it's broken
    if(is_on_road()){ //TODO: FIXME: 
        rotate_road(&most.beams[roadIndex]);
        int i;
        for(i = 0; i < beamPointer; i++){
            if(beamPointer != roadIndex);
            rotate_road(&most.beams[i]);
            reset_bridge_load();
            load_bridge();
        }
    }
    
    
    if(round(carX) == 650)// Car has reached the other side 
        gameOver = 1;
    if(round(carX) >= 170 && carY >= 455) // Car fell off the bridge
        gameOver = 1; 
    
    glColor3f(vaporPurple);
    
    // Save old line width
    GLfloat old_line_width[1];
    glGetFloatv(GL_LINE_WIDTH, old_line_width);
    
    glLineWidth(2.0);
    
    // Initialize GLUquadricObj for wheels
    GLUquadricObj* Cylinder;
    Cylinder = gluNewQuadric();
    gluQuadricDrawStyle(Cylinder, GLU_LINE);
    gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
   
    glPushMatrix();
   
    
    /*
     * Animate car motion and update its coordinates
     * TODO: remove magic numbers
    */
    carX = 35 + 37*animation_parameter; 
    glTranslatef(animation_parameter/10, 0, 0); // Forward motion
    
    // If car fell of the bridge, translate it downwards
    if(!(is_on_road()) && carX >= 170 && carX <= 530){ 
        carY = 445 + animation_parameter; //TODO: improve accuracy
        freeFall_parameter += 0.01; 
        glTranslatef(0, -freeFall_parameter/5, 0); 
        glRotatef(-freeFall_parameter*10, 0, 0, 1); 
    }
   
    
    glRotatef(90, 0, 1, 0);
    glScalef(0.1,0.1,0.1);
    glTranslatef(-1.8,-2.8,-9);
   
    
    // Draw car body and simulate car running
    glPushMatrix();
         glTranslatef(0, sin(animation_parameter*40)/50, 0);
         glScalef (1.0, 0.3, 1.8);
         glutWireCube (1.0);
         glScalef(1,1,0.5);
         glTranslatef(0,0.9,0);
         glutWireCube(1.0);
    glPopMatrix();
   
    // Draw wheels //////////////////////////////////
    // Back left
    glPushMatrix();
        glTranslatef(0.5,-0.1,-0.6);
        glRotatef(90,0,1,0);
        gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
    glPopMatrix();
 
    // Front left
    glPushMatrix();
         glTranslatef(0.5,-0.1,0.6);
         glRotatef(90,0,1,0);
         gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
     glPopMatrix();
   
   
    // Back right
    glPushMatrix();
        glTranslatef(-0.6,-0.1,-0.6);
        glRotatef(90,0,1,0);
        gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
    glPopMatrix();

   // Front right
   glPushMatrix();
        glTranslatef(-0.6,-0.1,0.6);
        glRotatef(90,0,1,0);
        gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
   glPopMatrix();
   ///////////////////////////////////////////////// 
   
   glPopMatrix();
   
   // Revert to old line width
    glLineWidth(old_line_width[0]);
}
