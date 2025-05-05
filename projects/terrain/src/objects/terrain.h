#ifndef TERRAIN_H
#define TERRAIN_H

#include <cglm/cglm.h>

#include "../core/model.h"
#include "../core/camera.h"


//#define CHUNK_WIDTH  100
//#define CHUNK_HEIGHT 100
//#define CHUNK_SCALE 0.01
//#define TERRAIN_DEPTH 0.10

#define GENMODE_GEN  1
#define GENMODE_LOAD 2


#define GEN_MODEL(__n) struct { \
    int mode;                   \
                                \
    void* loaddata;             \
    int loadbytes;              \
                                \
    float scale;                \
    float x_off, y_off, z_off;  \
    int rows, cols;             \
    int sincount;               \
    struct {                    \
        float period;           \
        float height;           \
        int power;              \
        bool damp_enable;       \
        float damp_period;      \
    } sindata[__n];             \
}

#define GEN_MODEL_PAD(__n)

typedef GEN_MODEL() GEN_MODEL;

typedef typeof(((GEN_MODEL*)NULL)->sindata[0]) GEN_SINDATA;


typedef struct {
    vec3 pos;
    vec3 scale;
    //int rows;
    //int cols;

    COLOR color;
    TEXTURE* texture;
    SHADER* shader;
    MODEL model;
    MESH* mesh;
    GEN_MODEL* gen_model;
    //LIGHTSOURCE* light;
    
} TERRAIN;




void terrain_init(TERRAIN* t, vec3 pos, vec3 scale, GEN_MODEL* gen_model, COLOR color, SHADER* shader/*, LIGHTSOURCE* light*/);
void terrain_destroy(TERRAIN* t);
void terrain_draw(TERRAIN* t, double _t);
void terrain_update(TERRAIN* t, double _, float dt);

float terrain_get_height(TERRAIN* t, float x, float y);
void terrain_get_flat_norm(TERRAIN* t, float x, float y, vec3 norm);


#endif
