#ifndef AUDIO_H
#define AUDIO_H


#include <stdbool.h>


int sound_play_unsafe(const char* _sound_id);
int sound_play(const char* _sound_id, bool loop);
int sound_stop(const char* _sound_id);
int sound_pause(const char* _sound_id);
bool sound_isfinished(const char* _sound_id);
bool sound_isplaying(const char* _sound_id);


int sound_play_unsafe2(const char* _sound_id, double volume, double speed, bool preserve_pitch);
int sound_play2(const char* _sound_id, double volume, double speed, bool loop, bool preserve_pitch);


#endif
