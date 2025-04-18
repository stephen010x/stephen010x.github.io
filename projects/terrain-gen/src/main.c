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

/*#define __gl_h_
#define GLT_DEBUG
#define GLT_DEBUG_PRINT
#define GLT_IMPLEMENTATION
#include "lib/gltext/gltext.h"*/

// TODO: get rid of main header
#include "main.h"
#include "core.h"
#include "objects.h"
#include "shaders/shaders.h"
#include "helper.h"



#define WIREFRAME_MODE



//*DEBUG*/ extern CUBE_MESH_TYPE cube_mesh;


#define COLOR_RED    ((COLOR){0.8, 0.2, 0.2, 1.0})
#define COLOR_ORANGE ((COLOR){0.8, 0.3, 0.2, 1.0})
//#define COLOR_BLUE   ((COLOR){0.3, 0.4, 0.9, 1.0})
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
#define COLOR_GREY  ((COLOR){0.8, 0.8, 0.8, 1.0})
#define COLOR_GREEN  ((COLOR){0.20*2, 0.3*2*0.75, 0.15*2, 1.0})
//#define COLOR_GREEN  ((COLOR){0.20, 0.4, 0.15, 1.0})
// #define COLOR_BLUE  ((COLOR){0.15, 0.1, 0.3, 1.0})
// #define COLOR_BLUE  ((COLOR){0.1, 0.2, 0.35, 1.0})
#define COLOR_BLUE  ((COLOR){0.05*2, 0.15*2, 0.25*2, 1.0})

#define COLOR_LIGHT_WARM  ((COLOR){1.0, 0.84+0.1, 0.78+0.1, 1.0})
//#define COLOR_LIGHT_WARM  ((COLOR){0.98, 0.5, 0.5, 1.0})
//#define COLOR_LIGHT_COOL  ((COLOR){0.65, 0.75, 1.0, 1.0})
#define COLOR_LIGHT_COOL  ((COLOR){0.55/1.5, 0.65/1.5, 1.0, 1.0})


//#define COLOR_WATER  ((COLOR){0.01*1.5, 0.03*1.5, 0.1*1.5, 1.0})
#define COLOR_WATER  ((COLOR){0.01*1.0, 0.03*1.0, 0.1*1.0, 1.0})
//#define COLOR_GRASS  ((COLOR){0.20*2, 0.3*1.75, 0.15*2, 1.0})
#define COLOR_GRASS  ((COLOR){0.20*4, 0.3*4, 0.15*4, 1.0})

//#define COLOR_WATER2  ((COLOR){COLOR_WATER.r/2, COLOR_WATER.g/2, COLOR_WATER.b/2, 1.0})



#define DT_DIVISOR 100


MOUSE mouse;


GEN_MODEL(20) _gen_model;
#define gen_model (*(GEN_MODEL*)&_gen_model)

TERRAIN terrain;
TERRAIN water;




WORLD world = {
    .light = {
        .type = LIGHTSOURCE_TYPE_DIR,
        .dir = {
            .norm = {0.8, 1, 2},
            .amb = 0.2*2*2,
            //.amb = 0.4,
            .bright = 1.96/2*1.5,
            .diff_color = COLOR_LIGHT_WARM,
            .amb_color = COLOR_LIGHT_COOL,
        },
    },
};


CAMERA camera = {
    .pos = {0,0,0},
    .rot = {MATH_PI/2, MATH_PI, MATH_PI},
    .fov = DEG_TO_RAD(90),
    .type = CAMERA_PERSPECTIVE,
};

/*CAMERA screencam = {
    .mod_viewmat = GLM_MAT4_IDENTITY_INIT,
};*/

CAMERA screencam = {
    .zoom = 1.0,
    //.fov = DEG_TO_RAD(90),
    //.type = CAMERA_PERSPECTIVE,
    .type = CAMERA_ORTHOGRAPHIC,
};



/*
    TEXTURE RULES:
    if below a certain depth, then riverbed only.
    If above that depth:
        if too steep, then cliffs
        if not too steep:
            if below a certain depth, then sand
            otherwise, use grass

*/





void init_scene(void);
EM_BOOL frame_loop(double t, void *user_data);
int __main(void);




