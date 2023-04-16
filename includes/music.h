#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>

extern Mix_Music* music;

int initMus();
void playMus();
void stopMus();
void cleanMu();

#endif