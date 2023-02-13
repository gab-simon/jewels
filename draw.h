#ifndef DRAW_H
#define DRAW_H
#include "game.h"
#include "utils.h"

allegroEssencials_t *createEssencials(ALLEGRO_FONT *font, ALLEGRO_BITMAP *jewels[6], ALLEGRO_BITMAP *menu[5], ALLEGRO_BITMAP *background[5], FILE *score);
void drawPoints(board_t *board);
void transitingJewels(board_t *board, slot_t *slot, int direction);
void fallJewels(board_t *board);
void drawBoard(board_t *board);
void drawBackground(board_t *board, int xMouse, int yMouse);
void drawMenu(allegroEssencials_t *essencials, int xMouse, int yMouse);
void drawHelp(allegroEssencials_t *essencials);

#endif
