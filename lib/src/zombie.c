#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../lib/include/spelare_data.h"
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

Zombie *createZombieleft(ZombieImage *pZombieImage, int window_width, int window_height)
{
    Zombie *pZombie = malloc(sizeof(struct zombie));
    pZombie->pRenderer = pZombieImage->pRenderer;
    pZombie->pTexture = pZombieImage->pTexture;
    pZombie->window_width = window_width;
    pZombie->window_height = window_height;
    SDL_QueryTexture(pZombieImage->pTexture, NULL, NULL, &(pZombie->rect.w), &(pZombie->rect.h));
    pZombie->rect.w /= 4;
    pZombie->rect.h /= 4;
    pZombie->x = 0;
    pZombie->y= pZombie->window_height /2;
    pZombie->renderAngle = 0;
    int v = 5;
    int angle=-35;
    pZombie->vx = v * sin(angle * 0.75);
    pZombie->vy = v * cos(angle * 0.75);
    pZombie->rect.x = pZombie->x;
    pZombie->rect.y = pZombie->y;
    return pZombie;
}

Zombie *createZombierup(ZombieImage *pZombieImage, int window_width, int window_height)
{
    Zombie *pZombie = malloc(sizeof(struct zombie));
    pZombie->pRenderer = pZombieImage->pRenderer;
    pZombie->pTexture = pZombieImage->pTexture;
    pZombie->window_width = window_width;
    pZombie->window_height = window_height;
    SDL_QueryTexture(pZombieImage->pTexture, NULL, NULL, &(pZombie->rect.w), &(pZombie->rect.h));
    pZombie->rect.w /= 4;
    pZombie->rect.h /= 4;
    pZombie->x = pZombie->window_width / 2;
    pZombie->y= 0;
    pZombie->renderAngle = 0;
    int v = 5;
    int angle =180;
    pZombie->vx = v * sin(angle * 0.75);
    pZombie->vy = v * cos(angle * 0.75);
    pZombie->rect.x = pZombie->x;
    pZombie->rect.y = pZombie->y;
    return pZombie;
}

Zombie *createZombierright(ZombieImage *pZombieImage, int window_width, int window_height)
{
    Zombie *pZombie = malloc(sizeof(struct zombie));
    pZombie->pRenderer = pZombieImage->pRenderer;
    pZombie->pTexture = pZombieImage->pTexture;
    pZombie->window_width = window_width;
    pZombie->window_height = window_height;
    SDL_QueryTexture(pZombieImage->pTexture, NULL, NULL, &(pZombie->rect.w), &(pZombie->rect.h));
    pZombie->rect.w /= 4;
    pZombie->rect.h /= 4;
    pZombie->x = pZombie->window_width;
    pZombie->y= pZombie->window_height / 2;
    pZombie->renderAngle = 0;
    int v = 5;
    int angle =-45;
    pZombie->vx = v * sin(angle * 0.75);
    pZombie->vy = v * cos(angle * 0.75);
    pZombie->rect.x = pZombie->x;
    pZombie->rect.y = pZombie->y;
    return pZombie;
}


Zombie *createZombiedown(ZombieImage *pZombieImage, int window_width, int window_height)
{
    Zombie *pZombie = malloc(sizeof(struct zombie));
    pZombie->pRenderer = pZombieImage->pRenderer;
    pZombie->pTexture = pZombieImage->pTexture;
    pZombie->window_width = window_width;
    pZombie->window_height = window_height;
    SDL_QueryTexture(pZombieImage->pTexture, NULL, NULL, &(pZombie->rect.w), &(pZombie->rect.h));
    pZombie->rect.w /= 4;
    pZombie->rect.h /= 4;
    pZombie->x = pZombie->window_width /2;
    pZombie->y = pZombie->window_height;
    pZombie->renderAngle = 0;
    int v = 5;
    int angle=180;
    pZombie->vx = v * sin(angle * 0.75);
    pZombie->vy = v * cos(angle * 0.75);
    pZombie->rect.x = pZombie->x;
    pZombie->rect.y = pZombie->y;
    return pZombie;
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

void destroyZombieImage(ZombieImage *pZombieImage)
{
    SDL_DestroyTexture(pZombieImage->pTexture);
}