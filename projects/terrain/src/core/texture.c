#include <stdlib.h>
#include <string.h>

#include <emscripten.h>
#include <emscripten/html5.h>

#include "texture.h"
#include "../main.h"
#include "../helper.h"
#include "../objects/shapes.h"
#include "model.h"




/*int asset_load_jpg(ASSET* asset, const char* url) {
    ASSERT(emscripten_wget(url, "asset_buffer.jpg") == 0, -1, "failed to load asset\n");
    // since this returns a pointer that needs to be freed, I am assuming a copy is made
    // from the file, so I don't have to worry about copying the data myself to reuse the file buffer.
    asset->data = emscripten_get_preloaded_image_data("asset_buffer.jpg", &asset->width, &asset->height);
    printf("%p\n", asset->data);
    ASSERT(asset->data != NULL, -2, "failed to process image data\n");

    asset->type = ASSET_JPG;
    asset->size = asset->width * asset->height * sizeof(char);

    return 0;
}




int asset_load_png(ASSET* asset, const char* url) {
    ASSERT(emscripten_wget(url, "asset_buffer.png") == 0, -1, "failed to load asset\n");
    // since this returns a pointer that needs to be freed, I am assuming a copy is made
    // from the file, so I don't have to worry about copying the data myself to reuse the file buffer.
    asset->data = emscripten_get_preloaded_image_data("asset_buffer.png", &asset->width, &asset->height);
    ASSERT(asset->data != NULL, -2, "failed to load image data\n");

    asset->type = ASSET_PNG;
    asset->size = asset->width * asset->height * sizeof(char);

    return 0;
}




int asset_load_mp3(ASSET* asset, const char* url) {
    ASSERT(0, -1, "load_asset_mp3 is not implemented yet\n");
}




int asset_load_raw(ASSET* asset, const char* url) {
    int error;
    emscripten_wget_data(url, &asset->data, &asset->size, &error);
    ASSERT(error == 0, -1, "error loading data");

    asset->type = ASSET_RAW;

    return 0;
}*/




EM_JS(bool, is_page_loaded, (void), {
    return document.readyState === "complete";
})




EM_JS(int, load_image_to_canvas, (const char* _canvas_id, const char* _image_id), {
    const canvas_id = UTF8ToString(_canvas_id);
    const image_id = UTF8ToString(_image_id);
    
    //console.log(canvas_id, image_id);
    const canvas = document.getElementById(canvas_id);
    if (!canvas) return -1;
    const context = canvas.getContext("2d");
    if (!context) return -2;
    const image = document.getElementById(image_id);
    if (!image) return -3;

    canvas.width = image.width;
    canvas.height = image.height;
    context.drawImage(image, 0, 0);

    return 0;
})



EM_JS(int, load_canvas_to_buffer, (char* buffer, const char* _canvas_id, int width, int height), {
    const canvas_id = UTF8ToString(_canvas_id);
    const context = document.getElementById(canvas_id).getContext("2d");
    if (!context) return -1;
    
    const image_data = context.getImageData(0, 0, width, height);
    HEAPU8.set(new Uint8Array(image_data.data.buffer), buffer);
})



// god kill me. This was a nightmare
int asset_load_img(ASSET* asset, const char* id) {
    // wait for all page resources to load
    while (!is_page_loaded())
        emscripten_sleep(50);

    int error;

    // draw image to hidden canvas
    ASSERT(!load_image_to_canvas(HIDDEN_CANVAS_ID, id), -1,
        "Error: Failed to draw resource \"%s\" to hidden canvas\n", id);

    // get canvas width and height
    int width, height, size;
    //emscripten_get_canvas_element_size("#" HIDDEN_CANVAS_ID, &width, &height);
    get_elementid_size(HIDDEN_CANVAS_ID, &width, &height);

    size = width * height * 4;

    // allocate buffer for image data
    asset->data = malloc(size); // uses RGBA I guess.

    // Alright, it turns out trying to create a new context was a mistake. 
    // We might as well use the current one, and load the image via javascript

    error = load_canvas_to_buffer(asset->data, HIDDEN_CANVAS_ID, width, height);

    if (error) {
        free(asset->data);
        asset->data = NULL;
        ASSERT(0, -2, "Error: Failed to load resource from hidden canvas");
    }

    asset->width = width;
    asset->height = height;
    asset->size = size;
    asset->format = GL_RGBA;

    return 0;
}




void free_asset(ASSET* asset) {
    free(asset->data);
    *asset = (ASSET){0};
}







