// CSprite class

#include "csprite.h"

void CSprite::setHome(int tileX, int tileY)
{
     home.x = tileX;
     home.y = tileY;    

     setTarget(tileX, tileY);

     destTile.x = tileX;
     destTile.y = tileY;
}

POINT CSprite::getHome()
{
     return home;
}

void CSprite::goHome()
{
     setTarget(home.x, home.y);
}
void CSprite::gotoTile(int tileX, int tileY)
{
     // current window location is {x,y}
     // now we want to move to a tile, in a straight line
     // one tile at a time
     // we'll assume that this is (1) square at a time
     destTile.x = tileX;
     destTile.y = tileY;

     POINT destPT = {tileX * TILEWIDTH, tileY * TILEHEIGHT };

     if (windowX < destPT.x)
          setFacing(EAST);
     else if (windowX > destPT.x)
          setFacing(WEST);
     else if (windowY > destPT.y)
          setFacing(NORTH);
     else if (windowY < destPT.y)
          setFacing(SOUTH);
}

// sets the TILE location ... duh
void CSprite::setTarget(int tileX, int tileY)
{
     targetTile.x = tileX;
     targetTile.y = tileY;
};

POINT CSprite::getTarget()
{
     return targetTile;
};
void CSprite::setSpeed(int newSpd)
{
 speed = newSpd;
}

int CSprite::nextFrame()
{ 
     if (++animcount > animdelay)
          animcount=0;
     else
          return 1; // use anim delay to slow down animations

     // increments current frame and checks for max
     if (++curframe > lastframe)
     {
          curframe=1;
          return 1;
     }
     else
          return 0;
}
int CSprite::getFrame()
{
     return curframe; 
}
void CSprite::setFrame(int newFrame)
{
     curframe=newFrame; 
}
void CSprite::setLastFrame(int newLastFrame)
{
     lastframe = newLastFrame; 
}
int CSprite::getLastFrame()
{
     return lastframe; 
}

void CSprite::setDelay(int newDelay)
{
     animdelay = newDelay; 
}
int CSprite::getDelay()
{
     return animdelay;
}

void CSprite::setupAnim(int numFrames, DIRS newDir, int perRow)
{
     curframe = 1;
     lastframe = numFrames-1;
     frames_per_anim = numFrames;
     facingDir = newDir;
     animdelay = 2;
     animcount = 0;
     columns = perRow; // sprite per row in sprite sheet
}

void CSprite::setXY(int newWindowX, int newWindowY)
{
     windowX = newWindowX; 
     windowY = newWindowY; 
}	

void CSprite::setMoveXY(int moveX, int moveY)
{ 
     movex = moveX;
     movey = moveY;
}

int CSprite::getX()
{
     return windowX; 
}
int CSprite::getY()
{
     return windowY; 
}

int CSprite::getMoveY()
{
     return movey; 
}
void CSprite::setMoveY(int newY)
{
     movey = newY; 
}

int CSprite::getMoveX()
{
     return movex; 
}
void CSprite::setMoveX(int newX)
{
     movex = newX;	
}

void CSprite::setX(int newWindowX)
{
     windowX = newWindowX;	
}
void CSprite::addX(int addtoWindowX)
{
     windowX += addtoWindowX; 
}

void CSprite::setY(int newWindowY)
{
     windowY = newWindowY;	
}
void CSprite::addY(int addtoWindowY)
{
     windowY += addtoWindowY; 
}

int CSprite::getHeight()
{
     return height; 
}
int CSprite::getWidth()
{
     return width;	
}

void CSprite::doMove()
{
     //move the sprite
     windowX += movex;
     windowY += movey;
}

LPDIRECT3DTEXTURE9 CSprite::getImage()
{
     return _image;
}

