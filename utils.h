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

#define STRING_BUFFER 16

// size of the window
#define HEIGHT 700
#define WIDTH 950

// size of the board
#define BOARD_SIZE_X 8
#define BOARD_SIZE_Y 16

// distance between flowers
#define DISTANCE 72

#define OFFSET_Y -500
#define OFFSET_X 70

// Minimum movement to move a jewel
#define MOV 30

#endif