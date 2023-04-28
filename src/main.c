#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/zombie.h" // include the zombies header file
#include "../includes/spelare.h"
#include "../includes/music.h"
#include "../includes/bullet.h"
#include "../includes/text.h"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 750
#define MAX_ZOMBIES 200

enum gameState{START, ONGOING, GAME_OVER};
typedef enum gameState Gamestate;

struct game{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Spelare *pSpelare;
    Bullet *pBullet;
    SDL_Rect zombieRect[MAX_ZOMBIES];
    SDL_Surface *pbackgroundImage;
    SDL_Texture *pbackgroundTexture;
    SDL_Surface *pZombieImage; 
    SDL_Texture *pZombieTexture;
    int MoveUp;
    int MoveLeft;
    int MoveDown;
    int MoveRight;
    TTF_Font *pScoreFont, *pFont;
    Text *pScoreText, *pOverText, *pStartText;;
    int gameTimeM;
    int startTime;//in ms
    int gameTime;//in s
    Gamestate state;
};
typedef struct game Game;

int initiate(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(SDL_Event *pEvent, Game *pGame, int keys[]);
int getTime(Game *pGame);
int getMilli(Game *pGame);
void updateGameTime(Game *pGame);
void CheckCollison( Game *pGame, int zombieCount);


int main(int argv, char** args){
    Game g={0};
    if(!initiate(&g)) return 1;
    run(&g);
    close(&g);

    return 0;
}


int initiate(Game *pGame){

    
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){
        printf("Error: %s\n",SDL_GetError());
        return 0;
    }
    if(TTF_Init()!=0){
        printf("Error: %s\n",TTF_GetError());
        SDL_Quit();
        return 0;
    }

    if (initMus() == -1)
    {
        printf("Kunde inte initiera ljudsystemet!\n");
        return 1;
    }

    playMus("resources/spel.MP3");

    pGame->pWindow = SDL_CreateWindow("Zombies COD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

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

    
    pGame->MoveUp=1;
    pGame->MoveLeft=0;
    pGame->MoveDown=0;
    pGame->MoveRight=0;

    pGame->pSpelare = createSpelare(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    pGame->pFont = TTF_OpenFont("resources/arial.ttf", 100);
    pGame->pScoreFont = TTF_OpenFont("resources/arial.ttf", 70);
    if(!pGame->pFont || !pGame->pScoreFont){
        printf("Error: %s\n",TTF_GetError());
        close(pGame);
        return 0;
    }

    pGame->pOverText = createText(pGame->pRenderer,238,168,65,pGame->pFont,"Game over",WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    pGame->pStartText = createText(pGame->pRenderer,238,168,65,pGame->pScoreFont,"Press space to start",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);
    pGame->startTime = SDL_GetTicks64();
    pGame->gameTime = -1;
    pGame->state = START;
    return 1;
}



void run(Game *pGame){
    
    int keys[SDL_NUM_SCANCODES] = {0}; // Initialize an array to store key states
    int isRunning = 1;
    SDL_Event event;
    int zombieCount = 0;      // Keep track of the current number of zombies
    Uint32 lastSpawnTime = 0; // Keep track of the time since the last zombie spawn
    while (isRunning)
    {   
        switch (pGame->state)
        {
            case ONGOING:
                while (SDL_PollEvent(&event)){   
                    switch (event.type){
                    case SDL_QUIT:
                        isRunning = 0;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            isRunning = 0;
                        }
                    default: handleInput(&event,pGame,keys);
                        break;
                    }
                }

                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - lastSpawnTime >= 1000 && zombieCount < MAX_ZOMBIES)
                {
                    // Spawn a new zombie at a random location //funkar inte när man gör till funktion????
                    int randomX = rand() % WINDOW_WIDTH;
                    int randomY = rand() % WINDOW_HEIGHT;
                    int randomEdge = rand()%4;
                    if(randomEdge==0) randomX=0;
                    else if (randomEdge==1) randomY=0;
                    else if (randomEdge==2) randomX=WINDOW_WIDTH;
                    else if (randomEdge==3) randomY=WINDOW_HEIGHT;

                    pGame->zombieRect[zombieCount].x = randomX;
                    pGame->zombieRect[zombieCount].y = randomY;
                    pGame->zombieRect[zombieCount].w = pGame->pZombieImage->w / 4;
                    pGame->zombieRect[zombieCount].h = pGame->pZombieImage->h / 4;
                    zombieCount++;
                    lastSpawnTime = currentTime;
                }
                updateSpelare(pGame->pSpelare);
                updateZombies(pGame->zombieRect, zombieCount); // update the zombies' positions
                updateGameTime(pGame);
                SDL_RenderClear(pGame->pRenderer);        
                SDL_RenderCopy(pGame->pRenderer, pGame->pbackgroundTexture, NULL, NULL);
                drawSpelare(pGame->pSpelare);
                // Render all zombies
                for (int i = 0; i < zombieCount; i++)
                {
                    SDL_RenderCopy(pGame->pRenderer, pGame->pZombieTexture, NULL, &pGame->zombieRect[i]);
                }
                
                if(getTime(pGame)>2){ // FUNKTIONEN ANROPAS EFTER 2 SEKUNDER OCH DÅ BLIR DET SEGMENTATION FAULT, 
                    CheckCollison(pGame, zombieCount); // MOSES! NÅGOT I DENNA FUNKTION GÖR ATT MAN INTE KAN KÖRA 
                }
                if(pGame->pScoreText) 
                {
                    drawText(pGame->pScoreText);
                }
                SDL_Delay(10);

                SDL_RenderPresent(pGame->pRenderer);
                if(getTime(pGame)==60){
                    isRunning = 0;
                }
                break;
            case START:
                drawText(pGame->pStartText);
                SDL_RenderPresent(pGame->pRenderer);
                while(SDL_PollEvent(&event)){
                    if(event.type==SDL_QUIT) isRunning = 0;
                    else if(event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_SPACE){
                        resetSpelare(pGame->pSpelare);
                        pGame->startTime = SDL_GetTicks64();
                        pGame->gameTime = -1;
                        pGame->state = ONGOING;
                    }
                }
                break;
        }   
    }
}


// Function to handle input
void handleInput(SDL_Event *pEvent, Game *pGame, int keys[]) {
    if (pEvent->type == SDL_KEYDOWN) {
        keys[pEvent->key.keysym.scancode] = 1; // Set the corresponding key state to true
    } else if (pEvent->type == SDL_KEYUP) {
        keys[pEvent->key.keysym.scancode] = 0; // Set the corresponding key state to false
    }

    // Check for combination of keys pressed together
    
    if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A]){
        moveUp(pGame->pSpelare);
        moveLeft(pGame->pSpelare);
    } else if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_D]){
        moveUp(pGame->pSpelare);
        moveRight(pGame->pSpelare);
    } else if (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D]){
        moveDown(pGame->pSpelare);
        moveRight(pGame->pSpelare);
    } else if (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_A]){
        moveDown(pGame->pSpelare);
        moveLeft(pGame->pSpelare);
    } else if (keys[SDL_SCANCODE_W]) {
        moveUp(pGame->pSpelare);
        pGame->MoveUp=1;
        pGame->MoveLeft=0;
        pGame->MoveDown=0;
        pGame->MoveRight=0;
    } else if (keys[SDL_SCANCODE_A]) {
        moveLeft(pGame->pSpelare);
        pGame->MoveLeft=1;
        pGame->MoveUp=0;
        pGame->MoveDown=0;
        pGame->MoveRight=0;
    } else if (keys[SDL_SCANCODE_S]) {
        moveDown(pGame->pSpelare);
        pGame->MoveDown=1;
        pGame->MoveLeft=0;
        pGame->MoveUp=0;
        pGame->MoveRight=0;
    } else if (keys[SDL_SCANCODE_D]) {
        moveRight(pGame->pSpelare);
        pGame->MoveRight=1;
        pGame->MoveLeft=0;
        pGame->MoveDown=0;
        pGame->MoveUp=0;
    } else if(keys[SDL_SCANCODE_SPACE]){
        fireSpelare(pGame->pSpelare, pGame->MoveUp, pGame->MoveLeft, pGame->MoveDown, pGame->MoveRight);
        //for (int i = 0; i < MAX_ZOMBIES; i++) {
    //if (SDL_IntersectRect(&bulletRect, &zombieRect[i], &intersectionRect)) {
        
        //printf("test");
    //}
}
}
//    }

