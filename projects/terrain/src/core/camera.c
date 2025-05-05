#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>
#include "camera.h"
#include "../main.h"
#include "../helper.h"




void camera_init(CAMERA* c) {
    // this was a bad idea, as it overrides all previous values of the camera
    /* *c = (CAMERA){
        .viewmat = GLM_MAT4_IDENTITY_INIT,
    };*/
    glm_mat4_copy((mat4)GLM_MAT4_IDENTITY_INIT, c->viewmat);

    get_elementid_size("canvas", &c->swidth, &c->sheight);
    c->ratio = (float)c->swidth/c->sheight;

    // TODO: pull these values from canvas + style width and height values
    // rather than assert modes in the c code. Instead assert modes solely
    // in the js code.

    // TODO good god, why did I put these zoom scales here?
    // get rid of these and just make the z direction the zoom of orthographic
    #if CAM_SCALE_MODE == CAM_SCALE_DYNAMIC
    if (c->ratio > 1) {
        c->wmin[0] = -1 * c->ratio;
        c->wmax[0] =  1 * c->ratio;
        c->wmin[1] = -1;
        c->wmax[1] =  1;
    } else {
        c->wmin[0] = -1;
        c->wmax[0] =  1;
        c->wmin[1] = -1/c->ratio;
        c->wmax[1] =  1/c->ratio;
    }

    #elif CAM_SCALE_MODE == CAM_SCALE_STATICWIDTH
    c->wmin[0] = -1;
    c->wmax[0] =  1;
    c->wmin[1] = -1/c->ratio;
    c->wmax[1] =  1/c->ratio;
    
    #elif CAM_SCALE_MODE == CAM_SCALE_STATICHEIGHT
    c->wmin[0] = -1*c->ratio;
    c->wmax[0] =  1*c->ratio;
    c->wmin[1] = -1;
    c->wmax[1] =  1;
    
    #endif
    
    //c->wmin[2] = 0.001;
    //c->wmin[2] = 0.0001;
    //c->wmax[2] = 1000.0;
    c->wmin[2] = 0.1;
    c->wmax[2] = 1000.0;

    // some default initilization. Makes things easy for static cameras
    // that rely on the pos and rot class parameters
    camera_update(c);
    
    //return c;
}



void camera_setviewport(CAMERA* c, vec3 min, vec3 max) {
    glm_vec3_copy(min, c->wmin);
    glm_vec3_copy(max, c->wmax);
}



// call this when you want to apply the parameters in the camera class
// to the camera viewport matrix
// TODO this function doesn't work right. But things are using it, so fix this later
void camera_update(CAMERA* c) {
    camera_setup(c);

    // translate first so that it rotates about the camera origin
    glm_translate(c->viewmat, c->pos);

    // rotate second, x, y, z axes in order
    glm_rotate_x(c->viewmat, c->rot[0], c->viewmat);
    glm_rotate_y(c->viewmat, c->rot[1], c->viewmat);
    glm_rotate_z(c->viewmat, c->rot[2], c->viewmat);
}


// TODO: get rid of this duck-tape function
void camera_update_actual(CAMERA* c) {
    camera_setup(c);

    // rotate first, x, y, z axes in order
    glm_rotate_x(c->viewmat, c->rot[0], c->viewmat);
    glm_rotate_y(c->viewmat, c->rot[2], c->viewmat);
    glm_rotate_z(c->viewmat, c->rot[1], c->viewmat);

    // translate second so that it rotates about the camera origin
    vec3 pos;
    glm_vec3_negate_to(c->pos, pos);
    glm_translate(c->viewmat, pos);
}

// TODO: get rid of this duck-tape function
void camera_update_actual_flipx(CAMERA* c) {
    camera_setup(c);


    // rotate first, x, y, z axes in order
    glm_rotate_x(c->viewmat, c->rot[0], c->viewmat);
    glm_rotate_y(c->viewmat, c->rot[2], c->viewmat);
    glm_rotate_z(c->viewmat, c->rot[1], c->viewmat);

    glm_scale(c->viewmat, (vec3){-1,1,1});
    
    // translate second so that it rotates about the camera origin
    vec3 pos;
    glm_vec3_negate_to(c->pos, pos);
    glm_translate(c->viewmat, pos);
}



void camera_update_skybox(CAMERA* c) {
    camera_setup(c);

    // rotate first, x, y, z axes in order
    glm_rotate_x(c->viewmat, c->rot[0], c->viewmat);
    glm_rotate_y(c->viewmat, c->rot[2], c->viewmat);
    glm_rotate_z(c->viewmat, c->rot[1], c->viewmat);

    // translate second so that it rotates about the camera origin
    /*vec3 pos;
    glm_vec3_negate_to(c->pos, pos);
    pos[2] = 0;
    glm_translate(c->viewmat, pos);*/
}



