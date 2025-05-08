#ifndef MAIN_H
#define MAIN_H

#include <cglm/cglm.h>


// I just don't like this, so I pushed it into here.
typedef struct {
    int width;
    int height;
} FRAME;





// TODO:
// Delete this header eventually. Main doesn't need a header.
// I just wanted to get these assert macros out of there to reduce
// clutter. Eventually move these to a macro header file



//#define ASSERT(__cond, __code, ...) _ASSERT(__cond, __code, __VA_ARGS__, "")

#if 0
#ifdef DEBUG_MODE
    #define ASSERT(__cond, __code, ...)     \
        do {                                \
            if (!(__cond)) {                \
                printf(__VA_ARGS__);        \
                IFTHENELSE(ISVOID(__code),  \
                    return; ,               \
                    return (__code);        \
                )                           \
            }                               \
        } while(0)
#else
    #define ASSERT(__cond, __code, ...)     \
        do {                                \
            if (!(__cond))                  \
                IFTHENELSE(ISVOID(__code),  \
                    return; ,               \
                    return (__code);        \
                )                           \
        } while(0)
#endif
#else
// use this until the macro library works
#ifdef DEBUG_MODE
    #define ASSERT(__cond, __code, ...) \
        do {                            \
            if (!(__cond)) {            \
                printf(__VA_ARGS__);    \
                return (__code);        \
            }                           \
        } while(0)
    #define ASSERTVOID(__cond, ...)     \
        do {                            \
            if (!(__cond)) {            \
                printf(__VA_ARGS__);    \
                return;                 \
            }                           \
        } while(0)
#else
    #define ASSERT(__cond, __code, ...) \
        do {                            \
            if (!(__cond))              \
                return (__code);        \
        } while(0)
    #define ASSERTVOID(__cond, ...)     \
        do {                            \
            if (!(__cond))              \
                return;                 \
        } while(0)
#endif
#endif


#define MAX(__a, __b) ({        \
        typeof(__a) a = (__a);  \
        typeof(__b) b = (__b);  \
        a > b ? a : b;          \
    })

#define MIN(__a, __b) ({        \
        typeof(__a) a = (__a);  \
        typeof(__b) b = (__b);  \
        a <= b ? a : b;         \
    })


//#define LENOF(__n) (sizeof(__n)/sizeof(*(__n)))
#define LENOF(__n) (sizeof(__n)/sizeof((__n)[0]))
#define MATH_PI 3.141592653589793238462643383
#define ABS(__n) ({typeof(__n) n = (__n); n >= 0 ? n : -n;})
#define SIGN(__n) ({typeof(__n) n = (__n); n == 0 ? 0 : (n > 0 ? 1 : -1);})

#define MAP(__n, __i1, __i2, __o1, __o2) ({ \
        typeof(__n)  n  = (__n );   \
        typeof(__i1) i1 = (__i1);   \
        typeof(__i2) i2 = (__i2);   \
        typeof(__o1) o1 = (__o1);   \
        typeof(__o2) o2 = (__o2);   \
        (n-i1)*(o2-o1)/(i2-i1)+o1;  \
    })



// TODO: Add a point light


typedef struct {
    vec3 norm;
    vec2 range;
} DIR_LIGHTSOURCE;

typedef struct {
    vec3 pos;
    vec2 range;
} POINT_LIGHTSOURCE;


typedef struct {
    int type;
    union {
        DIR_LIGHTSOURCE dir;
        POINT_LIGHTSOURCE point;
    };
} LIGHTSOURCE;


enum lightsource_type {
    LIGHTSOURCE_TYPE_NONE = 0,
    LIGHTSOURCE_TYPE_DIR,
    LIGHTSOURCE_TYPE_POINT,
    LIGHTSOURCE_TYPE_AMBIENT,
};


typedef struct {
    LIGHTSOURCE light;
} WORLD;

extern WORLD world;









// Actually, lets just push some more junk code I don't want cluttering the
// main file into here that I am not yet willing to delete



/*typedef struct {
    struct {
        int width;
        int height;
    } canvas;
    
} GL_INSTANCE;*/



/*
void init_scene(
    CIRCLE* circles, MESH* circle_mesh, GLuint shader_program , int screen_width, int screen_height) {

    for (int i = 0; i <= CIRC_RES; i++) {
        circle_mesh->verts[i+0] = 0;
        circle_mesh->verts[i+1] = 0;
        circle_mesh->verts[i+2] = (float)cos( (float)i * (2*MATH_PI/CIRC_RES) );
        circle_mesh->verts[i+3] = (float)sin( (float)i * (2*MATH_PI/CIRC_RES) );
    }
    
    for (int i = 0; i < NUM_CIRCLES; i++) {
        CIRCLE_init( CIRCLE_default(
            &circles[i],
            screen_width,
            screen_height,
            shader_program,
            circle_mesh
        ));
    }
}
*/



    // update the scene
    //for (int i = 0; i < NUM_CIRCLES; i++)
    //    CIRCLE_update(circles+i, (float)dt);

    //for (int i = 0; i < NUM_CIRCLES; i++)
    //    CIRCLE_draw(circles+i);

    //SIMPLET_draw(&simp);




/*int main() {
    printf("starting webgl\n");

    //\*debug*\/ printf("%p %lu %p %lu\n", &_circle_mesh, sizeof(_circle_mesh), &circle_mesh_struct._circle_mesh_verts, sizeof(circle_mesh_struct._circle_mesh_verts));

    /\*int retval;
    pthread_t thread;
    pthread_create(&thread, NULL, _main, NULL);
    //pthread_join(thread, (void**)&retval);
    return retval;*\/
    
    int retval = __main();
    printf("main exited with code (%d)\n", retval);

    // this is just a busyloop
    //emscripten_sleep(1000);

    // TODO: This function may work to replace emscripten FETCH
    // int emscripten_wget(const char *url, const char *file)
    
    return retval;
}*/




/*
void* _main(void* args) {
    printf("detatching main\n");
    int retval =__main();
    // TODO: this errors for some reason??? Fix this
    //printf("main exited with code (%d)\n", retval);
    return (void*)retval;
}
*/






#endif