EMSCRIPTEN_KEEPALIVE int main() {
    printf("starting webgl\n");

    int retval = __main();
    printf("main exited with code (%d)\n", retval);

    return retval;
}




ASSET_PACK grass_assets;
ASSET_PACK water_assets;
ASSET_PACK sand_assets;
ASSET_PACK cliff_assets;
ASSET_PACK wetrock_assets;

TEXTURE_PACK grass_textures;
TEXTURE_PACK water_textures;
TEXTURE_PACK sand_textures;
TEXTURE_PACK cliff_textures;
TEXTURE_PACK wetrock_textures;

SHADER terrain_shader;
SHADER water_shader;
SHADER depth_shader;
SHADER skybox_shader;
SHADER underwater_shader;
SHADER wireframe_shader;
SHADER simple_shader;



void _frame_loop(void);




void terrain_draw_frame(SHADER* shader, double t, void* data);
void water_draw_frame(SHADER* shader, double t, void* data);
void depth_draw_frame(SHADER* shader, double t, void* data);
void sky_draw_frame(SHADER* shader, double t, void* data);
void underwater_draw_frame(SHADER* shader, double t, void* data);
void wire_draw_frame(SHADER* shader, double t, void* data);
void simple_draw_frame(SHADER* shader, double t, void* data);

GLuint zbuffer;
//GLuint ztexture;
TEXTURE ztexture;



int swidth, sheight;


/*GLTtext *text_hint;
GLTtext *text_fps;
GLTtext *text_coords;*/


