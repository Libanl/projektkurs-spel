#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include "../../lib/include/text.h"
#include "../../lib/include/music.h"
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
    SDL_Rect zombieRect[MAX_ZOMBIES];
    ZombieImage *pZombieImage;
    Zombie *pZombies[MAX_ZOMBIES];
    Powerup *pPowerup;
    int nrOfKills;
    int raknare;
    int plussa;
    int Nrofzombies;
    int timeForNextZombie;
    SDL_Surface *pGame_StartBackgroundimage;
    SDL_Texture *pGame_StartbackgroundTexture;
    SDL_Surface *pleaderboardImage;
    SDL_Texture *pleaderboardTexture;
    SDL_Surface *pbackgroundImage;
    SDL_Texture *pbackgroundTexture;
    SDL_Surface *TextSurface;
    SDL_Texture *TextTexture;
    int mouseState;
    TTF_Font *pScoreFont, *pFont, *pOverFont;
    Text *pScoreText, *pOverText, *pStartText, *pWaitingText, *pTestText, *pPlayerNrText, *pKillCountText1, *pKillCountText2, *pKillCountText3, *pKillCountText4, *pIDText;
    int gameTimeM;
    int startTime;//in ms
    int gameTime;//in s
    int startpowerTime;
    int timeforPower;
    int powerTime;
    GameState state;

    UDPsocket pSocket;
	IPaddress serverAddress;
	UDPpacket *pPacket;
    int nr_of_spelare, spelareNr;

};
typedef struct game Game;

