#ifndef WEBGL_H
#define WEBGL_H

#include <GLES2/gl2.h>


int init_webgl(const char* canvas_id);
GLuint shader_program(const char* progname, const char* vertname, const char* vertsrc, const char* fragname, const char* fragsrc);


#endif
