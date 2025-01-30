#include "webgl.h"
#include "main.h"

#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>



//int fetch_file(const char* url, char* data, uint64_t max, uint64_t* bytes);
//static GLuint compile_shader(GLenum shader_type, const char* url);
static GLuint compile_shader(GLenum shader_type, const char* name, const char* src);
static int link_program(GLuint program, const char* name);



GLuint shader_program(const char* progname, const char* vertname, const char* vertsrc, const char* fragname, const char* fragsrc) {
    // compile shaders
    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertname, vertsrc);
    ASSERT(vs, (GLuint)0, "vertex shader failed to compile\n");
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragname, fragsrc);
    ASSERT(fs, (GLuint)0, "fragment shader failed to compilen\n");

    // create program object
    GLuint program = glCreateProgram();

    // attach shaders to program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //TODO: Add glValidateProgram

    // link the program
    ASSERT(!link_program(program, progname), (GLuint)0, "");

    // install program as part of current rendering state
    glUseProgram(program);

    return program;
}





int init_webgl(const char* canvas_id) {
    //static const char const* vs_path = "./src/shaders/default.vert";
    //static const char const* fs_path = "./src/shaders/default.frag"

    // webgl attribute structure
    EmscriptenWebGLContextAttributes gl_attrib;

    // populate webgl attributes with default and custom values
    emscripten_webgl_init_context_attributes(&gl_attrib);
    gl_attrib.alpha = EM_FALSE;
    gl_attrib.antialias = EM_FALSE;
    gl_attrib.powerPreference = EM_WEBGL_POWER_PREFERENCE_LOW_POWER;
    gl_attrib.majorVersion = 2; // might be less compatible with browsers
    gl_attrib.enableExtensionsByDefault = EM_TRUE; // consider setting to false
    gl_attrib.explicitSwapControl = EM_TRUE;
    gl_attrib.renderViaOffscreenBackBuffer = EM_TRUE;

    // create webgl context to canvas element using id
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE gl_context = emscripten_webgl_create_context(
        canvas_id, &gl_attrib);
    ASSERT(gl_context, -1, "Error: Failed to create webgl context\n");

    // activate webgl context
    ASSERT(emscripten_webgl_make_context_current(gl_context) == 
        EMSCRIPTEN_RESULT_SUCCESS, -1,
        "Error: Failed to make webgl context current\n");

    return 0;
}



/*typedef struct {
    const char* url;
    char* data;
    uint64_t max;
    uint64_t* bytes;
} fetch_file_args;

void* fetch_sync(void* _args);
*/
/*
// this function is synchronous. It will block until it gets the data
// the bytes parameter is both input and output. As input it represents
// the max bytes, and as output it will be how many bytes were read.
// nevermind, an explicit extra member will be added for max
int fetch_file(const char* url, char* data, uint64_t max, uint64_t* bytes) {
    fetch_file_args args = {
        .url = url,
        .data = data,
        .max = max,
        .bytes = bytes
    };
    int retval;
    
    pthread_t thread;
    pthread_create(&thread, NULL, fetch_sync, &args);
    pthread_join(thread, (void**)&retval);

    return retval;
}


void* fetch_sync(void* _args) {
    fetch_file_args* args = (fetch_file_args*)_args;
    const char* url = args->url;
    char* data = args->data;
    uint64_t max = args->max;
    uint64_t* bytes = args->bytes;

    // fetch attributes
    emscripten_fetch_attr_t fetch_attrib;

    // populate fetch attributes with default and custom values
    emscripten_fetch_attr_init(&fetch_attrib);
    fetch_attrib.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY | 
                              EMSCRIPTEN_FETCH_SYNCHRONOUS; // |
                              //EMSCRIPTEN_FETCH_REPLACE;
    strcpy(fetch_attrib.requestMethod, "GET");

    // fetch file (blocking)
    emscripten_fetch_t *fetch = emscripten_fetch(&fetch_attrib, url);
    // the documentation on this is, like, nonexistant. So I have no idea why 
    // the success fetch status is 200. Maybe it is an http code.
    
    if (fetch->status == 200) {
        printf("Fetched \"%s\" (%llu)\n", fetch->url, fetch->numBytes);
        // *test* / printf("%s\n", (char*)fetch->data);
    } else {
        printf("Fetch \"%s\" FAILED! (HTTP status code: %d)\n", 
            fetch->url, fetch->status);
        // *test* / printf("%s\n", (char*)fetch->data);
        emscripten_fetch_close(fetch);
        return (void*)-1;
    }

    // copy data to data buffer
    memcpy(data, fetch->data, (uint32_t)MIN(max, fetch->numBytes));

    // set output of total length of file
    if (bytes != NULL)
        *bytes = fetch->numBytes;

    // free fetch object
    emscripten_fetch_close(fetch);

    return (void*)0;
}*/


//static GLuint compile_shader(GLenum shader_type, const char* url) {
static GLuint compile_shader(GLenum shader_type, const char* name, const char* src) {
    // load shader
    // make sure everything is zeroed out so that the
    // string terminator is there by default
    /*char src[1024] = {0};
    uint64_t len = sizeof(src);
    ASSERT(fetch_file(url, src, len-1, &len) == 0, 0, "fetch_file failed\n");
    ASSERT(len < sizeof(src), 0, 
        "ERROR: Loaded shader file exceeded buffer length \"%s\"\n", url);*/

    // create shader object (only returns handle)
    GLuint shader = glCreateShader(shader_type);

    //const char* _src = src;

    // set the shader source to a single shader that is null terminated
    // I am not sure what is going on with the const typecast warning here
    // I assumed that the const param only affects the callee
    // this is confirmed working by using glShaderGetSource
    // what the heck???
    // For some reason this function spectacularly fails when I try to 
    // pass our src as the pointer to a static array casted to a a const GLchar**
    // and yet using a pointer that points to our static array works.
    // turns out static arrays when referencing will just decay into a pointer
    // without referencing
    glShaderSource(shader, 1, (const GLchar**)&src, NULL);

    printf("Compiling shader \"%s\"\n", name);

    // compile the shader
    glCompileShader(shader);
    //GLenum err = glGetError();

    // print out compiler errors if any
    {
        GLint err, len;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (err == GL_FALSE) {
            char log[len];
            glGetShaderInfoLog(shader, len, NULL, log);
            //printf("SHADER COMPILER ERROR\n--------------------\n");
            printf("SHADER COMPILER ERROR:\n");
            printf(log);
            return 0;
        }
    }

    // returns shader handler
    return shader;
}


static int link_program(GLuint program, const char* name) {
    printf("Linking program \"%s\"\n", name);

    // link program
    glLinkProgram(program);

    // print out linker errors if any
    GLint err, len;
    glGetProgramiv(program, GL_LINK_STATUS, &err);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (err == GL_FALSE) {
        char log[len];
        glGetProgramInfoLog(program, len, NULL, log);
        printf(log);
        return -1;
    }

    return 0;
}

