#ifndef GAME_H
#define GAME_H
#include "utils.h"

enum TYPE_FLOWER
{
    FLOWER_NONE = 0,
    FLOWER_BLUE,
    FLOWER_RED,
    FLOWER_YELLOW,
    FLOWER_GREEN,
    FLOWER_ORANGE
};

enum DIRECTION
{
    DIR_LEFT = 1,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
};

typedef enum STATES
{
    MENU,
    SERVING,
    INFO,
    EXIT
} gameStates_t;

typedef struct
{
    ALLEGRO_FONT *big;
    ALLEGRO_FONT *medium;
    ALLEGRO_FONT *small;
} fonts_t;

typedef struct allegroEssencials
{
    fonts_t *fonts;
    ALLEGRO_BITMAP *jewels[6];
    ALLEGRO_BITMAP *background;
    FILE *score;
} essencials_t;

typedef struct tasks
{
    int flowersType;
    int flowersDestroy;
} mission_t;

typedef struct flower
{
    int type;
    int shown;
} flower_t;

typedef struct slot
{
    flower_t *flower;
    struct slot *left;
    struct slot *right;
    struct slot *up;
    struct slot *down;
    int xBoardPos, yBoardPos;
    int xDisplayPos, yDisplayPos;
} slot_t;

typedef struct board
{
    int points;
    int bestScore;
    int level;
    int emptySlotsColumns[8];
    char strBestScore[STRING_BUFFER];
    char strPoints[STRING_BUFFER];
    char strLevel[STRING_BUFFER];
    slot_t *slots[8][16];
    essencials_t *essencials;
    mission_t *tasks;
    gameStates_t STATES;
} board_t;

board_t *createBoard();
int checkMatch(board_t *board, int reset);
int checkSwap(slot_t *slot, int direction);
void swapPositions(slot_t *slot, int direction);
void putSpacesVague(board_t *board, int animation);
void restartDraw(board_t *board);
void restartGame(board_t *board);

#endif
