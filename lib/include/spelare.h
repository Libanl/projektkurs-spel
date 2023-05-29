#ifndef spelare_h
#define spelare_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct spelare Spelare;

Spelare *createSpelare(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height);
void updateSpelare(Spelare *pSpelare);
void drawSpelare(Spelare *pSpelare);
void destroySpelare(Spelare *pSpelare);
void moveUp(Spelare *pSpelare);
void moveDown(Spelare *pSpelare);
void moveLeft(Spelare *pSpelare);
void moveRight(Spelare *pSpelare);
void increaseKillCount(Spelare *pSpelare);
int getKillCount(Spelare *pSpelare);
void resetSpelare(Spelare *pSpelare);
int collideSpelare(Spelare *pSpelare, SDL_Rect rect);
void getSpelareSendData(Spelare *pSpelare, SpelareData *pSpelareData);
void updateSpelareWithRecievedData(Spelare *pSpelare, SpelareData *pSpelareData);
void Powerspeed(Spelare *pSpelare);
void regularspeed(Spelare *pSpelare);
#endif