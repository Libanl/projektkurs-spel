#ifndef powerup_H
#define powerup_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct powerup Powerup;

Powerup *createpowerup(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height);
void drawChest(Powerup *pPowerup);
void removeChest(Powerup *pPowerup);
SDL_Rect getRectchest(Powerup *pPowerup);
void newlocationchest(Powerup *pPowerup, int flag);
void destroyPowerup(Powerup *pPowerup);


#endif