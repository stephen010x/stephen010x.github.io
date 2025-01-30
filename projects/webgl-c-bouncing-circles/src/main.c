#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <emscripten.h>
#include <emscripten/html5.h>
//#include <emscripten/fetch.h>
#include <cglm/cglm.h>
#include "main.h"
#include "model.h"
#include "webgl.h"
//#include "gobjects.h"
#include "shaders/gen/shaders.h"
//#include "macros/macros.h"


#define NUM_MODELS 2
#define CIRC_RES 32


enum {
    ERROR_NONE = 0,
    ERROR_CREATE_CONTEXT,
};


//typedef unsigned char bool;
#define TRUE 1
#define FALSE 0



void init_scene(GLuint shader_program);
EM_BOOL frame_loop(double t, void *user_data);
int __main(void);




// TODO:
// I'm realizing if two models are using the same mesh, they
// should reference the opengl context for it rather than make a new
// one. In other words, the mesh should create a buffer, not the model
MESH(6) triangle_mesh = {
    .verts = 3,
    .mode = GL_TRIANGLES,
    .data = {
        0.0,  0.5,
       -0.5, -0.5,
        0.5, -0.5,
    }
};

MESH(CIRC_RES*2) circle_mesh = {
    .verts = CIRC_RES,
    .mode = GL_TRIANGLE_FAN,
    .data = {0}
};


MODEL models[NUM_MODELS];





int main() {
    printf("starting webgl\n");

    int retval = __main();
    printf("main exited with code (%d)\n", retval);

    return retval;
}



vec3 gravity = {0, -0.1, 0};




int __main(void) {

    // set starting random seed
    srand((unsigned int)time(NULL));

    // remember, you wrote this function. Pass in canvas name, 
    // and returns shader programs
    // a bit counterintuitive, but you can fix this later
    int err = init_webgl("#canvas");
    ASSERT(err == 0, -1, "init_webgl failed\n");

    GLuint program = shader_program(
        "program", "model.vert", model_vert, "model.frag", model_frag);

    // TODO: good lord fix this
    //emscripten_set_canvas_element_size("#canvas", 256, 256);

    //get canvas width and height
    //FRAME canvas;
    //EMSCRIPTEN_RESULT result = emscripten_get_canvas_element_size("#canvas", &canvas.width, &canvas.height);

    // TODO I don't really know where to go with this
    // I eventually want control over the screen buffer size/resolution
    //glViewport(0, 0, 256, 256);

    // set the uniform u_proj_mat
    GLint u_proj_mat_loc = glGetUniformLocation(program, "u_proj_mat");

    mat4 u_proj_mat;
    // TODO: Make this cooler. (ie. 0 to 256 rather than -1 to 1)
    glm_ortho(-1, 1, -1, 1, 0, (1<<16)-1, u_proj_mat);

    glUniformMatrix4fv(u_proj_mat_loc, 1, GL_FALSE, (GLfloat*)&u_proj_mat);


    // init scene
    init_scene(program);

    // set frame callback
    // a while loop would stall the webpage, so this is neccissary
    printf("starting animation frame loop\n");
    emscripten_request_animation_frame_loop(&frame_loop, NULL);

    return 0;
}








EM_BOOL frame_loop(double t, void *user_data) {
    (void)user_data;
    static double t0;
    double dt = t - t0;
    t0 = t;

    // clear the scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < NUM_MODELS; i++) {
        MODEL_update(models+i, dt);
    }
    for (int i = 0; i < NUM_MODELS; i++) {
        MODEL_draw(models+i);
    }

    // requests frame buffer swap. Will actually render stuff to screen
    // this is neccissary because explicitSwapControl was set to GL_TRUE
    emscripten_webgl_commit_frame();

    // return true to keep looping
    // return false to kill loop
    return EM_TRUE;
}





void tri_update(MODEL* model, float dt);



#define FRAND() ((float)rand()/(float)RAND_MAX)


typedef struct {
    vec3 vel;
    float scale;
} BEHAVE_DATA;


