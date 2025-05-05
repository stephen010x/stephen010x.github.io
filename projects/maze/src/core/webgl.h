#ifndef WEBGL_H
#define WEBGL_H

#include <stdbool.h>
#include <GLES2/gl2.h>



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


#endif
