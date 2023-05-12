#ifndef spelare_data_h
#define spelare_data_h
#define MAX_SPELARE 2

enum gameState{START, ONGOING, GAME_OVER};
typedef enum gameState GameState;

enum clientCommand{READY, UP, DOWN, LEFT, RIGHT, FIRE};
typedef enum clientCommand ClientCommand;

struct clientData{
    ClientCommand command;
    int playerNumber;
};
typedef struct clientData ClientData;

/*struct bulletData{
    float x, y, vx, vy;
    int time;
};
typedef struct bulletData BulletData;*/

struct spelareData{
    int x, y;
    //BulletData bData;
};
typedef struct spelareData SpelareData;   

struct serverData{
    SpelareData spelare[MAX_SPELARE];
    int playerNr;
    GameState gState;
};
typedef struct serverData ServerData;



#endif