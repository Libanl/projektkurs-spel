#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_mixer.h>
#include "../../lib/include/music.h"

Mix_Music *music = NULL;

int initMus()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer kunde inte initieras! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }
    return 0;
}

void playMus(const char *filename)
{
    music = Mix_LoadMUS(filename);
    Mix_PlayMusic(music, -1);
}

void stopMus()
{
    Mix_HaltMusic();
    Mix_FreeMusic(music);
}

void cleanMu()
{
    Mix_CloseAudio();
}
