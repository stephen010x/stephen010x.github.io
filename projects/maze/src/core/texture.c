#include <stdlib.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include "texture.h"
#include "../main.h"
#include "../helper.h"
#include "../objects/shapes.h"




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
        "Error: Failed to draw resource to hidden canvas\n");

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
void texture_init(TEXTURE* t, ASSET* asset, int mode, int flags) {
    t->asset = asset;
    //t->unit = tex_unit;
    t->mode = mode;
    t->flags = flags;

    // generate texture id
    glGenTextures(1, &t->id);

    // bind texture
    glBindTexture(GL_TEXTURE_2D, t->id);

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, asset->width, asset->height,
        0, asset->format, GL_UNSIGNED_BYTE, asset->data);

    // set texture parameters
    switch (mode) {
        case TEX_NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	        break;
        case TEX_LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        break;
        case TEX_MIPMAP:
            ASSERTVOID(0, "TEX_MIPMAP not implemented yet\n");
	}

	if (flags & TEX_WRAP_S)
	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	if (flags & TEX_WRAP_T)
	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


int texture_bind(TEXTURE* t, SHADER* shader, char* uvar, GLenum tex_unit) {
    glUseProgram(shader->program);

    // set active texture unit, and then bind texture to it
    glActiveTexture(tex_unit);
    glBindTexture(GL_TEXTURE_2D, t->id);
    
    GLint tex_u_loc = glGetUniformLocation(shader->program, uvar);

    if (tex_u_loc >= 0)
        glUniform1i(tex_u_loc, tex_unit-GL_TEXTURE0);

    return 0;
}



int texture_bind_scale(TEXTURE* t, SHADER* shader, char* uvar, GLenum tex_unit, char* svar, float scale) {
    texture_bind(t, shader, uvar, tex_unit);

    GLint scale_u_loc = glGetUniformLocation(shader->program, svar);

    if (scale_u_loc >= 0)
        glUniform1f(scale_u_loc, scale);

    return 0;
}
