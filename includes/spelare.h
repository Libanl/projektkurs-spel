#ifndef spelare_h
#define spelare_h



typedef struct spelare Spelare;

Spelare *createSpelare(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height);
void updateSpelare(Spelare *pSpelare);
void drawSpelare(Spelare *pSpelare);
void destroySpelare(Spelare *pSpelare); 
void moveUp(Spelare *pSpelare);         
void moveDown(Spelare *pSpelare);
void moveLeft(Spelare *pSpelare);
void moveRight(Spelare *pSpelare);


#endif