int __main(void) {
    // set starting random seed
    srand((unsigned int)time(NULL));

    // remember, you wrote this function.
    int err = init_webgl("#" HTML_CANVAS_ID);
    ASSERT(err == 0, -1, "init_webgl failed\n");


    SHADER_DESCRIPTOR terrain_desc    = SHADER_DESC_GEN(true, NULL,  terrain, terrain);
    SHADER_DESCRIPTOR water_desc      = SHADER_DESC_GEN(true, NULL,  terrain, water);
    SHADER_DESCRIPTOR depth_desc      = SHADER_DESC_GEN(true, NULL,  depth, depth);
    SHADER_DESCRIPTOR skybox_desc     = SHADER_DESC_GEN(true, NULL,  simple3d, skybox);
    SHADER_DESCRIPTOR underwater_desc = SHADER_DESC_GEN(true, NULL,  terrain, underwater);
    SHADER_DESCRIPTOR wireframe_desc  = SHADER_DESC_GEN(true, NULL,  simple3d, wireframe);
    SHADER_DESCRIPTOR simple_desc     = SHADER_DESC_GEN(true, NULL,  simple3d, simple);
    
    shader_init(&terrain_shader, &terrain_desc, terrain_draw_frame, 
        NULL, MESHTYPE_3D_VERT_NORM);
    shader_init(&water_shader, &water_desc, water_draw_frame, 
        NULL, MESHTYPE_3D_VERT_NORM);
    shader_init(&depth_shader, &depth_desc, depth_draw_frame, 
        NULL, MESHTYPE_3D_VERT_NORM);
    shader_init(&skybox_shader, &skybox_desc, sky_draw_frame, 
        NULL, MESHTYPE_3D_VERT_NORM);
    shader_init(&underwater_shader, &underwater_desc, underwater_draw_frame, 
        NULL, MESHTYPE_3D_VERT_NORM);
    shader_init(&wireframe_shader, &wireframe_desc, wire_draw_frame, 
        NULL, MESHTYPE_3D_VERT_NORM);
    shader_init(&simple_shader, &simple_desc, simple_draw_frame, 
        NULL, MESHTYPE_3D_VERT_NORM);

    wireframe_shader.drawtype = SHADER_DRAW_WIREFRAME;
    


    // TODO: enable clockwise vertex order here or whatever
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_DITHER);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_MULTISAMPLE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //glDepthRangef(1.0, 0.0);
    //glDepthFunc(GL_GEQUAL);
    glDepthFunc(GL_LESS);
    
    //glClearDepthf(1.0);

    //printf("Z-Buffer depth bits:%d\n", glGetParameteri(GL_DEPTH_BITS));

    //glClearDepthf(log2(-10000));



    get_elementid_size("canvas", &swidth, &sheight);


    // create zbuffer
    glGenFramebuffers(1, &zbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, zbuffer);

    static ASSET zasset;
    zasset = (ASSET){
        .width = swidth,
        .height = sheight,
    };
    texture_init(&ztexture, &zasset, TEX_MONO_FLOAT);

    // attach ztexture to zbuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ztexture.id, 0);

    // bind actual depth buffer
    GLuint depthbuffer;
    glGenRenderbuffers(1, &depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, swidth, sheight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    // unbind z buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    

    //glDisable(GL_CULL_FACE);
    //glDisable(GL_DEPTH_TEST);



    /*asset_load_img(&grass_norm_asset, "moss-norm-1024");
    asset_load_img(&grass_norm_asset, "moss-disp-1024");
    asset_load_img(&grass_tex_asset,  "moss-ao-1024");
    asset_load_img(&water_norm_asset, "water-norm-1024");*/


    asset_pack_load(&grass_assets,   &GEN_ASSET_NAMES("moss",    1024), ASSET_ALL);
    asset_pack_load(&water_assets,   &GEN_ASSET_NAMES("water",   1024), ASSET_ALL);
    asset_pack_load(&cliff_assets,   &GEN_ASSET_NAMES("cliffs",  512),  ASSET_ALL);
    asset_pack_load(&sand_assets,    &GEN_ASSET_NAMES("sand",    256),  ASSET_ALL);
    asset_pack_load(&wetrock_assets, &GEN_ASSET_NAMES("wetrock", 256),  ASSET_ALL);

    #define GENERIC_TEXFLAGS (TEX_NEAREST | TEX_MIPMAP_NEAREST | TEX_WRAP)
    
    texture_pack_init(&grass_textures,   &grass_assets,   GENERIC_TEXFLAGS, 40.0, 1.5);
    texture_pack_init(&water_textures,   &water_assets,   GENERIC_TEXFLAGS, 40.0, 0.125);
    texture_pack_init(&cliff_textures,   &cliff_assets,   GENERIC_TEXFLAGS, 20.0, 1.0);
    texture_pack_init(&sand_textures,    &sand_assets,    GENERIC_TEXFLAGS, 10.0, 1.0);
    texture_pack_init(&wetrock_textures, &wetrock_assets, GENERIC_TEXFLAGS, 10.0, 1.0);
    
    /*texture_gen_mipmaps(&grass_textures.tex,  MIPMAP_GEN_MIX_2_2, TEX_GEN_MAX);
    texture_gen_mipmaps(&grass_textures.norm, MIPMAP_GEN_MIX_2_2, TEX_GEN_MAX);
    texture_gen_mipmaps(&grass_textures.disp, MIPMAP_GEN_MIX_2_2, TEX_GEN_MAX);
    texture_gen_mipmaps(&grass_textures.occl, MIPMAP_GEN_MIX_2_2, TEX_GEN_MAX);*/
    
    texture_pack_gen_mipmaps(&grass_textures, MIPMAP_GEN_MIX_3_1, TEX_GEN_MAX);
    texture_pack_gen_mipmaps(&water_textures, MIPMAP_GEN_NEAREST, TEX_GEN_MAX);
    texture_pack_gen_mipmaps(&cliff_textures, MIPMAP_GEN_MIX_3_1, TEX_GEN_MAX);
    texture_pack_gen_mipmaps(&sand_textures,  MIPMAP_GEN_MIX_3_1, TEX_GEN_MAX);
    texture_pack_gen_mipmaps(&wetrock_textures, MIPMAP_GEN_MIX_3_1, TEX_GEN_MAX);
    


    
    
    /*texture_init(&grass_norm,   &grass_norm_asset,   GENERIC_TEXFLAGS);
    texture_init(&grass_tex,    &grass_tex_asset,    GENERIC_TEXFLAGS);
    texture_init(&sand_norm,    &sand_norm_asset,    GENERIC_TEXFLAGS);
    texture_init(&sand_tex,     &sand_tex_asset,     GENERIC_TEXFLAGS);
    texture_init(&dryrock_norm, &dryrock_norm_asset, GENERIC_TEXFLAGS);
    texture_init(&dryrock_tex,  &dryrock_tex_asset,  GENERIC_TEXFLAGS);
    texture_init(&wetrock_norm, &wetrock_norm_asset, GENERIC_TEXFLAGS);
    texture_init(&wetrock_tex,  &wetrock_tex_asset,  GENERIC_TEXFLAGS);
    texture_init(&water_norm,   &water_norm_asset,   GENERIC_TEXFLAGS);
    
    texture_gen_mipmaps(&grass_norm,   MIPMAP_GEN_NEAREST, TEX_GEN_MAX);
    texture_gen_mipmaps(&grass_tex,    MIPMAP_GEN_NEAREST, TEX_GEN_MAX);
    texture_gen_mipmaps(&sand_norm,    MIPMAP_GEN_NEAREST, TEX_GEN_MAX);
    texture_gen_mipmaps(&sand_tex,     MIPMAP_GEN_NEAREST, TEX_GEN_MAX);
    texture_gen_mipmaps(&dryrock_norm, MIPMAP_GEN_NEAREST, TEX_GEN_MAX);
    texture_gen_mipmaps(&dryrock_tex,  MIPMAP_GEN_NEAREST, TEX_GEN_MAX);
    texture_gen_mipmaps(&wetrock_norm, MIPMAP_GEN_NEAREST, TEX_GEN_MAX);
    texture_gen_mipmaps(&wetrock_tex,  MIPMAP_GEN_NEAREST, TEX_GEN_MAX);
    texture_gen_mipmaps(&water_norm,   MIPMAP_GEN_NEAREST, TEX_GEN_MAX);*/
    // MIPMAP_GEN_MULTISAMPLE, MIPMAP_GEN_NEAREST

    camera_init(&camera);
    camera_init(&screencam);
    
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

    //gltInit();

    //text_hint = gltCreateText();
    //gltSetText(text_hint, "press \'H\' for controls");

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





char* terrain_filename = "terrain.mesh";
char* uniquekey_filename = "key";

#define UNIQUE_KEY 0xABCD012D

unsigned int uniquekey = UNIQUE_KEY;

void init_scene(void) {
    // set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float height_mul = 0.1/2.0;

    unsigned int oldkey = 0;
    
    if (file_exists(uniquekey_filename)) {
        unsigned int* data = 0;
        int bytes = load_file(uniquekey_filename, (void**)&data);
        oldkey = *data;
        unload_file(data);
    }

    //printf("oldkey %u newkey %u\n", oldkey, uniquekey);
    

    if (file_exists(terrain_filename) && uniquekey == oldkey) {
        printf("%s exists. Loading terrain\n", terrain_filename);
    
        void* data;
        int bytes = load_file(terrain_filename, &data);

        //printf("bytes %d\n", bytes);

        gen_model = (GEN_MODEL){
            .mode = GENMODE_LOAD,
            .loaddata = data,
            .loadbytes = bytes,
            .rows = 250, //500,
            .cols = 250, //500,
            
            .scale = 4.0,
            .x_off = -125, //-250,
            .y_off = -125, //-250,
            .sincount = 8,
        };

        terrain_init(&terrain, (vec3){-250,-250,0}, (vec3){2,2,height_mul*gen_model.scale/2.0},
            &gen_model, COLOR_GRASS, &terrain_shader);
        
        unload_file(data);
        
    } else {
        printf("%s does not exist. Generating terrain\n", terrain_filename);

        gen_model = (GEN_MODEL){
            //.scale = 2.0,
            .mode = GENMODE_GEN,
            .scale = 4.0,
            .rows = 250,//500,
            .cols = 250,//500,
            .x_off = -125,
            .y_off = -125,
            .sincount = 8,
        };

        gen_model.sindata[0] = (GEN_SINDATA){
            .period = 1,
            .height = 0.5,
            .power = 1,
            .damp_enable = true,
            .damp_period = 4,
        }; gen_model.sindata[1] = (GEN_SINDATA){
            .period = 2,
            .height = 1,
            .power = 1,
            .damp_enable = true,
            .damp_period = 8,
        }; gen_model.sindata[2] = (GEN_SINDATA){
            .period = 8,
            .height = 10,
            .power = 1,
            .damp_enable = true,
            .damp_period = 200,
        }; gen_model.sindata[3] = (GEN_SINDATA){
            .period = 32,
            .height = 50,
            .power = 16,
            .damp_enable = true,
            .damp_period = 100,
        }; gen_model.sindata[4] = (GEN_SINDATA){
            .period = 40,
            .height = 100,
            .power = 8,
            .damp_enable = true,
            .damp_period = 120,
        }; gen_model.sindata[5] = (GEN_SINDATA){
            .period = 30,
            .height = 80,
            .power = 1,
            .damp_enable = true,
            .damp_period = 140,
        }; gen_model.sindata[6] = (GEN_SINDATA){
            .period = 100,
            .height = 200,
            .power = 1,
            .damp_enable = false,
        }; gen_model.sindata[7] = (GEN_SINDATA){
            .period = 200,
            .height = 300,
            .power = 1,
            .damp_enable = false,
        };

        terrain_init(&terrain, (vec3){-250,-250,0}, (vec3){2,2,height_mul*gen_model.scale/2.0},
            &gen_model, COLOR_GRASS, &terrain_shader);

        write_file(terrain_filename, terrain.mesh->data, mesh_data_sizeof(terrain.mesh));
        int err = write_file(uniquekey_filename, &uniquekey, sizeof(unsigned int));
    }

    mouse.grabby = true;

    camera.pos[1] = 100;

    static GEN_MODEL water_model = (GEN_MODEL){
        .mode = GENMODE_GEN,
        .scale = 1.0,
        .rows = 250,
        .cols = 250,
    };

    terrain_init(&water, (vec3){-250,-250,-20}, (vec3){2,2,1},
        &water_model, COLOR_WATER, &terrain_shader);


    /*shader_set_mat4(&water_shader, "u_dithermat", (mat4){
        {05.0, 00.0, 08.0, 02.0},
        {10.0, 12.0, 04.0, 14.0},
        {06.0, 03.0, 11.0, 01.0},
        {09.0, 15.0, 07.0, 13.0}
    });*/

    mat4 dithermat = (mat4){
        {06.0, 01.0, 09.0, 03.0},
        {11.0, 13.0, 05.0, 15.0},
        {07.0, 04.0, 12.0, 02.0},
        {10.0, 16.0, 08.0, 14.0}
    };

    shader_set_mat4(&water_shader, "u_dithermat", dithermat);
    shader_set_mat4(&terrain_shader, "u_dithermat", dithermat);
    shader_set_mat4(&underwater_shader, "u_dithermat", dithermat);

    shader_set_float(&terrain_shader, "u_ditherscale", 1.0/16.0);
    
    shader_set_float(&wireframe_shader, "u_wire_range", 200);

    camera_setup(&screencam);
}





//void update_cam(void);

int frame_div = 1;
bool wireframe_enable = false;


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
    static bool first_strike = true;
    if (key[KEY_B] || first_strike) {
        first_strike = false;
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

    static bool vkey_debounce = true;
    if (key[KEY_V]) {
        if (vkey_debounce) {
            vkey_debounce = false;
            wireframe_enable = !wireframe_enable;
        }
    } else {
        vkey_debounce = true;
    }
    
    //////////////////////////
    //// UPDATE SCENE  //////
    ////////////////////////



    ///////////////////////////
    //// UPDATE CAMERA  //////
    /////////////////////////


    if (mouse.grabbed) {
        camera.rot[1] += mouse.dx/200.0;
        camera.rot[0] += mouse.dy/200.0;
    }

    camera.rot[0] = CLAMP(camera.rot[0], MATH_PI/6, MATH_PI*7/6);
    //if (camera.rot[0] > MATH_PI*2/3) camera.rot[0] = MATH_PI*2/3;
    //if (camera.rot[0] < -MATH_PI*2/3) camera.rot[0] = -MATH_PI*2/3;
    
    //early camera update
    camera_update_actual(&camera);

    float tx = (key[KEY_RIGHT] || key[KEY_D]) - (key[KEY_LEFT] || key[KEY_A]);
    float ty = (key[KEY_UP]    || key[KEY_W]) - (key[KEY_DOWN] || key[KEY_S]);
    float tz = key[KEY_SPACE];// - key[KEY_CTRL];
    //float pythag = sqrt(tx*tx + ty*ty + tz*tz);
    //float mul = (key[KEY_SHIFT] ? (5) : (1)) / 10.0 / pythag;

    // get unrotated player translation
    //vec3 vel = (vec3){tx * mul, ty * mul, 0};
    vec3 vel = (vec3){tx, ty, 0};



    glm_vec3_rotate(vel, -(camera.rot[0]-MATH_PI/2.0), (vec3){1,0,0});
    glm_vec3_rotate(vel, -(camera.rot[1]-MATH_PI), (vec3){0,0,1});
    vel[2] += tz;

    float mul = (key[KEY_SHIFT] ? (5.0) : (1.0)) / 10.0 / glm_vec3_norm(vel);
    mul *= dt*6;

    glm_vec3_scale(vel, mul, vel);

    //printf("%f %f\n", camera.rot[0], camera.rot[1]);

    if (!(isnan(vel[0]) || isnan(vel[1]) || isnan(vel[2])))
        glm_vec3_add(camera.pos, vel, camera.pos);

    camera.pos[0] = CLAMP(camera.pos[0], -250.0+0.5, 250.0-0.5);
    camera.pos[1] = CLAMP(camera.pos[1], -250.0+0.5, 250.0-0.5);

    float theight = terrain_get_height(&terrain, camera.pos[0], camera.pos[1]);
    if (camera.pos[2] < theight + 2.0)
        camera.pos[2] = theight + 2.0;
    
    // final camera update;
    camera_update_actual(&camera);

    terrain_update(&terrain, t, dt);
    terrain_update(&water, t, dt);

    //glm_vec3_rotate(world.light.dir.norm, 0.01, (vec3){0,1,0});

    //////////////////////////
    //// DRAW SCENE  ////////
    ////////////////////////


    // clear the scene
    // note that with the new framebuffer draws, this is neccissary otherwise
    // weird overlap is going to occur
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_DEPTH_BUFFER_BIT);

    
    //shader_draw(&simple_shader, t);

    if (!wireframe_enable) {
        shader_draw(&skybox_shader, t);
        shader_draw(&depth_shader, t);
        shader_draw(&terrain_shader, t);
        if (camera.pos[2] > water.pos[2] + 0.5) {
            shader_draw(&water_shader, t);
        } else {
            shader_draw(&underwater_shader, t);
        }
    } else {
        shader_draw(&wireframe_shader, t);
    }

    //draw_rect2((vec3){-250,-250,-5}, (vec3){500,500}, NULL, 0, COLOR_BLUE, &terrain_shader);



    // Begin text drawing
    /*gltBeginDraw();

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(text_hint, 0.1, 0.1, 1.0);

    // Finish drawing text
    gltEndDraw();*/

    update_overlay_text("press \'h\' for controls\nfps %.1f\npos x:%.1f y:%.1f z:%.1f",
        fps, camera.pos[0], camera.pos[1], camera.pos[2]);
    


    // requests frame buffer swap. Will actually render stuff to screen
    // this is neccissary because explicitSwapControl was set to GL_TRUE
    emscripten_webgl_commit_frame();
    input_refresh();

    // return true to keep looping
    // return false to kill loop
    return EM_TRUE;
}





