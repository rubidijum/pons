#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static void on_timer(int value);
static void on_keyboard(unsigned char key, int x, int y);
void draw_scene(void);
void display(void);

static int animation_ongoing;
double animation_parameter = 0;


void coordsys(void){
    
    //x osa => crvena
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex3f(-100000, 0, 0);
    glVertex3f(100000, 0, 0);
    glEnd();
    
    //y osa => zelena
    glColor3f(0,1,0);
    glBegin(GL_LINES);
    glVertex3f(0, -100000, 0);
    glVertex3f(0, 100000, 0);
    glEnd();
    
    //z osa => plava
    glColor3f(0,0,1);
    glBegin(GL_LINES);
    glVertex3f(0, 0, -100000);
    glVertex3f(0, 0, 100000);
    glEnd();
    
}

void init(void) 
{
   glClearColor(0,0,0, 1);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();             /* clear the matrix */
   
           /* viewing transformation  */
   gluLookAt (1.0, 0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   
   draw_scene();
   
    GLUquadricObj* Cylinder;
    Cylinder = gluNewQuadric();
    gluQuadricDrawStyle(Cylinder, GLU_FILL);
    gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
//    
    glColor3f (1.0, 0, 1.0);
    glPushMatrix();
    glScalef(0.2,0.2,0.2);
    glTranslatef(animation_parameter, 0, 0);
    glRotatef(90, 0,1,0);
    
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
     glTranslatef(-0.5,-0.1,-0.6);
     glRotatef(90,0,1,0);
     gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
    glPopMatrix();
    
    
    glPushMatrix();
     glTranslatef(-0.5,-0.1,0.6);
     glRotatef(90,0,1,0);
     gluCylinder(Cylinder, 0.2,0.2, 0.1, 10, 10);
    glPopMatrix();
   
   coordsys();
   glFlush ();
   glPopMatrix();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

int main(int argc, char** argv)
{
   //inicijalizacija gluta
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(on_keyboard);
    
    glutReshapeFunc(reshape);
    
    
    glClearColor(0,0,0, 1);
    glClearDepth(10);
    
    //glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    
    
   return 0;
}

static void on_timer(int value){
    
    animation_parameter += 0.01;
    
    if(animation_ongoing){
        glutTimerFunc(10, on_timer, 0);        
    }
    printf("glutPostRedisplay\n");
    glutPostRedisplay();
    
}

static void on_keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27:
            exit(0);
            break;
        case 'g':
        case 'G':
            if(!animation_ongoing){
                printf("Hej\n");
                animation_ongoing = 1;
                glutTimerFunc(10, on_timer, 0);
                break;
            }
        case 's':
        case 'S':
            animation_ongoing = 0;
            break;
       
           
            
    }
}

void draw_scene(void){
       
    glColor3f(1,1,1);
    glLineWidth(2);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(-0.5, -0.5, -0.5);
        glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(0.5, 0.5, 0.5);
    glEnd();
    
    /*/nacrtaj levu obalu
    glPushMatrix();
        glColor3f(0,1,1);
        glScalef(0.5,0.8,0.5);
        glTranslatef(-1.54, -0.9, 0.3);
        glutSolidCube(1);
    glPopMatrix();
    
    //nacrtaj desnu obalu
    glPushMatrix();
        glColor3f(0,0.8,0);
        glScalef(0.5,0.8,0.5);
        glTranslatef(1.54, -0.9, 0.3);
        glutWireCube(1);
    glPopMatrix();*/
    
    //glLineWidth(old_line_width[0]);
    
}
