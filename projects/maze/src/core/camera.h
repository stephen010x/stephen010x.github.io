#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>



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

#endif