// TODO: I change my mind. Create a function that will update a camera based off of 
// position, rotation, etc attributes to be optionally used.
void camera_setup(CAMERA* c) {
    switch (c->type) {
        case CAMERA_ORTHOGRAPHIC:
            // TODO: figure out how to get the other ortho 
            //       function working that uses vectors
            glm_ortho(
                c->wmin[0]*c->zoom, c->wmax[0]*c->zoom,
                c->wmin[1]*c->zoom, c->wmax[1]*c->zoom,
                c->wmin[2]*c->zoom, c->wmax[2]*c->zoom,
                c->viewmat
            );
            break;
        case CAMERA_PERSPECTIVE:
            glm_perspective(c->fov, c->ratio, c->wmin[2], c->wmax[2], c->viewmat);
            break;
        case CAMERA_NONE:
            break;
        default:
            // TODO: create warning or error macro for this
            ASSERTVOID(false, "ERROR: unrecognized camera type\n");
    }
    //return c;
}



int camera_apply(CAMERA* c, GLuint shader_program) {
    ASSERT(shader_program > 0, -1, "ERROR: invalid shader program\n");
    GLint u_proj_mat_loc;
    glUseProgram(shader_program);
    
    u_proj_mat_loc = glGetUniformLocation(shader_program, "u_proj_mat");
    // TODO consider just making this an assert, as if you don't want to use 
    // a camera for a shader, then just don't call fhis function. /shrug
    if (u_proj_mat_loc > 0)
        glUniformMatrix4fv(u_proj_mat_loc, 1, GL_FALSE, (GLfloat*)c->raw);

    //return c;
    return 0;
}




void camera_rotate_fps(CAMERA* c, vec3 rot) {
    // setup camera projection
    camera_setup(c);

    // the reason why translate first and rotate second is because 
    // this is the camera moving the enviroment, not the other way around.

    // translate first so that it rotates about the camera origin
    glm_translate(c->viewmat, c->pos);

    // rotate second, x, y, z axes in order
    glm_rotate_x(c->viewmat, c->rot[0], c->viewmat);
    glm_rotate_y(c->viewmat, c->rot[1], c->viewmat);
    glm_rotate_z(c->viewmat, c->rot[2], c->viewmat);

    // apply mod matrix, then reset mod matrix
    //glm_mat4_mul(c->viewmat, c->mod_viewmat, c->viewmat);
    //glm_mat4_identity(c->mod_viewmat);
    
    //return c;
}


void camera_rotate_about(CAMERA* c, vec3 origin, vec3 rot, float radius) {
    // setup camera projection
    camera_setup(c);

    // clear position and rotation to let mod matrix handle it.
    glm_vec3_zero(c->pos);
    glm_vec3_zero(c->rot);

    // reset mod view matrix
    //glm_mat4_identity(c->mod_viewmat);

    // translate to origin
    /*glm_translate(c->mod_viewmat, origin);
    
    // rotate first, x, y, z axes in order
    glm_rotate_x(c->mod_viewmat, rot[0], c->mod_viewmat);
    glm_rotate_y(c->mod_viewmat, rot[1], c->mod_viewmat);
    glm_rotate_z(c->mod_viewmat, rot[2], c->mod_viewmat);

    // translate radius last so that it rotates about the global origin
    glm_translate_z(c->mod_viewmat, -radius);*/

    /*glm_translate_z(c->mod_viewmat, -radius);
    
    glm_rotate_x(c->mod_viewmat, rot[0], c->mod_viewmat);
    glm_rotate_z(c->mod_viewmat, rot[2], c->mod_viewmat);
    glm_rotate_y(c->mod_viewmat, rot[1], c->mod_viewmat);

    glm_translate(c->mod_viewmat, origin);*/

    glm_translate_z(c->viewmat, -radius);
    
    glm_rotate_x(c->viewmat, rot[0], c->viewmat);
    glm_rotate_z(c->viewmat, rot[2], c->viewmat);
    glm_rotate_y(c->viewmat, rot[1], c->viewmat);

    glm_translate(c->viewmat, origin);

    //return c;
}



void camera_get_direction(CAMERA* c, vec3 _forward, vec3 cam_dir) {
    vec4 temp;
    vec4 forward = (vec4){_forward[0], _forward[1], _forward[2]};
    
    glm_mat4_mulv(c->viewmat, forward, temp);
    glm_vec3_negate(temp);
    glm_vec3_normalize_to(temp, cam_dir);
}





// TODO: This is honestly rather inefficient. In a future version, 
// create a pseudo camera or something that you just attach a camera to.
void camera_set(CAMERA* c, CAMERA* s) {
    glm_mat4_copy(s->viewmat, c->viewmat);
    //return c;
}




