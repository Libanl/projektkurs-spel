#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct zombieImage ZombieImage;
typedef struct zombie Zombie;

void spawn_zombies(SDL_Rect *zombieRects, int numZombies, SDL_Surface *zombieImage, SDL_Texture *zombieTexture, int screenWidth, int screenHeight);
// void move_zombies(SDL_Rect *zombieRects, int numZombies, int movementSpeed, int screenWidth);
void render_zombies(SDL_Renderer *renderer, SDL_Texture *zombieTexture, SDL_Rect *zombieRects, int numZombies);
// void moveZombiesRandomly(SDL_Rect* zombieRect, int numZombies, int screenWidth, int screenHeight);
void updateZombies(SDL_Rect *zombieRect, int size);

#endif