BEHAVE_DATA behave[NUM_MODELS];


void init_scene(GLuint program) {
    // set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    // generate circle mesh
    for (int i = 0; i <= CIRC_RES; i++) {
        circle_mesh.data[i*2+0] = (float)cos( (float)i * (2*MATH_PI/CIRC_RES) );
        circle_mesh.data[i*2+1] = (float)sin( (float)i * (2*MATH_PI/CIRC_RES) );
    }


    
    // create model
    for (int i = 0; i < NUM_MODELS; i++) {
        models[i] = (MODEL){
            .color = {
                .r = FRAND(),
                .g = FRAND(),
                .b = FRAND(),
                .w = 1.0
            },
            .id = i,
            .mesh = (MESH*)&circle_mesh,
            .visable = true,
            .drawtype = 0, // not implemented yet
            .update_call = (UPDATE_CALLBACK)tri_update,
            .shader_prog = program,
            .view_mat = GLM_MAT4_IDENTITY_INIT,
        };

        vec3 init_pos = {
            FRAND()*2-1,
            FRAND()*2-1,
            0,
        };

        behave[i] = (BEHAVE_DATA){
            .vel = {
                (FRAND()*2-1)/40,
                (FRAND()*2-1)/40,
                0,
            },
            .scale = (FRAND()*0.9 + 0.1)/2,
        };

        glm_scale_uni(models[i].view_mat, behave[i].scale);
        glm_translate(models[i].view_mat, init_pos);

        MODEL_init(models+i);
    }
    

    // create model
    /*for (int i = 0; i < NUM_MODELS; i++) {
        models[i] = (MODEL){
            .color = {
                .r = FRAND(),
                .g = FRAND(),
                .b = FRAND(),
                .w = 1.0
            },
            .id = i,
            .mesh = (MESH*)&triangle_mesh,
            .visable = true,
            .drawtype = 0,
            .update_call = (UPDATE_CALLBACK)tri_update,
            .shader_prog = program,
            .view_mat = GLM_MAT4_IDENTITY_INIT,
        };

        vec3 init_pos = {
            FRAND()*2-1,
            FRAND()*2-1,
            0,
        };

        behave[i] = (BEHAVE_DATA){
            .vel = {
                (FRAND()*2-1)/40,
                (FRAND()*2-1)/40,
                0,
            },
            .scale = FRAND()*0.9 + 0.1,
        };

        glm_scale_uni(models[i].view_mat, behave[i].scale);
        glm_translate(models[i].view_mat, init_pos);

        MODEL_init(models+i);
    }*/
}






void tri_update(MODEL* model, float dt) {
    int id = model->id;
    //printf("%f, %f, %f\n", behave[id].vel[0], behave[id].vel[1], behave[id].vel[2]);

    // copy view matrix
    mat4 mat;
    glm_mat4_copy(model->view_mat, mat);

    vec3 pos = {
        model->view_mat[3][0],
        model->view_mat[3][1],
        0,
    };

    float x = pos[0];
    float y = pos[1];
    float rad = behave[id].scale;
    float dx = behave[id].vel[0];
    float dy = behave[id].vel[1];

    // TODO:
    // for the physics part, have each boundry create a collision vector
    // that represents the position of where the collision happens
    // so that the hard stuff can be easily incorperated with the circle-to-circle
    // collision
    // TODO:
    // also it is probably worth looking into doing shader based collisions.

    //printf("%f, %f\n", dx, ABS(dx));

    if (x+dx+rad >  1) dx = -ABS(dx);
    if (x+dx-rad < -1) dx =  ABS(dx);
    if (y+dy+rad >  1) dy = -ABS(dy);
    if (y+dy-rad < -1) dy =  ABS(dy);

    behave[id].vel[0] = dx;
    behave[id].vel[1] = dy;
    behave[id].vel[2] = 0;

    //printf("%f, %f\n", dx, dy);

    glm_translate(model->view_mat, behave[id].vel);
}
