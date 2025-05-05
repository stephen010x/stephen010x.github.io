#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
// TODO: in an ideal world, this wouldn't be needed in main
#include <GLES2/gl2.h>
// TODO" in an ideal world, these also wouldn't be needed in main
#include <emscripten.h>
#include <emscripten/html5.h>

// TODO: get rid of main header
#include "main.h"
#include "core.h"
#include "objects.h"
#include "shaders/shaders.h"
#include "helper.h"



//*DEBUG*/ extern CUBE_MESH_TYPE cube_mesh;


#define COLOR_RED    ((COLOR){0.8, 0.2, 0.2, 1.0})
#define COLOR_ORANGE ((COLOR){0.8, 0.3, 0.2, 1.0})
#define COLOR_BLUE   ((COLOR){0.3, 0.4, 0.9, 1.0})
#define COLOR_PURPLE ((COLOR){0.7, 0.2, 0.6, 1.0})
#define COLOR_WHITE  ((COLOR){1.0, 1.0, 1.0, 1.0})
#define COLOR_BLACK  ((COLOR){0.0, 0.0, 0.0, 1.0})
#define COLOR_ORANGE_ALPHA ((COLOR){0.8, 0.3, 0.2, 0.5})
//#define COLOR_BLUE_ALPHA   ((COLOR){0.3, 0.4, 0.9, 15/16.0})
#define COLOR_BLUE_ALPHA   ((COLOR){0.3, 0.4, 0.9, 8.0/16.0})
//#define COLOR_BLUE_ALPHA COLOR_ORANGE_ALPHA

//#define COLOR_DIRT_GREY  ((COLOR){64.0/256.0, 59.0/256.0, 53.0/256.0, 1.0})
//#define COLOR_DIRT_GREY  ((COLOR){64.0/256.0*2, 59.0/256.0*2, 53.0/256.0*2, 1.0})
#define COLOR_DIRT_GREY  ((COLOR){68.0/256.0*2, 55.0/256.0*2, 40.0/256.0*2, 1.0})



//#define MAX_CURVES 5
//BEZIER curves[MAX_CURVES];

//FRACTAL fractal;

#define DT_DIVISOR 100


//WINDOW window;
MOUSE mouse;
// player mouse
PMOUSE pmouse;




//#define SQRT_3 1.73205080757

WORLD world = {
    .light = {
        .type = LIGHTSOURCE_TYPE_DIR,
        .dir = {
            .norm = {0.8, 1, -0.8},
            .range = {0.05, 1.5},
        },
    },
};


CAMERA camera = {
    .zoom = 1,
    .type = CAMERA_ORTHOGRAPHIC,
};

CAMERA camera_static = {
    .zoom = 1,
    .pos = {0,0,-1},
    .type = CAMERA_ORTHOGRAPHIC,
};

CAMERA camera_1 = {
    .pos = {0,0,-1},
    .rot = {0,0,0},
    .zoom = 1.7,
    .type = CAMERA_ORTHOGRAPHIC,
};

CAMERA camera_2 = {
    .pos = {0,0,0},
    .rot = {0,0,0},
    .fov = DEG_TO_RAD(46.62), //45,
    .type = CAMERA_PERSPECTIVE,
};

// Man. I really wish I had made a "scene" class. It would make stuff like
// mirrors so much easier and cleaner to make
CAMERA camera_mirror = {
    .pos = {0,0,0},
    .rot = {0,0,0},
    .fov = DEG_TO_RAD(90), //45,
    .type = CAMERA_PERSPECTIVE,
};

/*CAMERA camera_3 = {
    .pos = {0,0,0},
    .rot = {0,0,0},
    .fov = DEG_TO_RAD(46.62), //45,
    .type = CAMERA_PERSPECTIVE,
};*/



void init_scene(void);
EM_BOOL frame_loop(double t, void *user_data);
int __main(void);



//MODEL models[NUM_MODELS];
//void controls(double t, float dt);
void update_camera(void);




EMSCRIPTEN_KEEPALIVE int main() {
    printf("starting webgl\n");

    int retval = __main();
    printf("main exited with code (%d)\n", retval);

    return retval;
}



//int swidth, sheight;
//vec3 wmin, wmax;




GLuint poly_program;
GLuint sphere_program;
GLuint bezier_program;
GLuint point_program;
GLuint fractal_program;
GLuint poly3d_program;
GLuint texplane_program;
GLuint texplane_dither_program;
GLuint maze_program;
GLuint mirror_program;

