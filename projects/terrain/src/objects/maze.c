#include <string.h>
#include <math.h>
//#include <cglm/cglm.h>
#include "maze.h"
#include "drawsurface.h"
#include "../main.h"



// TODO: move this to macro header
#define __FORCE_INLINE__ inline __attribute__((always_inline))
// it is so stupid that I need this
#define mod(__a, __b) ({    \
    typeof(__b) b = (__b);  \
    typeof(__a) n = (__a)%b;\
    n >= 0 ? n : n + b;     \
})


typedef struct {
    unsigned char* up;
    unsigned char* down;
    unsigned char* left;
    unsigned char* right;
} MAZE_WALLS;



WALL_FLAGS maze_findwalls(MAZE* maze, int c, int r, MAZE_WALLS* walls);
void maze_walls_write(MAZE_WALLS* walls, WALL_FLAGS flags);
void maze_draw_wall(MAZE* maze, vec3 pos, vec3 scale);






__FORCE_INLINE__ bool grid_isdead(void* vgrid, int width, int height, int x, int y) {
    GRID_PTR(width, height) grid = vgrid;

    return  ((x <= 0       ) || grid[x-1][y  ] == CELL_USED) &&
            ((x >= width-1 ) || grid[x+1][y  ] == CELL_USED) &&
            ((y <= 0       ) || grid[x  ][y-1] == CELL_USED) &&
            ((y >= height-1) || grid[x  ][y+1] == CELL_USED);
}




__FORCE_INLINE__ bool grid_isused(void* vgrid, int width, int height, int x, int y) {
    GRID_PTR(width, height) grid = vgrid;

    return  (grid[x][y] == CELL_USED)   ||
            (x < 0) || (x >= width )    ||
            (y < 0) || (y >= height);
}




/*__FORCE_INLINE__ int grid_random_path(void* vgrid, int width, int height, int x, int y) {
    GRID_PTR(width, height) grid = vgrid;

    if (grid_isdead(vgrid, width, height, x, y))
        return -1;

    int dir;

    switch (dir) {
        
    };
}*/

/*  ab
    -----
    00   0, -1
    01   0,  1
    10  -1,  0
    11   1,  0

    n = ((b<<1)-1);
    a ? n : 0, a ? 0 : n
*/


void maze_room(MAZE* maze, void* vgrid, int cols, int rows, int _x, int _y, int width, int height, WALL_FLAGS flags) {
    GRID_PTR(cols, rows) grid = vgrid;

    for (int y = _y; y < _y+height; y++) {
        for (int x = _x; x < _x+width; x++) {
            grid[x][y] = CELL_USED;
        }
    }

    for (int y = _y; y < _y+height-1; y++) {
        for (int x = _x; x < _x+width; x++) {
            maze_clearwalls(maze, x, y, WALL_UP);
        }
    }

    for (int y = _y; y < _y+height; y++) {
        for (int x = _x; x < _x+width-1; x++) {
            maze_clearwalls(maze, x, y, WALL_RIGHT);
        }
    }

    if (flags & WALL_UP)
        maze_clearwalls(maze, _x+width/2, _y+height, WALL_DOWN);
        
    if (flags & WALL_DOWN)
        maze_clearwalls(maze, _x+(width-0.5)/2, _y, WALL_DOWN);
        
    if (flags & WALL_RIGHT)
        maze_clearwalls(maze, _x+width, _y+height/2, WALL_LEFT);
        
    if (flags & WALL_LEFT)
        maze_clearwalls(maze, _x, _y+(height-0.5)/2, WALL_LEFT);
}





void maze_traverse(MAZE* maze, int buffer[][2], void* vgrid, int x, int y) {
    int xmax = maze->cols;
    int ymax = maze->rows;

    GRID_PTR(xmax, ymax) grid = vgrid;
    
    buffer[0][0] = x;
    buffer[0][1] = y;

    int k = mod(rand(), 4);

    for (int i = 0; i >= 0;) {
        int x = buffer[i][0];
        int y = buffer[i][1];

        WALL_FLAGS flags = maze_getwalls(maze, x, y);

        // if dead end, then traverse backwards
        if (grid_isdead(grid, xmax, ymax, x, y)) {
            i--;
            continue;
        }
        
        // randomly pick directions
        int dx, dy;

        // make paths straighter
        if (rand()%2 == 0)
            k = mod(rand(), 4);
            
        // random rotate direction is added to remove clockwise bias
        int dir = (mod(rand(), 2)<<1) - 1;

        bool flip = false;
        // rotate through directions until an empty cell is found.
        do {
            dx = dy = 0;

            if (flip)
                k = mod(k+dir, 4);
            
            switch (k) {
                case WALL_UP_BIT:    dy =  1; break;
                case WALL_DOWN_BIT:  dy = -1; break;
                case WALL_RIGHT_BIT: dx =  1; break;
                case WALL_LEFT_BIT:  dx = -1; break;
            }
            
            //dx = ((k & 0b01)<<1) - 1;
            //dy = ((k & 0b10)<<1) - 1;

            flip = true;
        } while (grid_isused(grid, xmax, ymax, x+dx, y+dy));

        // remove cell wall
        maze_clearwalls(maze, x, y, 1<<k);

        // step to the new cell
        x += dx;
        y += dy;

        // mark new cell as used
        grid[x][y] = CELL_USED;

        i++;
        buffer[i][0] = x;
        buffer[i][1] = y;
    }
}





