#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>


#include "../../lib/include/music.h"
#include "../../lib/include/text.h"
#include "../../lib/include/spelare_data.h"
#include "../../lib/include/zombie.h" // include the zombies header file
#include "../../lib/include/bullet.h"
#include "../../lib/include/spelare.h"


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 750
#define MAX_ZOMBIES 200


struct game{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Spelare *pSpelare[MAX_SPELARE];
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
    Text *pScoreText, *pOverText, *pStartText, *pTestText;
    int startTime;
    int gameTime;
    GameState state;
    UDPsocket pSocket;
	UDPpacket *pPacket;
    IPaddress clients[MAX_SPELARE];
    int nrOfClients;
    ServerData sData;
    int nr_of_spelare;
};
typedef struct game Game;



int initiate(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(SDL_Event *pEvent, Game *pGame, int keys[]);
int getTime(Game *pGame);
int getMilli(Game *pGame);
void updateGameTime(Game *pGame);
//void CheckCollison( Game *pGame, int zombieCount);
void add(IPaddress address, IPaddress clients[],int *pNrOfClients);
void executeCommand(Game *pGame,ClientData cData);
void sendGameData(Game *pGame);


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

    if (SDLNet_Init())
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        TTF_Quit();
        SDL_Quit();
		return 0;
	}

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

    pGame->pFont = TTF_OpenFont("resources/arial.ttf", 100);
    pGame->pScoreFont = TTF_OpenFont("resources/arial.ttf", 70);
    if(!pGame->pFont || !pGame->pScoreFont){
        printf("Error: %s\n",TTF_GetError());
        close(pGame);
        return 0;
    }

    if (!(pGame->pSocket = SDLNet_UDP_Open(2000)))
	{
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}
	if (!(pGame->pPacket = SDLNet_AllocPacket(512)))
	{
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}
    
    pGame->MoveUp=1;
    pGame->MoveLeft=0;
    pGame->MoveDown=0;
    pGame->MoveRight=0;

    for(int i=0;i<MAX_SPELARE;i++){
        //printf("%d", i);
        pGame->pSpelare[i] = createSpelare(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    pGame->nr_of_spelare = MAX_SPELARE;
   
    for(int i=0;i<MAX_SPELARE;i++){
        if(!pGame->pSpelare[i]){
            printf("Error: %s\n",SDL_GetError());
            close(pGame);
            return 0;
        }
    }
    
    pGame->pOverText = createText(pGame->pRenderer,238,168,65,pGame->pFont,"Game over",WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    pGame->pStartText = createText(pGame->pRenderer,238,168,65,pGame->pScoreFont,"Waiting for clients..",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);
    pGame->pTestText = createText(pGame->pRenderer,238,168,65,pGame->pFont,"hello world",WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    pGame->startTime = SDL_GetTicks64();
    pGame->gameTime = -1;
    pGame->state = START;
    pGame->nrOfClients = 0;

    return 1;
}



void run(Game *pGame){
    
    int keys[SDL_NUM_SCANCODES] = {0}; // Initialize an array to store key states
    int isRunning = 1;
    ClientData cData;
    SDL_Event event;
    int zombieCount = 0;      // Keep track of the current number of zombies
    Uint32 lastSpawnTime = 0; // Keep track of the time since the last zombie spawn
    while (isRunning)
    {   
        switch (pGame->state)
        {
            case ONGOING:

                sendGameData(pGame);
                while(SDLNet_UDP_Recv(pGame->pSocket,pGame->pPacket)==1){
                    memcpy(&cData, pGame->pPacket->data, sizeof(ClientData));
                    executeCommand(pGame,cData);
                }

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
                    }
                }

                for(int i=0;i<MAX_SPELARE;i++) updateSpelare(pGame->pSpelare[i]);
                
                updateGameTime(pGame);
                SDL_RenderClear(pGame->pRenderer);        
                SDL_RenderCopy(pGame->pRenderer, pGame->pbackgroundTexture, NULL, NULL);
                for(int i=0;i<MAX_SPELARE;i++)
                    drawSpelare(pGame->pSpelare[i]);
               
                if(pGame->pScoreText) {
                    drawText(pGame->pScoreText);
                }
                SDL_Delay(10);

                SDL_RenderPresent(pGame->pRenderer);
                /*if(getTime(pGame)==300){
                    isRunning = 0;
                }*/
                break;
            case START:
                drawText(pGame->pStartText);
                SDL_RenderPresent(pGame->pRenderer);
                    if(SDL_PollEvent(&event)){
                        if(event.type==SDL_QUIT) isRunning = 0;
                    } 
                    if(SDLNet_UDP_Recv(pGame->pSocket,pGame->pPacket)==1){
                        add(pGame->pPacket->address,pGame->clients,&(pGame->nrOfClients));
                        
                        if (pGame->nrOfClients==MAX_SPELARE)
                        {
                            for(int i=0;i<MAX_SPELARE;i++) resetSpelare(pGame->pSpelare[i]);
                            pGame->nr_of_spelare=MAX_SPELARE;
                            pGame->state = ONGOING;
                        } 
                    }
                break;
        }   
    }
}

void close(Game *pGame){
    stopMus();
    cleanMu();
    SDL_DestroyTexture(pGame->pbackgroundTexture);
    SDL_FreeSurface(pGame->pbackgroundImage);
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

/*void CheckCollison(Game *pGame, int zombieCount){
    int i;
    for(i = 0; i < zombieCount; i++){
        if((xBullet(pGame->pBullet) > pGame->zombieRect[i].x && xBullet(pGame->pBullet) < pGame->zombieRect[i].x + 30) && 
           (yBullet(pGame->pBullet) > pGame->zombieRect[i].y && yBullet(pGame->pBullet) < pGame->zombieRect[i].y + 30)){
            killBullet(pGame->pBullet);
        }
    }   
}*/

void add(IPaddress address, IPaddress clients[],int *pNrOfClients){
	for(int i=0;i<*pNrOfClients;i++){
        if(address.host==clients[i].host &&address.port==clients[i].port){
            return;
        }
    }
	clients[*pNrOfClients] = address;
	(*pNrOfClients)++;
}

void executeCommand(Game *pGame,ClientData cData){
    switch (cData.command)
    {   
        case UP:
            moveUp(pGame->pSpelare[cData.playerNumber]);
            pGame->MoveUp=1;
            pGame->MoveLeft=0;
            pGame->MoveDown=0;
            pGame->MoveRight=0;
            break;
        case LEFT:
            moveLeft(pGame->pSpelare[cData.playerNumber]);
            pGame->MoveLeft=1;
            pGame->MoveUp=0;
            pGame->MoveDown=0;
            pGame->MoveRight=0;
            break;
        case DOWN:
            moveDown(pGame->pSpelare[cData.playerNumber]);
            pGame->MoveDown=1;
            pGame->MoveLeft=0;
            pGame->MoveUp=0;
            pGame->MoveRight=0;
            break;
        case RIGHT:
            moveRight(pGame->pSpelare[cData.playerNumber]);
            pGame->MoveRight=1;
            pGame->MoveLeft=0;
            pGame->MoveDown=0;
            pGame->MoveUp=0;
            break;
        case FIRE:
            fireSpelare(pGame->pSpelare[cData.playerNumber], pGame->MoveUp, pGame->MoveLeft, pGame->MoveDown, pGame->MoveRight);
            break;
    }
}


void sendGameData(Game *pGame){
    pGame->sData.gState = pGame->state;
    for(int i=0;i<MAX_SPELARE;i++){
        getSpelareSendData(pGame->pSpelare[i], &(pGame->sData.spelare[i]));
    }
    for(int i=0;i<MAX_SPELARE;i++){
        pGame->sData.playerNr = i;
        memcpy(pGame->pPacket->data, &(pGame->sData), sizeof(ServerData));
		pGame->pPacket->len = sizeof(ServerData);
        pGame->pPacket->address = pGame->clients[i];
		SDLNet_UDP_Send(pGame->pSocket,-1,pGame->pPacket);
    }
}