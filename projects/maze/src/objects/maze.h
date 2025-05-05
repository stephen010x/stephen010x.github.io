#ifndef MAZE_H
#define MAZE_H

// TODO: I should create a directory to put all the headers in or something.
// and then use make to point to them or something
#include "../core/model.h"
#include "shapes.h"
#include "drawsurface.h"


//struct _DRAWSURFACE;
//typedef struct _DRAWSURFACE DRAWSURFACE;
struct _TEXTURE;
typedef struct _TEXTURE TEXTURE;


#define TRAIL_TEX_RES 256

//#define CELL_SIZE ((float)0.1)
//#define WALL_THICK ((float)0.01)
//#define CELL_SIZE ((float)0.05*1.5)
//#define WALL_THICK ((float)0.005*1.5)
#define CELL_SIZE ((float)0.075)
#define WALL_THICK ((float)0.02625)
#define WALL_HEIGHT ((float)0.05)

#define WALL_LENGTH (CELL_SIZE + WALL_THICK)


#define WALLS(__n, __m)    typeof(unsigned char[__m+1][__n])
#define WALLS_PTR(__n)     typeof(unsigned char (*)[__n])
#define GRID(__c, __r)     typeof(unsigned char[__c][__r])
#define GRID_PTR(__c, __r) typeof(unsigned char (*)[__r])



#define WALL_NONE  0
#define WALL_EXIST 1
// consider renaming to CELL_UNUSED
#define CELL_EMPTY 0
#define CELL_USED  1

#define WALL_UP_BIT     0
#define WALL_DOWN_BIT   1
#define WALL_RIGHT_BIT  2
#define WALL_LEFT_BIT   3

#define WALL_UP    (1<<WALL_UP_BIT)
#define WALL_DOWN  (1<<WALL_DOWN_BIT)
#define WALL_RIGHT (1<<WALL_RIGHT_BIT)
#define WALL_LEFT  (1<<WALL_LEFT_BIT)

typedef unsigned char WALL_FLAGS;

/*#define MAZE(__n) struct {
    int cols;
    int rows;

    COLOR color;

    //unsigned char (*hwalls)[__n];
    //unsigned char (*vwalls)[__n];

    WALLS(__n)* hwalls;
    WALLS(__n)* vwalls;
}*/




enum maze_mode {
    MAZE_MODE_NONE = 0,
    MAZE_MODE_2D,
    MAZE_MODE_3D,
    MAZE_MODE_DETAILED,
};



typedef struct {
    int mode;

    int cols;
    int rows;

    float x;
    float y;
    // scale is being ignored in favor of CELL_SIZE
    // TODO: implement this later
    float scale;

    COLOR color;
    COLOR color2;

    SHADER* shader_2d;
    SHADER* shader_3d;
    SHADER* shader_detailed;
    SHADER* shader_trail;

    void* hwalls;
    void* vwalls;
    void* grid;

    DRAWSURFACE surface;
    TEXTURE* texture_wall;
    TEXTURE* texture_wall2;
    TEXTURE* texture_wall_norm;
    TEXTURE* texture_wall_norm2;

    bool hide_upper_walls;
} MAZE;





MAZE* maze_init(   MAZE* maze, int cols, int rows, COLOR color, SHADER* shader);
void maze_destroy( MAZE* maze);
MAZE* maze_draw(   MAZE* maze, double t);

WALL_FLAGS maze_getwalls(  MAZE* maze, int c, int r);
WALL_FLAGS maze_setwalls(  MAZE* maze, int c, int r, WALL_FLAGS flags);
WALL_FLAGS maze_clearwalls(MAZE* maze, int c, int r, WALL_FLAGS flags);

void maze_getcell(MAZE* maze, float x, float y, int* c,   int* r);
void maze_getpos( MAZE* maze, int c,   int r,   float* x, float* y);



/* Holy crap this actually works...
 * This means that sizeof also works during runtime, not just compiletime


#define WALLS(__n) typeof(unsigned char[__n+1][__n])

int main() {
    for (int i = 1; i <= 10; i++) {
        WALLS(i) wall;
        printf("%lu\n", sizeof(WALLS(i)));
    }
    return 0;
}


*/



#endif
