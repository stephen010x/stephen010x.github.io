#ifndef SHAPES_H
#define SHAPES_H

#include <GLES2/gl2.h>
#include <cglm/cglm.h>

#include "../core/model.h"
#include "../core/texture.h"



#define CIRCLE_RES 32
#define SPHERE_RES 32




#define NULL_SHADER 0
#define NULL_BUFFER 0




#define SPHERE_V_RES ((int)SPHERE_RES/2)
#define SPHERE_D_RES SPHERE_RES

#define STRIP_VERTS ((SPHERE_V_RES+1)*2)
#define SPHERE_VERTS STRIP_VERTS*SPHERE_D_RES



// these are neccissary for some reason because the compiler can't
// friggin' recognize that two unnamed structs are the same!
typedef MESH2(3)          TRIANGLE_MESH_TYPE;
typedef MESH2(CIRCLE_RES)   CIRCLE_MESH_TYPE;
typedef MESH3(SPHERE_VERTS) SPHERE_MESH_TYPE;

typedef const MESH2(4)    SQUARE_MESH_TYPE;
typedef const MESH3(4*6*2)  CUBE_MESH_TYPE;

typedef const MESH3(4*3) TEX_SQUARE_MESH_TYPE;

// TODO:
// I'm realizing if two models are using the same mesh, they
// should reference the opengl context for it rather than make a new
// one. In other words, the mesh should create a buffer, not the model
extern TRIANGLE_MESH_TYPE triangle_mesh;
extern   SQUARE_MESH_TYPE   square_mesh;
extern   CIRCLE_MESH_TYPE   circle_mesh;
extern   SPHERE_MESH_TYPE   sphere_mesh;
extern     CUBE_MESH_TYPE     cube_mesh;

extern TEX_SQUARE_MESH_TYPE tex_square_mesh;



struct _SHADER;
typedef void (*SHADER_USE_CALL)(struct _SHADER* shader, void* data);

// TODO: move this to it's own file
typedef struct _SHADER {
    GLuint program;
    SHADER_USE_CALL callback;
    void* data;

    // these get populated by initilizing the shader
    union {
        struct {
            GLint vert_pos_loc;
            GLint vert_norm_loc;
            GLint vert_tex_loc;
        };
        GLint attrib_loc[3];
    };
    
    GLint u_mod_mat_loc;
    GLint u_norm_mat_loc;
    GLint u_color_loc;
    GLint u_light_norm_loc;
    GLint u_light_map_loc;
} SHADER;



SHADER* shader_init(SHADER* shader, GLuint program, SHADER_USE_CALL call, void* data);
SHADER* shader_use(SHADER* shader);

int shader_set_float(SHADER* shader, char* vname, float val);
int shader_set_int(  SHADER* shader, char* vname, int   val);
int shader_set_mat4( SHADER* shader, char* vname, mat4  val);
int shader_set_vec2( SHADER* shader, char* vname, vec2  val);
int shader_set_vec3( SHADER* shader, char* vname, vec3  val);
int shader_set_vec4( SHADER* shader, char* vname, vec4  val);



void shapes_init(void);

int draw_triangle(vec2 points[3], vec2 pos, SHADER* shader);
int draw_polygon(vec2* points, float rot, vec2 pos, SHADER* shader);
// TODO: rename to draw_rect2
int draw_rectangle( float width, float height, float rot, vec2 pos, COLOR color, SHADER* shader, int layer);
int draw_oval(      float width, float height, float rot, vec2 pos, SHADER* shader);
int draw_sphere(vec3 size, vec3 pos, SHADER* shader);

int draw_rect2(vec3 pos, vec2 scale, vec3 norm, float rot, COLOR color, SHADER* shader);
int draw_rect3(vec3 pos, vec3 scale, vec3 dir, float roll, COLOR color, SHADER* shader);

int draw_texture_plane(vec3 pos, vec2 scale, vec3 norm, float rot, TEXTURE* texture, SHADER* shader, bool flipped);




#endif
