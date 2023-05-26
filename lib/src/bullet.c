#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../lib/include/spelare_data.h"
#include "../../lib/include/spelare.h"
#include "../../lib/include/bullet.h"

struct bullet
{
    float x, y, vx, vy;
    int time, window_width, window_height;
    SDL_Texture *pTexture;
    SDL_Texture *rightTexture;
    SDL_Texture *downTexture;
    SDL_Texture *leftTexture;
    SDL_Texture *UpTexture;
    SDL_Rect bulletRect;
};

Bullet *createBullet(SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Bullet *pBullet = malloc(sizeof(struct bullet));
    pBullet->window_width = window_width;
    pBullet->window_height = window_height;
    pBullet->time = 0;
    // pBullet->bulletRect = {pBullet->x, pBullet->y, 16, 16};

    SDL_Surface *rightSurface = IMG_Load("resources/bullet.png");
    SDL_Surface *downSurface = IMG_Load("resources/bulletDown.png");
    SDL_Surface *leftSurface = IMG_Load("resources/bulletleft.png");
    SDL_Surface *UpSurface = IMG_Load("resources/bulletUp.png");
    if (!rightSurface || !downSurface || !leftSurface || !UpSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pBullet->rightTexture = SDL_CreateTextureFromSurface(pRenderer, rightSurface);
    pBullet->UpTexture = SDL_CreateTextureFromSurface(pRenderer, UpSurface);
    pBullet->downTexture = SDL_CreateTextureFromSurface(pRenderer, downSurface);
    pBullet->leftTexture = SDL_CreateTextureFromSurface(pRenderer, leftSurface);
    SDL_FreeSurface(UpSurface);
    SDL_FreeSurface(downSurface);
    SDL_FreeSurface(leftSurface);
    SDL_FreeSurface(rightSurface);

    pBullet->bulletRect.x = pBullet->x;
    pBullet->bulletRect.y = pBullet->y;
    pBullet->bulletRect.w = 16;
    pBullet->bulletRect.h = 16;

    return pBullet;
}

void updateBullet(Bullet *pBullet)
{
    if (pBullet->time == 0)
        return;
    pBullet->x += pBullet->vx;
    pBullet->y += pBullet->vy;
    if (pBullet->x < 0 || pBullet->x > pBullet->window_width || pBullet->y < 0 || pBullet->y > pBullet->window_height)
        killBullet(pBullet);
    (pBullet->time)--;
    return;
}

void startBullet(Bullet *pBullet, float x, float y, int moveUp, int moveLeft, int moveDown, int moveRight)
{
    pBullet->x = x;
    pBullet->y = y;
    pBullet->time = BULLETLIFETIME;

    // Set horizontal velocity
    if (moveLeft == 1)
    {
        pBullet->vx = -BULLETVELOCITY;
        pBullet->pTexture = pBullet->leftTexture;
    }
    else if (moveRight == 1)
    {
        pBullet->vx = BULLETVELOCITY;
        pBullet->pTexture = pBullet->rightTexture;
    }
    else
    {
        pBullet->vx = 0;
    }

    // Set vertical velocity
    if (moveUp == 1)
    {
        pBullet->vy = -BULLETVELOCITY;
        pBullet->pTexture = pBullet->UpTexture;
    }
    else if (moveDown == 1)
    {
        pBullet->vy = BULLETVELOCITY;
        pBullet->pTexture = pBullet->downTexture;
    }
    else
    {
        pBullet->vy = 0;
    }
}

void killBullet(Bullet *pBullet)
{
    pBullet->time = 0;
}

void drawBullet(Bullet *pBullet, SDL_Renderer *pRenderer)
{
    if (pBullet->time == 0)
        return;
    SDL_Rect bulletRect = {pBullet->x, pBullet->y, 16, 16};
    pBullet->bulletRect = bulletRect;
    SDL_RenderCopy(pRenderer, pBullet->pTexture, NULL, &pBullet->bulletRect);
}

float xBullet(Bullet *pBullet)
{
    return pBullet->x;
}

float yBullet(Bullet *pBullet)
{
    return pBullet->y;
}

void destroyBullet(Bullet *pBullet)
{
    SDL_DestroyTexture(pBullet->rightTexture);
    SDL_DestroyTexture(pBullet->UpTexture);
    SDL_DestroyTexture(pBullet->downTexture);
    SDL_DestroyTexture(pBullet->leftTexture);
    free(pBullet);
}

int aliveBullet(Bullet *pBullet)
{
    return pBullet->time > 0;
}

SDL_Rect getRectBullet(Bullet *pBullet)
{
    return pBullet->bulletRect;
}

/*int collideBullet(Bullet *pBullet, SDL_Rect rect)
{
    return distance(pBullet->bulletRect.x + pBullet->bulletRect.w / 2, pBullet->bulletRect.y + pBullet->bulletRect.h / 2, rect.x + rect.w / 2, rect.y + rect.h / 2) < (pBullet->bulletRect.w + rect.w) / 2;
}*/