void close(Game *pGame){
    stopMus();
    cleanMu();
    SDL_DestroyTexture(pGame->pbackgroundTexture);
    SDL_FreeSurface(pGame->pbackgroundImage);
    SDL_DestroyTexture(pGame->pZombieTexture);
    SDL_FreeSurface(pGame->pZombieImage);
    SDL_DestroyRenderer(pGame->pRenderer);
    SDL_DestroyWindow(pGame->pWindow);
    if(pGame->pScoreText) destroyText(pGame->pScoreText);
    if(pGame->pOverText) destroyText(pGame->pOverText);
    if(pGame->pStartText) destroyText(pGame->pStartText);   
    if(pGame->pFont) TTF_CloseFont(pGame->pFont);
    if(pGame->pScoreFont) TTF_CloseFont(pGame->pScoreFont);
    TTF_Quit();    
    SDL_Quit();

}

int getTime(Game *pGame){
    return (SDL_GetTicks64()-pGame->startTime)/1000;
}

int getMilli(Game *pGame)
{
    return ((SDL_GetTicks64()-pGame->startTime) % 1000) / 100;
}

void updateGameTime(Game *pGame){

        if(pGame->pScoreText) 
        {
            destroyText(pGame->pScoreText);
        }
        static char scoreString[30];
        sprintf(scoreString,"%d.%d",getTime(pGame), getMilli(pGame));
        if(pGame->pScoreFont) 
        {
            pGame->pScoreText = createText(pGame->pRenderer,255,255,255,pGame->pScoreFont,scoreString,WINDOW_WIDTH-75,50);    
        }
}

void CheckCollison(Game *pGame, int zombieCount)
{
    int i;
    for(i = 0; i < zombieCount; i++){
        if((xBullet(pGame->pBullet) > pGame->zombieRect[i].x && xBullet(pGame->pBullet) < pGame->zombieRect[i].x + 30) && 
           (yBullet(pGame->pBullet) > pGame->zombieRect[i].y && yBullet(pGame->pBullet) < pGame->zombieRect[i].y + 30)){
            killBullet(pGame->pBullet);
        }
    }   }