#include <SDL.h>
#include <math.h>
#include "../include/stars.h"
#include <stdlib.h>


struct stars{
    int nr_of_stars;
    SDL_Point *pStarList;
};

static void drawStar(SDL_Point *pPoint,SDL_Renderer *pRenderer);

Stars *createStars(int nr_of_stars, int width, int height){
    Stars *s = malloc(sizeof(struct stars));
    s->pStarList = malloc(nr_of_stars*sizeof(SDL_Point));
    s->nr_of_stars=nr_of_stars;
    for(int i=0;i<nr_of_stars;i++){
        s->pStarList[i].x = rand()%width;
        s->pStarList[i].y = rand()%height;
    }
    return s;
}

void drawStars(Stars *pstars,SDL_Renderer *pRenderer){
    for(int i=0;i<pstars->nr_of_stars;i++){
        drawStar(&(pstars->pStarList[i]),pRenderer);
    } 
}

static void drawStar(SDL_Point *pPoint,SDL_Renderer *pRenderer){
    SDL_RenderDrawPoint(pRenderer,pPoint->x,pPoint->y-1);
    SDL_RenderDrawPoint(pRenderer,pPoint->x+1,pPoint->y-1);
    SDL_RenderDrawPoint(pRenderer,pPoint->x-1,pPoint->y);
    SDL_RenderDrawPoint(pRenderer,pPoint->x,pPoint->y);
    SDL_RenderDrawPoint(pRenderer,pPoint->x+1,pPoint->y);
    SDL_RenderDrawPoint(pRenderer,pPoint->x+2,pPoint->y);
    SDL_RenderDrawPoint(pRenderer,pPoint->x-1,pPoint->y+1);
    SDL_RenderDrawPoint(pRenderer,pPoint->x,pPoint->y+1);
    SDL_RenderDrawPoint(pRenderer,pPoint->x+1,pPoint->y+1);
    SDL_RenderDrawPoint(pRenderer,pPoint->x+2,pPoint->y+1);
    SDL_RenderDrawPoint(pRenderer,pPoint->x,pPoint->y+2);
    SDL_RenderDrawPoint(pRenderer,pPoint->x+1,pPoint->y+2);
}

void destroyStars(Stars *pStars){
    free(pStars->pStarList);
    free(pStars);
}