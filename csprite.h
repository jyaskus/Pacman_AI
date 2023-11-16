// CSprite class header

#ifndef CSPRITE_H
#define CSPRITE_H

//windows/directx headers
#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>

//framework-specific headers
#include "dxgraphics.h"


// diamond at bottom of tiles
#define TILEWIDTH  32
#define TILEHEIGHT 32     

// unit logic
// range (in pixels) to check for player as target
#define SIGHT_RANGE 200

typedef enum
{ // matches the order of sprites in sprite sheets now //
     EAST	     = 1,
     NORTH	= 2,
     SOUTH	= 3,
     WEST	     = 4,
     DYING     = 5
} DIRS;

// these are used to define the unit states
typedef enum
{
     SPAWN     = 1,
     CHASE     = 2,
     EVADE     = 3,
     DEAD      = 4,
     ALIVE     = 5
} STATEtype;

// CUSTOM SPRITE CLASS //
class CSprite
{
public:	
     int nextFrame();				     // increments frame and returns 1 if its reached the end
     int getFrame();				     // returns current frame
     void setFrame(int newFrame);	          // sets current frame to new value

     void setLastFrame(int newLastFrame);    // sets the last frame of animation sequence
     int getLastFrame();					 // returns current "last" frame

     void setDelay(int newDelay);		     // sets the delay used for animations
     int getDelay();					// returns current value

     void setupAnim(int numFrames, DIRS newDir, int perRow);

     void setTileXY(int tileX, int tileY);

     void setXY(int newX, int newY);
     int getX();
     int getY();
     void setX(int newX);
     void setY(int newY);
     void addX(int addtoX);
     void addY(int addtoY);

     void setMoveXY(int moveX, int moveY);
     void setMoveX(int newX);
     void setMoveY(int newY);
     int getMoveY();
     int getMoveX();

     int getHeight();
     int getWidth();

     void doMove();
     void setSpeed(int newSpd);

     // holds the units sprite sheet
     LPDIRECT3DTEXTURE9 getImage();

     // returns a rect pointing to the sprites current frame 
     RECT getRect();

     // returns a rectangle in Window coordinates, of sprites current location
     RECT getWindowRect();

     // returns current facing of sprite
     DIRS getFacing();

     // sets the facing of sprite
     void setFacing(DIRS newFacing);

     // move towards tile
     void gotoTile(int tileX, int tileY);

     // returns true if curr location equals destination tile
     bool atDest();

     // returns current x,y tile occupying
     POINT getTile();

     // checks if the sprite is moving between tiles
     bool isBetweenTiles();

     void setTarget(int tX, int tY);
     POINT getTarget();

     // sets new facing and movexy at same time, using int
     void setNewDIR(int newDIR);

     bool isMoving();				// returns true if sprite is moving
     void setMoving(bool newState);	// sets if sprite is moving

     POINT getWindowPT();	// returns current Window xy of sprite as POINT

     // sets the sprites current STATE 
     void setSTATE(STATEtype newS);

     // returns the curren STATE of sprite
     int getSTATE();

     // constructor for new sprites ... curFrame will always be (1) .. scale and rotation not currently implemented
     // and this is for non-animated sprites
     CSprite(int X, int Y, int Width, int Height, char *Filename);

     // destructor
     ~CSprite(void); 

     void setHome(int x, int y);
     POINT getHome();
     void goHome(); // sets target to home location

     void setHasBody(bool newVal);
     bool getHasBody();

private:
     STATEtype STATE;

     DIRS facingDir;       // direction sprite is facing, used for choosing correct sprite from sprite sheet

     bool moving;

     POINT destTile;
     POINT targetTile; // tile location to try to get to
     POINT home; // location it returns to when SPAWN'ing, etc.

     int windowX,windowY;
     int width,height;
     int speed; 

     bool hasBody;

     int movex,movey;
     int curframe,lastframe;
     int animdelay,animcount;
     int columns;         // used to grabs sprites from sprite map
     int frames_per_anim; // using sprite sheet, will need this

     LPDIRECT3DTEXTURE9 _image;

     void LoadImage(char *Filename);

}; // end of CSprite class

#endif
