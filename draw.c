#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"
#include "draw.h"
#include "utils.h"



// ========================== OTHERS ==========================

allegroEssencials_t *createEssencials(ALLEGRO_FONT *font, ALLEGRO_BITMAP *jewels[6], ALLEGRO_BITMAP *menu[5], ALLEGRO_BITMAP *background[5], FILE *score) {

    allegroEssencials_t *newEssencials = malloc(sizeof(allegroEssencials_t));
    if(!newEssencials)
        return NULL;
    
    newEssencials->font = font;
    newEssencials->score = score;
    for(int i=0; i<6; i++)
        newEssencials->jewels[i] = jewels[i];
    for(int i=0; i<5; i++)
        newEssencials->menu[i] = menu[i];
    for(int i=0; i<5; i++)
        newEssencials->background[i] = background[i];
    newEssencials->backgroundImage = 0;

    return newEssencials;

}

// ========================== DRAWNING ==========================

// Verify if a slot exists in a slots array
int verifyExcludedSlots(slot_t *excludedSlots[BOARD_SIZE_X], slot_t *slot) {

    for(int i=0; i<BOARD_SIZE_X; i++) {
        if(slot == excludedSlots[i])
            return 1;
    }
    return 0;

}

// Draw game points
void drawPoints(board_t *board) {

    sprintf(board->strPoints, "%d", board->points);
    sprintf(board->strBestScore, "%d", board->bestScore);
    al_draw_text(board->essencials->font, al_map_rgb(255, 255, 255), POINTS_X, POINTS_Y, 0, "SCORE ");
    al_draw_text(board->essencials->font, al_map_rgb(255, 255, 255), POINTS_X+150, POINTS_Y, 0, board->strPoints);
    al_draw_text(board->essencials->font, al_map_rgb(255, 255, 255), POINTS_X, POINTS_Y+40, 0, "BEST ");
    al_draw_text(board->essencials->font, al_map_rgb(255, 255, 255), POINTS_X+150, POINTS_Y+40, 0, board->strBestScore);

}

// Draw board 
void drawBoard(board_t *board) {

    slot_t *slot = NULL;

    for(int j=BOARD_SIZE_Y-8; j<BOARD_SIZE_Y; j++) {
        for(int i=0; i<BOARD_SIZE_X; i++) {
            slot = board->slots[i][j];
            if(slot->candy->drawned)
                continue;

            switch(slot->candy->type) {
                case CT_BLUE:
                    al_draw_bitmap(board->essencials->jewels[1], slot->xDisplayPos, slot->yDisplayPos, 0);
                    break;

                case CT_RED:
                    al_draw_bitmap(board->essencials->jewels[2], slot->xDisplayPos, slot->yDisplayPos, 0);
                    break;

                case CT_YELLOW:
                    al_draw_bitmap(board->essencials->jewels[3], slot->xDisplayPos, slot->yDisplayPos, 0);
                    break;

                case CT_GREEN:
                    al_draw_bitmap(board->essencials->jewels[4], slot->xDisplayPos, slot->yDisplayPos, 0);
                    break;

                case CT_ORANGE:
                    al_draw_bitmap(board->essencials->jewels[5], slot->xDisplayPos, slot->yDisplayPos, 0);
                    break;
            }
        }
    }

}

