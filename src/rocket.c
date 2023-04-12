#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "../include/rocket.h"

struct rocket
{
    float x, y, vx, vy;
    int angle;
    int window_width, window_height;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect shipRect;
};

float distance(int x1, int y1, int x2, int y2);

Rocket *createRocket(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Rocket *pRocket = malloc(sizeof(struct rocket));
    pRocket->vx = pRocket->vy = 0;
    pRocket->angle = 0;
    pRocket->window_width = window_width;
    pRocket->window_height = window_height;
    SDL_Surface *pSurface = IMG_Load("resources/solider.png");
    if (!pSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pRocket->pRenderer = pRenderer;
    pRocket->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pRocket->pTexture)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_QueryTexture(pRocket->pTexture, NULL, NULL, &(pRocket->shipRect.w), &(pRocket->shipRect.h));
    pRocket->shipRect.w /= 4;
    pRocket->shipRect.h /= 4;
    pRocket->x = x - pRocket->shipRect.w / 2;
    pRocket->y = y - pRocket->shipRect.h / 2;
    return pRocket;
}

int collideRocket(Rocket *pRocket, SDL_Rect rect)
{
    // return SDL_HasIntersection(&(r->shipRect),&rect);
    return distance(pRocket->shipRect.x + pRocket->shipRect.w / 2, pRocket->shipRect.y + pRocket->shipRect.h / 2, rect.x + rect.w / 2, rect.y + rect.h / 2) < (pRocket->shipRect.w + rect.w) / 2;
}

float distance(int x1, int y1, int x2, int y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void turnLeft(Rocket *pRocket)
{
    pRocket->angle -= 5;
}

void turnRight(Rocket *pRocket)
{
    pRocket->angle += 5;
}

void accelerate(Rocket *pRocket)
{
    pRocket->vx += 0.1 * sin(pRocket->angle * 2 * M_PI / 360);
    pRocket->vy -= 0.1 * cos(pRocket->angle * 2 * M_PI / 360);
}

void updateRocket(Rocket *pRocket)
{
    pRocket->x += pRocket->vx;
    pRocket->y += pRocket->vy;
    if (pRocket->x < 0)
        pRocket->x += pRocket->window_width;
    else if (pRocket->x > pRocket->window_width)
        pRocket->x -= pRocket->window_width;
    if (pRocket->y < 0)
        pRocket->y += pRocket->window_height;
    else if (pRocket->y > pRocket->window_height)
        pRocket->y -= pRocket->window_height;
    pRocket->shipRect.x = pRocket->x;
    pRocket->shipRect.y = pRocket->y;
}

void drawRocket(Rocket *pRocket)
{
    SDL_RenderCopyEx(pRocket->pRenderer, pRocket->pTexture, NULL, &(pRocket->shipRect), pRocket->angle, NULL, SDL_FLIP_NONE);
}

void destroyRocket(Rocket *pRocket)
{
    SDL_DestroyTexture(pRocket->pTexture);
    free(pRocket);
}

void resetRocket(Rocket *pRocket)
{
    pRocket->x = pRocket->window_width / 2;
    pRocket->y = pRocket->window_height / 2;
    pRocket->angle = 0;
    pRocket->vx = pRocket->vy = 0;
}
