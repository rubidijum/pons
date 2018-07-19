#ifndef BRIDGE_H
#define BRIDGE_H

#define MAX_BEAMS 100

#ifdef MAIN_FILE
int beamPointer;
#else
extern int beamPointer = 0;
#endif

typedef struct{
    //koordinate zgloba
    int X;
    int Y;
    //susedi zgloba
    //Joint neighbours[5];    
    //int numOfNeighbours = 0;
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

void draw_bridge();
void draw_beam(Beam* beam); 


#endif