void maze_generate(MAZE* maze) {
    int xmax = maze->cols;
    int ymax = maze->rows;
    
    int buffer[xmax*ymax][2];
    GRID(xmax, ymax) grid;
    memset(&grid, 0, sizeof(GRID(xmax, ymax)));

    // create three main rooms pre-traversal
    maze_room(maze, &grid, xmax, ymax, xmax/2-2, 0, 4, 4, WALL_UP);
    maze_room(maze, &grid, xmax, ymax, xmax/2-2, ymax-4, 4, 4, WALL_DOWN);
    maze_room(maze, &grid, xmax, ymax, xmax/2-3, ymax/2-3, 6, 6, WALL_UP | WALL_DOWN);

    // generate random rooms
    for (int i = 0; i < 10; i++) {
        int width = RANDRANGE(2,5);
        int height = RANDRANGE(2,5);
        int x = RANDRANGE(1, xmax-width-1);
        int y = RANDRANGE(1, ymax-height-1);
        WALL_FLAGS flags = 1 << (rand()%4);
        if (rand()%2 == 0)
            flags |= 1 << (rand()%4);
        if (rand()%5 == 0)
            flags |= 1 << (rand()%4);
        if (rand()%11 == 0)
            flags |= 1 << (rand()%4);
        maze_room(maze, &grid, xmax, ymax, x, y, width, height, flags);
    }

    // generate random hallways
    for (int i = 0; i < 5; i++) {
        int width, height;
        WALL_FLAGS flags;
        if (rand()%2) {
            width = 2;
            height = RANDRANGE(5,(int)(ymax/1.5));
            flags = WALL_UP | WALL_DOWN;
            if (rand()%4 == 0) flags |= WALL_RIGHT;
            if (rand()%4 == 0) flags |= WALL_LEFT;
        } else {
            width = RANDRANGE(2,(int)(xmax/1.5));
            height = 2;
            flags = WALL_RIGHT | WALL_LEFT;
            if (rand()%4 == 0) flags |= WALL_UP;
            if (rand()%4 == 0) flags |= WALL_DOWN;
        }
        int x = RANDRANGE(1, xmax-width-1);
        int y = RANDRANGE(1, ymax-height-1);
        maze_room(maze, &grid, xmax, ymax, x, y, width, height, flags);
    }

    // find random unused spot on grid
    int x, y;
    do {
        x = rand()%xmax;
        y = rand()%ymax;
    } while (grid[x][y] == CELL_USED);

    // do initial maze traversal
    maze_traverse(maze, buffer, grid, x, y);

    // second pass of traversals to fill out any spaces the first traversal
    // was unable to.
    for (int x = 0; x < xmax; x++)
        for (int y = 0; y < ymax; y++)
            if (grid[x][y] == CELL_EMPTY)
                maze_traverse(maze, buffer, grid, x, y);
}







