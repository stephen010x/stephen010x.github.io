#ifndef FRACTAL_H
#define FRACTAL_H

#include "../core/model.h"



#define MOVE_SPEED 0.01
#define ROT_SPEED  0.02
#define ZOOM_SPEED 0.01
#define ZOOM_BASE  2.0


typedef struct {
    float x;
    float y;
    float zoom;
    float rot;
    MODEL model;
    MESH2(4) mesh;
} FRACTAL;


FRACTAL* FRACTAL_init(FRACTAL* self, vec2 window[2], vec3 pos);
FRACTAL* FRACTAL_update(FRACTAL* self, double t, float dt);
FRACTAL* FRACTAL_draw(FRACTAL* self, double t);


#endif
