<<<<<<< HEAD
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../includes/zombie.h"

void updateZombies(SDL_Rect *zombieRect, int size)
{
    for (int i = 0; i < size; i++)
    {
        // Move the zombie rect by a random amount between -5 and 5 pixels horizontally
        zombieRect[i].x += rand() % 11 - 5;

        // Move the zombie rect by a random amount between -5 and 5 pixels vertically
        zombieRect[i].y += rand() % 11 - 5;

        // Make sure the zombie rect stays within the screen bounds
        if (zombieRect[i].x < 0)
        {
            zombieRect[i].x = 0;
        }
        if (zombieRect[i].y < 0)
        {
            zombieRect[i].y = 0;
        }
        if (zombieRect[i].x > 800 - zombieRect[i].w)
        {
            zombieRect[i].x = 800 - zombieRect[i].w;
        }
        if (zombieRect[i].y > 600 - zombieRect[i].h)
        {
            zombieRect[i].y = 600 - zombieRect[i].h;
        }
    }
}
=======
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/zombie.h" // include the zombies header file
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
>>>>>>> libans-branch

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

void move_zombies(SDL_Rect *zombieRects, int numZombies, int movementSpeed, int screenWidth)
{
    for (int i = 0; i < numZombies; i++)
    {
        zombieRects[i].x -= movementSpeed;
        if (zombieRects[i].x < -zombieRects[i].w)
        {
            zombieRects[i].x = screenWidth + (zombieRects[i].w / 2);
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
<<<<<<< HEAD
void moveZombiesRandomly(SDL_Rect *zombieRect, int numZombies, int screenWidth, int screenHeight)
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
=======

void updateZombies(SDL_Rect *zombieRect, int size)
{
    for (int i = 0; i < size; i++)
    {
        // Move the zombie rect by a random amount between -5 and 5 pixels horizontally
        zombieRect[i].x += rand() % 11 - 5;

        // Move the zombie rect by a random amount between -5 and 5 pixels vertically
        zombieRect[i].y += rand() % 11 - 5;

        // Make sure the zombie rect stays within the screen bounds
>>>>>>> libans-branch
        if (zombieRect[i].x < 0)
        {
            zombieRect[i].x = 0;
        }
<<<<<<< HEAD
        else if (zombieRect[i].x > screenWidth - zombieRect[i].w)
        {
            zombieRect[i].x = screenWidth - zombieRect[i].w;
        }
=======
>>>>>>> libans-branch
        if (zombieRect[i].y < 0)
        {
            zombieRect[i].y = 0;
        }
<<<<<<< HEAD
        else if (zombieRect[i].y > screenHeight - zombieRect[i].h)
        {
            zombieRect[i].y = screenHeight - zombieRect[i].h;
        }
    }

=======
        if (zombieRect[i].x > 800 - zombieRect[i].w)
        {
            zombieRect[i].x = 800 - zombieRect[i].w;
        }
        if (zombieRect[i].y > 600 - zombieRect[i].h)
        {
            zombieRect[i].y = 600 - zombieRect[i].h;
        }
    }
>>>>>>> libans-branch
}
