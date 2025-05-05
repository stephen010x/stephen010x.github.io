#include <emscripten.h>
#include <emscripten/html5.h>

#include "helper.h"


// TODO: use this to set canvas size instead of javascript
//emscripten_set_canvas_element_size("#canvas", 256, 256);


void get_elementid_size(char* id, int* width, int* height) {
    *width = EM_ASM_INT({
        let idstr = UTF8ToString($0);
        let element = document.getElementById(idstr);
        return element.width;
    }, id);
    *height = EM_ASM_INT({
        let idstr = UTF8ToString($0);
        let element = document.getElementById(idstr);
        return element.height;
    }, id);
}
