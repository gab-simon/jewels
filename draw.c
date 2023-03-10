#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"
#include "draw.h"
#include "utils.h"

essencials_t *initEssencials(ALLEGRO_FONT *fontsBig, ALLEGRO_FONT *fontsMedium, ALLEGRO_FONT *fontsSmall, ALLEGRO_BITMAP *jewels[6], ALLEGRO_BITMAP *background, FILE *score)
{
    essencials_t *newEssencials = malloc(sizeof(essencials_t));
    if (!newEssencials)
        return NULL;

    if (!fontsBig || !fontsMedium || !fontsSmall || !jewels || !background || !score)
        return NULL;

    for (int i = 0; i < 6; i++)
        newEssencials->jewels[i] = jewels[i];
    newEssencials->background = background;
    newEssencials->fonts = malloc(sizeof(fonts_t));
    newEssencials->fonts->small = fontsSmall;
    newEssencials->fonts->medium = fontsMedium;
    newEssencials->fonts->big = fontsBig;
    newEssencials->score = score;

    return newEssencials;
}

// Verify if a slot exists in a slots array
int verifyExcludedSlots(slot_t *excludedSlots[BOARD_SIZE_X], slot_t *slot)
{
    for (int i = 0; i < BOARD_SIZE_X; i++)
    {
        if (slot == excludedSlots[i])
            return 1;
    }
    return 0;
}

// Draw game points
void buildHUD(board_t *board)
{
    sprintf(board->strPoints, "%d", board->points);
    sprintf(board->strBestScore, "%d", board->bestScore);
    sprintf(board->strLevel, "%d", board->level);
    al_draw_text(board->essencials->fonts->small, al_map_rgb(255, 255, 255), 700, 80, 0, "Score: ");
    al_draw_text(board->essencials->fonts->small, al_map_rgb(255, 255, 255), 700 + 150, 80, 0, board->strPoints);
    al_draw_text(board->essencials->fonts->small, al_map_rgb(255, 255, 255), 700, 80 + 40, 0, "Best: ");
    al_draw_text(board->essencials->fonts->small, al_map_rgb(255, 255, 255), 700 + 150, 80 + 40, 0, board->strBestScore);
    al_draw_text(board->essencials->fonts->small, al_map_rgb(255, 255, 255), 700, 80 + 80, 0, "Level: ");
    al_draw_text(board->essencials->fonts->small, al_map_rgb(255, 255, 255), 700 + 150, 80 + 80, 0, board->strLevel);
}

void muteSound(bool mute_bool)
{
    ALLEGRO_BITMAP *speaker = al_load_bitmap("resources/imgs/speaker.png");
    ALLEGRO_BITMAP *mute = al_load_bitmap("resources/imgs/mute.png");

    if (mute_bool)
        al_draw_bitmap(mute, 0, 0, 0);
    else
        al_draw_bitmap(speaker, 0, 0, 0);
}

// build the board with the candies
void buildBoard(board_t *board)
{
    slot_t *slot = NULL;

    for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y; j++)
    {
        for (int i = 0; i < BOARD_SIZE_X; i++)
        {
            slot = board->slots[i][j];
            if (slot->flower->shown)
                continue;

            switch (slot->flower->type)
            {
            case FLOWER_BLUE:
                al_draw_bitmap(board->essencials->jewels[1], slot->xDisplayPos, slot->yDisplayPos, 0);
                break;
            case FLOWER_RED:
                al_draw_bitmap(board->essencials->jewels[2], slot->xDisplayPos, slot->yDisplayPos, 0);
                break;
            case FLOWER_YELLOW:
                al_draw_bitmap(board->essencials->jewels[3], slot->xDisplayPos, slot->yDisplayPos, 0);
                break;
            case FLOWER_GREEN:
                al_draw_bitmap(board->essencials->jewels[4], slot->xDisplayPos, slot->yDisplayPos, 0);
                break;
            case FLOWER_ORANGE:
                al_draw_bitmap(board->essencials->jewels[5], slot->xDisplayPos, slot->yDisplayPos, 0);
                break;
            }
        }
    }
}

