#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/spelare.h"

struct spelare
{
    float x, y, vx, vy;
    int window_width, window_height;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect shipRect;
};

Spelare *createSpelare(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Spelare *pSpelare = malloc(sizeof(struct spelare));
    pSpelare->vx = pSpelare->vy = 0;
    pSpelare->window_width = window_width;
    pSpelare->window_height = window_height;
    SDL_Surface *pSurface = IMG_Load("resources/soldier.png");
    if (!pSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pSpelare->pRenderer = pRenderer;
    pSpelare->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pSpelare->pTexture)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_QueryTexture(pSpelare->pTexture, NULL, NULL, &(pSpelare->shipRect.w), &(pSpelare->shipRect.h));
    pSpelare->shipRect.w = 16;
    pSpelare->shipRect.h = 16;
    pSpelare->x = x - pSpelare->shipRect.w / 2;
    pSpelare->y = y - pSpelare->shipRect.h / 2;
    return pSpelare;
}

void drawSpelare(Spelare *pSpelare)
{
    SDL_RenderCopyEx(pSpelare->pRenderer, pSpelare->pTexture, NULL, &(pSpelare->shipRect), 1, NULL, SDL_FLIP_NONE);
}

void destroySpelare(Spelare *pSpelare)
{
    SDL_DestroyTexture(pSpelare->pTexture);
    free(pSpelare);
}