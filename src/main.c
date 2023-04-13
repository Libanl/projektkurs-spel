#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argv, char** args)
{
    
    SDL_Window *window = SDL_CreateWindow("Zombies COD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
     if(!window){
        printf("Error! Renderer failed\n", IMG_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        printf("Error! Renderer failed\n", IMG_GetError());
        return 1;
    }

    SDL_Surface* backgroundImage = IMG_Load("C:/Users/46729/Documents/GitHub/projektkurs-spel/resources/28256.jpg");
    if(!backgroundImage){
        printf("Error! BackgroundImage failed\n", IMG_GetError());
        return 1;
    }

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
    if (!backgroundTexture){
        printf("Failed to create background texture: %s\n", SDL_GetError());
        SDL_FreeSurface(backgroundImage);
        return 1;
    }


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

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}