#ifndef WEBGL_H
#define WEBGL_H

#include <stdbool.h>
#include <stdio.h>
#include <GLES2/gl2.h>
#include <emscripten/html5.h>


extern int GL_R32F;
extern int GL_RED;
extern int GL_COMPRESSED_RGBA;
extern int GL_COMPRESSED_RED;

typedef struct {
    bool active;
    unsigned int* program;
    char* program_name;
    char* vert_name;
    char* vert_src;
    char* frag_name;
    char* frag_src;
} SHADER_DESCRIPTOR;


#define SHADER_DESC_GEN(__active, __variable, __vert, __frag)   \
    (SHADER_DESCRIPTOR){                                \
        .active       = __active,                       \
        .program      = (__variable),                   \
        .program_name = #__vert "_" #__frag "_program", \
        .vert_name    = #__vert ".vert",                \
        .vert_src     = (char*)__vert ## _vert,         \
        .frag_name    = #__frag ".frag",                \
        .frag_src     = (char*)__frag ## _frag,         \
}


int init_webgl(const char* canvas_id);
GLuint shader_program(const char* progname, const char* vertname, const char* vertsrc, const char* fragname, const char* fragsrc);


void compile_shaders(int len, SHADER_DESCRIPTOR* descs);


// TODO: create a more robust api for this
#define update_overlay_text(...) do {                       \
    char buffer[256];                                       \
    snprintf(buffer, 256, __VA_ARGS__);                     \
    EM_ASM({                                                \
        let string = UTF8ToString($0);                      \
        let overlay = document.getElementById("overlay");   \
        overlay.textContent = string;                       \
    }, buffer);                                                     \
} while(0)


#endif
