#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../lib/include/zombie.h"

struct zombieImage
{
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
};

struct zombie
{
    float x, y, vx, vy;
    int window_width, window_height, renderAngle;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect rect;
};

static void getStartValues(Zombie *a);

ZombieImage *initiateZombie(SDL_Renderer *pRenderer)
{
    static ZombieImage *pZombieImage = NULL;
    if (pZombieImage == NULL)
    {
        pZombieImage = malloc(sizeof(struct zombieImage));
        SDL_Surface *surface = IMG_Load("resources/zombie.png");
        if (!surface)
        {
            printf("Error: %s\n", SDL_GetError());
            return NULL;
        }
        pZombieImage->pRenderer = pRenderer;
        pZombieImage->pTexture = SDL_CreateTextureFromSurface(pRenderer, surface);
        SDL_FreeSurface(surface);
        if (!pZombieImage->pTexture)
        {
            printf("Error: %s\n", SDL_GetError());
            return NULL;
        }
    }
    return pZombieImage;
}

Zombie *createZombie(ZombieImage *pZombieImage, int window_width, int window_height)
{
    Zombie *pZombie = malloc(sizeof(struct zombie));
    pZombie->pRenderer = pZombieImage->pRenderer;
    pZombie->pTexture = pZombieImage->pTexture;
    pZombie->window_width = window_width;
    pZombie->window_height = window_height;
    SDL_QueryTexture(pZombieImage->pTexture, NULL, NULL, &(pZombie->rect.w), &(pZombie->rect.h));
    pZombie->rect.w /= 4;
    pZombie->rect.h /= 4;
    getStartValues(pZombie);
    pZombie->renderAngle = 0;
    return pZombie;
}

static void getStartValues(Zombie *pZombie)
{

    int angle;
    int randomEdge = rand() % 4;
    pZombie->x = rand() % pZombie->window_width;
    pZombie->y = rand() % pZombie->window_height;
    if (randomEdge == 0)
    {
        pZombie->x = 0;
        angle = rand() % 90 - 45;
    }
    else if (randomEdge == 1)
    {
        pZombie->y = 0;
        angle = rand() % 90;
    }
    else if (randomEdge == 2)
    {
        pZombie->x = pZombie->window_width;
        angle = (rand() % 90) + 135;
    }
    else if (randomEdge == 3)
    {
        pZombie->y = pZombie->window_height;
        angle = (rand() % 90) + 90;
    }
    int v = rand() % 8 + 5;
    pZombie->vx = v * sin(angle * 0.75);
    pZombie->vy = v * cos(angle * 0.75);
    pZombie->rect.x = pZombie->x;
    pZombie->rect.y = pZombie->y;
}

void updateZombie(Zombie *pZombie)
{
    pZombie->x += pZombie->vx * 0.1;
    pZombie->y += pZombie->vy * 0.1;

    pZombie->rect.x = pZombie->x;
    pZombie->rect.y = pZombie->y;

    if (pZombie->rect.x < 0)
    {
        pZombie->rect.x -= (pZombie->x) * 3;
    }
    if (pZombie->rect.y < 0)
    {
        pZombie->rect.y -= (pZombie->y) * 3;
    }
    if (pZombie->rect.x > pZombie->window_width - pZombie->rect.w)
    {
        pZombie->rect.x = pZombie->window_width - pZombie->rect.w;
        pZombie->x = pZombie->rect.x;
    }
    if (pZombie->rect.y > (pZombie->window_height - 40) - pZombie->rect.h)
    {
        pZombie->rect.y = pZombie->window_height - 40 - pZombie->rect.h;
        pZombie->y = pZombie->rect.y;
    }
}

void drawZombie(Zombie *pZombie)
{
    SDL_RenderCopyEx(pZombie->pRenderer, pZombie->pTexture, NULL, &(pZombie->rect), pZombie->renderAngle, NULL, SDL_FLIP_NONE);
}

SDL_Rect getRectZombie(Zombie *pZombie)
{
    return pZombie->rect;
}

void destroyZombie(Zombie *pZombie)
{
    free(pZombie);
}
