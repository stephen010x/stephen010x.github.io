#include <emscripten.h>
#include <emscripten/html5.h>

#include <stdio.h>

#include "input.h"



bool key_event_handler(int etype, const EmscriptenKeyboardEvent* event, void* params);
bool mouse_event_handler(int etype, const EmscriptenMouseEvent* event, void* params);
bool touch_event_handler(int etype, const EmscriptenTouchEvent* event, void* params);




char key[256];



void input_init(void) {
    emscripten_set_keydown_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &key_event_handler);
    emscripten_set_keyup_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &key_event_handler);

    // for the mobiley gifted
    /*emscripten_set_touchstart_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &touch_event_handler);*/

    emscripten_set_mousedown_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &mouse_event_handler);
    emscripten_set_mouseup_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &mouse_event_handler);
    emscripten_set_mousemove_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &mouse_event_handler);
    /*emscripten_set_mouseenter_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &mouse_event_handler);
    emscripten_set_mouseleave_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &mouse_event_handler);*/

    emscripten_set_touchstart_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &touch_event_handler);
    emscripten_set_touchend_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &touch_event_handler);
    emscripten_set_touchmove_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, &touch_event_handler);

    // for mobile users. Not that mobile actually listens to it.
    emscripten_lock_orientation(EMSCRIPTEN_ORIENTATION_PORTRAIT_PRIMARY);
}



// these do nothing yet
void input_push_callback(EVENT_CALLBACK callback) {
    return;
}

void input_pop_callback(EVENT_CALLBACK callback) {
    return;
}






bool key_event_handler(int etype, const EmscriptenKeyboardEvent* event, void* params) {
    switch (etype) {
        case EMSCRIPTEN_EVENT_KEYDOWN:
            //printf("keydown %d\n", event->keyCode);
            key[event->keyCode] = KEYDOWN;
            return false;
            //return true;                    // return that it was handled
        case EMSCRIPTEN_EVENT_KEYUP:
            //printf("keyup %d\n", event->keyCode);
            key[event->keyCode] = KEYUP;
            return false;
            //return true;                    // return that it was handled
    }
    return false; // return that it wasn't handled
}


bool touch_event_handler(int etype, const EmscriptenTouchEvent* event, void* params) {
    static int px, py;
    const EmscriptenTouchPoint* point = event->touches+0;

    //emscripten_run_script("alert('hello');");

    //mouse.dx += 1000;
    
    switch (etype) {
        case EMSCRIPTEN_EVENT_TOUCHSTART:
            px = point->clientX;
            py = point->clientY;
            //EM_ASM({}, px, py);
            mouse.button[0 /*event->numTouches*/] = MOUSEDOWN;
            /*EM_ASM({
                let v1 = $0;
                let v2 = $1;
                alert(v1.toString() + ":" + v2.toString());
            }, event->numTouches, 0);*/
            return true;
        case EMSCRIPTEN_EVENT_TOUCHEND:
            mouse.button[0 /*event->numTouches*/] = MOUSEUP;
            return true;
        case EMSCRIPTEN_EVENT_TOUCHMOVE:
            mouse.x = point->clientX;
            mouse.y = point->clientY;
            mouse.dx += point->clientX - px;
            mouse.dy += point->clientY - py;
            /*EM_ASM({
                let v1 = $0;
                let v2 = $1;
                alert(v1.toString() + ":" + v2.toString());
            }, mouse.dx, mouse.dy);*/
            px = point->clientX;
            py = point->clientY;
            return true;
    }
    return false;
}


#include "../main.h"

bool mouse_event_handler(int etype, const EmscriptenMouseEvent *event, void *params) {
    //printf("%d, %d\n", etype, EMSCRIPTEN_EVENT_MOUSEENTER);

    // these disable the dx dy accumulate, as they seem to misbehave
    // with frame slowdowns
    //mouse.dx = 0;
    //mouse.dy = 0;

    EmscriptenPointerlockChangeEvent pointerlock;
    emscripten_get_pointerlock_status(&pointerlock);
    
    switch (etype) {
        case EMSCRIPTEN_EVENT_MOUSEMOVE:
            mouse.x = event->canvasX;
            mouse.y = event->canvasY;
            // ideally this should be set every frame too. But what can you do?
            mouse.dx += event->movementX; // gets reset every frame
            mouse.dy += event->movementY; // gets reset every frame
            return true;
        case EMSCRIPTEN_EVENT_MOUSEDOWN:
            mouse.first_interaction = true;
            mouse.button[event->button] = MOUSEDOWN;
            if (mouse.grabby && !pointerlock.isActive)
                emscripten_request_pointerlock("#" HTML_CANVAS_ID, EM_FALSE);
            return true;
        case EMSCRIPTEN_EVENT_MOUSEUP:
            mouse.button[event->button] = MOUSEUP;
            return true;
    }
    return false;
}




void input_refresh(void) {
    mouse.dx = 0;
    mouse.dy = 0;

    EmscriptenPointerlockChangeEvent pointerlock;
    emscripten_get_pointerlock_status(&pointerlock);

    if (pointerlock.isActive) {
        mouse.grabbed = true;
        if (!mouse.grabby)
            emscripten_exit_pointerlock();
    } else
        mouse.grabbed = false;
}




// TODO: do something with these

/*




#define MOTION_DIV 10


bool motion_event_handler(int etype, const EmscriptenDeviceMotionEvent* event, void* params) {
    return false;
    float x = event->accelerationIncludingGravityX / MOTION_DIV;
    float y = event->accelerationIncludingGravityY / MOTION_DIV;
    //float z = event->accelerationIncludingGravityZ / MOTION_DIV;

    if (x && y)
        disable_rotgrav = true;

    printf("motion %f. %f\n", x, y);

    glm_vec3_copy((vec3){x,y}, gravity);
    return false;
}

bool orient_event_handler(int etype, const EmscriptenDeviceOrientationEvent* event, void* params) {

    float zrot = (event->alpha - 180) * ((float)MATH_PI/180);
    float xrot = (event->beta  - 0  ) * ((float)MATH_PI/180);
    float yrot = (event->gamma - 0  ) * ((float)MATH_PI/180);

    if (zrot && xrot && yrot)
        disable_rotgrav = true;

    glm_vec3_copy((vec3)GRAVITY, gravity);
    glm_vec3_scale(gravity, (float)GRAV_MUL, gravity);
    // after testing on a temo site, I have confirmed they are in this order
    glm_vec3_rotate(gravity, -zrot, (vec3){0,0,1});
    glm_vec3_rotate(gravity, -xrot, (vec3){1,0,0});
    glm_vec3_rotate(gravity, -yrot, (vec3){0,1,0});

    // remove z part of gravity.
    //gravity[2] = 0;

    /\*EM_ASM_({
	        alert("Orien: " + $0 + " " + $1+ " " + $2);
	}, (int)xrot, (int)yrot, (int)zrot);*\/
    
    return true;
}*/
