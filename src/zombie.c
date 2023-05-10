#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../includes/zombie.h"


struct zombieImage{
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;    
};

struct zombie
{
    float x, y, vx, vy;
    int window_width,window_height,renderAngle;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect rect;
};

static void getStartValues(Zombie *a);

ZombieImage *initiateZombie(SDL_Renderer *pRenderer){
    static ZombieImage* pZombieImage = NULL;
    if(pZombieImage==NULL){
        pZombieImage = malloc(sizeof(struct zombieImage));
        SDL_Surface *surface = IMG_Load("resources/zombie.png");
        if(!surface){
            printf("Error: %s\n",SDL_GetError());
            return NULL;
        }
        pZombieImage->pRenderer = pRenderer;
        pZombieImage->pTexture = SDL_CreateTextureFromSurface(pRenderer, surface);
        SDL_FreeSurface(surface);
        if(!pZombieImage->pTexture){
            printf("Error: %s\n",SDL_GetError());
            return NULL;
        }
    }
    return pZombieImage;
}

Zombie *createZombie(ZombieImage *pZombieImage, int window_width, int window_height){
    Zombie *pZombie = malloc(sizeof(struct zombie));
    pZombie->pRenderer = pZombieImage->pRenderer;
    pZombie->pTexture = pZombieImage->pTexture;
    pZombie->window_width = window_width;
    pZombie->window_height = window_height;
    SDL_QueryTexture(pZombieImage->pTexture,NULL,NULL,&(pZombie->rect.w),&(pZombie->rect.h));
    pZombie->rect.w/=4;
    pZombie->rect.h/=4;
    getStartValues(pZombie);
    pZombie->renderAngle =0;
    return pZombie;
}


static void getStartValues(Zombie *pZombie){
   
   int angle;
   int randomEdge = rand()%4;
   pZombie->x=rand()%pZombie->window_width;
   pZombie->y=rand()%pZombie->window_height;
   if(randomEdge==0)
   {
    pZombie->x=0;
    angle=rand()%90-45;
   }
   else if (randomEdge==1)
    {
        pZombie->y=0;
        angle=rand()%90;
    }
    else if (randomEdge==2) 
    {
        pZombie->x=pZombie->window_width;
        angle=(rand()%90)+135;
    }
    else if (randomEdge==3)
    {
        pZombie->y=pZombie->window_height;
        angle=(rand()%90)+90;
    } 
    int v=rand()%8+5;
    pZombie->vx=v*sin(angle*0.75);
    pZombie->vy=v*cos(angle*0.75);
    pZombie->rect.x=pZombie->x;
    pZombie->rect.y=pZombie->y;
    
}


void updateZombie(Zombie *pZombie){
    pZombie->x+=pZombie->vx*0.1;
    pZombie->y+=pZombie->vy*0.1;
   
    pZombie->rect.x=pZombie->x;
    pZombie->rect.y=pZombie->y;

    if (pZombie->rect.x < 0)
        {
            pZombie->rect.x-=(pZombie->x)*3;
        }
        if (pZombie->rect.y < 0)
        {
            pZombie->rect.y-=(pZombie->y)*3;
        }
        if (pZombie->rect.x > pZombie->window_width - pZombie->rect.w)
        {
            pZombie->rect.x-=(pZombie->x)*3;
        }
        if (pZombie->rect.y > (pZombie->window_height-40) - pZombie->rect.h)
        {
            pZombie->rect.y-=(pZombie->y)*3;
        }
        
}

void drawZombie(Zombie *pZombie){
    SDL_RenderCopyEx(pZombie->pRenderer,pZombie->pTexture,NULL,&(pZombie->rect),pZombie->renderAngle,NULL,SDL_FLIP_NONE);
}

void destroyZombies(Zombie *pZombie){
    free(pZombie);
}

void destroyZombieimage(ZombieImage *pZombieImage){
    SDL_DestroyTexture(pZombieImage->pTexture);
}