MAZE* maze_init(MAZE* maze, int cols, int rows, COLOR color, SHADER* shader) {
    WALLS_PTR(cols) hwalls = malloc(sizeof(WALLS(cols, rows)));
    WALLS_PTR(rows) vwalls = malloc(sizeof(WALLS(rows, cols)));
    GRID_PTR(cols, rows) grid = malloc(sizeof(GRID(cols, rows)));
    memset(grid, 0, sizeof(GRID(cols, rows)));

    *maze = (MAZE){
        .mode = MAZE_MODE_2D,
        .cols = cols,
        .rows = rows,
        .color = color,
        .shader_2d = shader,
        .shader_3d = NULL,
        .shader_detailed = NULL,
        .hwalls = hwalls,
        .vwalls = vwalls,
        .grid = grid,
    };

    // generate horizontal walls
    for (int c = 0; c < cols; c++)
        for (int r = 0; r < rows+1; r++) {
            hwalls[r][c] = WALL_EXIST;
        }

    // generate vertical walls
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols+1; c++) {
            vwalls[c][r] = WALL_EXIST;
        }

    /*for (int i = 0; i < cols; i++) {
        hwalls[2][i] = 0;
    }
    for (int i = 0; i < rows; i++) {
        vwalls[4][i] = 0;
    }*/

    /*for (int i = 0; i < 30; i++) {
        hwalls[i][0] = 0;
    }*/
    /*for (int i = 0; i < cols+1; i++) {
        vwalls[i][8] = 0;
    }*/

    /*for (int i = 0; i < 11; i++) {
        vwalls[i][9] = 0;
        hwalls[i][9] = 0;
    }*/

    maze_generate(maze);

    vec2 scale = (vec2){cols*CELL_SIZE, rows*CELL_SIZE};

    // I am just going to let the mouse hijack the callback for this
    drawsurface_init(&maze->surface, TRAIL_TEX_RES, TRAIL_TEX_RES*rows/cols, TEX_NEAREST, NULL);

    return maze;
}





void maze_destroy(MAZE* maze) {
    free(maze->hwalls);
    free(maze->vwalls);
    free(maze->grid);
}






MAZE* maze_draw(MAZE* maze, double t) {

    // I moved this to the top so that everything below can benefit from the texture bindings
    switch (maze->mode) {
        case MAZE_MODE_DETAILED:
            if (maze->texture_wall)
                texture_bind(maze->texture_wall2, maze->shader_detailed, "tex0", GL_TEXTURE0);
                    /*texture_bind_scale(maze->texture_wall, maze->shader_detailed, 
                        "tex0", GL_TEXTURE0, "u_tex_scale", 6);*/
            if (maze->texture_wall_norm) {
                texture_bind(maze->texture_wall_norm2, maze->shader_detailed, "norm0", GL_TEXTURE1);
            }

            vec3 scale = (vec3){maze->cols*CELL_SIZE, maze->rows*CELL_SIZE, 1};
            shader_set_float(maze->shader_detailed, "u_tex_scale", 3);
            //shader_set_float(maze->shader_detailed, "u_tex_scale", 6);
            shader_set_int(maze->shader_detailed, "u_mode", 1);
            {
                vec3 pos = (vec3){maze->x, maze->y, WALL_HEIGHT*2};
                draw_rect3(pos, scale, NULL, 0, maze->color2, maze->shader_detailed);
            }
            {
                vec3 pos = (vec3){maze->x, maze->y, -1};
                draw_rect3(pos, scale, NULL, 0, maze->color2, maze->shader_detailed);
            }

            shader_set_float(maze->shader_detailed, "u_tex_scale", 6);
            shader_set_int(maze->shader_detailed, "u_mode", 0);

            if (maze->texture_wall)
                texture_bind(maze->texture_wall, maze->shader_detailed, "tex0", GL_TEXTURE0);
            if (maze->texture_wall_norm) {
                texture_bind(maze->texture_wall_norm, maze->shader_detailed, "norm0", GL_TEXTURE1);
            }
            break;
    }


    WALLS_PTR(maze->cols) hwalls = maze->hwalls;
    WALLS_PTR(maze->rows) vwalls = maze->vwalls;

    // draw the left border of maze
    if (!maze->hide_upper_walls) {
        for (int r = 0; r < maze->rows; r++) {
            WALL_FLAGS flags = maze_getwalls(maze, 0, r);
            if (flags & WALL_LEFT) {
                float x, y;
                maze_getpos(maze, 0, r, &x, &y);
                //x -= CELL_SIZE/2.0;
                maze_draw_wall(maze, (vec3){x, y, 0}, 
                    (vec3){WALL_THICK, WALL_LENGTH, WALL_HEIGHT});
            }
        }

        // draw the bottom border of maze
        for (int c = 0; c < maze->cols; c++) {
            WALL_FLAGS flags = maze_getwalls(maze, c, 0);
            if (flags & WALL_DOWN) {
                float x, y;
                maze_getpos(maze, c, 0, &x, &y);
                //y -= CELL_SIZE/2.0;
                maze_draw_wall(maze, (vec3){x, y, 0}, 
                    (vec3){WALL_LENGTH, WALL_THICK, WALL_HEIGHT});
            }
        }

        // draw the top and right walls of each of the cells
        for (int r = 0; r < maze->rows; r++)
            for (int c = 0; c < maze->cols; c++) {
                WALL_FLAGS flags = maze_getwalls(maze, c, r);
                float x, y;
                maze_getpos(maze, c, r, &x, &y);
                
                if (flags & WALL_RIGHT)
                    maze_draw_wall(
                        maze, (vec3){x + CELL_SIZE, y, 0}, 
                        (vec3){WALL_THICK, WALL_LENGTH, WALL_HEIGHT});

                if ((flags & WALL_UP) /*&& (!maze->hide_upper_walls || (r != maze->rows-1))*/)
                    maze_draw_wall(
                        maze, (vec3){x, y + CELL_SIZE, 0}, 
                        (vec3){WALL_LENGTH, WALL_THICK, WALL_HEIGHT});
            }
    } else {
        #define A_Y (maze->rows-4)
        #define A_X (maze->cols/2-2)

        // draw the top and right walls of each of the cells
        for (int r = A_Y; r < A_Y+4; r++)
            for (int c = A_X; c < A_X+4; c++) {
                WALL_FLAGS flags = maze_getwalls(maze, c, r);
                float x, y;
                maze_getpos(maze, c, r, &x, &y);

                if (flags & WALL_LEFT) {
                    maze_draw_wall(maze, (vec3){x, y, 0}, 
                        (vec3){WALL_THICK, WALL_LENGTH, WALL_HEIGHT});
                }

                if (flags & WALL_DOWN) {
                    maze_draw_wall(maze, (vec3){x, y, 0}, 
                        (vec3){WALL_LENGTH, WALL_THICK, WALL_HEIGHT});
                }
                
                if (flags & WALL_RIGHT)
                    maze_draw_wall(
                        maze, (vec3){x + CELL_SIZE, y, 0}, 
                        (vec3){WALL_THICK, WALL_LENGTH, WALL_HEIGHT});
            }
    }


    // TODO: move these to their own function to be called
    // whoops. No dt
    // render stuff to the surface
    drawsurface_draw(&maze->surface, t, INFINITY);

    switch (maze->mode) {
        case MAZE_MODE_3D:
        case MAZE_MODE_2D:
            // render surface as texture plane beneath maze
            if (maze->shader_trail) {
                vec2 scale = (vec2){maze->cols*CELL_SIZE, maze->rows*CELL_SIZE};
                vec3 pos = (vec3){maze->x, maze->y, -0.0001};
                draw_texture_plane(pos, scale, NULL, 0, 
                    &maze->surface.texture, maze->shader_trail, false);
            }
            break;
    }

    return maze;
}



