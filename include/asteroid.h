#ifndef asteroid_h
#define asteroid_h

typedef struct asteroidImage AsteroidImage;
typedef struct asteroid Asteroid;

AsteroidImage *initiateAsteroids(SDL_Renderer *pRenderer);

//Asteroid createAsteroid(SDL_Renderer *renderer, int window_width, int window_height);
Asteroid *createAsteroid(AsteroidImage *pAsteroidImage, int window_width, int window_height);
void updateAsteroid(Asteroid *pAsteroid);
void drawAsteroid(Asteroid *pAsteroid);
void destroyAsteroid(Asteroid *pAsteroid);
SDL_Rect getRectAsteroid(Asteroid *pAsteroid);
void destroyAsteroidImage(AsteroidImage *pAsteroidImage);

#endif