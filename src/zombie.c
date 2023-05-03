#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../includes/zombie.h"

typedef struct {
    int x;
    int y;
} Target;



void updateZombies(SDL_Rect *zombieRect, int size)
{
    static Target targets[40] = {0};
    
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
        if (targets[i].x == 0 && targets[i].y == 0)
        {
            targets[i].x = rand() % 1000;
            targets[i].y = rand() % 750;
        }

        // calculate distance to current target point
        int dx = targets[i].x - zombieRect[i].x;
        int dy = targets[i].y - zombieRect[i].y;
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
            targets[i].x = 0;
            targets[i].y = 0;
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
}

void spawn_zombies(SDL_Rect *zombieRects, int numZombies, SDL_Surface *zombieImage, SDL_Texture *zombieTexture, int screenWidth, int screenHeight)
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


void render_zombies(SDL_Renderer *renderer, SDL_Texture *zombieTexture, SDL_Rect *zombieRects, int numZombies)
{
    for (int i = 0; i < numZombies; i++)
    {
        SDL_RenderCopy(renderer, zombieTexture, NULL, &zombieRects[i]);
    }
}
