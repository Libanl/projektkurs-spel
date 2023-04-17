#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/zombie.h" // include the zombies header file
#include "../includes/spelare.h"
#include "../includes/music.h"
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 250

struct game{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Spelare *pSpelare;
    SDL_Rect zombieRect[5]; // create 3 zombies
    SDL_Surface *pbackgroundImage;
    SDL_Texture *pbackgroundTexture;
    SDL_Surface *pZombieImage; 
    SDL_Texture *pZombieTexture; 
    SDL_Surface *pSpelareImage;
    SDL_Texture *pSpelareTexture;
};
typedef struct game Game;

int initiate(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);

int main(int argv, char** args){
    Game g={0};
    if(!initiate(&g)) return 1;
    run(&g);
    close(&g);

    return 0;
}


int initiate(Game *pGame){

     if (initMus() == -1)
    {
        printf("Kunde inte initiera ljudsystemet!\n");
        return 1;
    }

    playMus("resources/spel.MP3");

    pGame->pWindow = SDL_CreateWindow("Zombies COD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, 0);
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, 0);

    pGame->pbackgroundImage = IMG_Load("resources/28256.jpg");
    if (!pGame->pbackgroundImage)
    {
        printf("Error! backgroundImage failed\n", IMG_GetError());
        return 1;
    }

    pGame->pbackgroundTexture = SDL_CreateTextureFromSurface(pGame->pRenderer, pGame->pbackgroundImage);
    if (!pGame->pbackgroundTexture)
    {
        printf("Failed to create background texture: %s\n", SDL_GetError());
        SDL_FreeSurface(pGame->pbackgroundImage);
        return 1;
    }

    pGame->pZombieImage = IMG_Load("resources/zombie.png");
    if (!pGame->pZombieImage)
    {
        printf("Error! ZombieImage failed\n", IMG_GetError());
        SDL_DestroyTexture(pGame->pbackgroundTexture);
        SDL_FreeSurface(pGame->pbackgroundImage);
        return 1;
    }

    pGame->pZombieTexture = SDL_CreateTextureFromSurface(pGame->pRenderer, pGame->pZombieImage);
    if (!pGame->pZombieTexture)
    {
        printf("Failed to create zombie texture: %s\n", SDL_GetError());
        SDL_FreeSurface(pGame->pZombieImage);
        SDL_DestroyTexture(pGame->pbackgroundTexture);
        SDL_FreeSurface(pGame->pbackgroundImage);
        return 1;
    }

    pGame->pSpelareImage = IMG_Load("resources/soldier2.png");
    if (!pGame->pSpelareImage)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    pGame->pSpelareTexture = SDL_CreateTextureFromSurface(pGame->pRenderer, pGame->pSpelareImage);
    if (!pGame->pSpelareTexture)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

     // create 3 zombies
    for (int i = 0; i < 5; i++)
    {
        pGame->zombieRect[i].x = 100 + i * 200;
        pGame->zombieRect[i].y = 300;
        pGame->zombieRect[i].w = pGame->pZombieImage->w / 2; // make zombies half size
        pGame->zombieRect[i].h = pGame->pZombieImage->h / 10;
    }

    Spelare *spelare1 = createSpelare(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
}


void run(Game *pGame){
    
    SDL_Rect spelareRect = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pGame->pSpelareImage->w / 7, pGame->pSpelareImage->h / 16};
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

        updateZombies(pGame->zombieRect, 5); // update the zombies' positions

        SDL_RenderClear(pGame->pRenderer);
        SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);

        SDL_RenderCopy(pGame->pRenderer, pGame->pbackgroundTexture, NULL, NULL);
        SDL_RenderCopy(pGame->pRenderer, pGame->pSpelareTexture, NULL, &spelareRect);

        // Render all zombies
        for (int i = 0; i < 5; i++)
        {
            SDL_RenderCopy(pGame->pRenderer, pGame->pZombieTexture, NULL, &pGame->zombieRect[i]);
        }

        SDL_RenderPresent(pGame->pRenderer);

        // Add delay to slow down the zombies' movement
        SDL_Delay(10); // add 10 millisecond delay
    }
}

    void close(Game *pGame){
        stopMus();
        cleanMu();
        SDL_DestroyTexture(pGame->pSpelareTexture);
        SDL_FreeSurface(pGame->pSpelareImage);
        SDL_DestroyTexture(pGame->pbackgroundTexture);
        SDL_FreeSurface(pGame->pbackgroundImage);
        SDL_DestroyTexture(pGame->pZombieTexture);
        SDL_FreeSurface(pGame->pZombieImage);
        SDL_DestroyRenderer(pGame->pRenderer);
        SDL_DestroyWindow(pGame->pWindow);
        SDL_Quit();

    }
