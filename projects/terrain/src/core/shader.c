#include <stdbool.h>

#include "shader.h"
#include "webgl.h"
#include "../main.h"
#include "model.h"


//static void setup_attributes(SHADER* shader);


SHADER* current_shader;



//SHADER* shader_init(SHADER* shader, GLuint program, SHADER_USE_CALL call, void* data) {
//int _shader_init(SHADER* shader, char* vertsrc, char* fragsrc, SHADER_USE_CALL call, void* data) {
int shader_init(SHADER* shader, SHADER_DESCRIPTOR* descriptor, SHADER_DRAW_CALL call, void* data, int meshtype) {

    GLuint program;

    if (!descriptor->active)
        return -1;

    if (descriptor->program == NULL)
        descriptor->program = &program;

    compile_shaders(1, descriptor);

    program = *descriptor->program;
    

    *shader = (SHADER){
        .program = program,
        .drawcall = call,
        .data = data,
        .drawtype = SHADER_DRAW_DEFAULT,
        .meshtype = meshtype,
    };

    // select current shader to get value locations
    glUseProgram(program);

    shader->vert_pos_loc  = glGetAttribLocation(program, "vert_pos");
    shader->vert_norm_loc = glGetAttribLocation(program, "vert_norm");
    shader->vert_tex_loc  = glGetAttribLocation(program, "vert_tex");
    ASSERT(shader->vert_pos_loc >= 0, -1, "no location for vert_pos_loc\n");

    // note, these may return -1, meaning they are not in the program
    shader->u_mod_mat_loc    = glGetUniformLocation(program, "u_mod_mat");
    shader->u_norm_mat_loc   = glGetUniformLocation(program, "u_norm_mat");
    shader->u_color_loc      = glGetUniformLocation(program, "u_color");
    shader->u_light_norm_loc = glGetUniformLocation(program, "u_light_norm");
    shader->u_light_map_loc  = glGetUniformLocation(program, "u_light_map");

    //#ifdef DEBUG_MODE
    #if 0
    {
        mat4 viewmat = GLM_MAT4_IDENTITY_INIT;
        
        // set uniform model view matrix
        if (shader->u_mod_mat_loc > 0)
            glUniformMatrix4fv(shader->u_mod_mat_loc, 
                1, GL_FALSE, (GLfloat*)viewmat);

        // what the heck is this for? I already implemented this below,
        // and yet this is broken code anyway
        /*if (shader->u_mod_mat_loc > 0)
            glUniformMatrix4fv(shader->u_norm_mat_loc, 
                1, GL_FALSE, (GLfloat*)viewmat);*/

        // debug color
        if (shader->u_color_loc > 0) 
            glUniform4fv(shader->u_color_loc, 1, (GLfloat[]){0.0, 1.0, 0.0, 1.0});

        // set uniform normal matrix
        if (shader->u_norm_mat_loc > 0) {
            //mat3 norm_mat = mat3(transpose(inverse(u_mod_mat)));
            mat3 norm_mat;
            glm_mat4_pick3(viewmat, norm_mat);
            glm_mat3_inv(norm_mat, norm_mat);
            glm_mat3_transpose(norm_mat);
            
            glUniformMatrix3fv(shader->u_norm_mat_loc, 
                1, GL_FALSE, (GLfloat*)norm_mat);
        }
    }
    #endif

    // deselect shader to undefined shader
    glUseProgram(NULL_SHADER);

    return 0;
}

void shader_use(SHADER* shader) {
    glUseProgram(shader->program);

    current_shader = shader;

    //setup_attributes(shader);

    if (shader->callback)
        shader->callback(shader, shader->data);
}


void shader_draw(SHADER* shader, double t) {
    shader_use(shader);

    if (shader->drawcall)
        shader->drawcall(shader, t, shader->data);

    // deselect shader to undefined shader
    glUseProgram(NULL_SHADER);
    current_shader = NULL;
}


