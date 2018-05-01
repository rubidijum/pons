#include <GL/glut.h>
#include <stdio.h>

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
void draw_scene(void);
void draw_grid(int HALF_GRID_SIZE);
static void coordsys(void);

GLdouble camX = 0;
GLdouble camY = 0;
GLdouble camZ = 0.5;

static unsigned build_bridge_mode = 1;

int main(int argc, char** argv){
    
    //inicijalizacija gluta
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    
    glClearColor(0,0,0, 1);
    
    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    
    return 0;
}

static void on_display(void){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    
    printf("%f %f %f\n", camX, camY, camZ);
        
    if(build_bridge_mode){ //build bridge
        glLoadIdentity();
        gluLookAt(0, 0, 0.5, 0, 0, 0, 0, 1, 0);
        draw_grid(25);
        draw_scene();
        coordsys();
    }
    else{ //animation + draw bridge 
        glLoadIdentity();
        gluLookAt(0.2, 0.3, 0.5, 0, 0, 0, 0, 1, 0);
        coordsys();
        draw_scene();
    }
    
    
    //nacrtaj scenu sa mrezom - hardkod posle mozda neki json 
    //dugme za kameru - sve ide u 3D
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
        case 'r':
        case 'R':
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

static void on_mouse(){
    
}

static void on_reshape(int width, int height){
    
    //TODO:fix this
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(0, (float) width / height, -10, 10);
       
}

void draw_grid(int HALF_GRID_SIZE){
    
    glColor3f(0.6,0.6,0.6);
    float GRID_STEP = 1.0/(float)HALF_GRID_SIZE;
    glBegin(GL_LINES);
        float i; 
        for(i = -1; i <= 1; i += GRID_STEP){
            //horizontal lines
            glVertex3f(-1, (float)i, 0);
            glVertex3f(1, (float)i, 0);
            
            //vertical lines
            glVertex3f((float)i, -1, 0);
            glVertex3f((float)i, 1, 0);
        }
    glEnd();
}

void draw_scene(void){
        
    //nacrtaj levu obalu
    glPushMatrix();
        glColor3f(0,1,1);
        glScalef(0.5,0.7,0.5);
        glTranslatef(-1.5, -1, 0.3);
        glutWireCube(1);
    glPopMatrix();
    
    //nacrtaj desnu obalu
    glPushMatrix();
        glColor3f(0,0.8,0);
        glScalef(0.5,0.7,0.5);
        glTranslatef(1.5, -1, 0.3);
        glutWireCube(1);
    glPopMatrix();
    
}

static void coordsys(void){
    
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
