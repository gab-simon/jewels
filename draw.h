#ifndef DRAW_H
#define DRAW_H
#include "game.h"
#include "utils.h"

essencials_t *createEssencials(ALLEGRO_FONT *fontsBig, ALLEGRO_FONT *fontsMedium, ALLEGRO_FONT *fontsSmall, ALLEGRO_BITMAP *jewels[6], ALLEGRO_BITMAP *background, FILE *score);
void buildHUD(board_t *board);
void muteSound(bool mute_bool);
void transitingJewels(board_t *board, slot_t *slot, int direction);
void fallJewels(board_t *board);
void buildBoard(board_t *board);
void buildBackground(board_t *board, int xMouse, int yMouse);
void drawMenu(essencials_t *essencials, int xMouse, int yMouse);
void buildInfo(essencials_t *essencials, board_t *board);

#endif
