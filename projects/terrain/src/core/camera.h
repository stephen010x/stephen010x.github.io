#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include "model.h"



#define CAM_SCALE_MODE CAM_SCALE_STATICHEIGHT

// this is mostly for orthographic
//#define ZOOM_SCALE 1
//#define ZOOM_SCALE 1.7


#define CAMERA_NONE        -1
#define CAMERA_NULL         0
#define CAMERA_ORTHOGRAPHIC 1
#define CAMERA_PERSPECTIVE  2

#define CAM_SCALE_DYNAMIC      0
#define CAM_SCALE_STATICWIDTH  1
#define CAM_SCALE_STATICHEIGHT 2


typedef struct _CAMERA {
    vec3 pos;
    // TODO: get rid of this in favor of separate function call
    vec3 rot; // rotated on x, y, z axes in that order around it's position, not origin
    float fov;
    //vec2 window[2];
    // TODO: just get rid of ratio and just have it be calculated every time it is needed
    float ratio;
    float zoom;
    int type;
    union {
        mat4 viewmat;
        float raw[16];
    };
    mat4 mod_viewmat;
    /*struct {
        int width;
        int height;
    } screen;*/
    int swidth, sheight;
    vec3 wmin, wmax;
} CAMERA;


extern CAMERA camera;







// TODO: Add a point light

typedef struct {
    vec3 norm;
    float amb, bright;
    COLOR diff_color, amb_color;
} LIGHTSOURCE_DIR;

typedef struct {
    vec3 pos;
    float amb, bright;
    COLOR diff_color, amb_color;
} LIGHTSOURCE_POINT;

/*typedef struct {
    vec3 pos;
    float amb, spec, bright;
} LIGHTSOURCE_SPEC;*/


typedef struct {
    int type;
    union {
        LIGHTSOURCE_DIR dir;
        LIGHTSOURCE_POINT point;
    };
} LIGHTSOURCE;




enum lightsource_type {
    LIGHTSOURCE_TYPE_NONE = 0,
    LIGHTSOURCE_TYPE_DIR,
    LIGHTSOURCE_TYPE_POINT,
    //LIGHTSOURCE_TYPE_AMBIENT,
    //LIGHTSOURCE_TYPE_SPECTRAL,
};




void camera_init(CAMERA* camera);
void camera_setup(CAMERA* c);
int camera_apply(CAMERA* c, unsigned int shader_program);

void camera_set(CAMERA* c, CAMERA* s);
void camera_setviewport(CAMERA* c, vec3 min, vec3 max);
void camera_interpolate(CAMERA* c, CAMERA* a, CAMERA* b, float n);

void camera_rotate_about(CAMERA* c, vec3 origin, vec3 rot, float radius);
// call this when you want to apply the parameters in the camera class
// to the camera viewport matrix
// TODO: add update modes (for instance, rotate about origin vs fps)
// TODO: replace zoom with z translation post-rotation
void camera_update(CAMERA* c);
// TODO: get rid of this duck-tape function
void camera_update_actual(CAMERA* c);
void camera_update_actual_flipx(CAMERA* c);
void camera_update_skybox(CAMERA* c);
//void camera_get_direction(CAMERA* c, vec3 forward, float (*cam_dir)[3]);
void camera_get_direction(CAMERA* c, vec3 forward, vec3 cam_dir);




int lightsource_apply(LIGHTSOURCE* light, unsigned int shader_program);
int lightsource_apply_spectral(LIGHTSOURCE* light, unsigned int shader_program, CAMERA* camera, float spec_bright, float spec_pow);
int lightsource_apply_spectral2(LIGHTSOURCE* light, unsigned int shader_program, CAMERA* camera, float spec_bright, float spec_pow);








__FORCE_INLINE__ COLOR color_add(COLOR c1, COLOR c2) {
    return (COLOR){
        .r = c1.r + c2.r,
        .g = c1.g + c2.g,
        .b = c1.b + c2.b,
        .a = c1.a + c2.a
    };
}


__FORCE_INLINE__ COLOR color_mul(COLOR c1, float n) {
    return (COLOR){
        .r = c1.r * n,
        .g = c1.g * n,
        .b = c1.b * n,
        .a = c1.a * n
    };
}


#define to_color(__color) ({                \
        typeof(__color) color = __color;    \
        (COLOR){                            \
            .r = color.r,                   \
            .g = color.g,                   \
            .b = color.b,                   \
            .a = color.a,                   \
        };                                  \
    })


#define to_icolor(__color) ({               \
        typeof(__color) color = __color;    \
        (ICOLOR){                           \
            .r = color.r,                   \
            .g = color.g,                   \
            .b = color.b,                   \
            .a = color.a,                   \
        };                                  \
    })






#endif