SHADER poly_shader;
SHADER poly3d_shader;
SHADER maze_shader;
SHADER point_shader;
SHADER texplane_shader;
SHADER texplane_dither_shader;
SHADER mirror_shader;

ASSET nyan_asset;
ASSET rock_wall_tex_asset;
ASSET rock_wall_tex_asset_512;
ASSET rock_wall_norm_asset;
ASSET rock_wall_norm_asset_512;
ASSET test_norm_asset;
ASSET torch_asset;
ASSET cat_avatar_asset;
TEXTURE nyan_texture;
TEXTURE rock_wall_tex_512;
TEXTURE rock_wall_tex;
TEXTURE rock_wall_norm;
TEXTURE rock_wall_norm_512;
TEXTURE test_norm;
TEXTURE torch_tex;
TEXTURE cat_avatar_tex;

MAZE maze;


DRAWSURFACE mirror;



// assume mirror always points (0, -1, 0)
struct {
    vec3 pos;
    vec2 scale;
} mirror_prop;


void mirror_frame(DRAWSURFACE* surface, void* data, double t, float dt);


//int swidth, sheight;


void _frame_loop(void);


/*EM_JS(float, calcFPS, (int count){
    var opts = {};
    
    var requestFrame = window.requestAnimationFrame ||
        window.webkitRequestAnimationFrame ||
        window.mozRequestAnimationFrame;
    if (!requestFrame) return true; // Check if "true" is returned; 
                                    // pick default FPS, show error, etc...
    function checker(){
        if (index--) requestFrame(checker);
        else {
            // var result = 3*Math.round(count*1000/3/(performance.now()-start));
            var result = count*1000/(performance.now()- start);
            if (typeof opts.callback === "function") opts.callback(result);
            //console.log("Calculated: "+result+" frames per second");
            return result
        }
    }
    //if (!opts) opts = {};
    //var count = opts.count||60, index = count, start = performance.now();
    var index = count, start = performance.now();
    checker();
}*/

//static float refresh_rate;


int __main(void) {
    // set starting random seed
    srand((unsigned int)time(NULL));

    // remember, you wrote this function.
    int err = init_webgl("#" HTML_CANVAS_ID);
    ASSERT(err == 0, -1, "init_webgl failed\n");

    //emscripten_get_canvas_element_size(HTML_CANVAS_ID, &swidth, &sheight);


    // TODO: allow for different programs to share shader programs
    // so that it doesn't have to recompile every single one.
    // perhaps generate a function to compile them all, and then user
    // their shader id's in manually calling them to compile
    SHADER_DESCRIPTOR sdesc[] = {
        SHADER_DESC_GEN( false, &sphere_program,   sphere,  simple  ),
        SHADER_DESC_GEN( false, &bezier_program,   bezier,  simple  ),
        SHADER_DESC_GEN( false, &fractal_program,  poly,    fractal ),
        SHADER_DESC_GEN( false, &point_program,    point,   simple  ),
        SHADER_DESC_GEN( true,  &poly_program,     poly,    poly    ),
        SHADER_DESC_GEN( true,  &poly3d_program,   poly3d,  simple  ),
        SHADER_DESC_GEN( true,  &texplane_program, texture, texture ),
        SHADER_DESC_GEN( true,  &maze_program,     maze,    texture_lighting ),
        SHADER_DESC_GEN( true,  &texplane_dither_program, texture, texture_dither ),
        SHADER_DESC_GEN( true,  &mirror_program,   texture, mirror),
    };

    compile_shaders(LENOF(sdesc), sdesc);

    shader_init(&poly_shader,     poly_program,     NULL, NULL);
    shader_init(&poly3d_shader,   poly3d_program,   NULL, NULL);
    //shader_init(&point_shader,    point_program,    NULL, NULL);
    shader_init(&texplane_shader, texplane_program, NULL, NULL);
    shader_init(&maze_shader,     maze_program,     NULL, NULL);
    shader_init(&texplane_dither_shader, texplane_dither_program, NULL, NULL);
    shader_init(&mirror_shader, mirror_program, NULL, NULL);

    // TODO: enable clockwise vertex order here or whatever
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_DITHER);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_MULTISAMPLE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);

    //asset_load_jpg(&nyan_asset,           "/assets/nyan-cat.jpg");
    asset_load_img(&nyan_asset,           "crying-cat");
    asset_load_img(&rock_wall_tex_asset,  "rock-wall-white");
    asset_load_img(&rock_wall_norm_asset, "rock-wall-norm");
    asset_load_img(&rock_wall_tex_asset_512,  "rock-wall-white-512");
    asset_load_img(&rock_wall_norm_asset_512, "rock-wall-norm-512");
    asset_load_img(&test_norm_asset,      "test-norm");
    asset_load_img(&torch_asset,          "torch");
    asset_load_img(&cat_avatar_asset,     "cat-avatar");
    texture_init(&nyan_texture,   &nyan_asset,           TEX_NEAREST, TEX_DEFAULT);
    texture_init(&rock_wall_tex,  &rock_wall_tex_asset,  TEX_NEAREST, TEX_WRAP);
    texture_init(&rock_wall_tex_512,  &rock_wall_tex_asset_512,  TEX_NEAREST, TEX_WRAP);
    texture_init(&rock_wall_norm_512, &rock_wall_norm_asset_512, TEX_NEAREST, TEX_WRAP);
    texture_init(&rock_wall_norm, &rock_wall_norm_asset, TEX_NEAREST, TEX_WRAP);
    texture_init(&test_norm,      &test_norm_asset,      TEX_NEAREST, TEX_WRAP);
    texture_init(&torch_tex,      &torch_asset,          TEX_NEAREST, TEX_DEFAULT);
    texture_init(&cat_avatar_tex, &cat_avatar_asset,     TEX_NEAREST, TEX_DEFAULT);

    //glDisable(GL_CULL_FACE);
    //glDisable(GL_DEPTH_TEST);

    camera_init(&camera); // this one probably not neccissary
    camera_init(&camera_1);
    camera_init(&camera_2);
    camera_init(&camera_static);
    camera_init(&camera_mirror);
    //camera_init(&camera_3);
    input_init();
    shapes_init();
        
    init_scene();

    // set frame callback
    // a while loop would stall the webpage, so this is neccissary
    printf("starting animation frame loop\n");
    //emscripten_set_main_loop(&_frame_loop, 60, false);
    emscripten_set_main_loop(&_frame_loop, 0, false);
    //emscripten_set_main_loop(&_frame_loop, 1000, false);
    //emscripten_request_animation_frame_loop(&frame_loop, NULL);

    return 0;
}


