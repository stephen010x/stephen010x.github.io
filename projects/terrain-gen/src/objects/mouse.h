#ifndef MOUSE_H
#define MOUSE_H

// includes COLOR
#include "../core/model.h"
#include "maze.h"
#include "shapes.h"
#include "../core/camera.h"




#define DEG_TO_RAD(__n) ((__n)*MATH_PI/180.0)


enum mouse_modes {
    MOUSE_NULL = 0,
    MOUSE_TOP,
    MOUSE_FPS,
};


typedef struct {
    int mode;
    float _x, _y;
    float x, y;
    float px, py;

    // for animations
    float dx, dy;
    vec3 wave;
    vec3 wave2;
    //vec3 _wave;

    bool is_moving;

    float sample[3];
    
    float scale;
    float speed;
    COLOR color;
    COLOR pcolor;
    MAZE* maze;
    SHADER* shader2;
    SHADER* shader3;
    CAMERA trail_camera;
    CAMERA camera;
    
    vec3 _cpos;
} PMOUSE;


PMOUSE* mouse_init(PMOUSE* mouse, float x, float y, float scale, COLOR color, MAZE* maze, SHADER* shader);
void mouse_draw(PMOUSE* mouse, double t);
void mouse_update(PMOUSE* mouse, double t, float dt);


#endif
