#ifndef MODEL_H
#define MODEL_H

#include <stddef.h>
#include <stdbool.h>
#include <GLES2/gl2.h>
#include <cglm/types.h>
// for the LENOF macro
// TODO: move the LENOF macro to a more general file instead of main.h
#include "main.h"


#define __FORCE_INLINE__ inline __attribute__((always_inline))


// OLD notes for old version of mesh
    /*
    Alright, I just wanted to demonstrate that there are three ways to
    allocate space for our mesh on the stack.

    --Method 1--
    
    MESH* circle_mesh = alloca(sizeof(MESH) + CIRC_RES*4);

    --Method 2--

    volatile char _reserved[sizeof(MESH) + CIRC_RES*4];
    MESH* circle_mesh = (MESH*)_reserved;

    --Method 3--
    
    MESH circle_mesh;
    volatile char _reserved[CIRC_RES*4];

    --NOTE--

    renaming _reserved to circle_mesh_verts would really twist some fingers
    */


/*typedef struct {
    int verts;
    GLenum mode;
    union {
        vec3 verts[0];
    }
    GLfloat data[];
} MESH;*/


//#define LENOF(__n) (sizeof(__n)/sizeof((__n)[0]))


// to be used for sizeof or LENOF only. Otherwise you will segfault
#define VEC2_DUMMY (*(vec2*)NULL)
#define VEC3_DUMMY (*(vec3*)NULL)


// TODO: I should've put the 3d/2d stuff in the mesh mode, not model drawtype!
#define MESH(__n) struct {  \
    int verts;              \
    GLenum mode;            \
    union {                 \
        vec2 v2[0];         \
        vec3 v3[0];         \
    };                      \
    GLfloat data[__n];      \
}
#define MESH2(__n) MESH((__n)*LENOF(VEC3_DUMMY))
#define MESH3(__n) MESH((__n)*LENOF(VEC3_DUMMY))
typedef MESH() MESH;





//typedef (int (*DRAW_CALLBACK)(int));
typedef void (*UPDATE_CALLBACK)(void*, double, float);




typedef union {
    struct {
        GLfloat r;
        GLfloat g;
        GLfloat b;
        GLfloat w;
    };
    GLfloat raw[4];
} COLOR;



#define MODEL_DRAWVOID 0


enum drawtype {
    DRAYTYPE_NONE = 0,
    DRAWTYPE_2D_PLAIN,
    DRAWTYPE_3D_PLAIN,
};


typedef struct {
    // user set items
    COLOR color;
    MESH* mesh;
    int id; // for indexing behavior data
    //float x;
    //float y;
    //float scale;
    //float rot;
    bool visable;
    int drawtype;
    UPDATE_CALLBACK update_call;
    GLuint shader_prog;
    mat4 view_mat;

    // background items
    GLuint vert_buff;
    GLint u_mod_mat_loc;
    GLint u_color_loc;
    GLint vert_pos_loc;
    GLint u_light_norm_loc;
    GLint u_light_map_loc;
} MODEL;


// expects the user set items to be populated
MODEL* MODEL_init(MODEL* model);
int MODEL_draw(MODEL* model);

__FORCE_INLINE__ void MODEL_update(MODEL* model, double t, float dt) {
    if (model->update_call != NULL)
        return model->update_call(model, t, dt);
}



// TODO: replace this with MESH_sizeof once you
// make the drawmode in mesh instead of model
__FORCE_INLINE__ size_t MODEL_MESH_sizeof(void* m) {
    MODEL* model = (MODEL*)m;
    MESH* mesh = model->mesh;

    switch (model->drawtype) {
        case DRAWTYPE_2D_PLAIN:
            return mesh->verts * sizeof(VEC2_DUMMY);
        case DRAWTYPE_3D_PLAIN:
            return mesh->verts * sizeof(VEC3_DUMMY);
    }
    // the *2 because two floats per vertex
    return 0;
}


#endif
