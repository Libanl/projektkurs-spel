#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../includes/zombie.h"

/*struct zombieImage
{
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
};

struct zombie
{
    float x, y, vx, vy;
    int window_width, window_height, renderAngle;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect rect;
};*/

void updateZombies(SDL_Rect *zombieRect, int size)
{
    for (int i = 0; i < size; i++)
    {
        /*zombieRect[i].x += rand() % 5 - 2;

        zombieRect[i].y += rand() % 5 - 2;*/

        // zombies lämmnar inte skärm efter dem har spawnat
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

        // zombies mot mitten av skrämen
        int center_x = 1000 / 2 - zombieRect[i].w / 2;
        int center_y = 750 / 2 - zombieRect[i].h / 2;

        if (zombieRect[i].x < center_x)
        {
            zombieRect[i].x += 0.75;
        }
        else if (zombieRect[i].x > center_x)
        {
            zombieRect[i].x -= 0.75;
        }

        if (zombieRect[i].y < center_y)
        {
            zombieRect[i].y += 0.75; // hur fort de rör sig i pixlar
        }
        else if (zombieRect[i].y > center_y)
        {
            zombieRect[i].y -= 0.75;
        }
    }
}

void spawn_zombies(SDL_Rect *zombieRects, int numZombies, SDL_Surface *zombieImage, SDL_Texture *zombieTexture, int screenWidth, int screenHeight)
{
    for (int i = 0; i < numZombies; i++)
    {
        zombieRects[i].x = screenWidth + i * (zombieImage->w / 2); // spawn zombies off-screen
        zombieRects[i].y = screenHeight - (zombieImage->h / 2);
        zombieRects[i].w = zombieImage->w / 2;
        zombieRects[i].h = zombieImage->h / 2;
    }
}

/*void move_zombies(SDL_Rect *zombieRects, int numZombies, int movementSpeed, int screenWidth)
{
    for (int i = 0; i < numZombies; i++)
    {
        zombieRects[i].x -= movementSpeed;
        if (zombieRects[i].x < -zombieRects[i].w)
        {
            zombieRects[i].x = screenWidth + (zombieRects[i].w / 2);
        }
    }
}*/

void render_zombies(SDL_Renderer *renderer, SDL_Texture *zombieTexture, SDL_Rect *zombieRects, int numZombies)
{
    for (int i = 0; i < numZombies; i++)
    {
        SDL_RenderCopy(renderer, zombieTexture, NULL, &zombieRects[i]);
    }
}
/*void moveZombiesRandomly(SDL_Rect *zombieRect, int numZombies, int screenWidth, int screenHeight)
{
    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < numZombies; i++)
    {
        // Move the zombie randomly
        int randomX = rand() % 3 - 1; // generate a random number between -1 and 1
        int randomY = rand() % 3 - 1;
        zombieRect[i].x += randomX;
        zombieRect[i].y += randomY;

        // Keep the zombie within the screen boundaries
        if (zombieRect[i].x < 0)
        {
            zombieRect[i].x = 0;
        }
        else if (zombieRect[i].x > screenWidth - zombieRect[i].w)
        {
            zombieRect[i].x = screenWidth - zombieRect[i].w;
        }
        if (zombieRect[i].y < 0)
        {
            zombieRect[i].y = 0;
        }
        else if (zombieRect[i].y > screenHeight - zombieRect[i].h)
        {
            zombieRect[i].y = screenHeight - zombieRect[i].h;
        }
    }
}*/
