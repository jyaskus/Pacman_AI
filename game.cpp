/*
 Author: Jeff Yaskus
 Adapated heavily from below ...

 Beginning Game Programming, 2nd Edition
 Chapter 8
 Tiled_Sprite program source code file
*/

#include "game.h"
 
// default background map
int MAPDATA[MAPWIDTH*MAPHEIGHT] =
{
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,
     0,1,0,1,1,1,0,1,1,1,0,0,1,0,1,0,0,0,1,0,
     0,1,0,0,0,0,0,1,0,1,0,0,1,1,1,1,0,0,1,0,
     0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,0,
     0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,1,0,0,1,0,
     0,1,0,0,1,0,1,1,1,1,1,1,1,0,0,1,0,0,1,0,
     0,1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,1,1,0,
     0,0,1,0,1,0,0,0,1,1,1,1,0,1,1,1,1,1,0,0,
     0,0,1,0,1,1,1,1,1,2,3,1,1,1,0,0,0,1,0,0,
     0,0,1,0,0,0,0,0,1,4,5,1,0,0,0,1,1,1,1,0,
     0,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,0,0,1,0,
     0,1,0,0,0,0,1,0,1,0,1,0,0,1,0,1,1,1,1,0,
     0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,0,0,1,0,
     0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,1,1,0,
     0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,1,1,0,0,
     0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,0,1,1,0,
     0,1,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,
     0,1,1,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// used for displaying the sprites
LPD3DXSPRITE sprite_handler;

// score
SCOREtype score;

// overlay tiles
LPDIRECT3DTEXTURE9 tiles;

// DX font
LPD3DXFONT dxfont;
string tStr; // for text output

//timing variable
const int FRAMES_PER_SECOND = 30;
int Frames;
int Death_Delay;
HRESULT result;
long start;

// used for tracking the "dots"
int dotList[MAPWIDTH][MAPHEIGHT];

// arrays used for pathfinding
bool inList[MAPWIDTH][MAPHEIGHT];
listType Queue[MAX_PATHS];

// these are used after path found, to determine which to take
int goN = MAX_PATHS;
int goS = MAX_PATHS;
int goE = MAX_PATHS;
int goW = MAX_PATHS;

// these are used during the path finding routines
int cnt = 0;
int tCnt = 0;

// UNITs
CSprite *enemy[NUM_ENEMY];
CSprite *player;
CSprite *eyeballs;
CSprite *eatable;

// "dots" for bunny to eat
CSprite *carrot;
CSprite *apple;

// power pill timer
unsigned long long ppStart=0;
unsigned long long ppEnd=0;
bool ppActive = false;

int LIVES = 3;

// open file to log errors
fstream errorLog("errorLog.txt",ios::out);

POINT corners[4] = { {1,1}, {18,1}, {1,18}, {18,18} };

// ----------------------- GAME FUNCTIONS ----------------------- //

//initializes the game
int Game_Init(HWND hwnd)
{
     //set random number seed
     srand((unsigned int)time(NULL));

     //initialize mouse
     if (!Init_Mouse(hwnd))
     {
          MessageBox(hwnd, "Error initializing the mouse", "Error", MB_OK);
          return 0;
     }

     //initialize keyboard
     if (!Init_Keyboard(hwnd))
     {
          MessageBox(hwnd, "Error initializing the keyboard", "Error", MB_OK);
          return 0;
     }

     //create sprite handler object
     result = D3DXCreateSprite(d3ddev, &sprite_handler);
     if (result != D3D_OK)
          return 0;

     // load the font
     D3DXCreateFont( d3ddev, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Comic Sans MS", &dxfont);

     // load units
     enemy[0] =  new CSprite(0,0,32,32,"avatar1.bmp");
     assert(enemy[0] != NULL);
     enemy[0]->setTileXY(1,1); // starting location
     enemy[0]->setHome(9,9);
     enemy[0]->setSTATE(SPAWN); // move towards target
     enemy[0]->setFacing(SOUTH);
     enemy[0]->setHasBody(false);

     if (NUM_ENEMY > 1)
     {
          enemy[1] =  new CSprite(0,0,32,32,"avatar2.bmp");
          assert(enemy[1] != NULL);
          enemy[1]->setTileXY(18,1);
          enemy[1]->setHome(10,9);
          enemy[1]->setSTATE(SPAWN); // move towards target
          enemy[1]->setFacing(SOUTH);
          enemy[1]->setHasBody(false);
     }

     if (NUM_ENEMY > 2)
     {
          enemy[2] =  new CSprite(0,0,32,32,"avatar3.bmp");
          assert(enemy[2] != NULL);
          enemy[2]->setTileXY(1,18);
          enemy[2]->setHome(9,10);
          enemy[2]->setSTATE(SPAWN); // move towards target
          enemy[2]->setFacing(NORTH);
          enemy[2]->setHasBody(false);
     }

     if (NUM_ENEMY > 3)
     {
          enemy[3] =  new CSprite(0,0,32,32,"avatar4.bmp");
          assert(enemy[3] != NULL);
          enemy[3]->setTileXY(18,18);
          enemy[3]->setHome(10,10);
          enemy[3]->setSTATE(SPAWN); // move towards target
          enemy[3]->setFacing(NORTH);
          enemy[3]->setHasBody(false);
     }

     player =  new CSprite(0,0,32,32,"bunny.bmp");
     assert(player != NULL);
     player->setupAnim(4,WEST,4);
     player->setDelay(5);
     player->setTileXY(9,15);
     player->setSTATE(ALIVE); // move towards target

     carrot = new CSprite(0,0,32,32,"carrot.bmp");
     assert(carrot != NULL);
     apple  = new CSprite(0,0,32,32,"apple.bmp");
     assert(apple != NULL);
     eyeballs = new CSprite(0,0,32,32,"eyeballs.bmp");
     assert(eyeballs != NULL );
     eatable = new CSprite(0,0,32,32,"eatable.bmp");
     assert(eatable != NULL );     

     //sprites for tile background
     tiles = LoadTexture("tiles.bmp", D3DCOLOR_XRGB(255,0,255));
     assert(tiles  != NULL);

     //reset timing
     start = GetTickCount();
     Frames = 0;
     Death_Delay = 0;

     // initialize array to keep track of dots
     for (int y=0; y < MAPHEIGHT; y++)
          for (int x=0; x < MAPWIDTH; x++)
               if ( getMap(x,y) == 1)
                    dotList[x][y] = 1;
               else
                    dotList[x][y] = 0;

     // assign the power up dots
     dotList[1][1]=2;
     dotList[18][1]=2;
     dotList[1][18]=2;
     dotList[18][18]=2;

     // set the score
     score.carrots = 0;
     score.deaths  = 0;
   
     //return okay
     return 1;
}

static long frame_cnt = 0;
//the main game loop
void Game_Run(HWND hwnd)
{
     //make sure the Direct3D device is valid
     if (d3ddev == NULL)
          return;

     //update mouse and keyboard
     Poll_Keyboard();
     Poll_Mouse();

     if (Key_Down(DIK_ESCAPE))
          PostMessage(hwnd, WM_DESTROY, 0,0);

     //this keeps the game running at a steady frame rate
     if (GetTickCount() - start >= (1000 / FRAMES_PER_SECOND))
     {
          //reset timing
          start = GetTickCount();

          // for debug purposes
          frame_cnt++;

          // Collision checks 
          // player vs dots
          int dots_remaining=0;
          for (int y=0; y < MAPHEIGHT; y++)
               for (int x=0; x < MAPWIDTH; x++)
                    if (dotList[x][y] > 0)
                    {
                         // carrot rect is only 30x30 for these purposes
                         RECT r1;
                         r1.left   = (x*32) + 12;
                         r1.top    = (y*32) + 12;
                         r1.right  = r1.left + 8;
                         r1.bottom = r1.top + 8;

                         RECT r2 = player->getWindowRect();

                         if ( rectCollision(r1, r2) )
                         {
                              if ( dotList[x][y] == 2)
                              {
                                   // power pill was eaten
                                   ppStart = GetTickCount();
                                   ppEnd   = ppStart + 6000;
                                   ppActive = true;
                                   player->setSpeed(4);
                              }
                              // yummy eats
                              dotList[x][y] = 0;
                              score.carrots++;
                         }
                         else
                              dots_remaining++;
                    }

          // see if "all" the dots got eatean
          if (dots_remaining < 1)
          {
               MessageBox(hwnd, "You won! Those carrots were yummy, huh?!", "You win - Game Over", MB_OK);
               exit(0);
          }

          // check if player is dead
          // update player sprite
          if ( player->getSTATE() == DEAD )
          {
               player->setSTATE(ALIVE);
               player->setFacing(SOUTH);

               // deduct a life
               LIVES--;

               if (LIVES > 0)
               {
                   MessageBox(hwnd, "You died! But still had a life left ... keep trying.", "Game Over", MB_OK);
                   // likely lost keyboard, try to re-aquire
                   Init_Keyboard(hwnd);
               }
               else
               {
                    MessageBox(hwnd, "Sorry, you died! Poor Rabbit ... no more lives left.", "Game Over", MB_OK);
                    exit(0);                      
                    // exit from game
               }               

               // if they killed player, they should move away - so he has a chance to get back up
               // a time is displayed in middle of the screen, giving about 60s to recover
               for (int a=0; a < NUM_ENEMY; a++)
               {
                    POINT home = enemy[a]->getHome();

                    enemy[a]->setTileXY(home.x,home.y);

                    // respawn enemies at "base"
                    enemy[a]->setSTATE( SPAWN );
               }  
          }

          // player and enemies
          // but only bother if player not already "dying"       
          // ... while we are at it, check if a power pill is active
          for (int a=0; a < NUM_ENEMY; a++)
          {
               RECT r1 = enemy[a]->getWindowRect();
               RECT r2 = player->getWindowRect();

               // check if power pill was eaten
               if ( ppActive )
               {
                    ppStart = GetTickCount();

                    if (ppStart > ppEnd) 
                    {
                         ppActive = false;
                         player->setSpeed(2);
                    }
               }

               // if they are in SPAWN state, pill won't affect them
               // but will catch them once they go back into CHASE mode
               switch(enemy[a]->getSTATE())
               {
               case SPAWN: 
                    // ignore if SPAWN state
                    break;
               case EVADE:
                    if (! ppActive )
                         enemy[a]->setSTATE(CHASE);
                    break;
               case CHASE:
                    if ( ppActive)
                         enemy[a]->setSTATE(EVADE);
                    break;
               }
               

               // make sure player is alive, ghost is chasing him and it has a body ... then check for collision
               if ( player->getSTATE() == ALIVE && enemy[a]->getSTATE() == CHASE )
                    if ( rectCollision(r1, r2) )
                    {
                         // re-orient player in case "died" in middle of square
                         POINT cT = player->getTile();
                         player->setTileXY(cT.x,cT.y);

                         // turn south and "die"
                         player->setFacing(SOUTH);
                         player->setSTATE(DEAD);
                         player->setMoveXY(0,0);
                         player->setMoving(false);
                         player->setFrame(0);
                    }

               // make sure player is alive, ghost is chasing him and it has a body ... then check for collision
               if ( player->getSTATE() == ALIVE && enemy[a]->getSTATE() == EVADE )
                    if ( rectCollision(r1, r2) )
                    {
                         // turn into eye balls and head back to respawn
                         enemy[a]->setHasBody(false);
                         enemy[a]->goHome();
                         enemy[a]->setSTATE(SPAWN);
                         score.deaths++;
                    }
          }

          // update player sprite
          if (player->isBetweenTiles())
          {
               player->nextFrame();
               player->doMove();
          }
          else
          {
               // stop in each square
               player->setFrame(0);
               player->setMoveXY(0,0);
          }

          // check for player input
          POINT cPT = player->getTile();

          errorLog << "------- player state = " << player->getSTATE() << " and the bool between tiles is " << player->isBetweenTiles() << endl;

          // only allow movement if ALIVE and not between tiles
          if ( (player->getSTATE() == ALIVE) && (! player->isBetweenTiles()) )
               if (Key_Down(DIK_UP))
               {// move NORTH	               
                    // first see if valid move               
                    if ( getMap( cPT.x, cPT.y-1 ) == 1 )
                    {               
                         player->setMoveXY(0,0);
                         player->setFacing(NORTH);
                         player->setMoving(true);
                         player->doMove();
                    }
               }
               else if (Key_Down(DIK_DOWN)) 
               {
                    // first see if valid move               
                    if ( getMap(cPT.x,cPT.y+1) == 1 )
                    {               
                         player->setMoveXY(0,0);
                         player->setFacing(SOUTH);
                         player->setMoving(true);
                         player->doMove();
                    }
               }
               else if (Key_Down(DIK_RIGHT))	
               {
                    // first see if valid move               
                    if ( getMap(cPT.x+1,cPT.y) == 1 )
                    {               
                         player->setMoveXY(0,0);
                         player->setFacing(EAST);
                         player->setMoving(true);
                         player->doMove();
                    }
               }
               else if (Key_Down(DIK_LEFT))
               {
                    // first see if valid move               
                    if ( getMap(cPT.x-1,cPT.y) == 1 )
                    {               
                         player->setMoveXY(0,0);
                         player->setFacing(WEST);
                         player->setMoving(true);
                         player->doMove();
                    }
               }
               else if (Key_Down(DIK_NUMPAD5))
               {
                    player->setMoveXY(0,0);
                    player->setMoving(false);
                    player->setFrame(0);
                    player->doMove();
               }

               // check AI states
               checkAI();
     }

     /* ----- DRAW WINDOW ----- */

     //start rendering
     if (d3ddev->BeginScene())
     {
          //erase the entire background
          d3ddev->ColorFill(backbuffer, NULL, D3DCOLOR_XRGB(0,0,0));

          //start sprite handler
          sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

          // draw tiles to back buffer -- using textures so it uses transparency properly
          DrawTilesBack();

          // draw "dots" -- carrots and apples
          for (int y=0; y < MAPHEIGHT; y++)
               for (int x=0; x < MAPWIDTH; x++)
               {
                    if (dotList[x][y] == 1)
                    {
                         POINT currPT = {x,y};
                         int startX = currPT.x * 32;
                         int startY = currPT.y * 32;           
                         DrawSprite(carrot->getImage(),0,32,32,1,startX,startY);
                    }
                    else if (dotList[x][y] == 2)
                    {
                         POINT currPT = {x,y};
                         int startX = currPT.x * 32;
                         int startY = currPT.y * 32;           
                         DrawSprite(apple->getImage(),0,32,32,1,startX,startY);
                    };
               }

               // draw the sprites ontop of the tiles
               DrawSprites();

               //stop drawing
               sprite_handler->End();

               // show the SCORE
               tStr = "Food: " + toString<int>(score.carrots);
               drawText(tStr, 60, 0);  

               tStr = "Lives: " + toString<int>(LIVES);
               drawText(tStr, 180, 0);  

               // show the number of frames 
               tStr = "Frame: " + toString<int>(frame_cnt);
               drawText(tStr, (WINDOW_WIDTH/2), WINDOW_HEIGHT-30);  

               // if power pill active, show timer count down
               if (ppActive)
               {
                    int remaining = (int) ((ppEnd - ppStart) / 100);
                    tStr = "(" + toString<int>(remaining) + ")";
                    drawText(tStr, 230,2);
               }

               // show states of enemies
               for (int e = 0; e < NUM_ENEMY; e++)
               {
                    POINT cPT = enemy[e]->getTile();
                    POINT tPT = enemy[e]->getHome();                    

                    switch( enemy[e]->getSTATE() )
                    {
                    case SPAWN: tStr = "Spawn"; break;
                    case CHASE: tStr = "Chase"; break;
                    case EVADE: tStr = "Evade"; break;
                    case DEAD:  tStr = "Dead";  break;
                    default:    tStr = "Error"; break;
                    }
                    drawText(tStr, 4, 260 + (20*e) );
                    
               }

               // show current state of player              
               switch(player->getSTATE())
               {
                    case ALIVE:drawText("Player: Alive", 320,0); break;
                    case DEAD: drawText("Player: Dead" , 320,0); break;
                    default:   drawText("Player: Error", 320,0); break;
               }

               if (player->isBetweenTiles())
                    drawText("moving",580,0);

               //stop rendering
               d3ddev->EndScene();
     } // done rendering

     //display the back buffer on the Window
     d3ddev->Present(NULL, NULL, NULL, NULL);
}

//frees memory and cleans up before the game ends
void Game_End(HWND hwnd)
{
     errorLog.close();

     Kill_Keyboard();
     Kill_Mouse();

     fflush(stdout); 
     fclose(stdout);

     if (sprite_handler != NULL)
          sprite_handler->Release();
}
// END ----------------------- GAME FUNCTIONS ----------------------- //

// test for collisions
int SpriteCollision(CSprite *sprite1, CSprite *sprite2)
{
     RECT rect1;
     rect1.left   = sprite1->getX() + 1;
     rect1.top    = sprite1->getY() + 1;
     rect1.right  = sprite1->getX() + sprite1->getWidth()  -1;
     rect1.bottom = sprite1->getY() + sprite1->getHeight() -1;

     RECT rect2;
     rect2.left   = sprite2->getX() + 1;
     rect2.top    = sprite2->getY() + 1;
     rect2.right  = sprite2->getX() + sprite2->getWidth()  -1;
     rect2.bottom = sprite2->getY() + sprite2->getHeight() -1;

     RECT dest;
     return IntersectRect(&dest, &rect1, &rect2);
}

//This function does the real work of drawing a single tile from the 
//source image onto the backbuffer. 
void DrawSprite(LPDIRECT3DTEXTURE9 source,	// source surface image
                int tilenum,				// tile #
                int width,				// tile width
                int height,				// tile height
                int columns,				// columns of tiles
                int destx,				// destination x
                int desty)				// destination y
{
     //create a RECT to describe the source image
     RECT r1;

     r1.left = (tilenum % columns) * width;
     r1.top = (tilenum / columns) * height;
     r1.right = r1.left + width;
     r1.bottom = r1.top + height;

     // location to draw sprite
     D3DXVECTOR3 position((float)destx, (float)desty, 0);

     //draw the player sprite
     sprite_handler->Draw(
          source, 
          &r1,
          NULL,
          &position,
          D3DCOLOR_XRGB(255,255,255));
}

// draws the layer #2 (player objects)
void DrawSprites()
{
     int tiles_row=6; // tiles per row in sprite sheet
     int columns, rows;
     int x, y;
     POINT destPT;

     //calculate the number of columns and rows
     columns = MAPWIDTH; 
     rows    = MAPHEIGHT;

     //draw tiles onto the scroll buffer surface
     for (int a=0; a < NUM_ENEMY; a++)
     {
          x = enemy[a]->getX();
          y = enemy[a]->getY();

          // offset for isometric maps on odd rows 
          destPT.x = x; // * TILEWIDTH;
          destPT.y = y; // * TILEHEIGHT;

          //draw the player sprite
          if ( enemy[a]->getHasBody() )
          {
               if (ppActive)
                    DrawSprite(eatable->getImage(),0,eatable->getWidth(),eatable->getHeight(),1,destPT.x,destPT.y);                                   
               else
                    DrawSprite(enemy[a]->getImage(),0,enemy[a]->getWidth(),enemy[a]->getHeight(),1,destPT.x,destPT.y);
          }
          else
               // draw eye floating balls
               DrawSprite(eyeballs->getImage(),0,eyeballs->getWidth(),eyeballs->getHeight(),1,destPT.x,destPT.y);

          //          sprite_handler->Draw(avatars[a]->getImage(), &avatars[a]->getRect(),NULL,&destPT, D3DCOLOR_XRGB(255,255,255));
     }

     //  tile_num                                            columns    
     //DrawSprite(player->getImage(),player->getFrame(),player->getWidth(),player->getHeight(),4,player->getX(), player->getY());
     // location to draw sprite
     D3DXVECTOR3 position((float)player->getX(), (float)player->getY(), 0);
     sprite_handler->Draw(player->getImage(), &player->getRect(),NULL,&position, D3DCOLOR_XRGB(255,255,255));
}

//This function fills the tilebuffer with tiles representing
//the current scroll display based on scrollx/scrolly.
// draws straight to backbuffer
void DrawTilesBack()
{
     int tiles_per_row=BACK_PER_ROW; // tiles per row in sprite sheet
     int columns, rows;
     int x, y; // destX,destY;
     int tilenum;
     POINT destPT;

     //calculate the number of columns and rows
     columns = MAPWIDTH;  //WINDOWWIDTH  / TILEWIDTH;      // 32 * 20 = 640
     rows    = MAPHEIGHT; //WINDOWHEIGHT / (TILEHEIGHT/4); // 24 * 20 = 480

     //draw tiles onto the scroll buffer surface
     for (y=0; y < rows; y++)
     {
          for (x=0; x < columns; x++)
          {
               // offset for isometric maps on odd rows 
               destPT.x = x * TILEWIDTH;
               destPT.y = y * TILEHEIGHT;

               //retrieve the tile number from this position
               tilenum = MAPDATA[(y) * MAPWIDTH + (x)];

               // LAYER #1 - TILES
               //draw the tile onto the scroll buffer
               DrawSprite(tiles,tilenum,TILEWIDTH,TILEHEIGHT,tiles_per_row, destPT.x, destPT.y);
          } // for x ...
     } // for y ...
} // drawtiles ... 

// roll dice
int rollDice(int Dice, int Number = 1)
{
     int sum=0;
     for (int i=1; i <= Number; i++)
          sum += rand() % Dice + 1;

     return sum;
};

void drawText(string outText, int windowX, int windowY)
{
     RECT textbox;
     textbox.left   = windowX;
     textbox.top    = windowY;
     textbox.right  = windowX + ( outText.size() * 8);
     textbox.bottom = windowY + 16; // 90x30 box ok?

     dxfont->DrawTextA(  NULL,
                         outText.c_str(),
                         outText.size(),
                         &textbox,
                         DT_LEFT,
                         D3DCOLOR_ARGB(255, 120, 255, 120)
                         );
};

// used for debug purposes, writing numbers to Window
// draw numbers to Window
void drawNumbers(int number, int windowX, int windowY)
{
     tStr = toString<int>(number);

     RECT textbox;
     textbox.left   = windowX;
     textbox.top    = windowY;
     textbox.right  = windowX + 90;
     textbox.bottom = windowY + 30; // 90x30 box ok?

     dxfont->DrawTextA(  NULL,
                         tStr.c_str(),
                         tStr.size(),
                         &textbox,
                         DT_RIGHT,
                         D3DCOLOR_ARGB(255, 120, 255, 120)
                         );
};

int getMap( int c_x, int c_y)
{
     if ( (c_x < 1) || (c_y < 1) || (c_x > (MAPWIDTH-2)) || (c_y > (MAPHEIGHT-2)) )
     {
          return 0;
     }
     return MAPDATA[(MAPWIDTH * c_y) + (c_x)];
};

void do_FSM(int a)
{
     // first lets make sure he knows where his target location is
     switch(enemy[a]->getSTATE())
     {
     case SPAWN:
          {
               enemy[a]->goHome();

               // see if have arrived at spawn point
               if ( enemy[a]->atDest() )
               {
                    enemy[a]->setSTATE(CHASE);

                    // check if it just eyes or actually alive
                    if (! enemy[a]->getHasBody() )
                         enemy[a]->setHasBody(true);

                    // this is used to make it appear as eye-balls until its respawned
               }
          }; break;

     case CHASE:
          {
               // target players location
               POINT dest;
               dest.x = player->getTile().x;
               dest.y = player->getTile().y;
               enemy[a]->setTarget( dest.x, dest.y );
          }; break;
     case EVADE:
          {
               // find the most distant corner and go there
               int pX = player->getTile().x;
               int pY = player->getTile().y;

               int cX = enemy[a]->getTile().x;
               int cY = enemy[a]->getTile().y;

               int range=0, highest=0;
               for (int r=0; r<4; r++)
               {
                    int distance = abs(corners[r].x - pX) + (corners[r].y - pY);
                    if (distance > range)
                    {
                         range = distance;
                         highest = r;
                    }
               }

               errorLog << "corner " << highest<< " at " << corners[highest].x << "," << corners[highest].y << " is most distance from player, go there" << endl;

               POINT dest = corners[highest];
               enemy[a]->setTarget( dest.x, dest.y );          
          }; break;
     }
}

bool within( int value, int min, int max)
{
     return ((value > min) && (value < max));     
};

bool findPath(int a)
{
     /*
     This is a home-brewed version of A* based off http://en.wikipedia.org/wiki/Pathfinding#Sample_algorithm
     Also added a bit more logic to speed things up and reduce redundancy 

     Basic Rules:
     1. Create a list of the four adjacent cells, with a counter variable of the current element's counter variable + 1
     2. Check all cells in each list for the following two conditions:
     a. If the cell is a wall, remove it from the list
     b. If there is an element in the main list with the same coordinate and an equal or lower counter, remove it from the list
     c. Add all remaining cells in the list to the end of the main list
     d. Go to the next item in the list
     */

     errorLog << "START findPath ----" << endl;

     // store the current and target tile locations
     POINT currTile = enemy[a]->getTile();
     POINT target = enemy[a]->getTarget();

     errorLog << "current tile = " << currTile.x << "," << currTile.y << " and target = " << target.x << "," << target.y << endl;

     // initialize main Queue
     for (int i=0; i < MAX_PATHS; i++)
     {
          Queue[i].x = 0;
          Queue[i].y = 0;
          Queue[i].cnt = 0;
     }
 //    errorLog << "initialized main Queue (" << MAX_PATHS << ")" << endl;

     // initializing boolean list to track map locations looked at
     for (int y=0;y < MAPHEIGHT; y++)
          for (int x=0; x < MAPWIDTH; x++)
               inList[x][y] = false;

 //    errorLog << "set inList[" << MAPWIDTH << "][" << MAPHEIGHT << "] to false" << endl;

     // start list/queue with destination target location (assuming its a valid location)
     Queue[0].x = target.x;
     Queue[0].y = target.y;
     Queue[0].cnt = 1;
     cnt=1;
     inList[target.x][target.y] = true;

  //   errorLog << "set start of Queue to " << target.x << "," << target.y << " and set inList[][] = true" << endl;

// CHECK BETWEEN HERE --
     // cycle through list, until we reach current location
     bool found = false;
  //   errorLog << "found set to false" << endl;
     while ((cnt < MAX_PATHS) && (!found))
     {
  //        errorLog << " ... inside while loop ... " << endl;

          // only create this once ... to avoid fragmenting memory & other issues
          static listType tQueue[MAX_PATHS];          

          // initialize temp Queue
          for (int i=0; i < MAX_PATHS;i++)
          {
               tQueue[i].x = 0;
               tQueue[i].y = 0;
               tQueue[i].cnt = 0;
          }
          tCnt=0;
  //        errorLog << " initialized tQueue " << MAX_PATHS << " and tCnt=" << tCnt << endl;

 //         errorLog << " checking neighbors : " << endl;
          // find neighbors of each entry in list
          for (int i=0; i < cnt; i++)
          {
               int c_x   = Queue[i].x;
               int c_y   = Queue[i].y;
               int c_cnt = Queue[i].cnt;  
    //           errorLog << "Queue [" << i << "] is " << c_x << "," << c_y << "," << c_cnt << " tCnt=" << tCnt << endl;

               // if not out of range, or already in neighbor list, and map location is "walk-able" ... then add it.
               // x-1
               if (getMap((c_x-1),c_y) == 1)
                    if ( ! inList[c_x-1][c_y] )                         
                         {
  //                            errorLog << "not in list and map[" << (c_x-1) << "][" << (c_y) << "] = " << getMap((c_x-1),(c_y)) << " tCnt=" << tCnt  << endl;
                              // add to temp List
                              tQueue[tCnt].x = c_x -1;
                              tQueue[tCnt].y = c_y;
                              tQueue[tCnt].cnt = c_cnt+1;
                              tCnt++;
                         }

               // if not out of range, or already in neighbor list, and map location is "walk-able" ... then add it.
               // x+1
               if (getMap((c_x+1), c_y) == 1)
                    if ( ! inList[c_x+1][c_y] )                         
                         {
   //                           errorLog << "not in list and map[" << (c_x+1) << "][" << (c_y) << "] = " << getMap((c_x+1),(c_y)) << " tCnt=" << tCnt  << endl;
                              // add to temp List
                              tQueue[tCnt].x = c_x + 1;
                              tQueue[tCnt].y = c_y;
                              tQueue[tCnt].cnt = c_cnt+1;
                              tCnt++;
                         }

               // if not out of range, or already in neighbor list, and map location is "walk-able" ... then add it.
               // y-1
               if (getMap((c_x), (c_y-1)) == 1)
                    if ( ! inList[c_x][c_y-1] )                         
                         {
  //                            errorLog << "not in list and map[" << (c_x) << "][" << (c_y-1) << "] = " << getMap((c_x),(c_y-1)) << " tCnt=" << tCnt  << endl;
                              // add to List
                              tQueue[tCnt].x = c_x;
                              tQueue[tCnt].y = c_y-1;
                              tQueue[tCnt].cnt = c_cnt+1;
                              tCnt++;
                         }

               // if not out of range, or already in neighbor list, and map location is "walk-able" ... then add it.
               // y+1
               if (getMap((c_x), (c_y+1)) == 1)
                    if ( ! inList[c_x][c_y+1] )                         
                         {
//                              errorLog << "not in list and map[" << (c_x) << "][" << (c_y+1) << "] = " << getMap((c_x),(c_y+1)) << " tCnt=" << tCnt << endl;
                              // add to List
                              tQueue[tCnt].x = c_x;
                              tQueue[tCnt].y = c_y+1;
                              tQueue[tCnt].cnt = c_cnt+1;
                              tCnt++;
                         }
          }

          // now check if any of these temp
          // If there is an element in the main list with the same coordinate and an equal or lower counter, remove it from the list
          // update: this is redundant,since we ignore those already in the list now
          /*
          for (int p=0; p < tCnt; p++)
          {
          //if its already in list, we can assume its a lower value and ignore this one
          if (tQueue[p].cnt > 0)
          if ( inList[tQueue[p].x][tQueue[p].y] )
          tQueue[p].cnt = -1;
          }
          */

          // add it to main list if not found
          for (int p=0; p < tCnt; p++)
               if (tQueue[p].cnt > 0)
               {
                    int c_x   = tQueue[p].x;
                    int c_y   = tQueue[p].y;
                    int c_cnt = tQueue[p].cnt; 

   //                 errorLog << "adding (" << c_x << "," << c_y  << "," << c_cnt  << ") to main Queue - at " << cnt << endl;
                    // add to main list                                 
                    Queue[cnt].x = c_x;
                    Queue[cnt].y = c_y;
                    Queue[cnt].cnt = c_cnt;
                    cnt++;

                    // make sure to keep track of whats been added
                    inList[c_x][c_y] = true;
               }

          // see if current tile has been found
          found = inList[currTile.x][currTile.y];
//          errorLog << "current found boolean is = " << found << endl;
     }
     errorLog << "------------------------------" << endl;

     // dump map
     int tMAP[20][20];
     for (int ty=0; ty < MAPHEIGHT; ty++)
          for (int tx=0; tx < MAPWIDTH; tx++)
               tMAP[tx][ty]=0;
     for (int q=0; q < cnt; q++)
     {
          int tx = Queue[q].x;
          int ty = Queue[q].y;
          int tcnt = Queue[q].cnt;
          tMAP[tx][ty] = tcnt;          
     }
     
     tMAP[currTile.x][currTile.y] = -1; // mark current location with (-1)

     for (int ty=0; ty < MAPHEIGHT; ty++)
     {
          for (int tx=0; tx < MAPWIDTH; tx++)
               errorLog <<  setw(4) << tMAP[tx][ty];
          errorLog << endl;
     }
// AND -- HERE --

     // cancel all previous move settings
     enemy[a]->setMoveXY(0,0);

     // check to see if NO PATH FOUND ...
     if (!found)
     {
          errorLog << "path not found!" << endl;
          return false;
     }
     else
     {
          errorLog << "path found, choosing next step" << endl;
          // PATH was found 

          // Now, see which of the squares around the current location has the lowest cnt value
          // this will determine which path is the shortest to target

          // update: use the temporary xy map to make this easier
          // NORTH
          goN =tMAP[currTile.x][currTile.y-1]; if (goN == 0) goN = MAX_PATH;
          // SOUTH
          goS =tMAP[currTile.x][currTile.y+1]; if (goS == 0) goS = MAX_PATH;
          // EAST
          goE =tMAP[currTile.x+1][currTile.y]; if (goE == 0) goE = MAX_PATH;
          // WEST
          goW =tMAP[currTile.x-1][currTile.y]; if (goW == 0) goW = MAX_PATH;

          errorLog << " E,N,S,W = " << goE << "," << goN << "," << goS << "," << goW << endl;

          // determine which direction to move
          POINT newTile = currTile;
          switch( lowest(goE, goN, goS, goW) )
          {
               case EAST:  newTile.x++; break; // east
               case NORTH: newTile.y--; break; // north
               case SOUTH: newTile.y++; break; // south               
               case WEST:  newTile.x--; break; //west
          }

          enemy[a]->gotoTile(newTile.x, newTile.y);
          errorLog << "new goto Tile is (" << newTile.x << "," << newTile.y << ") from " << currTile.x << "," << currTile.y << endl;
     }

     // ok, we found a path ... and are moving towards it
     return true;
}

void checkAI()
{
     // check AI states                
     // change unit location(s)
     for (int a = 0; a < NUM_ENEMY; a++)
     {             
          POINT currTile = enemy[a]->getTile();

          if ( enemy[a]->isBetweenTiles() )
          {
               // dont change behavior between tiles, just keep moving
               enemy[a]->doMove();
          }
          else 
          {
               // FIND NEXT TILE ... IN PATH TOWARDS GOAL

               // FSM
               do_FSM(a);

               // OK - we should have a destination now ... lets find a path to it ...

               if ( findPath(a) )
               {
                    errorLog << "--------------------- DONE findPath() and returned true ... moving" << endl << endl;
                    enemy[a]->doMove();
               }
               else
               {
                    errorLog << "---------------------- DONE - findPath) returned FALSE! " << endl;

                    // no path found - now what ?!
                    // pick a random open direction to go ... 
                    POINT dest = currTile;

                    DIRS currDir = enemy[a]->getFacing();

                    switch(currDir)
                    {
                    case NORTH:
                         {
                              // try to continue going North
                              // scan for y-1
                              if ( getMap(currTile.x, currTile.y-1) == 1 ) // 1 = walkable
                                   dest.y--;
                         }; break;
                    case SOUTH:
                         {
                              // scan for y+1
                              if ( getMap(currTile.x, currTile.y+1) == 1 ) // 1 = walkable
                                   dest.y++;
                         }; break;
                    case EAST:
                         {
                              // scan for x+1
                              if ( getMap(currTile.x+1, currTile.y) == 1 ) // 1 = walkable
                                   dest.x++;
                         }; break;
                    case WEST:
                         {
                              // scan for x-1
                              if ( getMap(currTile.x-1, currTile.y) == 1 ) // 1 = walkable
                                   dest.x--;
                         }; break;
                    };

                    // if cant continue in current direction
                    if (currTile.x == dest.x && currTile.y == dest.y)
                    {
                         // scan for x-1 (WEST)
                         if (currDir != EAST)
                              if ( getMap(currTile.x-1, currTile.y) == 1 ) // 1 = walkable
                                   dest.x--;

                         // scan for x+1 (EAST)
                         if (currDir != WEST)
                              if ( getMap(currTile.x+1, currTile.y) == 1 ) // 1 = walkable
                                   dest.x++;

                         // scan for y-1 (NORTH)
                         if (currDir != SOUTH)
                              if ( getMap(currTile.x, currTile.y-1) == 1 ) // 1 = walkable
                                   dest.y--;

                         // scan for y+1 (SOUTH)
                         if (currDir != NORTH)
                              if ( getMap(currTile.x, currTile.y+1) == 1 ) // 1 = walkable
                                   dest.y++;
                    }

                    enemy[a]->setMoveXY(0,0);
                    enemy[a]->gotoTile(dest.x,dest.y);
                    enemy[a]->doMove();
               }
          }
     }    
}