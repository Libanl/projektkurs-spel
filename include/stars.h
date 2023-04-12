#ifndef stars_h
#define stars_h

typedef struct stars Stars;

Stars *createStars(int nr_of_stars, int width, int height);
void drawStars(Stars *pStars, SDL_Renderer *pRenderer);
void destroyStars(Stars *pStars);


#endif