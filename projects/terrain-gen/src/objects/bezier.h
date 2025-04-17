#ifndef BEZIER_H
#define BEZIER_H

#include "../core/model.h"


#define BEZIER_RESOLUTION 64

#define _BEZIER_RESOLUTION (BEZIER_RESOLUTION+1)



typedef struct {
    //MESH3(256) line;
    //MESH3(1) dot[2];
    vec3 point[4];
    MESH(BEZIER_RESOLUTION) mesh;
    MODEL model;
    bool linemode;
    GLuint point_vert_buff;
    vec3 vel[4];
} BEZIER;



void BEZIER_init(BEZIER* self, vec3 point[4], bool linemode);
void BEZIER_update(BEZIER* self, double t, float dt);
void BEZIER_draw(BEZIER* self, double t);



#endif
