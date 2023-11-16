#include "game.h"

int lowest(int a, int b, int c, int d);

int highest(int a, int b, int c, int d);

//returns Window coordinates for a given x,y tile location
POINT tile_to_world(int tilex, int tiley);

// test for collisions
bool rectCollision(RECT r1, RECT r2);

// returns a RECT for collisions
// input is tile x,y not Window xy
RECT getTileRECT(int tilex,int tiley);