void camera_interpolate(CAMERA* c, CAMERA* a, CAMERA* b, float n) {
    //if (n < 0 || n > 1) return NULL;

    // TODO: create a clamp macro
    if (n < 0) n = 0;
    else if (n > 1) n = 1;
    
    for (int i = 0; i < 16; i++) {
        register float k0 = ((float*)a->viewmat)[i];
        register float k1 = ((float*)b->viewmat)[i];
        ((float*)c->viewmat)[i] = (k1 - k0) * n + k0;
    }
    //return c;
}





int lightsource_apply(LIGHTSOURCE* light, unsigned int shader_program) {
    // TODO: make this a DEBUG_ASSERT, that only asserts in debug mode
    ASSERT(shader_program > 0, -1, "ERROR: invalid shader program\n");
    glUseProgram(shader_program);

    GLint u_light_amb_loc    = glGetUniformLocation(shader_program, "u_light_amb");
    GLint u_light_bright_loc = glGetUniformLocation(shader_program, "u_light_bright");
    GLint u_light_diff_color_loc = glGetUniformLocation(shader_program, "u_light_diff_color");
    GLint u_light_amb_color_loc  = glGetUniformLocation(shader_program, "u_light_amb_color");

    if (u_light_amb_loc > 0)
        glUniform1fv(u_light_amb_loc, 1, (GLfloat*)&light->dir.amb);

    if (u_light_bright_loc > 0)
        glUniform1fv(u_light_bright_loc, 1, (GLfloat*)&light->dir.bright);

    if (u_light_diff_color_loc > 0)
        glUniform4fv(u_light_diff_color_loc, 1, light->dir.diff_color.raw);

    if (u_light_amb_color_loc > 0)
        glUniform4fv(u_light_amb_color_loc, 1, light->dir.amb_color.raw);


    switch (light->type) {
        case LIGHTSOURCE_TYPE_DIR: {
            GLint u_light_norm_loc = 
                glGetUniformLocation(shader_program, "u_light_norm");

            if (u_light_norm_loc > 0)
                glUniform3fv(u_light_norm_loc, 1, (GLfloat*)light->dir.norm);

        } break;

        case LIGHTSOURCE_TYPE_POINT: {
            GLint u_light_pos_loc = 
                glGetUniformLocation(shader_program, "u_light_pos");

            if (u_light_pos_loc > 0)
                glUniform3fv(u_light_pos_loc, 1, (GLfloat*)light->point.pos);

        } break;
    }

    return 0;
}





// TODO: this only works if the camera is using it's pos vector to 
// set it's view matrix
int lightsource_apply_spectral(LIGHTSOURCE* light, unsigned int shader_program, CAMERA* camera, float spec_bright, float spec_pow) {
    lightsource_apply(light, shader_program);
    
    GLint u_cam_pos_loc      = glGetUniformLocation(shader_program, "u_cam_pos");
    GLint u_light_spec_bright_loc = glGetUniformLocation(shader_program, "u_light_spec_bright");
    GLint u_light_spec_pow_loc    = glGetUniformLocation(shader_program, "u_light_spec_pow");

    if (u_cam_pos_loc > 0 && camera != NULL)
        glUniform3fv(u_cam_pos_loc, 1, (GLfloat*)camera->pos);

    if (u_light_spec_bright_loc > 0)
        glUniform1fv(u_light_spec_bright_loc, 1, (GLfloat*)&spec_bright);

    if (u_light_spec_pow_loc > 0)
        glUniform1fv(u_light_spec_pow_loc, 1, (GLfloat*)&spec_pow);


    return 0;
}


int lightsource_apply_spectral2(LIGHTSOURCE* light, unsigned int shader_program, CAMERA* camera, float spec_bright, float spec_pow) {
    lightsource_apply(light, shader_program);
    
    GLint u_cam_pos_loc      = glGetUniformLocation(shader_program, "u_cam_pos");
    GLint u_light_spec_bright_loc = glGetUniformLocation(shader_program, "u_light_spec_bright2");
    GLint u_light_spec_pow_loc    = glGetUniformLocation(shader_program, "u_light_spec_pow2");

    if (u_cam_pos_loc > 0 && camera != NULL)
        glUniform3fv(u_cam_pos_loc, 1, (GLfloat*)camera->pos);

    if (u_light_spec_bright_loc > 0)
        glUniform1fv(u_light_spec_bright_loc, 1, (GLfloat*)&spec_bright);

    if (u_light_spec_pow_loc > 0)
        glUniform1fv(u_light_spec_pow_loc, 1, (GLfloat*)&spec_pow);


    return 0;
}