void _frame_loop(void) {
    double _t = EM_ASM_DOUBLE({
        return performance.now();
    });
    frame_loop(_t, NULL);
}







//BEHAVE behave[NUM_MODELS];

void model_transform(MODEL* model);
void model_update_pipeline(double t, float dt);




#define UPDATE_DIVISOR 1

void update_frame_time(double _t, double* t, float* dt, float* fps) {
    static double t0;
    double _dt = _t - t0;
    t0 = _t;
    *t = _t/DT_DIVISOR;
    *dt = _dt/DT_DIVISOR;

    static float _fps = 0;
    _fps = (_fps*7 + 1000/(float)_dt)/8;
    *fps = _fps;
}






void init_scene(void) {
    // set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    /*FRACTAL_init(&fractal, 
        (vec2[]){
            {wmin[0], wmin[1]},
            {wmax[0], wmax[1]},
        }, (vec3){0,0,0});*/

    maze_init(&maze, 20, 40, COLOR_WHITE, &poly_shader);
    maze.x = -CELL_SIZE*10;
    maze.y = -CELL_SIZE*20;
    maze.shader_3d = &poly3d_shader;
    maze.shader_trail = &texplane_shader;
    maze.shader_detailed = &maze_shader;
    maze.texture_wall = &rock_wall_tex;
    maze.texture_wall2 = &rock_wall_tex_512;
    //maze.texture_wall = &torch_tex;
    maze.texture_wall_norm = &rock_wall_norm;
    maze.texture_wall_norm2 = &rock_wall_norm_512;
    //maze.texture_wall_norm = &test_norm;
    maze.color2 = COLOR_DIRT_GREY;
    maze.mode = MAZE_MODE_3D;

    float x, y;
    maze_getpos(&maze, 10, 1, &x, &y);
    
    //mouse_init(&pmouse, x, y, 0.03, COLOR_PURPLE, &maze, &poly_shader);
    mouse_init(&pmouse, x, y, 0.03, COLOR_BLACK, &maze, &poly_shader);
    //mouse_init(&pmouse, x, y, CELL_SIZE-WALL_THICK, COLOR_BLACK, &maze, &poly_shader);
    pmouse.pcolor = COLOR_BLUE_ALPHA;
    pmouse.shader3 = &poly3d_shader;
    //pmouse.torch_tex = &torch_tex;

    // to encourage user to unblock sound on certain browsers that
    // block it by default
    sound_play("silent-sound", false);

    // init mirror
    int swidth, sheight;
    get_elementid_size("canvas", &swidth, &sheight);
    drawsurface_init(&mirror, swidth, sheight, TEX_NEAREST, mirror_frame);

    //mirror_prop.scale[0] = CELL_SIZE*20;
    //mirror_prop.scale[1] = CELL_SIZE*20;
    mirror_prop.scale[0] = CELL_SIZE/2;
    mirror_prop.scale[1] = CELL_SIZE;
    mirror_prop.pos[0] = maze.x + maze.cols * CELL_SIZE/2 - mirror_prop.scale[0]/2 - CELL_SIZE/2 + CELL_SIZE/8;
    //mirror_prop.pos[0] = maze.x;
    //mirror_prop.pos[1] = maze.y + CELL_SIZE*3;
    mirror_prop.pos[1] = maze.y + maze.rows * CELL_SIZE - CELL_SIZE;
    mirror_prop.pos[2] = CELL_SIZE/8;
}