void terrain_draw_frame(SHADER* shader, double t, void* data) {
    
    camera_apply(&camera, shader->program);
    
    shader_set_bool(shader, "u_enable_texture", true);
    terrain.model.color = COLOR_WHITE;
    
    lightsource_apply_spectral(&world.light, shader->program, &camera, 0.28, 10.0);
    
    /*texture_bind_scale(&grass_norm, shader, "u_norm0", GL_TEXTURE0, "u_tex_scale", 16.0,
        "u_tex_strength", 1.0);
    texture_bind_scale(&grass_tex,  shader, "u_tex0",  GL_TEXTURE1, NULL, 0.0, NULL, 0.0);*/

    texture_pack_bind(&grass_textures,   shader, "u_tex0", GL_TEXTURE0);
    texture_pack_bind(&cliff_textures,   shader, "u_tex1", GL_TEXTURE4);
    texture_pack_bind(&sand_textures,    shader, "u_tex2", GL_TEXTURE8);
    texture_pack_bind(&wetrock_textures, shader, "u_tex3", GL_TEXTURE12);
    
    terrain_draw(&terrain, t);

    /*draw_rect2((vec3){-swidth/2, 0, -10}, (vec2){swidth, sheight}, (vec3){0,0,1}, 0, COLOR_WHITE, shader);
    draw_rect2((vec3){-swidth/2, 0, -10}, (vec2){swidth, sheight}, (vec3){0,0,0}, 0, COLOR_WHITE, shader);*/
}




