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

// Screen ratio
#define HEIGHT 700
#define WIDTH 950

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

// Minimum movement to move a jewel
#define MIN_MOVE 30

#endif