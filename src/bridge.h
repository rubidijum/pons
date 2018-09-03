#ifndef BRIDGE_H
#define BRIDGE_H

#include <stdbool.h>
#include "colors.h"

#define MAX_BEAMS 100
//TODO: adjust this to be more realistic
#define MAX_LOAD 200 


#ifdef MAIN_FILE
int beamPointer;
#else
extern int beamPointer = 0;
#endif

typedef struct _joint{
    //Joint coordinates
    int X;
    int Y;
    //Indicator wheter or not Joint is broken
    int broken;
    //Current load on Joint
    int loadM;
}Joint;

typedef struct{
    //Contains Beam purpose -> road or (supporting) beam
    char purpose;
    //Starting Joint
    Joint begin;
    //Ending Joint
    Joint end;
}Beam;

typedef struct{
    //Initial pins
    Joint startPin, endPin;
    //Array of all beams in the bridge
    Beam beams[MAX_BEAMS];
}Bridge;

/*
 * Adds current beam to the bridge
*/
void add_beam_to_bridge();

/*
 * Prints all beams in the bridge by their X and Y coordinates
*/
void print_bridge();

/*
 * Removes last added beam
*/
void undo_add_beam();

/*
 * Checks wheter Joint j already exists in the bridge
*/
bool joint_exists(Joint* j);

/*
 * Calculates and updates Joint j's current load
*/
void load_joint(Joint *j);

/*
 * Draws bridge 
*/
void draw_bridge();

/*
 * Draws particular Beam, beam
*/
void draw_beam(Beam* beam); 

/*
 * Calculates load for all Joints in the bridge
*/
void load_bridge();
/*
 * Rotates particular Beam, beam
*/
void rotate_road(Beam* beam);
/*
 * Resets load for all joints in the bridge
*/
void reset_bridge_load();
/*
 * Resets load for particular joint
*/
void reset_joint_load(Joint *j);


#endif
