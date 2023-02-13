#ifndef GAME_H
#define GAME_H
#include "utils.h"

enum CANDY_TYPE {

    CT_NONE = 0,
    CT_BLUE,
    CT_RED,
    CT_YELLOW,
    CT_GREEN,
    CT_ORANGE

}; 

enum DIRECTION {

    D_LEFT = 1,
    D_RIGHT,
    D_UP,
    D_DOWN

};

enum STATE {

    S_MENU = 0,
    S_PLAYING,
    S_HELP

};

typedef struct allegroEssencials {

    ALLEGRO_FONT *font;
    ALLEGRO_BITMAP *jewels[6];
    ALLEGRO_BITMAP *menu[5];
    ALLEGRO_BITMAP *background[5];
    int backgroundImage;
    FILE *score;

} allegroEssencials_t;

typedef struct mission {

    int candyType;
    int candiesDestroyed;
    int missionsCompleted;

} mission_t;

typedef struct candy {

    int type;
    int drawned;

} candy_t;

typedef struct slot {

    candy_t *candy;
    struct slot *left;
    struct slot *right;
    struct slot *up;
    struct slot *down;
    int xBoardPos, yBoardPos;
    int xDisplayPos, yDisplayPos;

} slot_t;


typedef struct board {

    int points;
    int bestScore;
    char strBestScore[STRING_SIZE];
    char strPoints[STRING_SIZE];
    int emptySlotsColumns[8];
    slot_t *slots[8][16];
    allegroEssencials_t *essencials;
    mission_t *mission;

} board_t;

board_t *createBoard();
int verifyMatch(board_t *board, int reset);
void changePositions(slot_t *slot, int direction);
void fillEmptySlots(board_t *board, int animation);
void resetDrawned(board_t *board);
int validTransition(slot_t *slot, int direction);
void resetBoard(board_t *board);

#endif
