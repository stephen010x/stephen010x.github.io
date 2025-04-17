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


enum mipmap_gen_modes {
    MIPMAP_GEN_NULL = 0,
    MIPMAP_GEN_NEAREST,
    MIPMAP_GEN_MULTISAMPLE,
    MIPMAP_GEN_MIX_1_3,
    MIPMAP_GEN_MIX_2_2,
    MIPMAP_GEN_MIX_3_1,
};


/*enum texture_mode {
    TEX_NULL = 0,
    
    TEX_NEAREST = 1<<0,
    TEX_LINEAR  = 2<<0,
    
    TEX_MIPMAP  = 1<<4,
};*/


#define TEX_DEFAULT 0

#define TEX_WRAP_S (1<<0)
#define TEX_WRAP_T (1<<1)
#define TEX_WRAP   (TEX_WRAP_S | TEX_WRAP_T)

//#define TEX_INTER_SHIFT 2
#define TEX_INTER_MASK (0b111<<2)
#define TEX_NEAREST (0<<2)
#define TEX_LINEAR  (1<<2)
#define TEX_MIPMAP_NONE    (0<<3)
#define TEX_MIPMAP_NEAREST (1<<3)
#define TEX_MIPMAP_LINEAR  (2<<3)

//#define TEX_USEMIPMAP (1<<5)

#define TEX_DEPTH_MASK (0b11<<5)
#define TEX_DEPTH16 (1<<5)
#define TEX_DEPTH32 (2<<5)
#define TEX_MONO_FLOAT (3<<5)


#define TEX_GEN_MAX (-1)


#define TEX_SINGLE_ASSET (0<<7)
#define TEX_ASSET_PACK   (1<<7)




typedef struct _ASSET {
    //int type;   // TODO: get rid of this
    int width;
    int height;
    //size_t size;
    int size;
    GLenum format;
    void* data;
} ASSET;




// NOTE: These need to be in-line with their indexes as they appear in the ASSET_PACK struct
#define ASSET_TEX  (1<<0)
#define ASSET_NORM (1<<1)
#define ASSET_DISP (1<<2)
#define ASSET_OCCL (1<<3)

#define ASSET_ALL (ASSET_TEX | ASSET_NORM | ASSET_DISP | ASSET_OCCL)

#define ASSET_TYPE_COUNT 4


#define ASSET_POSTFIXES {   \
    "_tex",                 \
    "_norm",                \
    "_disp",                \
    "_occl",                \
}


#define GEN_ASSET_NAMES(__name, __size) (ASSET_GEN_TYPE){   \
    __name "-tex-"  #__size,        \
    __name "-norm-" #__size,        \
    __name "-disp-" #__size,        \
    __name "-occl-" #__size,        \
}



typedef struct _ASSET_PACK {
    unsigned int flags;
    union {
        ASSET index[ASSET_TYPE_COUNT];
        struct {
            ASSET tex;
            ASSET norm;
            ASSET disp;
            ASSET occl;
        };
    };
} ASSET_PACK;




typedef struct _TEXTURE {
    GLuint id;
    //GLenum unit;
    int mode;
    int flags;
    union {
        ASSET* asset;
        ASSET_PACK* assets;
    };
    //mat3 view; // unneccissary in light of how texture mapping works

    /*bool wrap_s;
    bool wrap_t;
    bool use_mipmap;*/
    
} TEXTURE;




typedef struct _TEXTURE_PACK {
    float scale;
    float strength;
    ASSET_PACK* assets;
    union {
        TEXTURE index[ASSET_TYPE_COUNT];
        struct {
            TEXTURE tex;
            TEXTURE norm;
            TEXTURE disp;
            TEXTURE occl;
        };
    };
} TEXTURE_PACK;




typedef char* ASSET_GEN_TYPE[ASSET_TYPE_COUNT];



//  Texture Units: GL_TEXTUREi



// TODO: merge these all into one function, and just use the url string to determine it
/*int asset_load_jpg(ASSET* asset, const char* url);
int asset_load_png(ASSET* asset, const char* url);
int asset_load_mp3(ASSET* asset, const char* url);
int asset_load_raw(ASSET* asset, const char* url);*/

int asset_load_img(ASSET* asset, const char* id);
void asset_free(ASSET* asset);


// I should start making all class init helpers return void. It enforces cleaner code rather than
// inline hard-to-read garbage
void texture_init(TEXTURE* t, ASSET* asset, int flags);
int texture_bind(TEXTURE* t, SHADER* shader, char* uvar, GLenum tex_unit);
// used mostly for repeating textures that aren't mapped to vertices
int texture_bind_scale(TEXTURE* t, SHADER* shader, char* uvar, GLenum tex_unit, char* svar, float scale, char* tvar, float strength);
int texture_gen_mipmaps(TEXTURE* t, int mipmode, int depth);


bool is_page_loaded(void);




int asset_pack_load(ASSET_PACK* pack, char* (*id)[ASSET_TYPE_COUNT], unsigned int flags);

void texture_pack_init(TEXTURE_PACK* tp, ASSET_PACK* assets, int flags, float scale, float strength);
int texture_pack_gen_mipmaps(TEXTURE_PACK* tp, int mipmode, int depth);
// note, all variables passed to shader are prefixed with tvar_prefix
// postfixes include: tex, norm, disp, occl, strength, scale,
// will take up 4 tex units past tex_unit_start
int texture_pack_bind(TEXTURE_PACK* tp, SHADER* shader, char* svar_prefix, GLenum tex_unit_start);


#endif