void drawBackground(board_t *board, int xMouse, int yMouse) {

    char candiesDestroyed[STRING_SIZE];
    // Background draw
    al_draw_bitmap(board->essencials->background[board->essencials->backgroundImage],0,0,0);

    // Buttons draw
    if(xMouse > MENU_X0 && xMouse < MENU_X && yMouse > MENU_Y0 && yMouse < MENU_Y)
        al_draw_filled_rectangle(MENU_X0, MENU_Y0, MENU_X, MENU_Y, al_map_rgb(95,25,69));
    else
        al_draw_filled_rectangle(MENU_X0, MENU_Y0, MENU_X, MENU_Y, al_map_rgb(23,25,43));
    if(xMouse > RESET_SCORE_X0 && xMouse < RESET_SCORE_X && yMouse > RESET_SCORE_Y0 && yMouse < RESET_SCORE_Y)
        al_draw_filled_rectangle(RESET_SCORE_X0, RESET_SCORE_Y0, RESET_SCORE_X, RESET_SCORE_Y, al_map_rgb(95,25,69));
    else
        al_draw_filled_rectangle(RESET_SCORE_X0, RESET_SCORE_Y0, RESET_SCORE_X, RESET_SCORE_Y, al_map_rgb(23,25,43));
    al_draw_text(board->essencials->font, al_map_rgb(255,255,255), MENU_X0+20, MENU_Y0+10, 0, "MENU");
    al_draw_text(board->essencials->font, al_map_rgb(255,255,255), RESET_SCORE_X0+10, RESET_SCORE_Y0+10, 0, "ZERAR");

    // Mission draw
    sprintf(candiesDestroyed, "%d", board->mission->candiesDestroyed);
    strcat(candiesDestroyed, "/10");
    al_draw_text(board->essencials->font, al_map_rgb(255,255,255), 900, 400, 0, candiesDestroyed);
    // al_draw_bitmap(board->essencials->jewels[board->mission->candyType], 825, 380, 0);
    al_draw_scaled_bitmap(board->essencials->jewels[board->mission->candyType], 0, 0, 64, 64, 840, 388, 50, 50, 0);

}

void drawMenu(allegroEssencials_t *essencials, int xMouse, int yMouse) {

    if(xMouse > START_X0 && xMouse < START_X && yMouse > START_Y0 && yMouse < START_Y) {
        al_draw_bitmap(essencials->menu[1], 0, 0, 0);
        al_draw_rectangle(START_X0,START_Y0,START_X,START_Y, al_map_rgb(255,255,255), 4);
    }
    else if(xMouse > HELP_X0 && xMouse < HELP_X && yMouse > HELP_Y0 && yMouse < HELP_Y) {
        al_draw_bitmap(essencials->menu[2], 0, 0, 0);
        al_draw_rectangle(HELP_X0,370,HELP_X,HELP_Y, al_map_rgb(255,255,255), 4);
    }
    else if(xMouse > EXIT_X0 && xMouse < EXIT_X && yMouse > EXIT_Y0 && yMouse < EXIT_Y) {
        al_draw_bitmap(essencials->menu[3], 0, 0, 0);
        al_draw_rectangle(EXIT_X0,EXIT_Y0,EXIT_X,EXIT_Y, al_map_rgb(255,255,255), 4);
    }
    else
        al_draw_bitmap(essencials->menu[0], 0, 0, 0);
 

}

void drawHelp(allegroEssencials_t *essencials) {

    al_draw_text(essencials->font, al_map_rgb(255, 255, 255), START_X, START_Y, 0, "Ajuda");

}

// ========================== ANIMATIONS ==========================

