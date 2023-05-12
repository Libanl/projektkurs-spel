#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../lib/include/spelare_data.h"
#include "../../lib/include/spelare.h"
#include "../../lib/include/bullet.h"




struct spelare
{
    float x, y, vx, vy;
    int angle;
    int frame;
    int nyframe;
    int alive;
    int window_width, window_height;
    Bullet *pBullet;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect shipRect[16];
    SDL_Rect gsprites[16];
    SDL_RendererFlip flip;

};

    Spelare *createSpelare(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height){
    Spelare *pSpelare = malloc(sizeof(struct spelare));
    pSpelare->vx=pSpelare->vy=0;
    pSpelare->angle=0;
    pSpelare->frame=7;
    pSpelare->nyframe=7;
    pSpelare->pBullet = createBullet(pRenderer, window_width, window_height);
    pSpelare->window_width = window_width;
    pSpelare->window_height = window_height;
    pSpelare->flip = SDL_FLIP_NONE;
    pSpelare->pRenderer = pRenderer;
    SDL_Surface *pSurface = IMG_Load("resources/sprite1.png");
    if(!pSurface){
        printf("Error: %s\n",SDL_GetError());
        return NULL;
    }
    pSpelare->pTexture= SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if(!pSpelare->pTexture){
        printf("Error: %s\n",SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(pSurface);
    SDL_QueryTexture(pSpelare->pTexture,NULL,NULL,&(pSpelare->shipRect[pSpelare->frame].w),&(pSpelare->shipRect[pSpelare->frame].h));
    //Down spriteSheets
    pSpelare->gsprites[ 0 ].x =0;
    pSpelare->gsprites[ 0 ].y =0;
    pSpelare->gsprites[ 0 ].w =64;
    pSpelare->gsprites[ 0 ].h =70;
    
    pSpelare->gsprites[ 1 ].x =64;
    pSpelare->gsprites[ 1 ].y =0;
    pSpelare->gsprites[ 1 ].w =64;
    pSpelare->gsprites[ 1 ].h =70;
    
    //Left spriteSheets
    pSpelare->gsprites[ 2 ].x =0;
    pSpelare->gsprites[ 2 ].y =70;
    pSpelare->gsprites[ 2 ].w =64;
    pSpelare->gsprites[ 2 ].h =70;
    
    pSpelare->gsprites[ 3 ].x =64;
    pSpelare->gsprites[ 3 ].y =70;
    pSpelare->gsprites[ 3 ].w =64;
    pSpelare->gsprites[ 3 ].h =70;
    
    //Right spriteSheets
    pSpelare->gsprites[ 4 ].x =0;
    pSpelare->gsprites[ 4 ].y = 140;
    pSpelare->gsprites[ 4 ].w =64;
    pSpelare->gsprites[ 4 ].h =70;
    
    pSpelare->gsprites[ 5 ].x =64;
    pSpelare->gsprites[ 5 ].y =140;
    pSpelare->gsprites[ 5 ].w =64;
    pSpelare->gsprites[ 5 ].h =70;
    
    //Up spriteSheets
    pSpelare->gsprites[ 6 ].x =0;
    pSpelare->gsprites[ 6 ].y =210;
    pSpelare->gsprites[ 6 ].w =64;
    pSpelare->gsprites[ 6 ].h =70;

    pSpelare->gsprites[ 7 ].x =64;
    pSpelare->gsprites[ 7 ].y =210;
    pSpelare->gsprites[ 7 ].w =64;
    pSpelare->gsprites[ 7 ].h =70;

    for(int i=0;i<8;i++)
    {
        pSpelare->shipRect[i]=pSpelare->gsprites[i];
        pSpelare->shipRect[i].w=pSpelare->shipRect[i].w;
        pSpelare->shipRect[i].h=pSpelare->shipRect[i].h;
        pSpelare->x=x-pSpelare->shipRect[i].w;
        pSpelare->y=y-pSpelare->shipRect[i].h;
    }
    printf("%d", pSpelare->x);
    return pSpelare;
    
}

void moveLeft(Spelare *pSpelare){
   
    if (pSpelare->shipRect[pSpelare->frame].x>0)
    {
        pSpelare->x-=5;
    }
    pSpelare->flip = SDL_FLIP_NONE;
    if(pSpelare->frame == 2)
    {
        pSpelare->nyframe = 3;
    }
    else
    {
         pSpelare->nyframe = 2;
    }

}

void moveRight(Spelare *pSpelare){
    
    if (pSpelare->shipRect[pSpelare->frame].x<970)
    {
        pSpelare->x+=5;
    }
    pSpelare->flip = SDL_FLIP_NONE;
    if(pSpelare->frame == 4)
    {
         pSpelare->nyframe = 5;
    }
    else
    {
        pSpelare->nyframe = 4;
    }

}

void moveUp(Spelare *pSpelare){

    if (pSpelare->shipRect[pSpelare->frame].y>0)
    {
        pSpelare->y-=5;
    }
    pSpelare->flip = SDL_FLIP_NONE;
    if(pSpelare->frame == 6)
    {
        pSpelare->nyframe = 7;
    }
    else
    {
        pSpelare->nyframe = 6;
    }
}

void moveDown(Spelare *pSpelare){

    if (pSpelare->shipRect[pSpelare->frame].y<600)
    {
        pSpelare->y+=5;
    }
    pSpelare->flip = SDL_FLIP_NONE;
    if(pSpelare->frame == 0)
    {
        pSpelare->nyframe = 1;
    }
    else
    {
        pSpelare->nyframe = 0;
    }
}


void updateSpelare(Spelare *pSpelare){
    pSpelare->shipRect[pSpelare->frame].x=pSpelare->x;
    pSpelare->shipRect[pSpelare->frame].y=pSpelare->y;
    pSpelare->frame=pSpelare->nyframe;
    updateBullet(pSpelare->pBullet);
}

void fireSpelare(Spelare *pSpelare, int m1, int m2, int m3, int m4)
{
    if (!pSpelare->alive || aliveBullet(pSpelare->pBullet)) return;
    int moveUp = m1;
    int moveLeft = m2;
    int moveDown = m3;
    int moveRight = m4;
    startBullet(pSpelare->pBullet, pSpelare->x + pSpelare->shipRect[pSpelare->frame].w / 2, pSpelare->y + pSpelare->shipRect[pSpelare->frame].h / 2, moveUp, moveLeft, moveDown, moveRight);
}


void drawSpelare(Spelare *pSpelare)
{
    if (aliveBullet(pSpelare->pBullet)) drawBullet(pSpelare->pBullet, pSpelare->pRenderer);
    SDL_RenderCopyEx(pSpelare->pRenderer,pSpelare->pTexture,&(pSpelare->gsprites[pSpelare->frame]),&(pSpelare->shipRect[pSpelare->frame]),pSpelare->angle,NULL,pSpelare->flip);
}


void destroySpelare(Spelare *pSpelare)
{
    SDL_DestroyTexture(pSpelare->pTexture);
    free(pSpelare);
}

void resetSpelare(Spelare *pSpelare){
    pSpelare->x=pSpelare->window_width/2;
    pSpelare->y=pSpelare->window_height/2;
}

void getSpelareSendData(Spelare *pSpelare, SpelareData *pSpelareData){
 
    pSpelareData->x = pSpelare->x;
    pSpelareData->y = pSpelare->y;
    //getBulletSendData(pSpelare->pBullet,&(pSpelareData->bData));
}

void updateSpelareWithRecievedData(Spelare *pSpelare, SpelareData *pSpelareData){
    pSpelare->x = pSpelareData->x;
    pSpelare->y = pSpelareData->y;
    //updateBulletWithRecievedData(pSpelare->pBullet,&(pSpelareData->bData));
}