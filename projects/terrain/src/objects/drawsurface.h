#ifndef DRAWSURFACE_H
#define DRAWSURFACE_H

//TODO: consider renaming this to "canvas.h" or something similar that is shorter. "viewport?"

#include <GLES2/gl2.h>
#include <cglm/cglm.h>


/*// This means it will support at least 10 viewports
// but this also means 70-79 is reserved
#define DRAWSURFACE_TEX_UNIT_MIN GL_TEXTURE70
#define DRAWSURFACE_TEX_UNIT_MAX GL_TEXTURE79*/



// since I actually use the structs, and not pointers to the structs,
// I will have to include them
/*struct _ASSET;
struct _TEXTURE;
//struct _SHADER;
//struct _CAMERA;
typedef struct _ASSET   ASSET;
typedef struct _TEXTURE TEXTURE;
//typedef struct _SHADER  SHADER;
//typedef struct _CAMERA  CAMERA;*/

#include "../core/texture.h"




struct _DRAWSURFACE;
// TODO: I probably don't need to pass the surface object to a virtual rendering scene. 
// so consider getting rid of the surface param and just have the user pass the surface through the
// data param if they really need it
typedef void (*DRAWSURFACE_CALLBACK)(struct _DRAWSURFACE* surface, void* data, double t, float dt);

// I am really split on whether or not a drawsurface should have it's own camera.
// I suppose it is more versatile this way.
typedef struct _DRAWSURFACE {
    ASSET asset;
    TEXTURE texture;
    //SHADER shader;
    //CAMERA camera;
    DRAWSURFACE_CALLBACK callback;
    void* data;
    GLuint framebuffer;
    GLuint depthbuffer;
    //bool preserve; // turns out I just misunderstood how framebuffers work
    /*vec3 pos;
    vec3 norm;
    float rot;*/
} DRAWSURFACE;



int drawsurface_init(DRAWSURFACE* s, int width, int height, int tex_mode, DRAWSURFACE_CALLBACK call);
//void drawsurface_free(DRAWSURFACE* s);
// offloaded to optional external shape call
//void drawsurface_set(DRAWSURFACE* s, vec3 pos, vec3 norm);
void drawsurface_draw(DRAWSURFACE* s, double t, float dt);


#endif
