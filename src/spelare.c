#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/spelare.h"
#include "../includes/bullet.h"

struct spelare
{
    float x, y, vx, vy;
    int angle, alive;
    int window_width, window_height;
    Bullet *pBullet;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect shipRect;
};

Spelare *createSpelare(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Spelare *pSpelare = malloc(sizeof(struct spelare));
    pSpelare->vx = pSpelare->vy = 0;
    pSpelare->angle = 0;
    pSpelare->window_width = window_width;
    pSpelare->window_height = window_height;
    pSpelare->pBullet = createBullet(pRenderer, window_width, window_height);
    SDL_Surface *pSurface = IMG_Load("resources/soldier2.png");
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
    pSpelare->shipRect.w /= 10;
    pSpelare->shipRect.h /= 10;
    pSpelare->x = x - pSpelare->shipRect.w / 2;
    pSpelare->y = y - pSpelare->shipRect.h / 2;
    return pSpelare;
}

void moveLeft(Spelare *pSpelare)
{
    if (pSpelare->shipRect.x > 0)
    {
        pSpelare->x -= 5;
    }
}

void moveRight(Spelare *pSpelare)
{
    if (pSpelare->shipRect.x < 970)
    {
        pSpelare->x += 5;
    }
}

void moveUp(Spelare *pSpelare)
{
    if (pSpelare->shipRect.y > 0)
    {
        pSpelare->y -= 5;
    }
}

void moveDown(Spelare *pSpelare)
{
    if (pSpelare->shipRect.y < 500)
    {
        pSpelare->y += 5;
    }
}

void fireSpelare(Spelare *pSpelare, int m1, int m2, int m3, int m4)
{
    if (!pSpelare->alive || aliveBullet(pSpelare->pBullet))
        return;
    int moveUp = m1;
    int moveLeft = m2;
    int moveDown = m3;
    int moveRight = m4;
    startBullet(pSpelare->pBullet, pSpelare->x + pSpelare->shipRect.w / 2, pSpelare->y + pSpelare->shipRect.h / 2, moveUp, moveLeft, moveDown, moveRight);
}

void updateSpelare(Spelare *pSpelare)
{
    pSpelare->shipRect.x = pSpelare->x;
    pSpelare->shipRect.y = pSpelare->y;
    updateBullet(pSpelare->pBullet);
}

void drawSpelare(Spelare *pSpelare)
{
    if (aliveBullet(pSpelare->pBullet))
        drawBullet(pSpelare->pBullet, pSpelare->pRenderer);
    SDL_RenderCopyEx(pSpelare->pRenderer, pSpelare->pTexture, NULL, &(pSpelare->shipRect), pSpelare->angle, NULL, SDL_FLIP_NONE);
}

void destroySpelare(Spelare *pSpelare)
{
    SDL_DestroyTexture(pSpelare->pTexture);
    free(pSpelare);
}

