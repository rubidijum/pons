#include "callbacks.h"


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
