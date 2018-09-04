#include <GL/glut.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "bridge.h"
#include "help.h"
#include "colors.h"

extern Joint A, B;
extern Beam AB;
extern Bridge most;

extern double animation_parameter;

extern unsigned bridge_loaded;

void print_bridge(){
    int i;
    printf("Bridge:\n");
    for(i = 0; i < beamPointer; i++){
        printf("[%d %d - %d %d]\n", most.beams[i].begin.X, most.beams[i].begin.Y, most.beams[i].end.X, most.beams[i].end.Y);
    }
}

void add_beam_to_bridge(){
    printf("Adding beam: Xbegin %d Ybegin %d, Xend %d, Yend %d to bridge\n", A.X, A.Y, B.X, B.Y); 
    most.beams[beamPointer++] = AB;
    print_bridge();
    
}

void undo_add_beam(){
    
    if(beamPointer >= 0){
        beamPointer--;
        print_bridge();
    }
    
}

void draw_beam(Beam* beam){
    switch(beam->purpose){
        case 'r':
            glColor3f(vaporBlue);
            break;
        case 'b':
            glColor3f(vaporPink);
            break;
    }
    draw_road(beam->begin.X, beam->begin.Y, beam->end.X, beam->end.Y);
}

void draw_bridge(){
    int i;
    if(beamPointer != 0)
        for (i = 0; i < beamPointer; i++){
            draw_point(most.beams[i].begin.X,most.beams[i].begin.Y);
            draw_point(most.beams[i].end.X,most.beams[i].end.Y);
            draw_beam(&most.beams[i]);
        }
} 

void rotate_road(Beam* beam){
    
    float dX = beam->begin.X - beam->end.X;
    float dY = beam->begin.Y - beam->end.Y;
    float distance = sqrt(dX*dX + dY*dY);
    if(beam->begin.broken){
        printf("begin is broken\n");
//          beam->begin.Y += radius*sin(animation_parameter);
        if(beam->begin.X <= beam->end.X && (distance - beam->end.Y <= 0)){
            beam->begin.X += animation_parameter/2;
            beam->begin.Y += animation_parameter;
            //glutPostRedisplay();
        }
    }else if(beam->end.broken){
        printf("end is broken\n");
        printf("%f distance\n", distance);
        //printf("cos: %f\n", cos(animation_parameter)*2);
        //printf("cos: %f\n", -sin(animation_parameter)*2);
        if(beam->end.X >= beam->begin.X/* && beam->end.Y <= */){
            beam->end.X -= animation_parameter/2;
            beam->end.Y += animation_parameter;
            
//             beam->end.X -= cos(animation_parameter);
//             beam->end.Y += -sin(animation_parameter);
        }
        printf("XXX %d YYY %d\n", beam->end.X, beam->end.Y);
    }
    
}

/*
 * Checks wheter joint j exists in the bridge
*/
bool joint_exists(Joint* j){
    int i;
    //TODO: remove magic numbers
    //170 - left coast X
    //460 - left coast Y
    if(abs(j->X - 170) <= 10 && abs(j->Y - 460) <=10){
        printf("Updating Joint j->X : %d j->Y : %d to 170, 460\n", j->X, j->Y);
        j->X = 170;
        j->Y = 460;
        return true;
    }
    //170 - right coast X
    //460 - right coast Y
    else if(abs(j->X - 530) <= 10 && abs(j->Y - 460) <=10){
        j->X = 530;
        j->Y = 460;
        return true;
    }
    for(i = 0; i <= beamPointer; i++){
        if(abs(most.beams[i].begin.X - j->X) <= 10 && abs(most.beams[i].begin.Y - j->Y) <= 10){
            j->X = most.beams[i].begin.X;
            j->Y = most.beams[i].begin.Y;
            return true;
        }else if(abs(most.beams[i].end.X - j->X) <= 10 && abs(most.beams[i].end.Y - j->Y) <= 10){
            j->X = most.beams[i].end.X;
            j->Y = most.beams[i].end.Y;
            return true;
        }
    }
    return false;
}

void load_joint(Joint *j){ //TODO: Needs a lot more work => unrealistic physics
    
    int i;
    int numOfNeighbours = 0;
    for(i = 0; i < beamPointer; i++){
        //printf("j: %d,%d BEAMbegin: %d%d BEAMend: %d%d\n", j.X, j.Y, most.beams[i].begin.X ,most.beams[i].begin.Y, most.beams[i].end.X,most.beams[i].end.X);
        
        if(j->X == most.beams[i].begin.X && j->Y == most.beams[i].begin.Y){
            numOfNeighbours++;
            j->loadM += (most.beams[i].end.X - j->X) + (most.beams[i].end.Y - j->Y);
            
        }else if(j->X == most.beams[i].end.X && j->Y == most.beams[i].end.Y){
            numOfNeighbours++;
            j->loadM += (most.beams[i].begin.X - j->X) + (most.beams[i].begin.Y - j->Y);
        }
        
        
    }
    // Break 'hanging' joints FIXME
    if((j->X == 170 && j->Y == 460) || (j->X == 530 && j->Y == 460))
        j->broken = 0;
    else if(numOfNeighbours == 1)
        j->broken = 1;
    else if(j->loadM >= MAX_LOAD)
        j->broken = 1;
    
    else j->broken = 0;
    printf("[%d, %d]  load = %d broken = %d\n",j->X, j->Y, j->loadM, j->broken);
    printf("num %d\n", numOfNeighbours);
    
}


void load_bridge(){
    
    int i;
    for(i = 0; i < beamPointer; i++){
        
        load_joint(&most.beams[i].begin);
        load_joint(&most.beams[i].end);
        
    }
    bridge_loaded = 1;
    
}


void reset_joint_load(Joint *j){
    j->loadM = 0;
}

void reset_bridge_load(){
    int i;
    for(i = 0; i < beamPointer; i++){
        reset_joint_load(&most.beams[i].begin);
        reset_joint_load(&most.beams[i].end);
    }
}