void update_cam(void);

int frame_div = 1;


EM_BOOL frame_loop(double _t, void *user_data) {
    (void)user_data;

    double t;
    float dt;
    float fps;

    // update frame times
    update_frame_time(_t, &t, &dt, &fps);

    static int fps_count = 0;
    if ((fps_count++ % 60) == 0)
        printf("fps: %f\n", fps);

    /////////////////////////////////////////////
    //// UPDATE HIDDEN FRAMEBUFFER DRAWS  //////
    ///////////////////////////////////////////

    // lol. Nevermind.

    
    // TODO: add a function that will test for screen refresh rate
    // and then automatically adjust the frame divider using the current
    // frame per second to prevent overbudget frames, which messes up
    // stuff like player camera controls
    static bool bkey_debounce = true;
    if (key[KEY_B]) {
        if (bkey_debounce) {
            bkey_debounce = false;
            frame_div++;
            frame_div %= 5;
            if (frame_div == 0) frame_div = 1;
            printf("frames divided by %d\n", frame_div);
            emscripten_set_main_loop_timing(EM_TIMING_RAF, frame_div);
        }
    } else {
        bkey_debounce = true;
    }
    
    //////////////////////////
    //// UPDATE SCENE  //////
    ////////////////////////

    static bool dither = false;
    static int frag_mode = 0;
    static bool debounce = true;
    if (key[KEY_3] && maze.mode != MAZE_MODE_DETAILED) {
        if (debounce) {
            debounce = false;
            dither = !dither;
            if (dither)
                maze.shader_trail = &texplane_dither_shader;
            else
                maze.shader_trail = &texplane_shader;
        }
    } else if (key[KEY_5] && maze.mode == MAZE_MODE_DETAILED) {
        if (debounce) {
            debounce = false;
            frag_mode++;
            if (frag_mode == 8) frag_mode += 1;
            if (frag_mode == 11) frag_mode += 2;
            frag_mode %= 14;
            //printf("%d, %d, %d\n", frag_mode, frag_mode%4, frag_mode/4);
            printf("debug mode %d\n", frag_mode);
            shader_set_int(&maze_shader, "u_f_mode", frag_mode%7);
            switch (frag_mode/7) {
                case 0:
                    maze.texture_wall_norm = &rock_wall_norm;
                    maze.texture_wall_norm2 = &rock_wall_norm_512;
                    break;
                case 1:
                    maze.texture_wall_norm = &test_norm;
                    maze.texture_wall_norm2 = &test_norm;
                    break;
            }
        }
    } else {
        debounce = true;
    }

    /*if (maze.mode == MAZE_MODE_2D || maze.mode == MAZE_MODE_3D) {
        mouse_update(&pmouse, t, dt);
    }*/

    mouse_update(&pmouse, t, dt);


    // update models
    /*if (count%UPDATE_DIVISOR == 0 && behave_flag)
        model_update_pipeline(t, dt);*/

    ///////////////////////////
    //// UPDATE CAMERA  //////
    /////////////////////////

    update_cam();

    //////////////////////////
    //// DRAW SCENE  ////////
    ////////////////////////

    /*for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            printf("%f, ", camera.viewmat[y][x]);
        }
        printf(";; ");
    }
    printf("\n");*/

    // Some witchcraft happenin' here
    vec3 off = (vec3){pmouse.scale*4/8, pmouse.scale/4, pmouse.scale/2};
    vec3 pos = (vec3){pmouse.x, pmouse.y, 0};
    //vec3 pos = (vec3){maze.x+maze.cols*CELL_SIZE/2, maze.y+CELL_SIZE, CELL_SIZE/4};
    glm_vec3_rotate(off, -pmouse.camera.rot[1]-MATH_PI, (vec3){0,0,1});
    
    glm_vec3_add(off, pos, pos);
    glm_vec3_add((vec3){pmouse.scale/2, pmouse.scale/2, pmouse.scale/2}, pos, pos);
    shader_set_vec3(&maze_shader, "point_light_pos", pos);
    shader_set_float(&maze_shader, "point_light_int", 0.005);
    //shader_set_float(&maze_shader, "point_light_int", 0.0025);



    // clear the scene
    // note that with the new framebuffer draws, this is neccissary otherwise
    // weird overlap is going to occur
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_DEPTH_BUFFER_BIT);


    if (maze.mode == MAZE_MODE_DETAILED) {

        /*vec3 _wave;
        static vec3 wave;
        if (dx || dy) {
            float k = t/1.5 * (key[KEY_SHIFT] ? 1.5 : 1);
            _wave[0] = cos(k)/400.0/2/2 * (key[KEY_SHIFT] ? 2 : 1);
            _wave[1] = (-cos(k*2.0) + 1.0) / 2.0 / 400.0/8;
            _wave[2] = -(ABS(sin(k))-0.6)/400.0/2 * (key[KEY_SHIFT] ? 2 : 1);
            glm_vec3_rotate(_wave, -m->camera.rot[Y], (vec3){0,0,1});
        } else {
            glm_vec3_zero(_wave);
        }

        for (int i = 0; i < 3; i++)
            wave[i] = (m->wave[i]*15 + _wave[i]*4)/16;

        glm_translate(m->camera.viewmat, m->wave);*/

    
        camera_update(&camera_static);
        camera_apply(&camera_static, texplane_shader.program);
        //texture_bind(&torch_tex, &poly_shader, "tex0", GL_TEXTURE0);
        /*draw_texture_plane((vec3){-0.5, -0.5, -0.5}, (vec2){1,1}, NULL, 0, 
            &torch_tex, &texplane_shader, false);*/
        float x = camera_static.wmax[0]-1.6;
        if (x > 0) x = 0;

        vec3 wave;
        glm_vec3_copy(pmouse.wave2, wave);
        //glm_vec3_rotate(pmouse.wave, pmouse.camera.rot[1], (vec3){0,0,1});

        vec3 off = {wave[0]*30, -wave[1]*100, 0};
        //glm_vec3_rotate(off, -pmouse.camera.rot[1], (vec3){1,0,0});
        
        draw_texture_plane((vec3){x + off[0]/*1.6*/, -1 + off[1], 0}, (vec2){1.6,1.6}, (vec3){0,0,1}, 0, 
            &torch_tex, &texplane_shader, true);

        mouse.grabby = true;

        // play sound here
        if (mouse.first_interaction) {
            sound_play2("tense-drone-sound", 1.0, 1.0, true, true);
            sound_play2("somber-sound", 0.5, 1.0, true, true);
            //sound_play2("intense-drone-sound", true);
            //sound_play("somber-sound", true);
        }
    } else {
        // pause sounds here
        sound_pause("tense-drone-sound");
        sound_pause("somber-sound");

        mouse.grabby = false;
    }
    
    

    camera_apply(&camera, poly_program);
    camera_apply(&camera, poly3d_program);
    //camera_apply(&camera, point_program);
    camera_apply(&camera, texplane_program);
    camera_apply(&camera, texplane_dither_program);
    camera_apply(&camera, maze_program);
    camera_apply(&camera, mirror_program);

    

    vec3 light_norm = (vec3){0, 0, 1};
    glm_vec3_rotate(light_norm, t/50, (vec3){0,1,0});
    shader_set_vec3(&maze_shader, "u_light_norm_debug", light_norm);

    if (maze.mode == MAZE_MODE_DETAILED) {
        //glm_vec3_add(pos, (vec3){-0.004, -0.004, -0.01}, pos);
        //draw_rect3(pos, (vec3){0.008,0.008,0.008}, NULL, 0, COLOR_WHITE, &poly3d_shader);
        /*draw_rect3((vec3){maze.x+CELL_SIZE*maze.cols/2, maze.y+CELL_SIZE, CELL_SIZE/2}, 
            (vec3){0.0008,0.0008,0.004}, light_norm, 0, COLOR_WHITE, &poly3d_shader);*/
    }

    /*// draw models
    for (int i = 0; i < NUM_MODELS; i++)
        MODEL_draw(models+i);
        glUniformMatrix4fv(u_proj_mat_loc, 1, GL_FALSE, (GLfloat*)&u_proj_mat);
    }*/

    //draw_rectangle(0.8, 0.8, 0, (vec2){-0.4,-0.4}, COLOR_RED, &poly_shader, 0);
    //draw_rect3((vec3){-0.4,-0.4,-0.4}, (vec3){0.8,0.8,0.8}, (vec3){0,0,0}, 0, COLOR_RED, &poly3d_shader);
    /*cube_mesh.mode = GL_POINTS;
    draw_rect3((vec3){-0.4,-0.4,-0.4}, (vec3){0.8,0.8,0.8}, (vec3){0,0,0}, 0, COLOR_RED, &point_shader);
    cube_mesh.mode = GL_TRIANGLE_STRIP;*/

    //draw_rect3((vec3){-0.4,-0.4,-0.4}, (vec3){0.8,0.8,0.8}, (vec3){0,0,0}, 0, COLOR_ORANGE_ALPHA, &poly3d_shader);

    if (maze.mode == MAZE_MODE_DETAILED) {
        texture_bind(maze.texture_wall2, maze.shader_detailed, "tex0", GL_TEXTURE0);
        draw_rect3(
            (vec3){mirror_prop.pos[0]-0.01, mirror_prop.pos[1]+0.0001, 0}, 
            (vec3){mirror_prop.scale[0]+0.02,0.01, CELL_SIZE*2},
            (vec3){0,0,0}, 0,
            COLOR_DIRT_GREY, maze.shader_detailed);
    }
        

    mouse_draw(&pmouse, t);

    //draw_texture_plane((vec3){maze.x + maze.cols*CELL_SIZE/2, maze.y+CELL_SIZE, 0}, (vec2){CELL_SIZE/2*1.18,CELL_SIZE/2*1.18}, (vec3){0,-1,0}, MATH_PI, &cat_avatar_tex, &texplane_shader, true);

    //draw_texture_plane((vec3){0, 0, 0}, (vec2){1,1}, (vec3){0,0,1}, 0, &nyan_texture, &texplane_shader, true);


    if (maze.mode == MAZE_MODE_DETAILED) {
        // draw mirror
        drawsurface_draw(&mirror, t, dt);

        // reapply some of the cameras because some of them got overriden by the mirror
        camera_apply(&camera, poly_program);
        camera_apply(&camera, poly3d_program);
        //camera_apply(&camera, point_program);
        camera_apply(&camera, texplane_program);
        camera_apply(&camera, texplane_dither_program);
        camera_apply(&camera, maze_program);
        camera_apply(&camera, mirror_program);
        // fix z buffer sampling
        //glDepthFunc(GL_LESS);
        glCullFace(GL_BACK);

        shader_set_float(&mirror_shader, "u_swidth", mirror.asset.width);
        shader_set_float(&mirror_shader, "u_sheight", mirror.asset.height);
        
        // draw mirror texture
        draw_texture_plane(
            mirror_prop.pos,
            mirror_prop.scale, 
            (vec3){0,1,0}, /*MATH_PI*/ 0, 
            &mirror.texture, &mirror_shader, false);
    }



    // TODO: this overrides some of the cameras for some shaders, which is why it is last
    // please fix this, yeah?
    // TODO: perhaps just create a separate draw function for the maze for the texture? Call it manually?
    maze_draw(&maze, t);
    
    
    // requests frame buffer swap. Will actually render stuff to screen
    // this is neccissary because explicitSwapControl was set to GL_TRUE
    emscripten_webgl_commit_frame();
    input_refresh();

    // return true to keep looping
    // return false to kill loop
    return EM_TRUE;
}