void buildBackground(board_t *board, int xMouse, int yMouse)
{
    char flowersDestroy[STRING_BUFFER];
    al_draw_bitmap(board->essencials->background, 0, 0, 0);

    al_draw_filled_rectangle(695, 600, 760, 620, al_map_rgb(246, 242, 132));
    al_draw_filled_rectangle(790, 600, 880, 620, al_map_rgb(246, 242, 132));

    al_draw_text(board->essencials->fonts->small, al_map_rgb(0, 0, 0), 700, 600, 0, "MENU");
    al_draw_text(board->essencials->fonts->small, al_map_rgb(0, 0, 0), 800, 600, 0, "ZERAR");

    sprintf(flowersDestroy, "%d", board->tasks->flowersDestroy);
    strcat(flowersDestroy, "/15");
    al_draw_text(board->essencials->fonts->small, al_map_rgb(255, 255, 255), 770, 400, 0, flowersDestroy);

    al_draw_scaled_bitmap(board->essencials->jewels[board->tasks->flowersType], 0, 0, 64, 64, 700, 388, 50, 50, 0);
}

void drawMenu(essencials_t *essencials, int xMouse, int yMouse)
{
    ALLEGRO_BITMAP *button = al_load_bitmap("resources/imgs/button.png");
    al_draw_bitmap(essencials->background, 0, 0, 0);

    al_draw_text(essencials->fonts->big, al_map_rgb(255, 255, 255), WIDTH * 0.5, HEIGHT * 0.2, ALLEGRO_ALIGN_CENTER, "JEWELS");

    al_draw_bitmap(button, (WIDTH - al_get_bitmap_width(button)) * 0.5, HEIGHT * 0.4, 0);
    al_draw_bitmap(button, (WIDTH - al_get_bitmap_width(button)) * 0.5, HEIGHT * 0.6, 0);

    al_draw_text(essencials->fonts->medium, al_map_rgb(0, 0, 0), WIDTH * 0.5, HEIGHT * 0.45, ALLEGRO_ALIGN_CENTER, "PLAY");
    al_draw_text(essencials->fonts->medium, al_map_rgb(0, 0, 0), WIDTH * 0.5, HEIGHT * 0.65, ALLEGRO_ALIGN_CENTER, "INFO");

    al_flip_display();
}

void buildInfo(essencials_t *essencials, board_t *board)
{
    ALLEGRO_BITMAP *button = al_load_bitmap("resources/imgs/button.png");
    al_draw_bitmap(essencials->background, 0, 0, 0);
    char text[128];

    al_draw_text(essencials->fonts->big, al_map_rgb(255, 255, 255), WIDTH * 0.5, HEIGHT * 0.2, ALLEGRO_ALIGN_CENTER, "INFO");

    al_draw_bitmap(button, (WIDTH - al_get_bitmap_width(button)) * 0.5, HEIGHT * 0.4, 0);
    al_draw_text(essencials->fonts->medium, al_map_rgb(0, 0, 0), WIDTH * 0.5, HEIGHT * 0.45, ALLEGRO_ALIGN_CENTER, "MENU");

    al_draw_text(essencials->fonts->small, al_map_rgb(0, 0, 0), WIDTH * 0.5, HEIGHT * 0.55, ALLEGRO_ALIGN_CENTER, "M - MENU, 9 - MUTAR SOM, I - INFO, ESC - SAIR");

    al_draw_text(essencials->fonts->medium, al_map_rgb(0, 0, 0), WIDTH * 0.5, HEIGHT * 0.65, ALLEGRO_ALIGN_CENTER, "JOGO DE COMBINACOES DE JOIAS");

    sprintf(text, "Maior pontuacao: %d", board->bestScore);
    al_draw_text(essencials->fonts->medium, al_map_rgb(0, 0, 0), WIDTH * 0.5, HEIGHT * 0.75, ALLEGRO_ALIGN_CENTER, text);
}

