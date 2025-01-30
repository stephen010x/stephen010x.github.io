#ifndef MODEL_H
#define MODEL_H

#include <stddef.h>
#include <stdbool.h>
#include <GLES2/gl2.h>
#include <cglm/types.h>


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


typedef struct {
    int verts;
    GLenum mode;
    GLfloat data[];
} MESH;


#define MESH(__n) struct {  \
    int verts;              \
    GLenum mode;            \
    GLfloat data[__n];      \
}



__FORCE_INLINE__ size_t MESH_sizeof(void* m) {
    MESH* mesh = (MESH*)m;
    // the *2 because two floats per vertex
    return mesh->verts * sizeof(mesh->data[0]) * 2;
}



//typedef (int (*DRAW_CALLBACK)(int));
typedef void (*UPDATE_CALLBACK)(void*, float);




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
} MODEL;


// expects the user set items to be populated
MODEL* MODEL_init(MODEL* model);
int MODEL_draw(MODEL* model);


__FORCE_INLINE__ void MODEL_update(MODEL* model, float dt) {
    if (model->update_call != NULL)
        return model->update_call(model, dt);
}



#endif