// returns a rect pointing to the sprites frame 
RECT CSprite::getRect()
{
     /*
     EAST	     = 1,
     NORTH	= 2,
     SOUTH	= 3,
     WEST	     = 4		
     */

     // offset starting location based on direction sprite is facing
     int offset = (facingDir-1) * frames_per_anim; // 

     RECT srcRect;

     // will return the RECT point to current sprite, within sprite sheet		
     srcRect.left   = ((curframe+offset) % columns) * width;
     srcRect.top    = ((curframe+offset) / columns) * height;
     srcRect.right  = srcRect.left + width;
     srcRect.bottom = srcRect.top  + height;

     return srcRect;
}

DIRS CSprite::getFacing()
{
     return facingDir; 
}

void CSprite::setFacing(DIRS newFacing)
{
     facingDir = newFacing; 

     switch(newFacing)
     {
     case  NORTH:	movey= -speed;	break;	// NORTH
     case  SOUTH:	movey=  speed;	break;	// SOUTH
     case  EAST:	movex=  speed;	break;	// EAST
     case  WEST:	movex= -speed;	break;	// WEST
     };
} 

bool CSprite::atDest()
{
     return ((windowX == targetTile.x * TILEWIDTH) && (windowY == targetTile.y * TILEHEIGHT));
}

POINT CSprite::getTile()
{
     POINT temp = { windowX / TILEWIDTH, windowY / TILEHEIGHT };

     return temp;
}

// returns true if not 100% in a tile
bool CSprite::isBetweenTiles()
{
     POINT offset = {windowX % TILEWIDTH, windowY % TILEHEIGHT };     

     // true if not in a tile 
     return (! (offset.x == 0 && offset.y == 0) );
}

bool CSprite::isMoving()
{
     return moving; 
}

void CSprite::setMoving(bool newState)
{
     moving = newState;	
}

void CSprite::setTileXY(int tileX, int tileY)
{
     windowX = tileX * 32;
     windowY = tileY * 32;
}

// returns a rectangle in window coordinates, of sprites current location
RECT CSprite::getWindowRect()
{
     RECT r1;
     r1.left   = windowX;
     r1.right  = r1.left + width -1;
     r1.top    = windowY;
     r1.bottom = r1.top + height -1 ;

     return r1;
}

POINT CSprite::getWindowPT()
{
     POINT tPT;
     tPT.x = windowX;
     tPT.y = windowY;

     return tPT;
}

void CSprite::setNewDIR(int newDir)
{
     switch(newDir)
     {
     case  1: movey=-speed;	facingDir = NORTH;	break;	// NORTH
     case  2: movey= speed;	facingDir = SOUTH;	break;	// SOUTH
     case  3: movex= speed;	facingDir = EAST;	break;	// EAST
     case  4: movex=-speed;	facingDir = WEST;	break;	// WEST
     };
}

// sets new state for object
void CSprite::setSTATE(STATEtype newS)
{
     STATE = newS;
     if (newS == DEAD)
          facingDir = DYING;
}

// returns the current State of object
int CSprite::getSTATE()
{
     return STATE;
}

// constructor for new sprites ... curFrame will always be (1) .. scale and rotation not currently implemented
// and this is for non-animated sprites
CSprite::CSprite(int X, int Y, int Width, int Height, char *Filename)
{
     windowX = X;
     windowY = Y;
     width = Width;
     height = Height;

     // initialize these, but not currently in use since this is non-animated sprite
     curframe = lastframe = 1;
     animdelay = 3;
     animcount = 0;
     movex = movey = 0;
     facingDir = WEST; // default

     curframe = 0;
     lastframe = 0;
     frames_per_anim = 1;
     columns = 0;

     speed = 2;

     // Load the sprite file
     LoadImage(Filename);		
}

// destructor -- the default one should release all memory allocated for this sprite
CSprite::~CSprite(void)
{
     // release the sprite sheet
     _image->Release();	
} 


void CSprite::LoadImage(char *Filename)
{
     _image = LoadTexture(Filename, D3DCOLOR_XRGB(255,0,255));
     assert(_image);        
}

void CSprite::setHasBody(bool newVal)
{
     hasBody = newVal;     
};

bool CSprite::getHasBody()
{
     return hasBody;
}