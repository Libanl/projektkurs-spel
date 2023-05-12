#ifndef bullet_h
#define bullet_h

#define BULLETLIFETIME 300
#define BULLETVELOCITY 10

typedef struct bullet Bullet;

Bullet *createBullet(SDL_Renderer *pRenderer, int window_width, int window_height);
void updateBullet(Bullet *pBullet);
void startBullet(Bullet *pBullet, float x, float y, int moveup, int moveleft, int movedown, int moveright);
void killBullet(Bullet *pBullet);
void drawBullet(Bullet *pBullet, SDL_Renderer *pRenderer);
float xBullet(Bullet *pBullet);
float yBullet(Bullet *pBullet);
void destroyBullet(Bullet *pBullet);
int aliveBullet(Bullet *pBullet);
SDL_Rect getRectBullet(Bullet *pBullet);
int collideBullet(Bullet *pBullet, SDL_Rect rect);

#endif