void water_draw_frame(SHADER* shader, double t, void* data) {
    camera_apply(&camera, shader->program);

    shader_set_float(shader, "u_swidth", (float)swidth);
    shader_set_float(shader, "u_sheight", (float)sheight);

    //shader_set_bool(shader, "u_enable_texture", false);
    //lightsource_apply_spectral(&world.light, shader->program, &camera, 1.4*2, 1000.0);
    lightsource_apply_spectral(&world.light, shader->program, &camera, 1.4*2, 300.0);
    //lightsource_apply_spectral(&world.light, shader->program, &camera, 1.4*2*100, 20.0);
    /*texture_bind_scale(&water_norm, shader, "u_norm0", GL_TEXTURE0, "u_tex_scale", 4.0*16.0,
        "u_tex_strength", 1.0/4.0);*/

    //shader_set_bool(shader, "u_enable_texture", true);
    //water.model.color = COLOR_WHITE;
        
    texture_pack_bind(&water_textures,   shader, "u_tex0", GL_TEXTURE0);
    texture_bind(&ztexture, shader, "u_depth0", GL_TEXTURE4);

    shader_set_float(shader, "u_time", t/1.0);
    shader_set_bool(shader, "u_animate", true);
    
    terrain_draw(&water, t);
    shader_set_bool(shader, "u_animate", false);
}




