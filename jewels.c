#include "draw.h"
#include "game.h"
#include "utils.h"

// Test if certain allegro feature was sucessfuly added
void must_init(bool test, const char *description) {

    if(test)
        return;
    printf("couldn't initialize %s\n", description);
    exit(1);

}

// Main game
int main() {
    
    // -------- Set up essencials --------
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    must_init(al_init_font_addon(), "font");
    must_init(al_init_ttf_addon(), "font");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGH);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_load_ttf_font("resources/fonts/Minecraft.ttf", 25, 0);
    must_init(font, "font");

    // Inicialize images
    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* red = al_load_bitmap("resources/imgs/red_jewel.png");
    must_init(red, "red");
    ALLEGRO_BITMAP* green = al_load_bitmap("resources/imgs/green_jewel.png");
    must_init(green, "green");
    ALLEGRO_BITMAP* blue = al_load_bitmap("resources/imgs/blue_jewel.png");
    must_init(blue, "blue");
    ALLEGRO_BITMAP* orange = al_load_bitmap("resources/imgs/orange_jewel.png");
    must_init(orange, "orange");
    ALLEGRO_BITMAP* yellow = al_load_bitmap("resources/imgs/yellow_jewel.png");
    must_init(yellow, "yellow");
    ALLEGRO_BITMAP* background1 = al_load_bitmap("resources/imgs/background1.jpg");
    must_init(background1, "background");
    ALLEGRO_BITMAP* background2 = al_load_bitmap("resources/imgs/background1.jpg");
    must_init(background2, "background");
    ALLEGRO_BITMAP* background3 = al_load_bitmap("resources/imgs/background1.jpg");
    must_init(background3, "background");
    ALLEGRO_BITMAP* background4 = al_load_bitmap("resources/imgs/background1.jpg");
    must_init(background4, "background");
    ALLEGRO_BITMAP* background5 = al_load_bitmap("resources/imgs/background1.jpg");
    must_init(background5, "background");
    ALLEGRO_BITMAP* menu = al_load_bitmap("resources/imgs/background1.jpg");
    must_init(menu, "menu1");
    ALLEGRO_BITMAP* menu_exit = al_load_bitmap("resources/imgs/background1.jpg");
    must_init(menu_exit, "menu2");
    ALLEGRO_BITMAP* menu_play = al_load_bitmap("resources/imgs/background1.jpg");
    must_init(menu_play, "menu3");
    ALLEGRO_BITMAP* menu_help = al_load_bitmap("resources/imgs/background1.jpg");
    must_init(menu_help, "menu4");
    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());


    // -------- Game variables --------
    FILE *score = fopen("./score", "a+");
    char scoreString[STRING_SIZE];
    ALLEGRO_BITMAP *jewelsImages[6] = {NULL, blue, red, yellow, green, orange};
    ALLEGRO_BITMAP *backgroundImages[5] = {background1, background2, background3, background4, background5};
    ALLEGRO_BITMAP *menuImages[5] = {menu, menu_play, menu_help, menu_exit, NULL};
    allegroEssencials_t *allegroEssencials = createEssencials(font, jewelsImages, menuImages, backgroundImages, score);
    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE ks;
    board_t *board = createBoard();
    board->essencials = allegroEssencials;
    int xMouse, xMouseInit, xMouseEnd, yMouse, yMouseInit, yMouseEnd;   // Mouse positions
    int dx, dy = 0;
    int direction = 0;
    int pressed = 0;    // Mouse button pressed
    int state = S_MENU;

    // Inicialize the board
    srand(time(NULL));
    for(int j=0; j<BOARD_SIZE_Y; j++)
        for(int i=0; i<BOARD_SIZE_X; i++) {
            board->slots[i][j]->candy->type = 1+(rand() % 5);
            board->slots[i][j]->xDisplayPos = i*DISTANCE + OFFSET_X;
            board->slots[i][j]->yDisplayPos = (j)*DISTANCE + OFFSET_Y;
        }
    
    // Inicialize unmatch board
    resetBoard(board);

    // Get the max score  
    fgets(scoreString, STRING_SIZE, score);
    board->bestScore = atoi(scoreString);


    slot_t *slotAux;
    al_start_timer(timer);
    al_hide_mouse_cursor(disp);
    al_grab_mouse(disp);

    // -------- Main loop --------
    while(1) {
        al_wait_for_event(queue, &event);
        switch(event.type) {
            // ======== GAME UPDATE ========
            case ALLEGRO_EVENT_TIMER:
                // Exit game
                if(al_key_down(&ks, ALLEGRO_KEY_ESCAPE))
                    done = true;
                // Return to menu
                if(al_key_down(&ks, ALLEGRO_KEY_M))
                    state = S_MENU;
                // Enter Help screen
                else if((al_key_down(&ks, ALLEGRO_KEY_H) || al_key_down(&ks, ALLEGRO_KEY_F1)) && state != S_HELP)
                    state = S_HELP;
                // Reset keyboard state
                al_get_keyboard_state(&ks);

                if(state == S_PLAYING) {
                    // Change candies positions logic
                    if(pressed && xMouseInit < 640 && xMouseInit > 70 && yMouseInit < 650 && yMouseInit > 80) {
                        slotAux = board->slots[(xMouseInit-OFFSET_X)/DISTANCE][(yMouseInit-OFFSET_Y)/DISTANCE];
                        // Test if it's a valid transition
                        if(!validTransition(slotAux, direction)) {
                            pressed = 0;
                            continue;
                        }
                        transitingJewels(board, slotAux, direction);
                        changePositions(slotAux, direction);
                        if(!verifyMatch(board, 0)) {
                            transitingJewels(board, slotAux, direction);
                            resetDrawned(board);
                            changePositions(slotAux, direction);
                        }
                        else {
                            fillEmptySlots(board, 1);
                            while(verifyMatch(board, 0))
                                fillEmptySlots(board, 1);
                        }
                        if(board->points > board->bestScore) {
                            board->bestScore = board->points;
                            freopen("./score", "w", score);
                            fprintf(score, "%d", board->bestScore);
                        }
                    }
                    else if(pressed && xMouse > MENU_X0 && xMouse < MENU_X && yMouse > MENU_Y0 && yMouse < MENU_Y) {
                        state = S_MENU;
                        resetBoard(board);
                    }
                    else if(pressed && xMouse > RESET_SCORE_X0 && xMouse < RESET_SCORE_X && yMouse > RESET_SCORE_Y0 && yMouse < RESET_SCORE_Y) 
                        resetBoard(board);
 
                }
                else if(state == S_MENU) {
                    if(xMouse > START_X0 && xMouse < START_X && yMouse > START_Y0 && yMouse < START_Y && pressed) {
                        state = S_PLAYING;
                        // Pick a background image
                        allegroEssencials->backgroundImage = (rand() % 5);
                    }
                    else if(xMouse > HELP_X0 && xMouse < HELP_X && yMouse > HELP_Y0 && yMouse < HELP_Y && pressed)
                        state = S_HELP;
                    else if(xMouse > EXIT_X0 && xMouse < EXIT_X && yMouse > EXIT_Y0 && yMouse < EXIT_Y && pressed)
                        done = 1;

                }
                pressed = 0;
                redraw = true;
                break;

            // ======== INPUT ========
            case ALLEGRO_EVENT_KEY_DOWN:
                al_get_keyboard_state(&ks);
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                xMouse = event.mouse.x;
                yMouse = event.mouse.y;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                xMouseInit = event.mouse.x;
                yMouseInit = event.mouse.y;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                pressed = 1;
                xMouseEnd = event.mouse.x;
                yMouseEnd = event.mouse.y;
                dx = xMouseEnd - xMouseInit;
                dy = yMouseEnd - yMouseInit;
                direction = abs(dx) - abs(dy);
                if(direction > 0 && dx < 0 && abs(dx) > MIN_MOVE)
                    direction = D_LEFT;
                else if(direction > 0 && dx > 0 && abs(dx) > MIN_MOVE)
                    direction = D_RIGHT;
                else if(direction < 0 && dy > 0 && abs(dy) > MIN_MOVE)
                    direction = D_DOWN;
                else if(direction < 0 && dy < 0 && abs(dy) > MIN_MOVE)
                    direction = D_UP;
                // else if(state == S_PLAYING)
                //     pressed = 0;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        // Draw Game when no movement is make
        if(redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            if(state == S_PLAYING) {
                // Background
                drawBackground(board, xMouse, yMouse);
                // Points
                drawPoints(board);
                // Jewels
                drawBoard(board);
                // Mouse
            }
            else if(state == S_MENU) {
                // Menu
                drawMenu(allegroEssencials, xMouse, yMouse);
            }
            else if(state == S_HELP) {
                // Menu
                drawHelp(allegroEssencials);
            }
            al_draw_filled_circle(xMouse, yMouse, 5, al_map_rgb(255,255,255));
            al_flip_display();
            redraw = false;
        }
    }


    // -------- Destroy functions --------
    al_destroy_bitmap(red);
    al_destroy_bitmap(orange);
    al_destroy_bitmap(yellow);
    al_destroy_bitmap(green);
    al_destroy_bitmap(blue);
    al_destroy_bitmap(background1);
    al_destroy_bitmap(background2);
    al_destroy_bitmap(background3);
    al_destroy_bitmap(background4);
    al_destroy_bitmap(background5);
    al_destroy_bitmap(menu);
    al_destroy_bitmap(menu_exit);
    al_destroy_bitmap(menu_help);
    al_destroy_bitmap(menu_play);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    fclose(score);

    return 0;
}
