#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/zombie.h" // include the zombies header file
<<<<<<< Updated upstream
=======
#include "../includes/spelare.h"
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 250
>>>>>>> Stashed changes


int main(int argv, char **args)
{
    SDL_Window *window = SDL_CreateWindow("Zombies COD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
<<<<<<< Updated upstream
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
=======
    SDL_Renderer *pRenderer = SDL_CreateRenderer(window, -1, 0);
    
>>>>>>> Stashed changes

    SDL_Surface *backgroundImage = IMG_Load("resources/28256.jpg");
    if (!backgroundImage)
    {
        printf("Error! BackgroundImage failed\n", IMG_GetError());
        return 1;
    }

<<<<<<< Updated upstream
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
=======
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(pRenderer, backgroundImage);
>>>>>>> Stashed changes
    if (!backgroundTexture)
    {
        printf("Failed to create background texture: %s\n", SDL_GetError());
        SDL_FreeSurface(backgroundImage);
        return 1;
    }

    SDL_Surface *zombieImage = IMG_Load("resources/zombie.png");
    if (!zombieImage)
    {
        printf("Error! ZombieImage failed\n", IMG_GetError());
        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundImage);
        return 1;
    }

<<<<<<< Updated upstream
    SDL_Texture *zombieTexture = SDL_CreateTextureFromSurface(renderer, zombieImage);
=======
    SDL_Texture *zombieTexture = SDL_CreateTextureFromSurface(pRenderer, zombieImage);
>>>>>>> Stashed changes
    if (!zombieTexture)
    {
        printf("Failed to create zombie texture: %s\n", SDL_GetError());
        SDL_FreeSurface(zombieImage);
        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundImage);
        return 1;
    }
<<<<<<< Updated upstream
=======
    
    SDL_Surface *spelareImage = IMG_Load("resources/zombie.png");
    if(!spelareImage){
        printf("Error: %s\n",SDL_GetError());
        return 1;
    }

    SDL_Texture *spelareTexture = SDL_CreateTextureFromSurface(pRenderer, spelareImage);
    if(!spelareTexture){
        printf("Error: %s\n",SDL_GetError());
        return 1;
    }
>>>>>>> Stashed changes

    SDL_Rect zombieRect[3]; // create 3 zombies
    for (int i = 0; i < 3; i++)
    {
        zombieRect[i].x = 100 + i * 200;
        zombieRect[i].y = 300;
        zombieRect[i].w = zombieImage->w / 2; // make zombies half size
        zombieRect[i].h = zombieImage->h / 10;
    }
<<<<<<< Updated upstream
=======

    Spelare *spelare1 = createSpelare(WINDOW_WIDTH/2,WINDOW_HEIGHT/2, pRenderer,WINDOW_WIDTH,WINDOW_HEIGHT);

>>>>>>> Stashed changes

    int isRunning = 1;
    SDL_Event event;

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = 0;
                break;
            }
        }

        updateZombies(zombieRect, 3); // update the zombies' positions

<<<<<<< Updated upstream
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
=======
        SDL_RenderClear(pRenderer);
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

        SDL_RenderCopy(pRenderer, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(pRenderer,spelareTexture, NULL, NULL);
>>>>>>> Stashed changes

        // Render all zombies
        for (int i = 0; i < 3; i++)
        {
<<<<<<< Updated upstream
            SDL_RenderCopy(renderer, zombieTexture, NULL, &zombieRect[i]);
        }
        
        SDL_RenderPresent(renderer);
=======
            SDL_RenderCopy(pRenderer, zombieTexture, NULL, &zombieRect[i]);
        }
        
        SDL_RenderPresent(pRenderer);
>>>>>>> Stashed changes

        // Add delay to slow down the zombies' movement
        SDL_Delay(10); // add 10 millisecond delay
    }

    SDL_DestroyTexture(spelareTexture);
    SDL_FreeSurface(spelareImage);
    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(backgroundImage);
    SDL_DestroyTexture(zombieTexture);
    SDL_FreeSurface(zombieImage);
<<<<<<< Updated upstream
    SDL_DestroyRenderer(renderer);
=======
    SDL_DestroyRenderer(pRenderer);
>>>>>>> Stashed changes
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
