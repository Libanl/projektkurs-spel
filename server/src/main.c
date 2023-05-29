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
#include "../../lib/include/spelare.h"
#include "../../lib/include/powerup.h"


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 750
#define MAX_ZOMBIES 40


struct game{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Spelare *pSpelare[MAX_SPELARE];
    ZombieImage *pZombieImage;
    Zombie *pZombies[MAX_ZOMBIES];
    Powerup *pPowerup;
    SDL_Rect zombieRect[MAX_ZOMBIES];
    SDL_Surface *pbackgroundImage;
    SDL_Texture *pbackgroundTexture;
    SDL_Surface *pleaderboardImage;
    SDL_Texture *pleaderboardTexture;
    int plussa;
    int raknare;
    int Nrofzombies;
    int timeForNextZombie;
    int MoveUp;
    int MoveLeft;
    int MoveDown;
    int MoveRight;
    TTF_Font *pScoreFont, *pFont;
    Text *pScoreText, *pOverText, *pStartText, *pTestText,*pKillCountText1, *pKillCountText2, *pKillCountText3, *pKillCountText4;
    int startTime;
    int gameTime;
    int startpowerTime;
    int timeforPower;
    int powerTime;
    int nrOfKills;
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
int getPowertime(Game *pGame);
int getMilli(Game *pGame);
void showKillScore(Game *pGame, int nrOfkills[]);
void updateGameTime(Game *pGame);
void updateNrOfZombies(Game *pGame);
void resetZombies(Game *pGame);
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
    pGame->pleaderboardImage = IMG_Load("resources/latch.jpg");
    if (!pGame->pbackgroundImage)
    {
        printf("Error! backgroundImage failed\n", IMG_GetError());
        return 1;
    }

    pGame->pleaderboardTexture = SDL_CreateTextureFromSurface(pGame->pRenderer, pGame->pleaderboardImage);
    if (!pGame->pbackgroundTexture)
    {
        printf("Failed to create background texture: %s\n", SDL_GetError());
        SDL_FreeSurface(pGame->pbackgroundImage);
        return 1;
    }

    pGame->pFont = TTF_OpenFont("resources/arial.ttf", 100);
    pGame->pScoreFont = TTF_OpenFont("resources/arial.ttf", 40);
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

