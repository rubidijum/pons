#include <GL/glut.h>
#include <stdio.h>
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
    for (i = 0; i <= beamPointer; i++){
        draw_beam(&most.beams[i]);
    }
} 