void maze_draw_wall(MAZE* maze, vec3 _pos, vec3 _scale) {    
    switch (maze->mode) {
        case MAZE_MODE_DETAILED:
            if (maze->shader_detailed) {
                vec3 pos, scale;
                glm_vec3_copy(_pos, pos);
                glm_vec3_copy(_scale, scale);
                scale[2] *= 2;
                /*if (maze->texture_wall)
                    texture_bind_scale(maze->texture_wall, maze->shader_detailed, 
                        "tex0", GL_TEXTURE0, "u_tex_scale", 6);
                if (maze->texture_wall_norm)
                    texture_bind(maze->texture_wall_norm, maze->shader_detailed, "norm0", GL_TEXTURE1);*/
                draw_rect3(pos, scale, NULL, 0, maze->color2, maze->shader_detailed);
                break;
            }
        case MAZE_MODE_3D:
            if (maze->shader_3d) {
                vec3 pos, scale;
                glm_vec3_copy(_pos, pos);
                glm_vec3_copy(_scale, scale);
                pos[2] -= scale[2]*1;
                scale[2] = scale[2]*2;
                draw_rect3(pos, scale, NULL, 0, maze->color, maze->shader_3d);
                break;
            }
        case MAZE_MODE_2D:
            draw_rectangle(_scale[0], _scale[1], 0, (vec2){_pos[0], _pos[1]}, maze->color, maze->shader_2d, 0);
            break;
    }
}







// TODO: finish implementing this
// it will basically just return the wall status on each four sides of the
// square pointed to with the x and y
// TODO: inline this
WALL_FLAGS maze_getwalls(MAZE* maze, int c, int r) {
    MAZE_WALLS walls;
    return maze_findwalls(maze, c, r, &walls);
}


WALL_FLAGS maze_setwalls(MAZE* maze, int c, int r, WALL_FLAGS flags) {
    MAZE_WALLS walls;
    WALL_FLAGS newflags = maze_findwalls(maze, c, r, &walls) | flags;
    maze_walls_write(&walls, newflags);
    return newflags;
}