void mirror_frame(DRAWSURFACE* surface, void* data, double t, float dt) {

    // clear scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // for some reason the z-buffer is inverted for this, so...
    //glDepthFunc(GL_MORE);
    glCullFace(GL_FRONT);

    // Adjust light sources
    // probably not neccissary as they don't exactly get overwritten from earlier
    /*
    vec3 off = (vec3){pmouse.scale*4/8, pmouse.scale/4, pmouse.scale/2};
    vec3 pos = (vec3){pmouse.x, pmouse.y, 0};
    glm_vec3_rotate(off, -pmouse.camera.rot[1]-MATH_PI, (vec3){0,0,1});    
    glm_vec3_add(off, pos, pos);
    glm_vec3_add((vec3){pmouse.scale/2, pmouse.scale/2, pmouse.scale/2}, pos, pos);
    shader_set_vec3(&maze_shader, "point_light_pos", pos);
    shader_set_float(&maze_shader, "point_light_int", 0.005);*/

    // Update mirror camera
    //float dx = mirror_prop.pos[0] - pmouse.camera.pos[0];
    //float dy = mirror_prop.pos[1] - pmouse.camera.pos[1];
    //camera_mirror.pos[0] = mirror_prop.pos[0] + dx;
    //camera_mirror.pos[1] = mirror_prop.pos[1] - dy;
    //camera_mirror.pos[2] = pmouse.camera.pos[2];

    //vec4 pos;
    //glm_decompose(camera.viewmat, pos, NULL, NULL);
    //printf("%f, %f, %f, %f\n", pos[0], pos[1], pos[2], pos[3]);
    camera_mirror.pos[0] = pmouse._cpos[0];
    camera_mirror.pos[1] = mirror_prop.pos[1] * 2 - pmouse._cpos[1];
    camera_mirror.pos[2] = pmouse._cpos[2];
    camera_mirror.fov = pmouse.camera.fov;
    camera_mirror.rot[0] = pmouse.camera.rot[0];
    camera_mirror.rot[1] = pmouse.camera.rot[1] + MATH_PI;
    camera_mirror.rot[2] = pmouse.camera.rot[2];
    //camera_mirror.rot[2] = MATH_PI/2;
    ////
    //camera_update_actual(&camera_mirror);
    camera_update_actual_flipx(&camera_mirror);

    /*mat4 reflection_matrix = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f, 1.0f * mirror_prop.pos[1]},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };*/
    /*mat4 reflection_matrix = {
        {-1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    glm_mat4_mul(reflection_matrix, camera_mirror.viewmat, camera_mirror.viewmat);*/

    // apply camera
    camera_apply(&camera_mirror, poly_program);
    camera_apply(&camera_mirror, poly3d_program);
    camera_apply(&camera_mirror, texplane_program);
    camera_apply(&camera_mirror, texplane_dither_program);
    camera_apply(&camera_mirror, maze_program);


    // draw player cat avatar
    /*draw_texture_plane(
        (vec3){maze.x + maze.cols*CELL_SIZE/2, maze.y+CELL_SIZE, 0},
        (vec2){CELL_SIZE/2*1.18,CELL_SIZE/2*1.18}, 
        (vec3){0,-1,0}, MATH_PI, 
        &cat_avatar_tex, &texplane_shader, true);*/

    vec3 pos;
    //glm_vec3_add(pmouse.camera.pos, pmouse._cpos, pos);
    glm_vec3_copy(pmouse._cpos, pos);
    pos[2] = 0;
    pos[0] += CELL_SIZE/4;
    vec3 rot = (vec3){0,-1,0};
    //glm_vec3_rotate(rot, pmouse.camera.rot[1]+ MATH_PI, (vec3){0,0,1});
    
    draw_texture_plane(
        pos,
        (vec2){CELL_SIZE/2*1.18,CELL_SIZE/2*1.18}, 
        rot, MATH_PI, 
        &cat_avatar_tex, &texplane_shader, true);


    //draw_rect3((vec3){maze.x + maze.cols*CELL_SIZE/2, maze.y + CELL_SIZE*3-0.01, 0}, (vec3){0.01,0.01,0.01}, NULL, 0, COLOR_RED, &poly3d_shader);

    // draw maze
    // REMEMBER DO THIS LAST, AS ITS PATH SCENE MESSES UP DRAW CALLS FOLLOWING IT!
    maze.hide_upper_walls = true;
    maze_draw(&maze, t);
    maze.hide_upper_walls = false;
    
    //glClearDepth(1.0);
    //glClear(GL_DEPTH_BUFFER_BIT);

}