    pGame->pPowerup= createpowerup(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    for(int i=0;i<MAX_SPELARE;i++){
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

    pGame->pZombieImage = initiateZombie(pGame->pRenderer);

    
    pGame->pOverText = createText(pGame->pRenderer,238,168,65,pGame->pFont,"Time ran out",WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    pGame->pStartText = createText(pGame->pRenderer,0,255,0,pGame->pScoreFont,"Waiting for clients..",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);
    pGame->startTime = SDL_GetTicks64();
    pGame->gameTime = -1;
    pGame->state = START;

    pGame->nrOfClients = 0;

    pGame->Nrofzombies = 0;
    pGame->timeForNextZombie = 3;
    pGame->timeforPower = 5;
    pGame->nrOfKills=0;

    pGame->startpowerTime =SDL_GetTicks64();
    pGame->powerTime = -1;
    pGame->raknare = 0;
    pGame->plussa = 20;

    return 1;
}



void run(Game *pGame){
    
    int keys[SDL_NUM_SCANCODES] = {0}; // Initialize an array to store key states
    int isRunning = 1;
    int nrOfKills[MAX_SPELARE] = {0};
    ClientData cData;
    SDL_Event event;
    int zombieCount = 0;
    int leaderflag=0;      
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
                updateNrOfZombies(pGame);
                for (int i = 0; i < (pGame->Nrofzombies); i++) updateZombie(pGame->pZombies[i]);

                for(int i=0;i<MAX_SPELARE;i++) drawSpelare(pGame->pSpelare[i]);
                    
                for (int i = 0; i < pGame->Nrofzombies; i++) drawZombie(pGame->pZombies[i]);

                 if(getTime(pGame)>pGame->timeforPower){
                    drawChest(pGame->pPowerup);
                }
                
                for(int i=0; i<MAX_SPELARE; i++){
                    if (collideSpelare(pGame->pSpelare[i], getRectchest(pGame->pPowerup))){
                    removeChest(pGame->pPowerup);
                    pGame->startpowerTime = -1;
                    Powerspeed(pGame->pSpelare[i]);
                    pGame->startpowerTime = SDL_GetTicks64();  // Spara starttiden för kraften
                    }
                }
                

                if (pGame->startpowerTime != -1){  // Om kraften är aktiv
                    pGame->powerTime = SDL_GetTicks64(); 
                    if (getPowertime(pGame) >= 5){
                        for(int i=0; i<MAX_SPELARE; i++) regularspeed(pGame->pSpelare[i]);
                        pGame->startpowerTime = -1;  // Nollställ starttiden för kraften
                    }
                }

                if(getTime(pGame)==20 || getTime(pGame)==40){
                    int flag;
                    if(getTime(pGame)==20) flag = 0;
                    else if(getTime(pGame)==40) flag = 1;
                    newlocationchest(pGame->pPowerup, flag);
                }

                for (int k = 0; k < MAX_SPELARE; k++)
                {
                    for (int i = 0; i < pGame->Nrofzombies; i++)
                    {
                        if (collideSpelare(pGame->pSpelare[k], getRectZombie(pGame->pZombies[i])))
                        {
                            nrOfKills[k]++;
                            destroyZombie(pGame->pZombies[i]);
                            for (int j = i; j < pGame->Nrofzombies - 1; j++)
                            {
                                pGame->pZombies[j] = pGame->pZombies[j + 1];
                            }
                            pGame->Nrofzombies--;
                        }
                    }
                }
                
                if(pGame->pScoreText) {
                    drawText(pGame->pScoreText);
                }
                SDL_Delay(10);
                if(getTime(pGame)>60){
                    SDL_RenderCopy(pGame->pRenderer, pGame->pleaderboardTexture, NULL, NULL);
                    if(leaderflag==0)showKillScore(pGame, nrOfKills);
                    if(pGame->pKillCountText1){
                        drawText(pGame->pKillCountText1);
                    }
                    if(pGame->pKillCountText2){
                        drawText(pGame->pKillCountText2);
                    }
                    if(pGame->pKillCountText3){
                        drawText(pGame->pKillCountText3);
                    }
                    if(pGame->pKillCountText4){
                        drawText(pGame->pKillCountText4);
                    }
                    leaderflag=1;
                }
                if(getTime(pGame)==70){
                    pGame->state=GAME_OVER;
                }
                SDL_RenderPresent(pGame->pRenderer);
              
                break;
            case GAME_OVER:
            SDL_RenderCopy(pGame->pRenderer, pGame->pleaderboardTexture, NULL, NULL);
            drawText(pGame->pOverText);
            sendGameData(pGame);
            if(pGame->nrOfClients==MAX_SPELARE) pGame->nrOfClients = 0;
            pGame->Nrofzombies=0;
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
                            pGame->startTime = SDL_GetTicks64();
                            pGame->gameTime = -1;
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
    for(int i=0;i<MAX_SPELARE;i++) if(pGame->pSpelare[i]) destroySpelare(pGame->pSpelare[i]);
    resetZombies(pGame);
    if(pGame->pZombieImage) destroyZombieImage(pGame->pZombieImage);
    SDL_DestroyTexture(pGame->pbackgroundTexture);
    SDL_FreeSurface(pGame->pbackgroundImage);
    SDL_DestroyTexture(pGame->pleaderboardTexture);
    SDL_FreeSurface(pGame->pleaderboardImage);
    SDL_DestroyRenderer(pGame->pRenderer);
    SDL_DestroyWindow(pGame->pWindow);
    if(pGame->pKillCountText2) destroyText(pGame->pKillCountText1);
    if(pGame->pKillCountText2) destroyText(pGame->pKillCountText2);
    if(pGame->pKillCountText3) destroyText(pGame->pKillCountText3);
    if(pGame->pKillCountText4) destroyText(pGame->pKillCountText4);
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

int getPowertime(Game *pGame)
{
    return (SDL_GetTicks64() - pGame->startpowerTime) / 1000;
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

void showKillScore(Game *pGame, int nrOfkills[]) {

    // Destroy existing text objects if they exist
    if (pGame->pKillCountText1) {
        destroyText(pGame->pKillCountText1);
    }
    if (pGame->pKillCountText2) {
        destroyText(pGame->pKillCountText2);
    }
    if (pGame->pKillCountText3) {
        destroyText(pGame->pKillCountText3);
    }
    if (pGame->pKillCountText4) {
        destroyText(pGame->pKillCountText4);
    }

    static char KillCountString1[30];
    static char KillCountString2[30];
    static char KillCountString3[30];
    static char KillCountString4[30];

    // Convert kill scores to strings
    sprintf(KillCountString1, "%d", nrOfkills[0]);
    sprintf(KillCountString2, "%d", nrOfkills[1]);
    sprintf(KillCountString3, "%d", nrOfkills[2]);
    sprintf(KillCountString4, "%d", nrOfkills[3]);

    if (pGame->pScoreFont) {
        // Create new text objects with the kill scores
        pGame->pKillCountText1 = createText(pGame->pRenderer, 255, 255, 255, pGame->pScoreFont, KillCountString1, WINDOW_WIDTH-175, 250);
        pGame->pKillCountText2 = createText(pGame->pRenderer, 255, 255, 255, pGame->pScoreFont, KillCountString2, WINDOW_WIDTH-175, 300);
        pGame->pKillCountText3 = createText(pGame->pRenderer, 255, 255, 255, pGame->pScoreFont, KillCountString3, WINDOW_WIDTH-175, 350);
        pGame->pKillCountText4 = createText(pGame->pRenderer, 255, 255, 255, pGame->pScoreFont, KillCountString4, WINDOW_WIDTH-175, 400);
    }
}


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
            break;
        case LEFT:
            moveLeft(pGame->pSpelare[cData.playerNumber]);
            break;
        case DOWN:
            moveDown(pGame->pSpelare[cData.playerNumber]);
            break;
        case RIGHT:
            moveRight(pGame->pSpelare[cData.playerNumber]);
            break;
    }
}

void resetZombies(Game *pGame)
{
    for (int i = 0; i < pGame->Nrofzombies; i++)
    {
        destroyZombie(pGame->pZombies[i]);  
    }
}

void updateNrOfZombies(Game *pGame)
{
    if (getTime(pGame) > pGame->timeForNextZombie && pGame->Nrofzombies < MAX_ZOMBIES)
    {
        if(pGame->raknare<5)
        {
            (pGame->timeForNextZombie) = (pGame->timeForNextZombie) + 1; // seconds till next asteroid
            pGame->pZombies[pGame->Nrofzombies] = createZombieleft(pGame->pZombieImage, WINDOW_WIDTH, WINDOW_HEIGHT - pGame->plussa);
        }
        else if(pGame->raknare>=5 && pGame->raknare<10)
        {
            (pGame->timeForNextZombie) = (pGame->timeForNextZombie) + 1; // seconds till next asteroid
            pGame->pZombies[pGame->Nrofzombies] = createZombierright(pGame->pZombieImage, WINDOW_WIDTH, WINDOW_HEIGHT - pGame->plussa);
        }
        else if(pGame->raknare>=10 && pGame->raknare<15)
        {
            (pGame->timeForNextZombie) = (pGame->timeForNextZombie) + 1; // seconds till next asteroid
            pGame->pZombies[pGame->Nrofzombies] = createZombierup(pGame->pZombieImage, WINDOW_WIDTH - pGame->plussa, WINDOW_HEIGHT);
        }
        else if(pGame->raknare>=15 && pGame->raknare<20)
        {
            (pGame->timeForNextZombie) = (pGame->timeForNextZombie) + 1; // seconds till next asteroid
            pGame->pZombies[pGame->Nrofzombies] = createZombiedown(pGame->pZombieImage, WINDOW_WIDTH - pGame->plussa, WINDOW_HEIGHT);
        }
        pGame->Nrofzombies++;
        pGame->raknare++;
        pGame->plussa=pGame->plussa+300;
        if(pGame->raknare==19)
        {
            pGame->raknare=0;
        }
        if(WINDOW_HEIGHT<pGame->plussa)
        {
            pGame->plussa=0;
        }
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