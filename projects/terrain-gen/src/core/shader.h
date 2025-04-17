#ifndef CORE_SHADER_H
#define CORE_SHADER_H


#include <GLES2/gl2.h>
#include <cglm/cglm.h>

#include "webgl.h"



#define NULL_SHADER 0




// this isn't really useful yet because draws are handled one-by-one by the model class
// rather than all at once by the shader class
enum shader_drawtype {
    SHADER_DRAW_NULL = 0,
    SHADER_DRAW_DEFAULT,
    SHADER_DRAW_WIREFRAME,
    SHADER_DRAW_POINTS,
};


struct _SHADER;
typedef void (*SHADER_USE_CALL)(struct _SHADER* shader, void* data);
typedef void (*SHADER_DRAW_CALL)(struct _SHADER* shader, double t, void* data);

// TODO: move this to it's own file
typedef struct _SHADER {
    
    GLuint program;
    SHADER_USE_CALL callback;
    SHADER_DRAW_CALL drawcall;
    void* data;                 // I suppose I will just use this for both of the callbacks
    int drawtype;

    int meshtype;

    // these get populated by initilizing the shader
    // TODO: I should probably get rid of these in favor of the shader_set functions
    union {
        struct {
            GLint vert_pos_loc;
            GLint vert_norm_loc;
            GLint vert_tex_loc;
        };
        GLint attrib_loc[3];
    };

    // TODO: I should also get rid of these
    GLint u_mod_mat_loc;
    GLint u_norm_mat_loc;
    GLint u_color_loc;
    GLint u_light_norm_loc;
    GLint u_light_map_loc;
} SHADER;


extern SHADER* current_shader;



//SHADER* shader_init_old(SHADER* shader, GLuint program, SHADER_USE_CALL call, void* data);
//int _shader_init(SHADER* shader, char* vertsrc, char* fragsrc, SHADER_USE_CALL call, void* data);
int shader_init(SHADER* shader, SHADER_DESCRIPTOR* descriptor, SHADER_DRAW_CALL call, void* data, int meshtype);
void shader_use(SHADER* shader);
void shader_draw(SHADER* shader, double t);

void shader_setup_attributes(SHADER* shader);

bool shader_var_exists(SHADER* shader, char* vname);

int shader_set_float(SHADER* shader, char* vname, float val);
int shader_set_int(  SHADER* shader, char* vname, int   val);
int shader_set_mat4( SHADER* shader, char* vname, mat4  val);
int shader_set_vec2( SHADER* shader, char* vname, vec2  val);
int shader_set_vec3( SHADER* shader, char* vname, vec3  val);
int shader_set_vec4( SHADER* shader, char* vname, vec4  val);
int shader_set_bool( SHADER* shader, char* vname, bool  val);



#endif
