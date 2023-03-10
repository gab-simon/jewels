#include "draw.h"
#include "game.h"
#include "utils.h"

// DeBug init Allegro
void must_init(bool test, const char *description)
{
    if (test)
        return;
    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    must_init(al_init_font_addon(), "font");
    must_init(al_init_ttf_addon(), "font");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY *disp = al_create_display(WIDTH, HEIGHT);
    must_init(disp, "display");

    ALLEGRO_FONT *font = al_load_ttf_font("resources/fonts/Minecraft.ttf", 18, 0);
    must_init(font, "font");
    char font_path[100];
    sprintf(font_path, "%s/resources/fonts/Minecraft.ttf", al_get_current_directory());
    printf("font_path: %s\n", font_path);

    fonts_t *fonts = malloc(sizeof(fonts_t));
    fonts->big = al_load_font(font_path, 60, 0);
    fonts->medium = al_load_font(font_path, 40, 0);
    fonts->small = al_load_font(font_path, 20, 0);

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP *red = al_load_bitmap("resources/imgs/red_flower.png");
    must_init(red, "red");
    ALLEGRO_BITMAP *green = al_load_bitmap("resources/imgs/green_flower.png");
    must_init(green, "green");
    ALLEGRO_BITMAP *blue = al_load_bitmap("resources/imgs/blue_flower.png");
    must_init(blue, "blue");
    ALLEGRO_BITMAP *purple = al_load_bitmap("resources/imgs/purple_flower.png");
    must_init(purple, "purple");
    ALLEGRO_BITMAP *yellow = al_load_bitmap("resources/imgs/yellow_flower.png");
    must_init(yellow, "yellow");
    ALLEGRO_BITMAP *bg_menu = al_load_bitmap("resources/imgs/bg_menu.png");
    must_init(bg_menu, "background");
    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    ALLEGRO_SAMPLE *music_menu = al_load_sample("resources/sounds/sfx_menu.ogg");
    ALLEGRO_SAMPLE_ID sfx_menu;
    al_reserve_samples(1);

    FILE *score = fopen("./resources/score", "a+");
    char scoreString[STRING_BUFFER];

    ALLEGRO_BITMAP *jewelsImages[6] = {NULL, blue, red, yellow, green, purple};
    essencials_t *allegroEssencials = initEssencials(fonts->big, fonts->medium, fonts->small, jewelsImages, bg_menu, score);

    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE keyboard;

    board_t *board = createBoard();

    bool mute = false;
    bool redraw = true;

    board->essencials = allegroEssencials;
    int xMouse, xMouseInit, xMouseEnd, yMouse, yMouseInit, yMouseEnd;
    int dx, dy = 0;
    int direction = 0;
    int pressed = 0;
    board->STATES = MENU;

    srand(time(NULL));
    for (int j = 0; j < BOARD_SIZE_Y; j++)
        for (int i = 0; i < BOARD_SIZE_X; i++)
        {
            board->slots[i][j]->flower->type = 1 + (rand() % 5);
            board->slots[i][j]->xDisplayPos = i * DISTANCE + OFFSET_X;
            board->slots[i][j]->yDisplayPos = (j) * DISTANCE + OFFSET_Y;
        }

    restartGame(board);

    fgets(scoreString, STRING_BUFFER, score);
    board->bestScore = atoi(scoreString);

    slot_t *slotSupport;
    al_start_timer(timer);
    al_hide_mouse_cursor(disp);
    al_grab_mouse(disp);

    while (board->STATES != EXIT)
    {
        al_wait_for_event(queue, &event);
        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            al_get_keyboard_state(&keyboard);

            // Serving a game
            if (board->STATES == SERVING)
            {
                if (pressed && xMouseInit < 640 && xMouseInit > 70 && yMouseInit < 650 && yMouseInit > 80)
                {
                    slotSupport = board->slots[(xMouseInit - OFFSET_X) / DISTANCE][(yMouseInit - OFFSET_Y) / DISTANCE];
                    if (!checkSwap(slotSupport, direction))
                    {
                        pressed = 0;
                        continue;
                    }
                    animationFlowers(board, slotSupport, direction);
                    swapPositions(slotSupport, direction);
                    if (!checkMatch(board, 0))
                    {
                        animationFlowers(board, slotSupport, direction);
                        restartDraw(board);
                        swapPositions(slotSupport, direction);
                    }
                    else
                    {
                        putSpacesVague(board, 20);
                        while (checkMatch(board, 0))
                            putSpacesVague(board, 20);
                    }
                    if (board->points > board->bestScore)
                    {
                        board->bestScore = board->points;
                        freopen("./score", "w", score);
                        fprintf(score, "%d", board->bestScore);
                    }
                }
                else if (xMouse > 700 && xMouse < 780 && yMouse > 600 && yMouse < 645 && pressed)
                {
                    board->STATES = MENU;
                    restartGame(board);
                }
                else if (xMouse > 800 && xMouse < 880 && yMouse > 600 && yMouse < 645 && pressed)
                    restartGame(board);
            }

            // Menu
            else if (board->STATES == MENU)
            {
                if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE))
                    board->STATES = EXIT;
                if (xMouse > WIDTH * 0.5 - 160 && xMouse < WIDTH * 0.5 + 160 && yMouse > HEIGHT * 0.4 && yMouse < HEIGHT * 0.4 + 90 && pressed)
                {
                    board->STATES = SERVING;
                }
                else if (xMouse > WIDTH * 0.5 - 160 && xMouse < WIDTH * 0.5 + 160 && yMouse > HEIGHT * 0.6 && yMouse < HEIGHT * 0.6 + 90 && pressed)
                    board->STATES = INFO;
            }

            // Info
            else if (board->STATES == INFO)
            {
                if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE))
                    board->STATES = EXIT;
                if (xMouse > WIDTH * 0.5 - 160 && xMouse < WIDTH * 0.5 + 160 && yMouse > HEIGHT * 0.4 && yMouse < HEIGHT * 0.4 + 90 && pressed)
                {
                    board->STATES = MENU;
                }
            }
            pressed = 0;
            redraw = true;
            break;

        // Keyboard events and Mouses (press and release)
        case ALLEGRO_EVENT_KEY_DOWN:
            al_get_keyboard_state(&keyboard);
            if (al_key_down(&keyboard, ALLEGRO_KEY_I))
                al_show_native_message_box(disp, "JEWELS", "Instru????es", "Como jogar: utilize o mouse para direcionar a bola nos quadrados\nObjetivo: Conseguir a maior quantidade de pontos\nObs: Acerte os circulos brancos para obter mais bolas\n\nGabriel Simon GRR20210575\n\nExperimente apertar a tecla O :D", NULL, 0);
            if (al_key_down(&keyboard, ALLEGRO_KEY_O))
                board->level += 3;
            if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE))
                board->STATES = EXIT;
            if (al_key_down(&keyboard, ALLEGRO_KEY_M))
                board->STATES = MENU;
            if (al_key_down(&keyboard, ALLEGRO_KEY_R))
                restartGame(board);
            if (al_key_down(&keyboard, ALLEGRO_KEY_9))
            {
                mute = !mute;
                if (mute)
                    al_stop_sample(&sfx_menu);
                else
                    al_play_sample(music_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sfx_menu);
            }
            else if ((al_key_down(&keyboard, ALLEGRO_KEY_H) || al_key_down(&keyboard, ALLEGRO_KEY_F1)))
                board->STATES = INFO;
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
            if (direction > 0 && dx < 0 && abs(dx) > MOV)
                direction = DIR_LEFT;
            else if (direction > 0 && dx > 0 && abs(dx) > MOV)
                direction = DIR_RIGHT;
            else if (direction < 0 && dy > 0 && abs(dy) > MOV)
                direction = DIR_DOWN;
            else if (direction < 0 && dy < 0 && abs(dy) > MOV)
                direction = DIR_UP;
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            board->STATES = EXIT;
            break;
        }

        // Game rendering here (if needed)
        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            if (board->STATES == SERVING)
            {
                buildBackground(board, xMouse, yMouse);
                buildHUD(board);
                buildBoard(board);
                muteSound(mute);
            }
            else if (board->STATES == MENU)
            {
                al_play_sample(music_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                drawMenu(allegroEssencials, xMouse, yMouse);
            }
            else if (board->STATES == INFO)
            {
                buildInfo(allegroEssencials, board);
            }
            al_draw_filled_circle(xMouse, yMouse, 6, al_map_rgb(255, 123, 0));
            al_flip_display();
            redraw = false;
        }
    }

    // Clean up
    al_destroy_sample(music_menu);
    al_destroy_bitmap(red);
    al_destroy_bitmap(purple);
    al_destroy_bitmap(yellow);
    al_destroy_bitmap(green);
    al_destroy_bitmap(blue);
    al_destroy_bitmap(bg_menu);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    fclose(score);

    return 0;
}
