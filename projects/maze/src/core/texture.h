#ifndef TEXTURE_H
#define TEXTURE_H

#include <stddef.h>
#include <stdbool.h>
#include <cglm/cglm.h>
#include <GLES2/gl2.h>

// for the shader stuff
// maybe I should replace this with extern struct _SHADER or something
// Yup. header inclusion issues occur
//#include "../objects/shapes.h"

// TODO: I should probably do this for all of my headers
// to avoid circular inclusion conflicts
// huh, I guess extern is not neccissary for structs that arent being defined.
// extern struct _SHADER;
struct _SHADER;
typedef struct _SHADER SHADER;



#define HIDDEN_CANVAS_ID "asset-loader"


// TODO: get rid of this
enum asset_type {
    ASSET_NULL = 0,
    ASSET_JPG,
    ASSET_PNG,
    ASSET_MP3,
    ASSET_RAW,
};


enum texture_mode {
    TEX_NULL = 0,  
    TEX_NEAREST,
    TEX_LINEAR,
    TEX_MIPMAP,
};


#define TEX_DEFAULT 0
#define TEX_WRAP_S (1<<0)
#define TEX_WRAP_T (1<<1)
#define TEX_WRAP   (TEX_WRAP_S | TEX_WRAP_T)


typedef struct _ASSET {
    int type;   // TODO: get rid of this
    int width;
    int height;
    //size_t size;
    int size;
    GLenum format;
    void* data;
} ASSET;



typedef struct _TEXTURE {
    GLuint id;
    //GLenum unit;
    int mode;
    int flags;
    ASSET* asset;
    //mat3 view; // unneccissary in light of how texture mapping works
} TEXTURE;



//  Texture Units: GL_TEXTUREi



// TODO: merge these all into one function, and just use the url string to determine it
int asset_load_jpg(ASSET* asset, const char* url);
int asset_load_png(ASSET* asset, const char* url);
int asset_load_mp3(ASSET* asset, const char* url);
int asset_load_raw(ASSET* asset, const char* url);

int asset_load_img(ASSET* asset, const char* id);
void asset_free(ASSET* asset);


// I should start making all class init helpers return void. It enforces cleaner code rather than
// inline hard-to-read garbage
void texture_init(TEXTURE* t, ASSET* asset, int mode, int flags);
int texture_bind(TEXTURE* t, SHADER* shader, char* uvar, GLenum tex_unit);
// used mostly for repeating textures that aren't mapped to vertices
int texture_bind_scale(TEXTURE* t, SHADER* shader, char* uvar, GLenum tex_unit, char* svar, float scale);


bool is_page_loaded(void);

#endif
