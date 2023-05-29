#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../lib/include/spelare_data.h"
#include "../../lib/include/spelare.h"

struct spelare
{
    float x, y, vx, vy;
    int angle;
    int frame;
    int nrOfKills;
    int nyframe;
    int alive;
    int window_width, window_height;
    SpelareData *pSpelareData;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect shipRect[16];
    SDL_Rect gsprites[16];
    SDL_RendererFlip flip;
    int hastighet;
};

Spelare *createSpelare(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Spelare *pSpelare = malloc(sizeof(struct spelare));
    pSpelare->vx = pSpelare->vy = 0;
    pSpelare->angle = 0;
    pSpelare->nrOfKills=0;
    pSpelare->hastighet = 5;
    pSpelare->frame = 6;
    pSpelare->nyframe = 6;
    pSpelare->window_width = window_width;
    pSpelare->window_height = window_height;
    pSpelare->flip = SDL_FLIP_NONE;
    pSpelare->pRenderer = pRenderer;
    SDL_Surface *pSurface = IMG_Load("resources/soldiertest.png");
    if (!pSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pSpelare->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if (!pSpelare->pTexture)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(pSurface);
    SDL_QueryTexture(pSpelare->pTexture, NULL, NULL, &(pSpelare->shipRect[pSpelare->frame].w), &(pSpelare->shipRect[pSpelare->frame].h));
    // Down spriteSheets
    pSpelare->gsprites[0].x = 0;
    pSpelare->gsprites[0].y = 0;
    pSpelare->gsprites[0].w = 96;
    pSpelare->gsprites[0].h = 70;

    pSpelare->gsprites[1].x = 288;
    pSpelare->gsprites[1].y = 0;
    pSpelare->gsprites[1].w = 96;
    pSpelare->gsprites[1].h = 70;

    // Left spriteSheets
    pSpelare->gsprites[2].x = 288;
    pSpelare->gsprites[2].y = 140;
    pSpelare->gsprites[2].w = 96;
    pSpelare->gsprites[2].h = 70;

    pSpelare->gsprites[3].x = 480;
    pSpelare->gsprites[3].y = 140;
    pSpelare->gsprites[3].w = 96;
    pSpelare->gsprites[3].h = 70;

    // Right spriteSheets
    pSpelare->gsprites[4].x = 0;
    pSpelare->gsprites[4].y = 210;
    pSpelare->gsprites[4].w = 96;
    pSpelare->gsprites[4].h = 70;

    pSpelare->gsprites[5].x = 288;
    pSpelare->gsprites[5].y = 210;
    pSpelare->gsprites[5].w = 96;
    pSpelare->gsprites[5].h = 70;

    // Up spriteSheets
    pSpelare->gsprites[6].x = 480;
    pSpelare->gsprites[6].y = 0;
    pSpelare->gsprites[6].w = 96;
    pSpelare->gsprites[6].h = 70;

    for (int i = 0; i < 7; i++)
    {
        pSpelare->shipRect[i] = pSpelare->gsprites[i];
        pSpelare->shipRect[i].w = pSpelare->shipRect[i].w / 1.5;
        pSpelare->shipRect[i].h = pSpelare->shipRect[i].h / 1.5;
        pSpelare->x = x - pSpelare->shipRect[i].w;
        pSpelare->y = y - pSpelare->shipRect[i].h;
    }
    return pSpelare;
}

void moveLeft(Spelare *pSpelare)
{

    if (pSpelare->shipRect[pSpelare->frame].x > 0)
    {
        pSpelare->x -= pSpelare->hastighet;
    }
    pSpelare->flip = SDL_FLIP_HORIZONTAL;
    if (pSpelare->frame == 2)
    {
        pSpelare->nyframe = 3;
    }
    else
    {
        pSpelare->nyframe = 2;
    }
    updateSpelare(pSpelare);
}

void moveRight(Spelare *pSpelare)
{

    if (pSpelare->shipRect[pSpelare->frame].x < 970)
    {
        pSpelare->x += pSpelare->hastighet;
    }
    pSpelare->flip = SDL_FLIP_NONE;
    if (pSpelare->frame == 2)
    {
        pSpelare->nyframe = 3;
    }
    else
    {
        pSpelare->nyframe = 2;
    }
    updateSpelare(pSpelare);
}

void moveUp(Spelare *pSpelare)
{

    if (pSpelare->shipRect[pSpelare->frame].y > 0)
    {
        pSpelare->y -= pSpelare->hastighet;
    }
    pSpelare->flip = SDL_FLIP_NONE;
    if (pSpelare->frame == 4)
    {
        pSpelare->nyframe = 5;
    }
    else
    {
        pSpelare->nyframe = 4;
    }
    updateSpelare(pSpelare);
}

void moveDown(Spelare *pSpelare)
{

    if (pSpelare->shipRect[pSpelare->frame].y < 700)
    {
        pSpelare->y += pSpelare->hastighet;
    }
    pSpelare->flip = SDL_FLIP_NONE;
    if (pSpelare->frame == 0)
    {
        pSpelare->nyframe = 1;
    }
    else
    {
        pSpelare->nyframe = 0;
    }
    updateSpelare(pSpelare);
}

void increaseKillCount(Spelare *pSpelare){
    pSpelare->nrOfKills++;
}

int getKillCount(Spelare *pSpelare){
    return pSpelare->nrOfKills;
}

void updateSpelare(Spelare *pSpelare)
{
    pSpelare->shipRect[pSpelare->frame].x = pSpelare->x;
    pSpelare->shipRect[pSpelare->frame].y = pSpelare->y;
    pSpelare->frame = pSpelare->nyframe;
}

void drawSpelare(Spelare *pSpelare)
{
    SDL_RenderCopyEx(pSpelare->pRenderer, pSpelare->pTexture, &(pSpelare->gsprites[pSpelare->frame]), &(pSpelare->shipRect[pSpelare->frame]), pSpelare->angle, NULL, pSpelare->flip);
}

void destroySpelare(Spelare *pSpelare)
{
    SDL_DestroyTexture(pSpelare->pTexture);
    free(pSpelare);
}

void resetSpelare(Spelare *pSpelare)
{
    pSpelare->x = pSpelare->window_width / 2;
    pSpelare->y = pSpelare->window_height / 2;
    pSpelare->angle=0;
    pSpelare->vx=pSpelare->vy=0;
    pSpelare->nyframe=6;
    pSpelare->hastighet=5;
}

void Powerspeed(Spelare *pSpelare)
{
    pSpelare->hastighet = 13;
}

void regularspeed(Spelare *pSpelare)
{
    pSpelare->hastighet = 5;
}

int collideSpelare(Spelare *pSpelare, SDL_Rect rect)
{
    int player_center_x = pSpelare->x + pSpelare->shipRect[pSpelare->frame].w / 2;
    int player_center_y = pSpelare->y + pSpelare->shipRect[pSpelare->frame].h / 2;

    int rect_center_x = rect.x + rect.w / 2;
    int rect_center_y = rect.y + rect.h / 2;

    int dx = abs(player_center_x - rect_center_x);
    int dy = abs(player_center_y - rect_center_y);

    int half_w = pSpelare->shipRect[pSpelare->frame].w / 2 + rect.w / 2;
    int half_h = pSpelare->shipRect[pSpelare->frame].h / 2 + rect.h / 2;

    if (dx > half_w || dy > half_h)
    {
        return 0; // no collision
    }

    int px = abs(dx - half_w);
    int py = abs(dy - half_h);

    if (px <= py)
    {
        if (player_center_x < rect_center_x)
        {
            pSpelare->x -= px;
        }
        else
        {
            pSpelare->x += px;
        }
    }
    else
    {
        if (player_center_y < rect_center_y)
        {
            pSpelare->y -= py;
        }
        else
        {
            pSpelare->y += py;
        }
    }

    return 1; // collision detected
}

void getSpelareSendData(Spelare *pSpelare, SpelareData *pSpelareData)
{
    pSpelareData->x = pSpelare->x;
    pSpelareData->y = pSpelare->y;
}

void updateSpelareWithRecievedData(Spelare *pSpelare, SpelareData *pSpelareData)
{
    pSpelare->x = pSpelareData->x;
    pSpelare->y = pSpelareData->y;
}