// Beginning Game Programming, 2nd Edition
// Chapter 8
// Tiled_Sprite program header file


#ifndef GAME_H
#define GAME_H

#ifndef DIRECTINPUT_VERSION
     #define DIRECTINPUT_VERSION 0x0800
#endif

//windows/directx headers
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dxerr9.h>
#include <dsound.h>
#include <dinput.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <string> 
#include <iostream> 
#include <iomanip>
#include <fstream>
#include <assert.h>
#include <sstream>
using namespace std;

//framework-specific headers

#include "dxaudio.h"
#include "dxinput.h"
#include "dxgraphics.h"
#include "csprite.h"

// misc utility functions
#include "util.h"

/* ------ DEFINE(s) ------ */

//application title
#define APPTITLE "Yaskus - Game AI example"

#define DEBUG_ON false
#define _DEBUG 1

//Window setup
#define FULLSCREEN 0       //0 = windowed, 1 = fullscreen
#define WINDOW_WIDTH  640 
#define WINDOW_HEIGHT 640

// tile info
#define TILEWIDTH  32
#define TILEHEIGHT 32      
#define BACK_PER_ROW 6  // number of brackground tiles per row

// map
#define MAPWIDTH   20
#define MAPHEIGHT  20 //  768 / 16 = 48
#define GAMEWORLDWIDTH  (TILEWIDTH * MAPWIDTH)
#define GAMEWORLDHEIGHT (TILEHEIGHT * MAPHEIGHT)

// scrolling window size
#define WINDOWWIDTH  (WINDOW_WIDTH / TILEWIDTH)   * TILEWIDTH
#define WINDOWHEIGHT (WINDOW_HEIGHT / TILEHEIGHT) * TILEHEIGHT

// Path finding
#define MAX_PATHS (MAPWIDTH * MAPHEIGHT)

// Enemy AIs
#define NUM_ENEMY 4

/* --------- STRUCTURES ------------- */
struct listType
{
     int x;
     int y;
     int cnt;
};

struct SCOREtype
{
     int deaths;
     int carrots;
} ;


/* ------ function prototypes ------ */
 int Game_Init(HWND);
void Game_Run(HWND);
void Game_End(HWND);

// dynamic scrolling map support functions
void DrawSprite(LPDIRECT3DTEXTURE9, int,int,int,int, int, int);
void DrawTilesBack();
void DrawSprites();

// roll dice
int rollDice(int , int);

// for drawing text and numbers using DX font
void drawText(string outText, int windowX, int windowY);

// used for debug purposes, writing numbers to Window
// draw numbers to Window
void drawNumbers(int number, int windowX, int windowY);

// returns the value at map location
int getMap( int x, int y);

// executes the AI code, for pathfinding and such
void checkAI();

template<class T>
    std::string toString(const T& t)
{
     std::ostringstream stream;
     stream << t;
     return stream.str();
}

#endif
