#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "help.h"

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



GLdouble camX = 0;
GLdouble camY = 0;
GLdouble camZ = 0.5;

int currentWidth = INIT_WINDOW_WIDTH;
int currentHeight = INIT_WINDOW_HEIGHT;

int beamPointer = 0;

float Xp, Yp, Xr, Yr;


static unsigned build_bridge_mode = 1;

typedef struct{
    //koordinate zgloba
    int X;
    int Y;
    //susedi zgloba
    //Joint neighbours[5];    
    //int numOfNeighbours = 0;
}Joint;

typedef struct{
    //pocetna i krajnja tacka
    Joint begin;
    Joint end;
}Beam;

typedef struct{
    Beam beams[MAX_BEAMS];
}Bridge;

Joint A, B;
Beam AB;
Bridge most;

void add_beam_to_bridge();
void print_bridge();
void undo_add_beam();

void draw_bridge();
void draw_beam(Beam* beam);

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
        coordsys();
    }
    else{ //animation + draw bridge
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(0.5, 1.3, 3.7, 0, 0, 0, 0, 1, 0);
        coordsys();
        draw_scene();
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
            
        case 'g':
        case 'G':
            break;
           
            
    }
}

//FIXME: add snap to grid
void draw_point(float x, float y){
    glColor3f(255, 69, 0);
    glPointSize(10);
    
    float x_t, y_t;
    
    
    if((x >= currentWidth/2 && x < currentWidth) && (y > 0 && y <= currentHeight/2)){ //prvi kvadrant
            x_t = (x - currentWidth/2)/(float)(currentWidth/2);
            y_t = (currentHeight/2 - y)/(float)(currentHeight/2);
    }else if((x <= currentWidth/2 && x >= 0) && (y <= currentHeight/2 && y >= 0)){ //drugi kvadrant
             x_t = (x - (currentWidth/2))/(float)(currentWidth/2);
             y_t = ((currentHeight/2) - y)/(float)(currentHeight/2);
    }else if((x <= (currentWidth/2) && x >= 0) && (y <= currentHeight && y >= (currentHeight/2))){ //treci kvadrant
             x_t = (x - (currentWidth/2))/(float)(currentWidth/2);
             y_t = ((currentHeight/2) - y)/(float)(currentHeight/2);
    }else if((x <= currentWidth && x >= (currentWidth/2)) && (y <= currentHeight && y >= (currentHeight/2))){ 
             x_t = (x - (currentWidth/2))/(float)(currentWidth/2);
             y_t = ((currentHeight/2) - y)/(float)(currentHeight/2);
    }
    
    printf("Before rounding: \nX = %f\nY = %f\n", x_t, y_t);
    
    float snapX = HALF_GRID_SIZE*1.f;
     x_t = x_t * snapX;
     x_t = roundf(x_t);
     x_t = x_t / snapX;
//         
     float snapY = HALF_GRID_SIZE*1.f;
     y_t = y_t * snapY;
     y_t = roundf(y_t);
     y_t = y_t / snapY;
    
    glBegin(GL_POINTS);
    
        //printf("Drawing %f %f\n", x_t, y_t);
        glVertex3f(x_t, y_t, 0.0);
    glEnd();
    glutSwapBuffers();
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

void draw_road(int Xp, int Yp, int Xr, int Yr){
    
    float Xp_t, Yp_t, Xr_t, Yr_t;
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //vrati na prosli mode
    
    //TODO: prebaci ovo u funkciju
     if((Xp >= currentWidth/2 && Xp < currentWidth) && (Yp > 0 && Yp <= currentHeight/2)){ //prvi kvadrant
            printf("prvi\n");
            Xp_t = (Xp - currentWidth/2)/(float)(currentWidth/2);
            Yp_t = (currentHeight/2 - Yp)/(float)(currentHeight/2);
     }else if((Xp <= currentWidth/2 && Xp >= 0) && (Yp <= currentHeight/2 && Yp >= 0)){ //drugi kvadrant
         printf("DRUGI\n");
            printf("%d => %d\n", Xp, Yp);
              Xp_t = (Xp - (currentWidth/2))/(float)(currentWidth/2);
              Yp_t = ((currentHeight/2) - Yp)/(float)(currentHeight/2);
              printf("%f => %f\n", Xp_t, Yp_t);
     }else if((Xp <= (currentWidth/2) && Xp >= 0) && (Yp <= currentHeight && Yp >= (currentHeight/2))){ //treci kvadrant
         printf("treci\n");
              Xp_t = (Xp - (currentWidth/2))/(float)(currentWidth/2);
              Yp_t = ((currentHeight/2) - Yp)/(float)(currentHeight/2);
     }else if((Xp <= currentWidth && Xp >= (currentWidth/2)) && (Yp <= currentHeight && Yp >= (currentHeight/2))){ 
         printf("cetvrti\n");
              Xp_t = (Xp - (currentWidth/2))/(float)(currentWidth/2);
              Yp_t = ((currentHeight/2) - Yp)/(float)(currentHeight/2);
     }
     
     printf("DRAWING\n Xp - %f\n Yp - %f\n Xr - %f\n Yr - %f\n\n", Xp_t, Yp_t, Xr_t, Yr_t);
     
     if((Xr >= currentWidth/2 && Xr < currentWidth) && (Yr > 0 && Yr <= currentHeight/2)){ //prvi kvadrant
             Xr_t = (Xr - currentWidth/2)/(float)(currentWidth/2);
             Yr_t = (currentHeight/2 - Yr)/(float)(currentHeight/2);
     }else if((Xr <= currentWidth/2 && Xr >= 0) && (Yr <= currentHeight/2 && Yr >= 0)){ //drugi kvadrant
              Xr_t = (Xr - (currentWidth/2))/(float)(currentWidth/2);
              Yr_t = ((currentHeight/2) - Yr)/(float)(currentHeight/2);
     }else if((Xr <= (currentWidth/2) && Xr >= 0) && (Yr <= currentHeight && Yr >= (currentHeight/2))){ //treci kvadrant
              Xr_t = (Xr - (currentWidth/2))/(float)(currentWidth/2);
              Yr_t = ((currentHeight/2) - Yr)/(float)(currentHeight/2);
     }else if((Xr <= currentWidth && Xr >= (currentWidth/2)) && (Yr <= currentHeight && Yr >= (currentHeight/2))){ 
              Xr_t = (Xr - (currentWidth/2))/(float)(currentWidth/2);
              Yr_t = ((currentHeight/2) - Yr)/(float)(currentHeight/2);
     }
    
    printf("drawing\n Xp - %f\n Yp - %f\n Xr - %f\n Yr - %f\n\n", Xp_t, Yp_t, Xr_t, Yr_t);
    
    glBegin(GL_POLYGON);
        glVertex3f(Xp_t, Yp_t, 0.42);
        glVertex3f(Xr_t, Yr_t, 0.42);
        glVertex3f(Xr_t, Yr_t, -0.1);
        glVertex3f(Xp_t, Yp_t, -0.1);
    glEnd();
    
    glutSwapBuffers();
    
}

void draw_scene(void){
    
    GLfloat old_line_width[1];
    glGetFloatv(GL_LINE_WIDTH, old_line_width);
    printf("%f\n" , old_line_width[0]);
    
    glLineWidth(4.0);
        
//     draw_point(400,400);
//     draw_point(192,465);
//     draw_point(610,465);
     draw_point(170,460); // 0.3 -0.2
     draw_point(530,460);
     //draw_road(150, 500, 400, 500);
//     
    //nacrtaj levu obalu
    glPushMatrix();
        glColor3f(0,1,1);
        glScalef(0.5,0.8,0.5);
        glTranslatef(-1.54, -0.9, 0.3);
        glutWireCube(1);
    glPopMatrix();
    
    //nacrtaj desnu obalu
    glPushMatrix();
        glColor3f(0,0.8,0);
        glScalef(0.5,0.8,0.5);
        glTranslatef(1.54, -0.9, 0.3);
        glutWireCube(1);
    glPopMatrix();
    
    glLineWidth(old_line_width[0]);
    
}

void print_bridge(){
    int i;
    printf("Bridge:\n");
    for(i = 0; i < beamPointer; i++){
        printf("[%d %d - %d %d]\n", most.beams[i+1].begin.X, most.beams[i+1].begin.Y, most.beams[i+1].end.X, most.beams[i+1].end.Y);
    }
}

void add_beam_to_bridge(){
    printf("Adding beam: Xbegin %d Ybegin %d, Xend %d, Yend %d to bridge\n", A.X, A.Y, B.X, B.Y); 
    most.beams[++beamPointer] = AB;
    print_bridge();
    
}

void undo_add_beam(){
    beamPointer--;
    print_bridge();
}

void draw_beam(Beam* beam){
    draw_road(beam->begin.X, beam->begin.Y, beam->end.X, beam->end.Y);
}

void draw_bridge(){
    int i;
    for (i = 0; i <= beamPointer; i++){
        
        draw_beam(&most.beams[i]);
    }
}