// TODO: create an error message system that collects different types of the same error
// and then have a handler that prints out the error and how many instances of it every interval
// to reduce error bloat
// that way I can let the programmer know that the uniforms arent being set if they wish
int shader_set_float(SHADER* shader, char* vname, float val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform1f(u_loc, (GLfloat)val);
        return 0;
    }
    return -1;
}

int shader_set_int(SHADER* shader, char* vname, int val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform1i(u_loc, (GLint)val);
        return 0;
    }
    return -1;
}

int shader_set_mat4(SHADER* shader, char* vname, mat4 val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniformMatrix4fv(u_loc, 1, GL_FALSE, (GLfloat*)val);
        return 0;
    }
    return -1;
}

int shader_set_vec2(SHADER* shader, char* vname, vec2 val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform2fv(u_loc, 1, (GLfloat*)val);
        return 0;
    }
    return -1;
}

int shader_set_vec3(SHADER* shader, char* vname, vec3 val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform3fv(u_loc, 1, (GLfloat*)val);
        return 0;
    }
    return -1;
}

int shader_set_vec4(SHADER* shader, char* vname, vec4 val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform4fv(u_loc, 1, (GLfloat*)val);
        return 0;
    }
    return -1;
}

int shader_set_bool(SHADER* shader, char* vname, bool val) {
    return shader_set_int(shader, vname, (int)val);
}







//static void setup_attributes(SHADER* shader) {
void shader_setup_attributes(SHADER* shader) {

    // TODO I just realized that while the shader has control over
    // the types, it technically doesn't have control over the format that is
    // being uploaded to the types.
    // oh well. It is still probably more efficient this way to keep this in the shader
    // as it would require people to just replicate this shader but using a different format
    // which would make it so that it is still only set per-shader rather than per-model.

    int type = shader->meshtype;

    int attribs;

    switch (type) {
        case MESHTYPE_3D_VERT_NORM:
            attribs = 2;
            break;
        case MESHTYPE_3D_VERT_NORM_TEX2:
            attribs = 3;
            break;
        default:
            attribs = 1;
    }


    GLint attrib_size[4];
    GLenum attrib_type[4];
    intptr_t attrib_off[4];
    GLint attrib_loc[4];
    
    GLsizei attrib_stride = 0*sizeof(GLfloat);

    for (int i = 0; i < attribs; i++) {
        attrib_type[i] = GL_FLOAT;
        attrib_off[i] = i * (3*sizeof(GLfloat));
        attrib_loc[i] = shader->attrib_loc[i];
    }
    

    switch (type) {
        case MESHTYPE_1D_PACKED:
            attrib_size[0] = 1;
            break;
        case MESHTYPE_2D_PACKED:
            attrib_size[0] = 2;
            break;
        case MESHTYPE_3D_PACKED:
            attrib_size[0] = 3;
            break;
        case MESHTYPE_3D_VERT_NORM:
            attrib_size[0] = 3;
            attrib_size[1] = 3;
            attrib_stride = 6 * sizeof(GLfloat);
            break;
        case MESHTYPE_3D_VERT_NORM_TEX2:
            attrib_size[0] = 3;
            attrib_size[1] = 3;
            attrib_size[2] = 2;
            //attrib_off[3] = attrib_off[2] + 2*sizeof(GLfloat);
            // TODO: add a function to mesh that will get vertex size
            attrib_stride = (3 + 3 + 2) * sizeof(GLfloat);
            break;
    }


    for (int i = 0; i < attribs; i++) {
        if (attrib_loc[i] < 0)
            continue;
            
        glVertexAttribPointer(
            attrib_loc[i],
            attrib_size[i],
            attrib_type[i],
            GL_FALSE,
            attrib_stride,
            (void*)attrib_off[i]
        );
        glEnableVertexAttribArray(attrib_loc[i]);
    }
}





bool shader_var_exists(SHADER* shader, char* vname) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0)
        return true;
    return false;
}