// Transiting jewels animation
void transitingJewels(board_t *board, slot_t *slot, int direction) {

    int count = 2;
    slot_t *slotAux = NULL;

    for(int j=BOARD_SIZE_Y-8; j<BOARD_SIZE_Y; j++) {
        for(int i=0; i<BOARD_SIZE_X; i++) {
            slotAux = board->slots[i][j];
            if(slotAux == slot && direction == D_LEFT) {
                slotAux->candy->drawned = 1;
                slotAux->left->candy->drawned = 1;
                while(slot->xDisplayPos-count>slot->left->xDisplayPos) {
                    al_clear_to_color(al_map_rgb(0,0,0));
                    drawBackground(board, 0, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->candy->type], slot->xDisplayPos-count, slot->yDisplayPos, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->left->candy->type], slot->left->xDisplayPos+count, slot->left->yDisplayPos, 0);
                    drawBoard(board);
                    drawPoints(board);
                    al_flip_display();
                    count += 5;
                }
            }
            else if(slotAux == slot && direction == D_RIGHT) {
                slotAux->candy->drawned = 1;
                slotAux->right->candy->drawned = 1;
                while(slot->xDisplayPos+count<slot->right->xDisplayPos) {
                    al_clear_to_color(al_map_rgb(0,0,0));
                    drawBackground(board, 0, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->candy->type], slot->xDisplayPos+count, slot->yDisplayPos, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->right->candy->type], slot->right->xDisplayPos-count, slot->right->yDisplayPos, 0);
                    drawBoard(board);
                    drawPoints(board);
                    al_flip_display();
                    count += 5;
                }
            }
            else if(slotAux == slot && direction == D_UP) {
                slotAux->candy->drawned = 1;
                slotAux->up->candy->drawned = 1;
                    while(slot->yDisplayPos-count>slot->up->yDisplayPos) {
                    al_clear_to_color(al_map_rgb(0,0,0));
                    drawBackground(board, 0, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->candy->type], slot->xDisplayPos, slot->yDisplayPos-count, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->up->candy->type], slot->up->xDisplayPos, slot->up->yDisplayPos+count, 0);
                    drawBoard(board);
                    drawPoints(board);
                    al_flip_display();
                    count += 5;
                }
            }
            else if(slotAux == slot && direction == D_DOWN) {
                slotAux->candy->drawned = 1;
                slotAux->down->candy->drawned = 1;
                while(slot->yDisplayPos+count<slot->down->yDisplayPos) {
                    al_clear_to_color(al_map_rgb(0,0,0));
                    drawBackground(board, 0, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->candy->type], slot->xDisplayPos, slot->yDisplayPos+count, 0);
                    al_draw_bitmap(board->essencials->jewels[slot->down->candy->type], slot->down->xDisplayPos, slot->down->yDisplayPos-count, 0);
                    drawBoard(board);
                    drawPoints(board);
                    al_flip_display();
                    count += 5;
                }
            }
            else
                continue;
            
            drawBoard(board);
            drawPoints(board);
            al_flip_display();
        }
    }

}

// Candies fall animation
void fallJewels(board_t *board) {

    int count = 1;
    
    slot_t *slotInit = NULL;    // Slot above the empty one
    slot_t *slotEnd = NULL;     // Lowest slot empty
    slot_t *slotAux = NULL;
    int done = 0;
    int emptySlotsAux[8] = {0,0,0,0,0,0,0,0};   // 1 = Empty slots, 2 = Empty slots filled, but still needs animation
    // Inicialize emptySlotsAux for animation
    for(int i=0; i<8; i++) {
        if(board->emptySlotsColumns[i] > 0)
        emptySlotsAux[i] = 1;
    }
    while(!done) {
        done = 1;
        for(int i=0; i<BOARD_SIZE_X; i++) {
            // Performs the animation if there are empty slots in the column
            if(!emptySlotsAux[i])
                continue;
            if(emptySlotsAux[i] == 1)
                done = 0;
            slotInit = board->slots[i][BOARD_SIZE_Y-9];
            slotEnd = board->slots[i][BOARD_SIZE_Y-1];

            //  Find the beggining of the fall
            while(slotInit->down->candy->type != CT_NONE)
                slotInit = slotInit->down;
            // Find the end of the fall
            while(slotEnd->candy->type != CT_NONE)
                slotEnd = slotEnd->up;

            // -------- Animation --------
            if(slotInit->yDisplayPos+count > slotEnd->yDisplayPos)
                emptySlotsAux[i] = 2;
            
            slotAux = slotInit;
            // Draw the candies falling in the column
            // Drawm falling candies
            if(emptySlotsAux[i] == 1) {
                while(slotAux->yDisplayPos+count > 50) {
                    al_draw_bitmap(board->essencials->jewels[slotAux->candy->type], slotAux->xDisplayPos, slotAux->yDisplayPos+count, 0);
                    slotAux->candy->drawned = 1;
                    slotAux = slotAux->up;     
                } 
            }
            // Maintaing the candies already in final position while others are still falling
            else {
                while(slotEnd->yBoardPos > BOARD_SIZE_Y-9) {
                    al_draw_bitmap(board->essencials->jewels[slotAux->candy->type], slotEnd->xDisplayPos, slotEnd->yDisplayPos, 0);
                    slotAux->candy->drawned = 1;
                    slotAux = slotAux->up;        
                    slotEnd = slotEnd->up;         
                }   
            }
            
        }
        if(done)
            break;
        count += 6;
        drawBoard(board);
        resetDrawned(board);
        drawPoints(board);
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        drawBackground(board, 0, 0);
    }
    resetDrawned(board);

}
