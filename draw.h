#ifndef DRAW_H
#define DRAW_H
#include "game.h"
#include "utils.h"

allegroEssencials_t *createEssencials(ALLEGRO_FONT *fontsBig, ALLEGRO_FONT *fontsMedium, ALLEGRO_FONT *fontsSmall, ALLEGRO_BITMAP *jewels[6], ALLEGRO_BITMAP *background, FILE *score);
void drawPoints(board_t *board);
void transitingJewels(board_t *board, slot_t *slot, int direction);
void fallJewels(board_t *board);
void drawBoard(board_t *board);
void drawBackground(board_t *board, int xMouse, int yMouse);
void drawMenu(allegroEssencials_t *essencials, int xMouse, int yMouse);
void buildInfo(allegroEssencials_t *essencials, board_t *board);

#endif
