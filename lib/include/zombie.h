#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct zombieImage ZombieImage;
typedef struct zombie Zombie;

ZombieImage *initiateZombie(SDL_Renderer *pRenderer);
Zombie *createZombie(ZombieImage *pZombieImage, int window_width, int window_height);
void updateZombie(Zombie *pZombie);
void drawZombie(Zombie *pZombie);
SDL_Rect getRectZombie(Zombie *pZombie);
void destroyZombie(Zombie *pZombie);
void destroyZombieImage(ZombieImage *pZombieImage);
void getZombieSendData(Zombie *pZombie, ZombieData *pZombieData);
void updateZombiesWithRecievedData(Zombie *pZombie, ZombieData *pZombieData);




#endif