// Note, this function would become unsustainable if enough textures are used
// that vram is filled up.
// In which case it would merit some sort of texture memory management.
void texture_init(TEXTURE* t, ASSET* asset, /*int mode,*/ int flags) {
    t->asset = asset;
    //t->unit = tex_unit;
    //t->mode = mode;
    t->flags = flags;

    // generate texture id
    glGenTextures(1, &t->id);

    // bind texture
    glBindTexture(GL_TEXTURE_2D, t->id);


    /*t->wrap_s = !!(flags & TEX_WRAP_S);
    t->wrap_t = !!(flags & TEX_WRAP_T);
    t->use_mipmap = !!(flags & TEX_USEMIPMAP);

    t->mode = (flags & TEX_INTER_MASK) >> TEX_INTER_SHIFT;*/
    

    // Set texture image data
    // TODO: the asset->format is unsustainable with a static internalformat parameter of GL_RGBA
    // it is fine to pass NULL data to glTexImage2D, but it creates a warning as it is
    // "lazy initilization", so I decided to make it more explicit
    /*if (asset->data == NULL)
        // will allocate storage without uploading texture data
        glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA, asset->width, asset->height);
    else
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, asset->width, asset->height,
    	    0, asset->format, GL_UNSIGNED_BYTE, asset->data);*/

    // nevermind, it looks like I really don't have that function right now
    // also, apparently webgl "warnings" are just errors in disguise, so I can't ignore it either.
    // TODO: the asset->format is unsustainable with a static internalformat parameter of GL_RGBA
    /*void* data = (asset->data != NULL) ? (asset->data) : (malloc(asset->size));
    printf("%p\n", data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, asset->width, asset->height,
        0, asset->format, GL_UNSIGNED_BYTE, data);
    printf("bongus\n");

    // Say that is a point. I could potentially just upload the image data to the 
    // gpu, and then free it in ram.
    if (asset->data != data)
        free(data);*/

    /*printf("%p, %d, %d\n", asset->data, asset->width, asset->height);
    /\*DEBUG*\/ if (asset->data == NULL)
        asset->data = calloc(asset->width * asset->height, 4);*/
    switch (flags & TEX_DEPTH_MASK) {
        case 0:
            glTexImage2D(GL_TEXTURE_2D, 0, 1 ? GL_RGBA : GL_COMPRESSED_RGBA, asset->width, asset->height,
                0, asset->format, GL_UNSIGNED_BYTE, asset->data);
            break;

        case TEX_DEPTH16:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, asset->width, asset->height,
                0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
            break;
            
        case TEX_DEPTH32:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, asset->width, asset->height,
                0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
            break;

        case TEX_MONO_FLOAT:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F /*GL_COMPRESSED_RED*/, asset->width, asset->height,
                0, GL_RED, GL_FLOAT, NULL);
            break;
    }

    // set texture parameters
    switch (flags & TEX_INTER_MASK) {
        case TEX_NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	        break;
        case TEX_LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        break;
	    case TEX_NEAREST | TEX_MIPMAP_NEAREST:
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	        break;
	    case TEX_LINEAR  | TEX_MIPMAP_NEAREST:
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        break;
	    case TEX_NEAREST | TEX_MIPMAP_LINEAR:
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	        break;
	    case TEX_LINEAR  | TEX_MIPMAP_LINEAR:
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        break;
	}

	if (flags & TEX_WRAP_S)
	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	if (flags & TEX_WRAP_T)
	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


int texture_bind(TEXTURE* t, SHADER* shader, char* uvar, GLenum tex_unit) {
    // TODO: I suppose this shouldn't be needed. The shader should already be bound
    // before this is called
    glUseProgram(shader->program);

    // set active texture unit, and then bind texture to it
    glActiveTexture(tex_unit);
    glBindTexture(GL_TEXTURE_2D, t->id);
    
    GLint tex_u_loc = glGetUniformLocation(shader->program, uvar);

    if (tex_u_loc >= 0)
        glUniform1i(tex_u_loc, tex_unit-GL_TEXTURE0);

    return 0;
}



int texture_bind_scale(TEXTURE* t, SHADER* shader, char* uvar, GLenum tex_unit, char* svar, float scale, char* tvar, float strength) {
    texture_bind(t, shader, uvar, tex_unit);

    GLint scale_u_loc = glGetUniformLocation(shader->program, svar);
    GLint stren_u_loc = glGetUniformLocation(shader->program, tvar);

    if (scale_u_loc >= 0 && svar != NULL)
        glUniform1f(scale_u_loc, scale);

    if (stren_u_loc >= 0 && tvar != NULL)
        glUniform1f(stren_u_loc, strength);

    return 0;
}


