#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/bullet.h"

struct bullet{
    float x, y, vx, vy;
    int time, window_width, window_height;
    SDL_Texture *pTexture;
};

Bullet *createBullet(SDL_Renderer *pRenderer, int window_width, int window_height){
    Bullet *pBullet = malloc(sizeof(struct bullet));
    pBullet->window_width = window_width;
    pBullet->window_height = window_height;
    pBullet->time = 0;
    
    SDL_Surface *pSurface = IMG_Load("resources/bullet.png");
    if(!pSurface){
        printf("Error: %s\n",SDL_GetError());
        return NULL;
    }
    pBullet->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    return pBullet;
}

void updateBullet(Bullet *pBullet){
    if(pBullet->time==0) return;
    pBullet->x+=pBullet->vx;
    pBullet->y+=pBullet->vy;
    if(pBullet->x<0 || pBullet->x>pBullet->window_width || pBullet->y<0 || pBullet->y>pBullet->window_height ) killBullet(pBullet);
    (pBullet->time)--;
    return;
}

void startBullet(Bullet *pBullet, float x, float y, int moveUp, int moveLeft, int moveDown, int moveRight ) {
    pBullet->x = x;
    pBullet->y = y;
    pBullet->time = BULLETLIFETIME;
    
    // Set horizontal velocity
    if (moveLeft==1) {
        pBullet->vx = -BULLETVELOCITY;
    } else if(moveRight==1) {
        pBullet->vx = BULLETVELOCITY;
    } else {
        pBullet->vx = 0;
    }
    
    // Set vertical velocity
    if (moveUp==1) {
        pBullet->vy = -BULLETVELOCITY;
    } else if(moveDown==1) {
        pBullet->vy = BULLETVELOCITY;
    } else {
        pBullet->vy = 0;
    }
}

void killBullet(Bullet *pBullet){
    pBullet->time=0;
}

void drawBullet(Bullet *pBullet,SDL_Renderer *pRenderer){
   if(pBullet->time==0) return;
    SDL_Rect bulletRect = {pBullet->x, pBullet->y, 16, 16}; // Replace 16 with your texture width and height
    SDL_RenderCopy(pRenderer, pBullet->pTexture, NULL, &bulletRect);
}

/*float xBullet(Bullet *pBullet){
    return pBullet->x;
}

float yBullet(Bullet *pBullet){
    return pBullet->y;
}*/

void destroyBullet(Bullet *pBullet){
    SDL_DestroyTexture(pBullet->pTexture);
    free(pBullet);
}

int aliveBullet(Bullet *pBullet){
    return pBullet->time>0;
}