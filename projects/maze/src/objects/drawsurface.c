#include <stdlib.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include "drawsurface.h"
#include "../core/texture.h"
#include "../core/camera.h"
#include "../objects/shapes.h"
#include "../main.h"
#include "../helper.h"



//void drawsurface_setup(SHADER* shader, DRAWSURFACE* data);






/*static SHADER drawsurface_shader;


static const char drawsurface_vert_shader[] = 
    "precision mediump float;"
    "attribute vec3 vert_pos; uniform mat4 u_mod_mat; varying vec2 tex_coord;"
    "void main() { gl_Position = u_mod_mat * vec4(vert_pos, 1.0); tex_coord = vert_tex; }"

static const char drawsurface_frag_shader[] = 
    "precision mediump float;"
    "uniform sampler2D tex0; varying vec2 tex_coord;"
    "void main() { gl_FragColor = texture2D(tex0, tex_coord); }"



void drawsurface_firstcall(void) {
    GLuint program = shader_program(
        "drawsurface_program",
        "drawsurface_vert",
        drawsurface_vert_shader,
        "drawsurface_frag", 
        drawsurface_frag_shader
    );
    shader_init(&drawsurface_shader, program, NULL, NULL);
}*/




int drawsurface_init(DRAWSURFACE* s, int width, int height, int tex_mode, DRAWSURFACE_CALLBACK call) {
    /*static int id = 0;
    if (id > DRAWSURFACE_TEX_UNIT_MAX-DRAWSURFACE_TEX_UNIT_MIN)
        return -1;*/

    /*static bool firstcall = true;
    if (firstcall) {
        drawsurface_firstcall()
        firstcall = false;
    }*/
    
    *s = (DRAWSURFACE){
        .callback = call,
        .asset = (ASSET){
            .width = width,
            .height = height,
            .size = width*height*4,
            .data = NULL, //calloc(res[0]*res[1], 4),
            .format = GL_RGBA,
        },
        /*.camera = (CAMERA){
            .zoom = 1,
            .type = CAMERA_ORTHOGRAPHIC,
        },*/
        //.preserve = true,
        .data = NULL,       // this might be redundant now
    };

    //CAMERA*  camera  = &s->camera;
    TEXTURE* texture = &s->texture;
    ASSET*   asset   = &s->asset;
    
    // init camera
    /*camera_init(camera);
    float ratio = (float)res[1]/res[0];
    // can be changed outside of class. this is just default
    camera->wmin[0] = -1*ratio;
    camera->wmax[0] =  1*ratio;
    camera->wmin[1] = -1;
    camera->wmax[1] =  1;*/


    // generate framebuffer
    glGenFramebuffers(1, &s->framebuffer);
    //printf("%d\n", s->framebuffer);
    // this is for my own sanity. Remove this later when it is determined to not be an issue
    ASSERT(s->framebuffer != 0, -2, "ERROR: Oh God it happened!\n");

    // TODO: add code to clear framebuffer
    
    
    // init texture
    glBindFramebuffer(GL_FRAMEBUFFER, s->framebuffer);
    // TODO: see if there is a way to generate texture ids rather than just 
    // haphazerdly assign them like this
    texture_init(texture, asset, tex_mode, TEX_DEFAULT);
    // bind texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);

    // whoops. Forgot to bind z-buffer
    glGenRenderbuffers(1, &s->depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, s->depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, s->depthbuffer);

    // not sure why the earlier one wasn't working, so I am just going to bind a texture for depth
    /*glGenTextures(1, &s->depthbuffer);
    glBindTexture(GL_TEXTURE_2D, s->depthbuffer);
    printf("%d, %d\n", width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, s->depthbuffer, 0);*/

    // restore framebuffer binding to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // assert that framebuffer is complete I guess? (consider removing this)
    // looks like webgl already does this for me
    /*ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, -3, 
        "ERROR: framebuffer is not complete for some reason?\n");*/

    return 0;
}



void drawsurface_free(DRAWSURFACE* s) {
    asset_free(&s->asset);
    // TODO: implement a texture and shader free
    // to free certain gpu resources
    // turns out this is a opengl 3+ feature.
    // nope, nevermind. Turns out this is apart of the webgl 1.0 specifications
    glDeleteFramebuffers(1, &s->framebuffer);
}



/*void drawsurface_set(DRAWSURFACE* s, vec3 pos, vec3 norm, float rot) {
    glm_vec3_copy(pos, s->pos);
    glm_vec3_copy(norm, s->norm);
}*/




//extern int swidth, sheight;

void drawsurface_draw(DRAWSURFACE* s, double t, float dt) {

    // save last frame buffer and viewport
    GLint last_frame_buffer;
    GLint last_viewport[4];
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_frame_buffer);
    glGetIntegerv(GL_VIEWPORT, last_viewport);

    // switch render context to draw to framebuffer for texture
    glBindFramebuffer(GL_FRAMEBUFFER, s->framebuffer);
    glViewport(0, 0, s->asset.width, s->asset.height);

    // Redraw the surface framebuffer if preserve enabled
    // for now no depth buffer is saved, so all draws will always overwrite
    // whatever is in the framebuffer
    // The default coordinates are -1 to 1, which should be mapped to the viewport
    // so with no camera matrix and with the right stretching, I can guarentee
    // the viewport will be filled with the texture
    // nevermind. The texture is not a framebuffer. I can have multiple framebuffers
    /*if (s->preserve)
        draw_texture_plane((vec3){-1,-1,0}, (vec2){2,2}, NULL, 0, 
            &s->texture, &drawsurface_shader);*/

    if (s->callback)
        s->callback(s, s->data, t, dt);

    //camera_apply(&s->camera, s->shader.program);

    // restore framebuffer and viewport previous
    //int width, height;
    //emscripten_get_canvas_element_size(/\*HTML_CANVAS_ID*\/ "canvas", &width, &height);
    //get_elementid_size(HTML_CANVAS_ID, &width, &height);
    glBindFramebuffer(GL_FRAMEBUFFER, last_frame_buffer);
	glViewport(last_viewport[0], last_viewport[1], last_viewport[2], last_viewport[3]);

	// I was going to just have this draw a rectangle. But then I realized 
	// it would be better to just pass this surfaces texture to an externally called
	// function like draw_rec2, or optionally other things for custom geometry
}





/*
    How exactly do I implement multiple shaders for this?
    Perhaps I could just do it all in the callback. Basically treat it as it's own
    little draw frame loop. I would just need to make sure shader properties
    are preserved or at least guarenteed to be properly overwritten after the fact.
*/