int texture_gen_mipmaps(TEXTURE* t, int mipmode, int depth) {
    // bind texture
    glBindTexture(GL_TEXTURE_2D, t->id);

    int width = t->asset->width;
    int height = t->asset->height;

    if (depth == TEX_GEN_MAX) {
        /*int n = MIN(width, height);
        int i = 0;
        while (!(n >> i++));
        depth = i;*/
        depth = 1000;
    }
    
    ICOLOR (*buffdata)[width] = malloc(width * height * 4);
    ICOLOR (*assdata)[width] = t->asset->data;

    // copy image to new buffer
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            buffdata[y][x] = assdata[y][x];

    //int div = 1;
    for (int i = 1; i <= depth; i++) {
        //div <<= 1;
        width >>= 1;
        height >>= 1;

        ICOLOR (*lastdata)[width<<1] = (void*)buffdata;
        ICOLOR (*nextdata)[width] = (void*)buffdata;

        if (height <= 0 || width <= 0)
            break;

        if (mipmode == MIPMAP_GEN_MULTISAMPLE || (mipmode >= MIPMAP_GEN_MIX_1_3 && mipmode <= MIPMAP_GEN_MIX_3_1))
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++) {
                    COLOR sum = {0};
                    for (char k = 0; k < 4; k++) {
                        sum.r += (float)lastdata[y*2+((k>>1)&0b1)][x*2+(k&0b1)].r;
                        sum.g += (float)lastdata[y*2+((k>>1)&0b1)][x*2+(k&0b1)].g;
                        sum.b += (float)lastdata[y*2+((k>>1)&0b1)][x*2+(k&0b1)].b;
                        sum.a += (float)lastdata[y*2+((k>>1)&0b1)][x*2+(k&0b1)].a;
                    }
                    nextdata[y][x] = (ICOLOR){
                        .r = (unsigned char)(sum.r/4.0),
                        .g = (unsigned char)(sum.g/4.0),
                        .b = (unsigned char)(sum.b/4.0),
                        .a = (unsigned char)(sum.a/4.0),
                    };
                    switch (mipmode) {
                        case MIPMAP_GEN_MIX_1_3:
                            nextdata[y][x] = to_icolor(color_mul(color_add(
                                                    to_color(lastdata[y*2][x*2]), 
                                                    color_mul(to_color(nextdata[y][x]), 3)), 0.25));
                            break;
                        case MIPMAP_GEN_MIX_2_2:
                            nextdata[y][x] = to_icolor(color_mul(color_add(
                                                    to_color(lastdata[y*2][x*2]), 
                                                    to_color(nextdata[y][x])),0.5));
                            break;
                        case MIPMAP_GEN_MIX_3_1:
                            nextdata[y][x] = to_icolor(color_mul(color_add(
                                                    color_mul(to_color(lastdata[y*2][x*2]), 3),
                                                    to_color(nextdata[y][x])),0.25));
                            break;
                    }
                }
        else if (mipmode == MIPMAP_GEN_NEAREST)
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++)
                    nextdata[y][x] = lastdata[y*2][x*2];

        //printf("w:%d h:%d i:%d\n", width, height, i);

        glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height,
            0, t->asset->format, GL_UNSIGNED_BYTE, (void*)buffdata);
    }

    // lets assume opengl determines this automatically
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, depth);

    free(buffdata);

    return 0;
}









int asset_pack_load(ASSET_PACK* pack, char* (*id)[ASSET_TYPE_COUNT], unsigned int flags) {
    int reterr = 0;
    for (int i = 0; i < ASSET_TYPE_COUNT; i++)
        if ((flags & (1<<i)) && (id[0][i][0] != '\0')) {
            int err = asset_load_img(&pack->index[i], id[0][i]);
            if (err) {
                reterr = err;
                continue;
            }
            pack->flags |= 1<<i;
        }
    return reterr;
}





void texture_pack_init(TEXTURE_PACK* tp, ASSET_PACK* assets, int flags, float scale, float strength) {
    tp->assets = assets;
    tp->scale = scale;
    tp->strength = strength;
    for (int i = 0; i < ASSET_TYPE_COUNT; i++)
        if (assets->flags & (1<<i))
            texture_init(&tp->index[i], &assets->index[i], flags);
}





int texture_pack_gen_mipmaps(TEXTURE_PACK* tp, int mipmode, int depth) {
    int reterr = 0;
    for (int i = 0; i < ASSET_TYPE_COUNT; i++)
        if (tp->assets->flags & (1<<i)) {
            int err = texture_gen_mipmaps(&tp->index[i], mipmode, depth);
            if (err) reterr = err;
        }
    return reterr;
}





__FORCE_INLINE__ void concat_string(char* buff, char* str1, char* str2) {
    strcpy(buff, str1);
    strcat(buff, str2);
}





static char* svar_postfix[] = ASSET_POSTFIXES;


int texture_pack_bind(TEXTURE_PACK* tp, SHADER* shader, char* svar_prefix, GLenum tex_unit_start) {
    char svar[64];

    ASSERT(strlen(svar_prefix) < 48, -1, "ERROR: texture_pack_bind svar_prefix exceeds 48 characters\n");

    //snprintf(svar, 64, "%s%s", svar_prefix, "_scale");
    concat_string(svar, svar_prefix, "_scale");
    shader_set_float(shader, svar, tp->scale);

    concat_string(svar, svar_prefix, "_strength");
    shader_set_float(shader, svar, tp->strength);
    

    for (int i = 0; i < ASSET_TYPE_COUNT; i++)
        if (tp->assets->flags & (1<<i)) {
            // set active texture unit, and then bind texture to it
            glActiveTexture(tex_unit_start+i);
            glBindTexture(GL_TEXTURE_2D, tp->index[i].id);

            concat_string(svar, svar_prefix, svar_postfix[i]);
            shader_set_int(shader, svar, tex_unit_start+i-GL_TEXTURE0);
        }

    return 0;
}