void animationFlowers(board_t *board, slot_t *slot, int direction)
{
    int count = 0;
    slot_t *slotSupport = NULL;

    for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y; j++)
    {
        for (int i = 0; i < BOARD_SIZE_X; i++)
        {
            slotSupport = board->slots[i][j];
            if (slotSupport == slot && direction == DIR_LEFT)
            {
                slotSupport->flower->shown = 1;
                slotSupport->left->flower->shown = 1;
                while (slot->xDisplayPos - count > slot->left->xDisplayPos)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    buildBackground(board, 0, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->flower->type], slot->xDisplayPos - count, slot->yDisplayPos, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->left->flower->type], slot->left->xDisplayPos + count, slot->left->yDisplayPos, 0);
                    buildBoard(board);
                    buildHUD(board);
                    al_flip_display();
                    count += 5;
                }
            }
            else if (slotSupport == slot && direction == DIR_RIGHT)
            {
                slotSupport->flower->shown = 1;
                slotSupport->right->flower->shown = 1;
                while (slot->xDisplayPos + count < slot->right->xDisplayPos)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    buildBackground(board, 0, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->flower->type], slot->xDisplayPos + count, slot->yDisplayPos, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->right->flower->type], slot->right->xDisplayPos - count, slot->right->yDisplayPos, 0);
                    buildBoard(board);
                    buildHUD(board);
                    al_flip_display();
                    count += 5;
                }
            }
            else if (slotSupport == slot && direction == DIR_UP)
            {
                slotSupport->flower->shown = 1;
                slotSupport->up->flower->shown = 1;
                while (slot->yDisplayPos - count > slot->up->yDisplayPos)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    buildBackground(board, 0, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->flower->type], slot->xDisplayPos, slot->yDisplayPos - count, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->up->flower->type], slot->up->xDisplayPos, slot->up->yDisplayPos + count, 0);
                    buildBoard(board);
                    buildHUD(board);
                    al_flip_display();
                    count += 5;
                }
            }
            else if (slotSupport == slot && direction == DIR_DOWN)
            {
                slotSupport->flower->shown = 1;
                slotSupport->down->flower->shown = 1;
                while (slot->yDisplayPos + count < slot->down->yDisplayPos)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    buildBackground(board, 0, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->flower->type], slot->xDisplayPos, slot->yDisplayPos + count, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->down->flower->type], slot->down->xDisplayPos, slot->down->yDisplayPos - count, 0);
                    buildBoard(board);
                    buildHUD(board);
                    al_flip_display();
                    count += 5;
                }
            }
            else
                continue;

            buildBoard(board);
            buildHUD(board);
            al_flip_display();
        }
    }
}

// flowers fall animation
void fallFlowers(board_t *board)
{
    int count = 1;

    slot_t *slotInit = NULL;
    slot_t *slotEnd = NULL;
    slot_t *slotSupport = NULL;
    int done = 0;
    int slotsEmpty[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // 1 = Empty slots, 2 = Empty slots filled, but still needs animation
    // Inicialize slotsEmpty for animation
    for (int i = 0; i < 8; i++)
    {
        if (board->emptySlotsColumns[i] > 0)
            slotsEmpty[i] = 1;
    }
    while (!done)
    {
        done = 1;
        for (int i = 0; i < BOARD_SIZE_X; i++)
        {
            // If the slot is empty, it will be filled with a flower and the animation will be performed
            if (!slotsEmpty[i])
                continue;
            if (slotsEmpty[i] == 1)
                done = 0;
            slotInit = board->slots[i][BOARD_SIZE_Y - 9];
            slotEnd = board->slots[i][BOARD_SIZE_Y - 1];

            // Find a slot with a flower in init
            while (slotInit->down->flower->type != FLOWER_NONE)
                slotInit = slotInit->down;
            // Find a slot with a flower in end
            while (slotEnd->flower->type != FLOWER_NONE)
                slotEnd = slotEnd->up;

            // If the slot is empty, it will be filled with a flower and the animation will be performed
            if (slotInit->yDisplayPos + count > slotEnd->yDisplayPos)
                slotsEmpty[i] = 2;

            slotSupport = slotInit;
            
            // animating the empty slots falls
            if (slotsEmpty[i] == 1)
            {
                while (slotSupport->yDisplayPos + count > 50)
                {
                    al_draw_bitmap(board->essencials->jewels[slotSupport->flower->type], slotSupport->xDisplayPos, slotSupport->yDisplayPos + count, 0);
                    slotSupport->flower->shown = 1;
                    slotSupport = slotSupport->up;
                }
            }
            // Hold the flower in the fill position
            else
            {
                while (slotEnd->yBoardPos > BOARD_SIZE_Y - 9)
                {
                    al_draw_bitmap(board->essencials->jewels[slotSupport->flower->type], slotEnd->xDisplayPos, slotEnd->yDisplayPos, 0);
                    slotSupport->flower->shown = 1;
                    slotSupport = slotSupport->up;
                    slotEnd = slotEnd->up;
                }
            }
        }
        if (done)
            break;

        count += 5;

        buildBoard(board);
        restartDraw(board);
        buildHUD(board);
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        buildBackground(board, 0, 0);
    }
    restartDraw(board);
}
