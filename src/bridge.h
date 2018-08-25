#ifndef BRIDGE_H
#define BRIDGE_H

#include <stdbool.h>

#define MAX_BEAMS 100

#ifdef MAIN_FILE
int beamPointer;
#else
extern int beamPointer = 0;
#endif

typedef struct _joint{
    //koordinate zgloba
    int X;
    int Y;
    //susedi zgloba
    struct _joint* neighbours;    
    int numOfNeighbours;
}Joint;

typedef struct{
    //pocetna i krajnja tacka
    Joint begin;
    Joint end;
}Beam;

typedef struct{
    Beam beams[MAX_BEAMS];
}Bridge;

void add_beam_to_bridge();
void print_bridge();
void undo_add_beam();
bool joint_exists(Joint j);

void draw_bridge();
void draw_beam(Beam* beam); 


#endif
