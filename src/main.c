#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "help.h"

#define INIT_WINDOW_WIDTH 700
#define INIT_WINDOW_HEIGHT 700

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse(int button, int state, int x, int y);
static void on_reshape(int width, int height);
void draw_point(float x, float y);
void draw_scene(void);

GLdouble camX = 0;
GLdouble camY = 0;
GLdouble camZ = 0.5;

int currentWidth = INIT_WINDOW_WIDTH;
int currentHeight = INIT_WINDOW_HEIGHT;

static unsigned build_bridge_mode = 1;

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
        draw_grid(25); //25
        draw_scene();
        coordsys();
    }
    else{ //animation + draw bridge
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(0.5, 1.3, 3.7, 0, 0, 0, 0, 1, 0);
        coordsys();
        draw_scene();
    }
    
    //dugme za pokretanje
    //dugme za reset
    
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
        case 'g':
        case 'G':
            break;
           
            
    }
}

//FIXME
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
    
//     x_t = x_t * 10.f;
//     x_t = roundf(x_t);
//     x_t = x_t / 10.f;
//         
//     y_t = y_t * 10.f;
//     y_t = roundf(y_t);
//     y_t = y_t / 10.f;
    
    glBegin(GL_POINTS);
    
        //printf("Drawing %f %f\n", x_t, y_t);
        glVertex3f(x_t, y_t, 0.0);
    glEnd();
    glutSwapBuffers();
}

static void on_mouse(int button, int state, int x, int y){
    
    float x_f, y_f;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        printf("X:%d Y:%d\n", x, y);
        draw_point((float)x,(float)y);
        
    }
//     if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
//         printf("Released X:%d Y:%d\n", x, y);
//     }
    
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

void draw_scene(void){
    
    GLfloat old_line_width[1];
    glGetFloatv(GL_LINE_WIDTH, old_line_width);
    printf("%f\n" , old_line_width[0]);
    
    glLineWidth(4.0);
        
//     draw_point(400,400);
//     draw_point(192,465);
//     draw_point(610,465);
//     draw_point(528,494); // 0.3 -0.2
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
