#ifndef MAIN_H
#define MAIN_H

#include <cglm/cglm.h>
// TODO: does this need to be here? Can I just move it to main.c?
#include "core/camera.h"


// I just don't like this, so I pushed it into here.
/*typedef struct {
    int width;
    int height;
} FRAME;
*/



#define HTML_CANVAS_ID "canvas"



#define FRAND() ((float)rand()/(float)RAND_MAX)
#define FRANDRANGE(__min, __max) MAP(FRAND(), 0, 1, __min, __max)
// non-inclusive
// TODO: don't directly use __min and __max. What if they were functions?
#define RANDRANGE(__min, __max) ( (rand() % ((__max)-(__min))) + (__min) )



// TODO: don't directly use __min and __max. What if they were functions?
#define VEC3_RANDRANGE(__min, __max) (vec3){    \
        FRANDRANGE(__min, __max),               \
        FRANDRANGE(__min, __max),               \
        FRANDRANGE(__min, __max),               \
    }
    




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


/*#define MAX(__a, __b) ({        \
        typeof(__a) a = (__a);  \
        typeof(__b) b = (__b);  \
        a > b ? a : b;          \
    })

#define MIN(__a, __b) ({        \
        typeof(__a) a = (__a);  \
        typeof(__b) b = (__b);  \
        a <= b ? a : b;         \
    })*/


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

#define CLAMP(__n, __min, __max) ({             \
        register typeof(__n) n = (__n);         \
        register typeof(__min) nmin = (__min);  \
        register typeof(__max) nmax = (__max);  \
        n = (n > nmax) ? (nmax) : (n);          \
        (n < nmin) ? (nmin) : (n);              \
    })

#define MIN(__a, __b) ({                \
        register typeof(__a) a = (__a); \
        register typeof(__b) b = (__b); \
        (a < b) ? (a) : (b);            \
    })

#define MAX(__a, __b) ({                \
        register typeof(__a) a = (__a); \
        register typeof(__b) b = (__b); \
        (a >= b) ? (a) : (b);           \
    })




typedef struct {
    LIGHTSOURCE light;
} WORLD;

extern WORLD world;






enum {
    ERROR_NONE = 0,
    ERROR_CREATE_CONTEXT,
};




//typedef unsigned char bool;
#define TRUE 1
#define FALSE 0


#endif








/*EM_JS(float, calcFPS, (int count){
    var opts = {};
    
    var requestFrame = window.requestAnimationFrame ||
        window.webkitRequestAnimationFrame ||
        window.mozRequestAnimationFrame;
    if (!requestFrame) return true; // Check if "true" is returned; 
                                    // pick default FPS, show error, etc...
    function checker(){
        if (index--) requestFrame(checker);
        else {
            // var result = 3*Math.round(count*1000/3/(performance.now()-start));
            var result = count*1000/(performance.now()- start);
            if (typeof opts.callback === "function") opts.callback(result);
            //console.log("Calculated: "+result+" frames per second");
            return result
        }
    }
    //if (!opts) opts = {};
    //var count = opts.count||60, index = count, start = performance.now();
    var index = count, start = performance.now();
    checker();
}*/

//static float refresh_rate;
