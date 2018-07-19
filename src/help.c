#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "help.h"

extern int currentWidth;
extern int currentHeight;

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
    
    float snapX = 25*1.f;
     x_t = x_t * snapX;
     x_t = roundf(x_t);
     x_t = x_t / snapX;
//         
     float snapY = 25*1.f;
     y_t = y_t * snapY;
     y_t = roundf(y_t);
     y_t = y_t / snapY;
    
    glBegin(GL_POINTS);
    
        //printf("Drawing %f %f\n", x_t, y_t);
        glVertex3f(x_t, y_t, 0.0);
    glEnd();
    glutSwapBuffers();
}

void draw_scene(void){
    
    GLfloat old_line_width[1];
    glGetFloatv(GL_LINE_WIDTH, old_line_width);
    printf("%f\n" , old_line_width[0]);
    
    glLineWidth(5.0);
        
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

void draw_road(int Xp, int Yp, int Xr, int Yr){
    
    float Xp_t, Yp_t, Xr_t, Yr_t;
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //vrati na prosli mode
    GLfloat old_line_width[1];
    glGetFloatv(GL_LINE_WIDTH, old_line_width);
    glLineWidth(2.0);
    
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
    glLineWidth(old_line_width[0]);
}