int initiate(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(SDL_Event *pEvent, Game *pGame, int keys[]);
int getTime(Game *pGame);
int getPowertime(Game *pGame);
int getMilli(Game *pGame);
void updateGameTime(Game *pGame);
void showKillScore(Game *pGame, int nrOfkills[]);
void CheckCollison( Game *pGame, int zombieCount);
void updateNrOfZombies(Game *pGame);
void resetZombies(Game *pGame);
void updateWithServerData(Game *pGame);

 
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

    if (SDLNet_Init())
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
        TTF_Quit();
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

    pGame->pGame_StartBackgroundimage = IMG_Load("resources/iFi08.png");
    if (!pGame->pGame_StartBackgroundimage)
    {
        printf("Error! backgroundImage failed hej\n", IMG_GetError());
        return 1;
    }

    pGame->pGame_StartbackgroundTexture = SDL_CreateTextureFromSurface(pGame->pRenderer, pGame->pGame_StartBackgroundimage);
    if (!pGame->pGame_StartbackgroundTexture)
    {
        printf("Failed to create background texture: %s\n", SDL_GetError());
        SDL_FreeSurface(pGame->pGame_StartBackgroundimage);
        return 1;
    }

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
    pGame->pOverFont = TTF_OpenFont("resources/arial.ttf", 40);
    if(!pGame->pFont || !pGame->pScoreFont){
        printf("Error: %s\n",TTF_GetError());
        close(pGame);
        return 0;
    }
    
    if (!(pGame->pSocket = SDLNet_UDP_Open(0)))//0 means not a server
	{
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		return 0;
	}
	if (SDLNet_ResolveHost(&(pGame->serverAddress), "127.0.0.1", 2000))
	{
		printf("SDLNet_ResolveHost(127.0.0.1 2000): %s\n", SDLNet_GetError());
		return 0;
	}
    if (!(pGame->pPacket = SDLNet_AllocPacket(512)))
	{
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		return 0;
	}
    pGame->pPacket->address.host = pGame->serverAddress.host;
    pGame->pPacket->address.port = pGame->serverAddress.port;
    
    pGame->pPowerup= createpowerup(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    for (int i = 0; i < MAX_SPELARE; i++){
        pGame->pSpelare[i] = createSpelare(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    pGame->pZombieImage = initiateZombie(pGame->pRenderer);

    pGame->nr_of_spelare = MAX_SPELARE;
    pGame->pOverText = createText(pGame->pRenderer,238,168,65,pGame->pFont,"Time ran out",WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    pGame->pStartText = createText(pGame->pRenderer, 238, 168, 65, pGame->pOverFont, "Press space to start OR M (TO GO BACK TO MENU)", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100);
    pGame->pWaitingText = createText(pGame->pRenderer,0,255,0,pGame->pScoreFont,"Waiting for server...",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);
    pGame->pTestText = createText(pGame->pRenderer,238,168,65,pGame->pFont,"hello world",WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    for(int i=0;i<MAX_SPELARE;i++){
        if(!pGame->pSpelare[i]){
            printf("Error: %s\n",SDL_GetError());
            close(pGame);
            return 0;
        }
    }

    pGame->Nrofzombies = 0;
    pGame->timeForNextZombie = 3;
    pGame->timeforPower = 5;
    pGame->nrOfKills = 0;
    //resetZombies(pGame);
    pGame->startTime = SDL_GetTicks64();
    pGame->gameTime = -1;
    pGame->state = START;
    pGame->startpowerTime =SDL_GetTicks64();
    pGame->powerTime = -1;
    pGame->raknare = 0;
    pGame->plussa = 20;
    return 1;
}



void run(Game *pGame){
    
    int keys[SDL_NUM_SCANCODES] = {0}; // Initialize an array to store key states
    int nrOfKills[MAX_SPELARE];
    int isRunning = 1;
    int first = 1;
    int pressed = 0;
    int mouseX, mouseY;
    int XPos, YPos;
    int check=0;
    int leaderflag=0;
    SDL_Event event;
    ServerData sData;
    ClientData cData;
    int joining=0;
    int zombieCount = 0;      // Keep track of the current number of zombies
    Uint32 lastSpawnTime = 0; // Keep track of the time since the last zombie spawn
    Uint32 endTime;
    while (isRunning)
    {   
        switch (pGame->state)
        {
            case ONGOING:
                while(SDLNet_UDP_Recv(pGame->pSocket, pGame->pPacket)){
                    updateWithServerData(pGame);
                }			                    
                if(SDL_PollEvent(&event)){   
                    switch (event.type){
                    case SDL_QUIT:
                        isRunning = 0;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            isRunning = 0;
                        }
                    default: 
                    handleInput(&event,pGame,keys);
                        break;
                    }
                }
                for (int i = 0; i < MAX_SPELARE; i++) updateSpelare(pGame->pSpelare[i]);
                SDL_RenderClear(pGame->pRenderer);
                updateNrOfZombies(pGame);
                
                for (int i = 0; i < (pGame->Nrofzombies); i++){
                    updateZombie(pGame->pZombies[i]);
                }
                updateGameTime(pGame);
                SDL_RenderCopy(pGame->pRenderer, pGame->pbackgroundTexture, NULL, NULL);

              
                

                for (int i = 0; i < pGame->Nrofzombies; i++){
                    drawZombie(pGame->pZombies[i]);
                }

                if(pGame->pScoreText) {
                    drawText(pGame->pScoreText);
                }
                if(pGame->pIDText){
                    drawText(pGame->pIDText);
                }
                for (int i = 0; i < MAX_SPELARE; i++){
                     drawSpelare(pGame->pSpelare[i]); 
                }
                
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
                //endTime = SDL_GetTicks();
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
                //SDL_Delay(10);
                break;
            case GAME_OVER:
                drawText(pGame->pOverText);
                drawText(pGame->pStartText);
                for (int i = 0; i < MAX_SPELARE; i++) resetSpelare(pGame->pSpelare[i]);
                pGame->Nrofzombies = 0;
                //pressed = 0;
            case START:
                if(!joining){
                    SDL_RenderCopy(pGame->pRenderer, pGame->pGame_StartbackgroundTexture, NULL, NULL);
                }
                else{
                    SDL_SetRenderDrawColor(pGame->pRenderer,0,0,0,255);
                    SDL_RenderClear(pGame->pRenderer);
                    SDL_SetRenderDrawColor(pGame->pRenderer,230,230,230,255);
                    drawText(pGame->pWaitingText);
                }
                SDL_RenderPresent(pGame->pRenderer);
                if(SDL_PollEvent(&event)){
                    if(event.type==SDL_QUIT) isRunning = 0;
                    if(event.type==SDL_MOUSEMOTION) pGame->mouseState = SDL_GetMouseState(&XPos, &YPos);
                    if(event.type==SDL_MOUSEBUTTONDOWN){ 
                        SDL_GetMouseState(&mouseX, &mouseY);
                        if (mouseX >= 149 && mouseX <= 439 && mouseY >= 134 && mouseY <= 217){
                            pressed=1;  
                            joining = 1;
                            cData.command=READY;
                            cData.playerNumber=-1;
                            memcpy(pGame->pPacket->data, &cData, sizeof(ClientData));
                            pGame->pPacket->len = sizeof(ClientData);
                        }
                        if (mouseX >= 149 && mouseX <= 437 && mouseY >= 433 && mouseY <= 510){
                            isRunning = 0;
                        }
                    }
                }
                if(joining) SDLNet_UDP_Send(pGame->pSocket, -1,pGame->pPacket);
                if(SDLNet_UDP_Recv(pGame->pSocket, pGame->pPacket)){
                    updateWithServerData(pGame);
                    pGame->startTime = SDL_GetTicks64();
                    pGame->gameTime = -1;
                    if(pGame->state == ONGOING) joining = 0;
                }                
                break;
        }   
    }
}



void updateWithServerData(Game *pGame){
    ServerData sData;
    memcpy(&sData, pGame->pPacket->data, sizeof(ServerData));
    pGame->spelareNr = sData.playerNr;
    pGame->state = sData.gState;
    for(int i=0;i<MAX_SPELARE;i++){
        updateSpelareWithRecievedData(pGame->pSpelare[i],&(sData.spelare[i]));
    }
}

// Function to handle input
void handleInput(SDL_Event *pEvent, Game *pGame, int keys[]) {
    if (pEvent->type == SDL_KEYDOWN) {
        ClientData cData;
        cData.playerNumber = pGame->spelareNr;
        
        if (pEvent->key.keysym.scancode==SDL_SCANCODE_W) {
            moveUp(pGame->pSpelare[pGame->spelareNr]);
            cData.command = UP;
        } if (pEvent->key.keysym.scancode==SDL_SCANCODE_A) {
            moveLeft(pGame->pSpelare[pGame->spelareNr]);
            cData.command = LEFT;
        } if (pEvent->key.keysym.scancode==SDL_SCANCODE_S) {
            moveDown(pGame->pSpelare[pGame->spelareNr]);
            cData.command = DOWN;
        } if (pEvent->key.keysym.scancode==SDL_SCANCODE_D) {
            moveRight(pGame->pSpelare[pGame->spelareNr]);
            cData.command = RIGHT;
        }
        memcpy(pGame->pPacket->data, &cData, sizeof(ClientData));
		pGame->pPacket->len = sizeof(ClientData);
        SDLNet_UDP_Send(pGame->pSocket, -1,pGame->pPacket);

    }       
} 

void close(Game *pGame){
    stopMus();
    cleanMu();
    for(int i=0;i<MAX_SPELARE;i++) if(pGame->pSpelare[i]) destroySpelare(pGame->pSpelare[i]);
    if(pGame->pPowerup) destroyPowerup(pGame->pPowerup);
    resetZombies(pGame);
    if(pGame->pZombieImage) destroyZombieImage(pGame->pZombieImage);
    SDL_DestroyTexture(pGame->pGame_StartbackgroundTexture);
    SDL_FreeSurface(pGame->pGame_StartBackgroundimage);
    SDL_DestroyTexture(pGame->pleaderboardTexture);
    SDL_FreeSurface(pGame->pleaderboardImage);
    SDL_DestroyTexture(pGame->pbackgroundTexture);
    SDL_FreeSurface(pGame->pbackgroundImage);
    SDL_DestroyRenderer(pGame->pRenderer);
    SDL_DestroyWindow(pGame->pWindow);
    if(pGame->pKillCountText1) destroyText(pGame->pKillCountText1);
    if(pGame->pKillCountText2) destroyText(pGame->pKillCountText2);
    if(pGame->pKillCountText3) destroyText(pGame->pKillCountText3);
    if(pGame->pKillCountText4) destroyText(pGame->pKillCountText4);
    if(pGame->pKillCountText4) destroyText(pGame->pIDText);
    if(pGame->pScoreText) destroyText(pGame->pScoreText);
    if(pGame->pOverText) destroyText(pGame->pOverText);
    if(pGame->pStartText) destroyText(pGame->pStartText);
    if(pGame->pWaitingText) destroyText(pGame->pWaitingText);   
    if(pGame->pFont) TTF_CloseFont(pGame->pFont);
    if(pGame->pOverFont) TTF_CloseFont(pGame->pOverFont);
    if(pGame->pScoreFont) TTF_CloseFont(pGame->pScoreFont);
    SDLNet_Quit();
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

void updateGameTime(Game *pGame) {
    if (pGame->pScoreText) {
        destroyText(pGame->pScoreText);
    }
    static char scoreString[30];
    static char IDstring[30];
    sprintf(scoreString, "%d.%d", getTime(pGame), getMilli(pGame));
    sprintf(IDstring, "Player %d", (pGame->spelareNr+1));
    if (pGame->pScoreFont) {
        pGame->pScoreText = createText(pGame->pRenderer, 255, 255, 255, pGame->pScoreFont, scoreString, WINDOW_WIDTH - 75, 50);
        pGame->pIDText = createText(pGame->pRenderer, 255, 255, 255, pGame->pScoreFont, IDstring, WINDOW_WIDTH - 500, 50);
    }
}


void showKillScore(Game *pGame, int nrOfkills[]) {
    // Print the first kill score
    //printf("%d\n", nrOfkills[0]);

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

void resetZombies(Game *pGame)
{
    for (int i = 0; i < pGame->Nrofzombies; i++)
    {
        destroyZombie(pGame->pZombies[i]);
    }
    pGame->Nrofzombies = 0;
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