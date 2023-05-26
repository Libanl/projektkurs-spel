#ifndef spelare_data_h
#define spelare_data_h

#define MAX_ZOMBIES 40
#define MAX_SPELARE 4

enum gameState{START, ONGOING, GAME_OVER};
typedef enum gameState GameState;

enum clientCommand{READY, UP, DOWN, LEFT, RIGHT, FIRE};
typedef enum clientCommand ClientCommand;

struct clientData{
    ClientCommand command;
    int playerNumber;
};
typedef struct clientData ClientData;

struct spelareData{
    int x, y, nrOfKills;
};
typedef struct spelareData SpelareData;  

struct zombieData{
    int x, y;
    int vx, vy;
};
typedef struct zombieData ZombieData;

struct serverData{
    SpelareData spelare[MAX_SPELARE];
    int playerNr;
    GameState gState;
};
typedef struct serverData ServerData;



#endif