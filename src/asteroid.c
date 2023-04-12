#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include "../include/asteroid.h"

struct asteroidImage{
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;    
};

struct asteroid{
    float x, y, vx, vy;
    int window_width,window_height,renderAngle;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect rect;
};

static void getStartValues(Asteroid *a);

AsteroidImage *initiateAsteroids(SDL_Renderer *pRenderer){
    static AsteroidImage* pAsteroidImage = NULL;
    if(pAsteroidImage==NULL){
        pAsteroidImage = malloc(sizeof(struct asteroidImage));
        SDL_Surface *surface = IMG_Load("resources/zombie.png");
        if(!surface){
            printf("Error: %s\n",SDL_GetError());
            return NULL;
        }
        pAsteroidImage->pRenderer = pRenderer;
        pAsteroidImage->pTexture = SDL_CreateTextureFromSurface(pRenderer, surface);
        SDL_FreeSurface(surface);
        if(!pAsteroidImage->pTexture){
            printf("Error: %s\n",SDL_GetError());
            return NULL;
        }
    }
    return pAsteroidImage;
}

Asteroid *createAsteroid(AsteroidImage *pAsteroidImage, int window_width, int window_height){
    Asteroid *pAsteroid = malloc(sizeof(struct asteroid));
    pAsteroid->pRenderer = pAsteroidImage->pRenderer;
    pAsteroid->pTexture = pAsteroidImage->pTexture;
    pAsteroid->window_width = window_width;
    pAsteroid->window_height = window_height;
    SDL_QueryTexture(pAsteroidImage->pTexture,NULL,NULL,&(pAsteroid->rect.w),&(pAsteroid->rect.h));
    int sizeFactor = rand()%8+1;
    pAsteroid->rect.w/=sizeFactor;
    pAsteroid->rect.h/=sizeFactor;
    getStartValues(pAsteroid);
    pAsteroid->renderAngle = rand()%360;

    return pAsteroid;
}

static void getStartValues(Asteroid *pAsteroid){
    int angle;
    if(rand()%2){
        pAsteroid->x=rand()%pAsteroid->window_width-pAsteroid->rect.w/2;
        pAsteroid->y=-pAsteroid->rect.h;
        angle=rand()%90-45;
    }else{
        pAsteroid->y=rand()%pAsteroid->window_height-pAsteroid->rect.h/2;
        pAsteroid->x=-pAsteroid->rect.w;
        angle=rand()%90;
    }
    int v=rand()%8+5;
    pAsteroid->vx=v*sin(angle*2*M_PI/360);
    pAsteroid->vy=v*cos(angle*2*M_PI/360);
    pAsteroid->rect.x=pAsteroid->x;
    pAsteroid->rect.y=pAsteroid->y;
}

SDL_Rect getRectAsteroid(Asteroid *pAsteroid){
    return pAsteroid->rect;
}

void updateAsteroid(Asteroid *pAsteroid){
    pAsteroid->x+=pAsteroid->vx*0.1;
    pAsteroid->y+=pAsteroid->vy*0.1;
    if (pAsteroid->x>pAsteroid->window_width||pAsteroid->y>pAsteroid->window_height){
        getStartValues(pAsteroid);
        return;
    }
    pAsteroid->rect.x=pAsteroid->x;
    pAsteroid->rect.y=pAsteroid->y;
}

void drawAsteroid(Asteroid *pAsteroid){
    SDL_RenderCopyEx(pAsteroid->pRenderer,pAsteroid->pTexture,NULL,&(pAsteroid->rect),pAsteroid->renderAngle,NULL,SDL_FLIP_NONE);
}

void destroyAsteroid(Asteroid *pAsteroid){
    free(pAsteroid);
}

void destroyAsteroidImage(AsteroidImage *pAsteroidImage){
    SDL_DestroyTexture(pAsteroidImage->pTexture);
}