WALL_FLAGS maze_clearwalls(MAZE* maze, int c, int r, WALL_FLAGS flags) {
    MAZE_WALLS walls;
    WALL_FLAGS newflags = maze_findwalls(maze, c, r, &walls) & (~flags);
    maze_walls_write(&walls, newflags);
    return newflags;
}






__FORCE_INLINE__ WALL_FLAGS maze_findwalls(MAZE* maze, int c, int r, MAZE_WALLS* walls) {
    WALL_FLAGS retval = 0;
    WALLS_PTR(maze->cols) hwalls = maze->hwalls;
    WALLS_PTR(maze->rows) vwalls = maze->vwalls;

    *walls = (MAZE_WALLS){
        .down  = &hwalls[ r   ][ c ],
        .up    = &hwalls[ r+1 ][ c ],
        .left  = &vwalls[ c   ][ r ],
        .right = &vwalls[ c+1 ][ r ]
    };

    if (*walls->up    == WALL_EXIST) retval |= WALL_UP;
    if (*walls->down  == WALL_EXIST) retval |= WALL_DOWN;
    if (*walls->left  == WALL_EXIST) retval |= WALL_LEFT;
    if (*walls->right == WALL_EXIST) retval |= WALL_RIGHT;

    return retval;
}




__FORCE_INLINE__ void maze_walls_write(MAZE_WALLS* walls, WALL_FLAGS flags) {
    *walls->up    = (flags & WALL_UP)    ? WALL_EXIST : WALL_NONE;
    *walls->down  = (flags & WALL_DOWN)  ? WALL_EXIST : WALL_NONE;
    *walls->left  = (flags & WALL_LEFT)  ? WALL_EXIST : WALL_NONE;
    *walls->right = (flags & WALL_RIGHT) ? WALL_EXIST : WALL_NONE;
}






// this converts position to cell and column
// TODO: finish implementing this
void maze_getcell(MAZE* maze, float x, float y, int* c, int* r) {
    *c = (int)((x - maze->x)/CELL_SIZE);
    *r = (int)((y - maze->y)/CELL_SIZE);
}




void maze_getpos(MAZE* maze, int c, int r, float* x, float* y) {
    *x = (float)c*CELL_SIZE + maze->x;
    *y = (float)r*CELL_SIZE + maze->y;
}

















// Remnants of super naive maze generation algorithm


    /*int start = RANDRANGE(0, rows);
    int end = RANDRANGE(0, rows);

    bool no_change = false;
    while (!no_change) {
        for (int y = 0; y < rows; y++)
    }*/

    // I don't have time for good maze generation, so...

    /*for (int y = 0; y < rows; y++)
        for (int x = 0; x < cols; x++) {
            MAZE_WALLS walls = maze_findwalls(maze, x, y);
            for (int i = 0; i < RANDRANGE(1,3); i++) {
                int k = rand()%4;
                switch (k) {
                    case 0:
                        *walls.right = WALL_NONE;
                        break;
                    case 1:
                        *walls.left = WALL_NONE;
                        break;
                    case 2:
                        *walls.up = WALL_NONE;
                        break;
                    case 3:
                        *walls.down = WALL_NONE;
                        break;
                }
            }
        }

    for (int x = 0; x < cols; x++) {
        MAZE_WALLS walls;
        walls = maze_findwalls(maze, x, 0);
        *walls.down = WALL_EXIST;
        walls = maze_findwalls(maze, x, rows-1);
        *walls.up = WALL_EXIST;
    }

    for (int y = 0; y < rows; y++) {
        WALL_FLAGS set = WALL_EXIST;
        if (y == maze->cols/2) set = WALL_NONE;
        MAZE_WALLS walls;
        walls = maze_findwalls(maze, 0, y);
        *walls.left = set;
        walls = maze_findwalls(maze, cols-1, y);
        *walls.right = set;
    }

    for (int i = 0; i < 3; i++) {
        MAZE_WALLS walls;
        walls = maze_findwalls(maze, i, maze->cols/2);
        *walls.left = WALL_NONE;
        walls = maze_findwalls(maze, cols-1-i, maze->cols/2);
        *walls.right = WALL_NONE;
    }*/

    /*for (int i = 0; i < 10; i++) {
        vwalls[1][i] = 0;
        hwalls[1][i] = 0;
    }*/

    /*for (int i = 0; i < 11; i++) {
        vwalls[i][9] = 0;
        hwalls[i][9] = 0;
    }*/

    /*for (int y = 1; y < 10; y++)
        for (int x = 1; x < 10; x++) {
            vwalls[x][y] = 0;
        }*/


    //MAZE_WALLS walls = maze_findwalls(maze, int 0, int 0);
    //*walls.up = 
