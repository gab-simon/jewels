#ifndef UTILS_H
#define UTILS_H
// Allegro libraries
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "allegro5/allegro_native_dialog.h"

#define STRING_SIZE 10
// Screen ratio
#define HEIGHT 700
#define WIDTH 1000
// Board size
#define BOARD_SIZE_X 8
#define BOARD_SIZE_Y 16
// Distance between jewels sprites
#define DISTANCE 72
// Offset of the board display position
#define OFFSET_Y -500
#define OFFSET_X 70
// Position of score
#define POINTS_X 750
#define POINTS_Y 80
// Positions of menu buttons
#define START_X0 470
#define START_X 728
#define START_Y0 260
#define START_Y 335
#define HELP_X0 470
#define HELP_X 728
#define HELP_Y0 370
#define HELP_Y 447
#define EXIT_X0 470
#define EXIT_X 728
#define EXIT_Y0 480
#define EXIT_Y 559
// Positions of buttons on playing state
#define MENU_X0 730
#define MENU_X 870
#define MENU_Y0 600
#define MENU_Y 645
#define RESET_SCORE_X0 890
#define RESET_SCORE_X 1030
#define RESET_SCORE_Y0 600
#define RESET_SCORE_Y 645
// Minimum movement to move a jewel
#define MIN_MOVE 30


#endif