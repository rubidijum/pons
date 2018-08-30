#include <GL/glut.h>
#include <stdio.h>
#include <stdbool.h>
#include "bridge.h"
#include "help.h"

// int beamPointer;
extern Joint A, B;
extern Beam AB;
extern Bridge most;


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
    //unisti susede 
    //remove_neighbours(most[beamPointer]);
    if(beamPointer >= 0){
        beamPointer--;
        print_bridge();
    }
    
}

void draw_beam(Beam* beam){
    draw_road(beam->begin.X, beam->begin.Y, beam->end.X, beam->end.Y);
}

void draw_bridge(){
    int i;
    if(beamPointer != 0)
        for (i = 0; i <= beamPointer; i++){
            draw_point(most.beams[i].begin.X,most.beams[i].begin.Y);
            draw_point(most.beams[i].end.X,most.beams[i].end.Y);
            draw_beam(&most.beams[i]);
        }
} 

bool joint_exists(Joint j){
    int i;
    /*
     draw_point(170,460); // 0.3 -0.2
    draw_point(530,460);*/
    if((abs(j.X - 170) <= 10 && abs(j.Y - 460) <=10) || (abs(j.X - 530) <= 10 && abs(j.Y - 460) <=10))
        return true;
    for(i = 0; i <= beamPointer; i++){
        //printf("********\n%d == %d || %d == %d\n**************", most.beams[i].begin.X, j.X, most.beams[i].end.X, j.X);
        //printf("--------\n%d == %d || %d == %d\n--------------", most.beams[i].begin.Y, j.Y, most.beams[i].end.Y, j.Y);
        if((abs(most.beams[i].begin.X - j.X) <= 10 && abs(most.beams[i].begin.Y - j.Y) <= 10) || (abs(most.beams[i].end.X - j.X) <= 10 && abs(most.beams[i].end.Y - j.Y) <= 10)){
            //printf("jeaHHHSHADJSAHJDHSAKDJASKh\n\n");
            return true;
        }
    }
    return false;
}


void add_neighbour(Joint* j, Joint n){
    
    j->neighbours[j->numOfNeighbours] = n;
    
}