// TODO: create a transition class for animations like this, 
// that will actually allow things to, you know, transision without having to 
// do all this extra variable setup and whatnot.
void update_cam(void) {

    static int lastmode = MAZE_MODE_2D;
    static int mode = MAZE_MODE_2D;
    static float inter = 1;

    static float cx = 0;
    static float cy = 0;
    static float cinter = 1;

    if (key[KEY_1] && mode != MAZE_MODE_2D && cinter > 0.99) {
        lastmode = mode;
        if (lastmode != MAZE_MODE_DETAILED)
            inter = cinter = 0;
        mode = MAZE_MODE_2D;
    } else if (key[KEY_2] && mode != MAZE_MODE_3D && cinter > 0.99) {
        lastmode = mode;
        if (lastmode != MAZE_MODE_DETAILED)
            inter = cinter = 0;
        mode = MAZE_MODE_3D;
    } else if (key[KEY_4] && mode != MAZE_MODE_DETAILED) {
        lastmode = mode;
        mode = MAZE_MODE_DETAILED;
        inter = cinter = 1;
    }

    inter += 0.016;
    inter = (inter > 1) ? (1) : (inter);

    //camera_rotate_about(&camera, (vec3){0,0,0}, (vec3){(float)(t/20), (float)(t/40)*0, 0}, 3.0);
    //camera_rotate_about(&camera, (vec3){0,0,0.3}, (vec3){-(float)MATH_PI/4, 0, (float)(t/40)}, 3.0);
    //camera.rot[0] += 0.01;
    //camera_update(&camera);

    //printf("pmode %d; mode %d; %f; keys %d%d%d\n", lastmode, mode, inter, key[KEY_1], key[KEY_2], key[KEY_3]);

    cx = (cx*15 + pmouse.x)/16;
    cy = (cy*15 + pmouse.y)/16;
    cinter = (cinter*15 + inter)/16;

    cinter = (cinter > 0.9999) ? (1) : (cinter);

    //printf("inter %f; cinter %f\n", inter, cinter);
    

    if (mode == MAZE_MODE_2D) {

        camera_rotate_about(&camera_1, (vec3){0,0,0}, (vec3){0, 0, 0}, 3.0);
        pmouse.mode = MOUSE_TOP;
    
    } else if (mode == MAZE_MODE_3D) {

        camera_rotate_about(&camera_2, (vec3){0,0,2}, (vec3){cy/3, -cx/3, 0}, 3.5);
        pmouse.mode = MOUSE_TOP;

    } else if (mode == MAZE_MODE_DETAILED) {

        //camera_3
        pmouse.mode = MOUSE_FPS;

    }
    

    switch (mode) {
        case MAZE_MODE_2D:
        
            if ((cinter < 1) && (lastmode == MAZE_MODE_3D)) {
                camera_interpolate(&camera, &camera_2, &camera_1, cinter);
                maze.mode = MAZE_MODE_3D;
            } else {
                camera_set(&camera, &camera_1);
                maze.mode = MAZE_MODE_2D;
            }
            break;


        case MAZE_MODE_3D:
        
            maze.mode = MAZE_MODE_3D;
            
            if ((cinter < 1) && (lastmode == MAZE_MODE_2D)) {
                camera_interpolate(&camera, &camera_1, &camera_2, cinter);
            } else {
                camera_set(&camera, &camera_2);
            }
            break;


        case MAZE_MODE_DETAILED:
        
            maze.mode = MAZE_MODE_DETAILED;
            camera_set(&camera, &pmouse.camera);
            break;
    }
}





