#ifndef spelare_h
#define spelare_h



typedef struct spelare Spelare;

Spelare *createSpelare(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height);
void drawSpelare(Spelare *pSpelare);
void destroySpelare(Spelare *pSpelare);

#endif