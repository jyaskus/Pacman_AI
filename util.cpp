#include "util.h"

int lowest(int a, int b, int c, int d)
{
     int sorted[4] = {a,b,c,d};
     int n = 4;

     // sort highest to top, lowest to bottom
       for ( int i = 0; i < n-1; ++i )
         for ( int j = 1; j < n-i; ++j )
           if ( sorted[j-1] > sorted[j] )
             // Note the use here of swap()
           {
                //  swap( array[j-1], array[j] );
                int temp = sorted[j];
                sorted[j] = sorted[j-1];
                sorted[j-1] = temp;                
           }

     if (a == sorted[0])
          return 1;
     if (b == sorted[0])
          return 2;
     if (c == sorted[0])
          return 3;
     if (d == sorted[0])
          return 4;

     return 0;
}

int highest(int a, int b, int c, int d)
{
     int sorted[4] = {a,b,c,d};
     int n = 4;

     // sort highest to top, lowest to bottom
       for ( int i = 0; i < n-1; ++i )
         for ( int j = 1; j < n-i; ++j )
           if ( sorted[j-1] < sorted[j] )
           {
                //  swap( array[j-1], array[j] );
                int temp = sorted[j];
                sorted[j] = sorted[j-1];
                sorted[j-1] = temp;                
           }

     if (a == sorted[0])
          return 1;
     if (b == sorted[0])
          return 2;
     if (c == sorted[0])
          return 3;
     if (d == sorted[0])
          return 4;

     return 0;
}

//returns Window coordinates for a given x,y tile location
POINT tile_to_world(int tilex, int tiley)
{
     POINT tPoint;

     tPoint.x = tilex * TILEWIDTH;
     tPoint.y = tiley * TILEHEIGHT;

     return tPoint;
}

// test for collisions
bool rectCollision(RECT r1, RECT r2)
{

     return ( r1.left   < r2.right  &&
          r1.top    < r2.bottom &&
          r1.right  > r2.left   &&		 
          r1.bottom > r2.top    );
}

// returns a RECT for collisions
// input is tile x,y not Window xy
RECT getTileRECT(int tilex,int tiley) 
{
     RECT tile;

     // returns a rectangle pointing to tile of tile (based on the xy coords provided)
     tile.left  = tilex * TILEWIDTH;
     tile.right = tile.left + TILEWIDTH -1;

     tile.top    = tiley * TILEHEIGHT;
     tile.bottom = tile.top + TILEHEIGHT -1;

     return tile;
}