/*void controls(double t, float dt) {
    if (mouse.button[0]) {
        camera.rot[1] += (float)mouse.dx/100;
        camera.rot[0] += (float)mouse.dy/100;
    }
    const register float k = MATH_PI*3.0/8.0;
    if (camera.rot[0] > k)
        camera.rot[0] = k;
    if (camera.rot[0] < -k)
        camera.rot[0] = -k;
}*/







// TODO: add this to a behavior object class
/*
if (!disable_rotgrav) {
        // rotate gravity
        // mainly for debugging purposes
        // but it also looks cool
        glm_vec3_copy((vec3)GRAVITY, gravity);
        // TODO: because delta-v isn't hooked up to dt yet,
        // We have to scale gravity manually    glm_vec3_copy((vec3)GRAVITY, gravity);
        glm_vec3_scale(gravity, (float)1/sqrt(BEHAVE_PASSES)*3*GRAV_MUL, gravity);
        glm_vec3_rotate(gravity, (float)MATH_PI/2, (vec3){1,0,0});
        glm_vec3_rotate(gravity, t/300, (vec3){0,0,1});
        glm_vec3_rotate(gravity, t/300, (vec3){0,1,0});
    }
    
void circle_update(MODEL* model, double t, float dt) {
    BEHAVE* b = behave + model->id;

    float sratio = (float)swidth/sheight;

    wall_collide_eval(b, 
        (vec3){wmin[0],wmin[1],wmin[2]}, 
        (vec3){wmax[0],wmax[1],wmax[2]}, dt);

    for (int i = model->id + 1; i < NUM_MODELS; i++)
        sphere_collide_eval(b, behave+i, dt);
}

void model_transform(MODEL* model) {
    BEHAVE* b = behave + model->id;

    // apply transformations to matrix
    vec3 pos;
    glm_vec3_scale(b->pos, 1/b->scale, pos);
    //glm_vec3_scale(b->pos, 1, pos);
    mat4 viewmat = GLM_MAT4_IDENTITY_INIT;
    glm_scale_uni(viewmat, b->scale);
    glm_translate_to(viewmat, pos, model->view_mat);
}*/
