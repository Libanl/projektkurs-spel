#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../include/rocket.h"
#include "../include/stars.h"
#include "../include/asteroid.h"
#include "../include/text.h"

#define SPEED 150
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
#define MAX_ASTEROIDS 200

// test av branch för net
// second change

enum gameState
{
    START,
    ONGOING,
    GAME_OVER
};
typedef enum gameState GameState;

struct game
{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Rocket *pRocket;
    Stars *pStars;
    AsteroidImage *pAsteroidImage;
    Asteroid *pAsteroids[MAX_ASTEROIDS];
    int nrOfAsteroids;
    TTF_Font *pFont, *pScoreFont;
    Text *pScoreText, *pOverText, *pStartText;
    int startTime; // in ms
    int gameTime;  // in s
    int timeForNextAsteroid;
    GameState state;
};
typedef struct game Game;

int initiate(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
int getTime(Game *pGame);
void updateGameTime(Game *pGame);
void updateNrOfAsteroids(Game *pGame);
void resetAsteroids(Game *pGame);
void handleInput(Game *pGame, SDL_Event *pEvent);

int main(int argv, char **args)
{
    Game g = {0};
    if (!initiate(&g))
        return 1;
    run(&g);
    close(&g);

    return 0;
}

int initiate(Game *pGame)
{
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 0;
    }
    if (TTF_Init() != 0)
    {
        printf("Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }

    pGame->pWindow = SDL_CreateWindow("Rocket Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!pGame->pWindow)
    {
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!pGame->pRenderer)
    {
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }

    pGame->pFont = TTF_OpenFont("arial.ttf", 100);
    pGame->pScoreFont = TTF_OpenFont("arial.ttf", 30);
    if (!pGame->pFont || !pGame->pScoreFont)
    {
        printf("Error: %s\n", TTF_GetError());
        close(pGame);
        return 0;
    }

    pGame->pRocket = createRocket(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->pStars = createStars(WINDOW_WIDTH * WINDOW_HEIGHT / 10000, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->pAsteroidImage = initiateAsteroids(pGame->pRenderer);
    pGame->pOverText = createText(pGame->pRenderer, 238, 168, 65, pGame->pFont, "Game over", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    pGame->pStartText = createText(pGame->pRenderer, 238, 168, 65, pGame->pScoreFont, "Press space to start", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100);

    if (!pGame->pRocket || !pGame->pAsteroidImage || !pGame->pOverText || !pGame->pStartText)
    {
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    pGame->nrOfAsteroids = 0;
    resetAsteroids(pGame);
    pGame->startTime = SDL_GetTicks64();
    pGame->gameTime = -1;
    pGame->timeForNextAsteroid = 5;
    pGame->state = START;

    return 1;
}

void run(Game *pGame)
{
    int close_requested = 0;
    SDL_Event event;
    pGame->pScoreText = NULL;
    while (!close_requested)
    {
        switch (pGame->state)
        {
        case ONGOING:
            // event io (inkl nätverk) - update - draw
            updateGameTime(pGame);
            updateNrOfAsteroids(pGame);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    close_requested = 1;
                else
                    handleInput(pGame, &event);
            }
            updateRocket(pGame->pRocket);
            for (int i = 0; i < pGame->nrOfAsteroids; i++)
                updateAsteroid(pGame->pAsteroids[i]);
            for (int i = 0; i < pGame->nrOfAsteroids; i++)
                if (collideRocket(pGame->pRocket, getRectAsteroid(pGame->pAsteroids[i])))
                    pGame->state = GAME_OVER;

            SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
            SDL_RenderClear(pGame->pRenderer);
            SDL_SetRenderDrawColor(pGame->pRenderer, 230, 230, 230, 255);
            drawStars(pGame->pStars, pGame->pRenderer);
            for (int i = 0; i < pGame->nrOfAsteroids; i++)
                drawAsteroid(pGame->pAsteroids[i]);
            drawRocket(pGame->pRocket);
            if (pGame->pScoreText)
                drawText(pGame->pScoreText);
            SDL_RenderPresent(pGame->pRenderer);
            SDL_Delay(1000 / 60 - 15);
            break;
        case GAME_OVER:
            drawText(pGame->pOverText);
        case START:
            drawText(pGame->pStartText);
            SDL_RenderPresent(pGame->pRenderer);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    close_requested = 1;
                else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    resetAsteroids(pGame);
                    resetRocket(pGame->pRocket);
                    pGame->startTime = SDL_GetTicks64();
                    pGame->gameTime = -1;
                    pGame->state = ONGOING;
                }
            }
            break;
        }
    }
}

void handleInput(Game *pGame, SDL_Event *pEvent)
{
    if (pEvent->type == SDL_KEYDOWN)
    {
        switch (pEvent->key.keysym.scancode)
        {
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
            accelerate(pGame->pRocket);
            break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
            turnLeft(pGame->pRocket);
            break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
            turnRight(pGame->pRocket);
            break;
        }
    }
}

void close(Game *pGame)
{
    if (pGame->pRocket)
        destroyRocket(pGame->pRocket);
    if (pGame->pStars)
        destroyStars(pGame->pStars);
    for (int i = 0; i < pGame->nrOfAsteroids; i++)
        destroyAsteroid(pGame->pAsteroids[i]);
    if (pGame->pAsteroidImage)
        destroyAsteroidImage(pGame->pAsteroidImage);
    if (pGame->pRenderer)
        SDL_DestroyRenderer(pGame->pRenderer);
    if (pGame->pWindow)
        SDL_DestroyWindow(pGame->pWindow);

    if (pGame->pScoreText)
        destroyText(pGame->pScoreText);
    if (pGame->pOverText)
        destroyText(pGame->pOverText);
    if (pGame->pStartText)
        destroyText(pGame->pStartText);
    if (pGame->pFont)
        TTF_CloseFont(pGame->pFont);
    if (pGame->pScoreFont)
        TTF_CloseFont(pGame->pScoreFont);
    TTF_Quit();
    SDL_Quit();
}

// return time since game started in s
int getTime(Game *pGame)
{
    return (SDL_GetTicks64() - pGame->startTime) / 1000;
}

void updateGameTime(Game *pGame)
{
    if (getTime(pGame) > pGame->gameTime)
    {
        (pGame->gameTime)++;
        if (pGame->pScoreText)
            destroyText(pGame->pScoreText);
        static char scoreString[30];
        sprintf(scoreString, "%d", getTime(pGame));
        if (pGame->pScoreFont)
            pGame->pScoreText = createText(pGame->pRenderer, 238, 168, 65, pGame->pScoreFont, scoreString, WINDOW_WIDTH - 50, 50);
    }
}

void updateNrOfAsteroids(Game *pGame)
{
    if (getTime(pGame) > pGame->timeForNextAsteroid && pGame->nrOfAsteroids < MAX_ASTEROIDS)
    {
        (pGame->timeForNextAsteroid) += 1; // seconds till next asteroid
        pGame->pAsteroids[pGame->nrOfAsteroids] = createAsteroid(pGame->pAsteroidImage, WINDOW_WIDTH, WINDOW_HEIGHT);
        pGame->nrOfAsteroids++;
    }
}

void resetAsteroids(Game *pGame)
{
    for (int i = 0; i < pGame->nrOfAsteroids; i++)
        destroyAsteroid(pGame->pAsteroids[i]);
    pGame->nrOfAsteroids = 30;
    for (int i = 0; i < pGame->nrOfAsteroids; i++)
    {
        pGame->pAsteroids[i] = createAsteroid(pGame->pAsteroidImage, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
}