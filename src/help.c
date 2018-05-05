#include <GL/glut.h>
#include <stdio.h>
#include "help.h"

void draw_grid(int HALF_GRID_SIZE){
    
    glColor3f(0.6,0.6,0.6);
    float GRID_STEP = 1.0/(float)HALF_GRID_SIZE;
    glBegin(GL_LINES);
        float i; 
        for(i = -1; i <= 1; i += GRID_STEP){
            //horizontal lines
            glVertex3f(-1000, (float)i, 0);
            glVertex3f(1000, (float)i, 0);
            
            //vertical lines
            glVertex3f((float)i, -1000, 0);
            glVertex3f((float)i, 1000, 0);
        }
    glEnd();
}

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