/*Zombie *createZombie(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Zombie *pZombie= malloc(sizeof(struct zombie));
    pZombie->window_width = window_width;
    pZombie->window_height = window_height;
    pZombie->angle=0;
    pZombie->maxzombies=50;
    pZombie->pRenderer = pRenderer;
    SDL_Surface *pSurfacezombie = IMG_Load("resources/zombie.png");
    if(!pSurfacezombie){
        printf("Error: %s\n",SDL_GetError());
        return NULL;
    }
    pZombie->pZombieTexture= SDL_CreateTextureFromSurface(pRenderer, pSurfacezombie);
    if(!pZombie->pZombieTexture){
        printf("Error: %s\n",SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(pSurfacezombie);
    SDL_QueryTexture(pZombie->pZombieTexture,NULL,NULL,&(pZombie->pZombie->rect.x.w),&(pZombie->pZombie->rect.x.h));
    for(int i=0;i<(pZombie->maxzombies);i++)
    {
        int randomX = rand() % 900;
        int randomY = rand() %  500;
        pZombie->ZombiesRect[i].x = randomX;
        pZombie->ZombiesRect[i].y = randomY;
        pZombie->ZombiesRect[i].w = pZombie->pZombie->rect.x.w / 4;
        pZombie->ZombiesRect[i].h = pZombie->pZombie->rect.x.h / 4;
    }
    return pZombie;

}

void drawZombies(Zombie *pZombie)
{
     for (int i = 0; i < (pZombie->maxzombies); i++)
         {
             SDL_RenderCopy(pZombie->pRenderer, pZombie->pZombieTexture, NULL, &pZombie->ZombiesRect[i]);
         }
}


void updateZombies(Zombie *pZombie, int size)
{
    static Target targets[40] = {0};

    for (int i = 0; i < size; i++)
    {
        /*ZombiesRect[i].x += rand() % 5 - 2;

        ZombiesRect[i].y += rand() % 5 - 2;*/

        // zombies lämmnar inte skärm efter dem har spawnat
        
       /** if (pZombie->ZombiesRect[i].x < 0)
        {
           pZombie->ZombiesRect[i].x = 0;
        }
        if (pZombie->ZombiesRect[i].y < 0)
        {
            pZombie->ZombiesRect[i].y = 0;
        }
        if (pZombie->ZombiesRect[i].x > 1000 - pZombie->ZombiesRect[i].w)
        {
            pZombie->ZombiesRect[i].x = 1000 - pZombie->ZombiesRect[i].w;
        }
        if (pZombie->ZombiesRect[i].y > 750 - pZombie->ZombiesRect[i].h)
        {
            pZombie->ZombiesRect[i].y = 750 - pZombie->ZombiesRect[i].h;
        }
        

        // zombies mot mitten av skrämen
        int center_x = 1000 / 2 - pZombie->ZombiesRect[i].w / 2;
        int center_y = 750 / 2 - pZombie->ZombiesRect[i].h / 2;

        if (pZombie->ZombiesRect[i].x < center_x)
        {
            pZombie->ZombiesRect[i].x += 0.75;
        }
        else if (pZombie->ZombiesRect[i].x > center_x)
        {
            pZombie->ZombiesRect[i].x -= 0.75;
        }

        if (pZombie->ZombiesRect[i].y < center_y)
        {
            pZombie->ZombiesRect[i].y += 0.75; // hur fort de rör sig i pixlar
        }
        else if (pZombie->ZombiesRect[i].y > center_y)
        {
            pZombie->ZombiesRect[i].y -= 0.75;
        }
    }
    /*
     static int targetX = -1;  // current target point
    static int targetY = -1;

    for (int i = 0; i < size; i++)
    {
        if (zombieRect[i].x < 0)
        {
            zombieRect[i].x = 0;
        }
        if (zombieRect[i].y < 0)
        {
            zombieRect[i].y = 0;
        }
        if (zombieRect[i].x > 1000 - zombieRect[i].w)
        {
            zombieRect[i].x = 1000 - zombieRect[i].w;
        }
        if (zombieRect[i].y > 750 - zombieRect[i].h)
        {
            zombieRect[i].y = 750 - zombieRect[i].h;
        }

        // if zombie has reached current target point, select new random target point
        if (targetX == -1 && targetY == -1)
        {
            targetX = rand() % 1000;
            targetY = rand() % 750;
        }

        // calculate distance to current target point
        int dx = targetX - zombieRect[i].x;
        int dy = targetY - zombieRect[i].y;
        int distance = sqrt(dx * dx + dy * dy);

        // update zombie position towards target point
        if (distance > 10)
        {
            int angle = atan2(dy, dx);
            zombieRect[i].x += 2 * cos(angle);
            zombieRect[i].y += 2 * sin(angle);
        }
        else
        {
            targetX = -1;
            targetY = -1;
        }
    }
    */




/*void spawn_zombies(SDL_Rect *zombieRects, int numZombies, SDL_Surface *zombieImage, SDL_Texture *zombieTexture, int screenWidth, int screenHeight)
{
    {
        for (int i = 0; i < numZombies; i++)
        {
            zombieRects[i].x = screenWidth + i * (zombieImage->w / 2); // spawn zombies off-screen
            zombieRects[i].y = screenHeight - (zombieImage->h / 2);
            zombieRects[i].w = zombieImage->w / 2;
            zombieRects[i].h = zombieImage->h / 2;
        }
    }
}

/*void render_zombies(SDL_Renderer *renderer, SDL_Texture *zombieTexture, SDL_Rect *zombieRects, int numZombies)
{
    for (int i = 0; i < numZombies; i++)
    {
        SDL_RenderCopy(renderer, zombieTexture, NULL, &zombieRects[i]);
    }
}
/*void moveZombiesRandomly(SDL_Rect *ZombiesRect, int numZombies, int screenWidth, int screenHeight)
{
    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < numZombies; i++)
    {
        // Move the zombie randomly
        int randomX = rand() % 3 - 1; // generate a random number between -1 and 1
        int randomY = rand() % 3 - 1;
        ZombiesRect[i].x += randomX;
        ZombiesRect[i].y += randomY;

        // Keep the zombie within the screen boundaries
        if (ZombiesRect[i].x < 0)
        {
            ZombiesRect[i].x = 0;
        }
        else if (ZombiesRect[i].x > screenWidth - ZombiesRect[i].w)
        {
            ZombiesRect[i].x = screenWidth - ZombiesRect[i].w;
        }
        if (ZombiesRect[i].y < 0)
        {
            ZombiesRect[i].y = 0;
        }
        else if (ZombiesRect[i].y > screenHeight - ZombiesRect[i].h)
        {
            ZombiesRect[i].y = screenHeight - ZombiesRect[i].h;
        }
    }
}*/
