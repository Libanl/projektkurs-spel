#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../lib/include/powerup.h"


struct powerup
{
    int x,y;
    int window_width, window_height;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect chestRect;
    int angle;
    SDL_RendererFlip flip;
};


Powerup *createpowerup(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Powerup *pPowerup = malloc(sizeof(struct powerup));
    pPowerup->window_width = window_width;
    pPowerup->window_height = window_height;
    pPowerup->pRenderer = pRenderer;
    pPowerup->angle = 0;
    pPowerup->flip = SDL_FLIP_NONE;
    SDL_Surface *pSurface = IMG_Load("resources/chest2.png");
    if (!pSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pPowerup->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if (!pPowerup->pTexture)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(pSurface);
    SDL_QueryTexture(pPowerup->pTexture, NULL, NULL, &(pPowerup->chestRect.w), &(pPowerup->chestRect.h));
    pPowerup->chestRect.w=pPowerup->chestRect.w/12;
    pPowerup->chestRect.h=pPowerup->chestRect.h/12;
    pPowerup->chestRect.x=700;
    pPowerup->chestRect.y= 400;
    return pPowerup;
}

void drawChest(Powerup *pPowerup)
{
    SDL_RenderCopyEx(pPowerup->pRenderer, pPowerup->pTexture, NULL, &(pPowerup->chestRect), pPowerup->angle, NULL,pPowerup->flip);
}

void removeChest(Powerup *pPowerup)
{
    pPowerup->chestRect.x=2000;
}

void newlocationchest(Powerup *pPowerup, int flag)
{
    if(pPowerup->chestRect.x==2000){
        if(flag==0){
            pPowerup->chestRect.x=350;
            pPowerup->chestRect.y=250;
        }
        else if(flag==1){
            pPowerup->chestRect.x=500;
            pPowerup->chestRect.y=450;
        }
    }
}

void destroyPowerup(Powerup *pPowerup)
{
    SDL_DestroyTexture(pPowerup->pTexture);
    free(pPowerup);
}

SDL_Rect getRectchest(Powerup *pPowerup)
{
    return pPowerup->chestRect;
}

