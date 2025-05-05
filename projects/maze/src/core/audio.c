#include <emscripten.h>
#include <emscripten/html5.h>

#include "camera.h"


EM_JS(int, sound_play_unsafe, (const char* _sound_id), {
    const sound_id = UTF8ToString(_sound_id);
    var sound = document.getElementById(sound_id);
    if (!sound) return -1;
    
    sound.cloneNode(true).play();

    return 0;
})




EM_JS(int, sound_play_unsafe2, (const char* _sound_id, double volume, double speed, bool preserve_pitch), {
    const sound_id = UTF8ToString(_sound_id);
    var sound = document.getElementById(sound_id);
    if (!sound) return -1;
    sound = sound.cloneNode(true);

    sound.volume = volume;
    sound.playbackRate = speed;
    sound.preservesPitch = !!preserve_pitch;
    sound.play();

    return 0;
})




// TODO: add a way to create sound instance id's, return them when you play them
// and be able to use those to stop sounds


EM_JS(int, sound_play, (const char* _sound_id, bool loop), {
    const sound_id = UTF8ToString(_sound_id);
    var sound = document.getElementById(sound_id);
    if (!sound) return -1;

    // stops sound if playing
    //sound.load();

    var sound = document.getElementById(sound_id);
    sound.loop = !!loop;
    sound.play();

    return 0;
})





EM_JS(int, sound_play2, (const char* _sound_id, double volume, double speed, bool loop, bool preserve_pitch), {
    const sound_id = UTF8ToString(_sound_id);
    var sound = document.getElementById(sound_id);
    if (!sound) return -1;

    // stops sound if playing
    //sound.load();

    sound.volume = volume;
    sound.playbackRate = speed;
    sound.preservesPitch = !!preserve_pitch;
    sound.loop = !!loop;
    sound.play();

    return 0;
})




EM_JS(int, sound_stop, (const char* _sound_id), {
    const sound_id = UTF8ToString(_sound_id);
    var sound = document.getElementById(sound_id);
    if (!sound) return -1;

    sound.load();
    
    return 0;
})




EM_JS(int, sound_pause, (const char* _sound_id), {
    const sound_id = UTF8ToString(_sound_id);
    var sound = document.getElementById(sound_id);
    if (!sound) return -1;

    sound.pause();

    return 0;
})




EM_JS(bool, sound_isfinished, (const char* _sound_id), {
    const sound_id = UTF8ToString(_sound_id);
    var sound = document.getElementById(sound_id);
    if (!sound) return false;
    
    return sound.ended;
})




EM_JS(bool, sound_isplaying, (const char* _sound_id), {
    const sound_id = UTF8ToString(_sound_id);
    var sound = document.getElementById(sound_id);
    if (!sound) return false;
    
    return !sound.paused;
})
