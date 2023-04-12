#ifndef rocket_h
#define rocket_h

typedef struct rocket Rocket;

Rocket *createRocket(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height);
void updateRocket(Rocket *pRocket);
void drawRocket(Rocket *pRocket);
void destroyRocket(Rocket *pRocket);
int collideRocket(Rocket *pRocket, SDL_Rect rect);
void resetRocket(Rocket *pRocket);
void turnLeft(Rocket *pRocket);
void turnRight(Rocket *pRocket);
void accelerate(Rocket *pRocket);

#endif