void depth_draw_frame(SHADER* shader, double t, void* data) {
    glBindFramebuffer(GL_FRAMEBUFFER, zbuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera_apply(&camera, shader->program);

    glCullFace(GL_FRONT);
    terrain_draw(&water, t);
    glCullFace(GL_BACK);
    
    terrain_draw(&terrain, t);

    /*draw_rect2((vec3){250,   250, -20}, (vec2){500, 100}, (vec3){0,1,0},  DEG_TO_RAD(180), COLOR_WATER, shader);
    draw_rect2((vec3){250,  -250, -20}, (vec2){500, 100}, (vec3){1,0,0},  DEG_TO_RAD(90),  COLOR_WATER, shader);
    draw_rect2((vec3){-250, -250, -20}, (vec2){500, 100}, (vec3){0,-1,0}, DEG_TO_RAD(0),   COLOR_WATER, shader);
    draw_rect2((vec3){-250,  250, -20}, (vec2){500, 100}, (vec3){-1,0,0}, DEG_TO_RAD(-90), COLOR_WATER, shader);
    */
    /*
    draw_rect2((vec3){250,   250, -20}, (vec2){500, 100}, (vec3){0,1,0},  DEG_TO_RAD(180), COLOR_WATER, shader);
    draw_rect2((vec3){250,  -250, -20}, (vec2){500, 100}, (vec3){1,0,0},  DEG_TO_RAD(90),  COLOR_WATER, shader);
    draw_rect2((vec3){-250, -250, -20}, (vec2){500, 100}, (vec3){0,-1,0}, DEG_TO_RAD(0),   COLOR_WATER, shader);
    draw_rect2((vec3){-250,  250, -20}, (vec2){500, 100}, (vec3){-1,0,0}, DEG_TO_RAD(-90), COLOR_WATER, shader);
    */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void sky_draw_frame(SHADER* shader, double t, void* data) {
    CAMERA skycam = camera;
    //camera_update_skybox(&skycam);
    //camera_apply(&skycam, shader->program);
    camera_apply(&camera, shader->program);
    
    //glCullFace(GL_FRONT);
    /*glDisable(GL_CULL_FACE);
    draw_rect2((vec3){-250,-250,50}, (vec3){500,500}, (vec3){0,0,1}, DEG_TO_RAD(0), COLOR_BLUE, shader);
    glEnable(GL_CULL_FACE);*/
    //glCullFace(GL_BACK);

    vec2 size = (vec2){1000, 900};

    vec3 cam_dir;
    camera_get_direction(&camera, (vec3){0,0,1}, cam_dir);
    shader_set_vec3(shader, "u_cam_dir", cam_dir);
    shader_set_vec3(shader, "u_cam_pos", camera.pos);

    draw_rect2((vec3){-500, 500, 50}, size, (vec3){0,1,0},  DEG_TO_RAD(0),   COLOR_BLUE, shader);
    draw_rect2((vec3){ 500, 500, 50}, size, (vec3){1,0,0},  DEG_TO_RAD(270), COLOR_BLUE, shader);
    draw_rect2((vec3){ 500,-500, 50}, size, (vec3){0,-1,0}, DEG_TO_RAD(180), COLOR_BLUE, shader);
    draw_rect2((vec3){-500,-500, 50}, size, (vec3){-1,0,0}, DEG_TO_RAD(90),  COLOR_BLUE, shader);
    glDisable(GL_CULL_FACE);
    draw_rect2((vec3){-500,-500, 500}, (vec2){1000, 1000}, (vec3){0,0,1}, 0,  COLOR_BLUE, shader);
    glEnable(GL_CULL_FACE);
}




void underwater_draw_frame(SHADER* shader, double t, void* data) {
    glClear(GL_DEPTH_BUFFER_BIT);
    camera_apply(&camera, shader->program);

    shader_set_float(shader, "u_swidth", (float)swidth);
    shader_set_float(shader, "u_sheight", (float)sheight);
    texture_bind(&ztexture, shader, "u_depth0", GL_TEXTURE0);
    
    shader_set_float(shader, "u_water_height", water.pos[2]);

    //camera_apply(&screencam, shader->program);
    camera_apply(&camera, shader->program);
    //draw_rect2((vec3){-5, -5, -1}, (vec2){10, 10}, NULL, 0, COLOR_WATER, shader);

    shader_set_vec4(shader, "u_fog_color", COLOR_WATER.raw);

    shader_set_int(shader, "u_mode", 0);
    glCullFace(GL_FRONT);
    terrain_draw(&water, t);
    glCullFace(GL_BACK);
    
    shader_set_int(shader, "u_mode", 1);
    terrain_draw(&terrain, t);

    shader_set_int(shader, "u_mode", 2);
    draw_rect2((vec3){250,   250, -20}, (vec2){500, 100}, (vec3){0,1,0},  DEG_TO_RAD(180), COLOR_WATER, shader);
    draw_rect2((vec3){250,  -250, -20}, (vec2){500, 100}, (vec3){1,0,0},  DEG_TO_RAD(90),  COLOR_WATER, shader);
    draw_rect2((vec3){-250, -250, -20}, (vec2){500, 100}, (vec3){0,-1,0}, DEG_TO_RAD(0),   COLOR_WATER, shader);
    draw_rect2((vec3){-250,  250, -20}, (vec2){500, 100}, (vec3){-1,0,0}, DEG_TO_RAD(-90), COLOR_WATER, shader);
}




void wire_draw_frame(SHADER* shader, double t, void* data) {
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    camera_apply(&camera, shader->program);

    shader_set_vec4(shader, "u_wire_color", COLOR_WHITE.raw);
    
    terrain_draw(&terrain, t);
    terrain_draw(&water, t);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void simple_draw_frame(SHADER* shader, double t, void* data) {
    camera_apply(&camera, shader->program);

    draw_rect2((vec3){250,  250,-20}, (vec2){500, 100}, (vec3){0,1,0},  DEG_TO_RAD(180), COLOR_BLACK, shader);
    draw_rect2((vec3){250, -250,-20}, (vec2){500, 100}, (vec3){1,0,0},  DEG_TO_RAD(90),  COLOR_BLACK, shader);
    draw_rect2((vec3){-250,-250,-20}, (vec2){500, 100}, (vec3){0,-1,0}, DEG_TO_RAD(0),   COLOR_BLACK, shader);
    draw_rect2((vec3){-250, 250,-20}, (vec2){500, 100}, (vec3){-1,0,0}, DEG_TO_RAD(-90), COLOR_BLACK, shader);
}
