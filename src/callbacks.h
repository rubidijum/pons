#ifndef _CALLBACKS_H
#define _CALLBACKS_H

#define INIT_WINDOW_WIDTH 700
#define INIT_WINDOW_HEIGHT 700
#define MAX_BEAMS 100

#define TIMER ID 0
#define TIMER_INTERVAL 20

#define HALF_GRID_SIZE 25 

extern int animation_ongoing;
extern double animation_parameter;

// Callback functions declaration
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse(int button, int state, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);

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


#endif
