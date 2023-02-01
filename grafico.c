#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "grafico.h"

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

window graph_init(int res_width, int res_height)
{
	window win = {NULL, NULL, NULL, {res_width, res_height, 0, 60}, NULL};

	/* Inicializa a Allegro */
	al_init();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_init_native_dialog_addon();
	al_init_font_addon();
	al_init_primitives_addon();
	al_init_ttf_addon();

	win.display = al_create_display(res_width, res_height);

	al_install_mouse();
	al_install_keyboard();
	win.timer = al_create_timer(1.0 / win.disp_data.refresh_rate);

	/* Define Eventos */
	win.event_queue = al_create_event_queue();

	al_register_event_source(win.event_queue, al_get_display_event_source(win.display));
	al_register_event_source(win.event_queue, al_get_mouse_event_source());
	al_register_event_source(win.event_queue, al_get_keyboard_event_source());
	al_register_event_source(win.event_queue, al_get_timer_event_source(win.timer));

	al_clear_to_color(PRETO);
	al_flip_display();
	al_start_timer(win.timer);

	char font_path[100];
	sprintf(font_path, "%s/resources/fonts/Minecraft.ttf", al_get_current_directory());
	printf("font_path: %s\n", font_path);
	fonts_t *fonts = malloc(sizeof(fonts_t));
	fonts->big = al_load_font(font_path, 85, 0);
	fonts->medium = al_load_font(font_path, 55, 0);
	fonts->small = al_load_font(font_path, 35, 0);

	win.fonts = fonts;
	printf("====================================================\n");

	return (win);
}

void draw_menu(window *win)
{
	ALLEGRO_BITMAP *bg_menu = al_load_bitmap("resources/imgs/bg_menu.png");
	al_draw_bitmap(bg_menu, 0, 0, 0);

	al_draw_text(win->fonts->big, BRANCO, win->disp_data.width * 0.5, win->disp_data.height * 0.2, ALLEGRO_ALIGN_CENTER, "JEWELS");

	ALLEGRO_BITMAP *button = al_load_bitmap("resources/imgs/button.png");
	al_draw_bitmap(button, (win->disp_data.width - al_get_bitmap_width(button)) * 0.5, win->disp_data.height * 0.6, 0);
	al_draw_bitmap(button, (win->disp_data.width - al_get_bitmap_width(button)) * 0.5, win->disp_data.height * 0.8, 0);

	al_draw_text(win->fonts->medium, PRETO, win->disp_data.width * 0.5, win->disp_data.height * 0.63, ALLEGRO_ALIGN_CENTER, "PLAY");
	al_draw_text(win->fonts->medium, PRETO, win->disp_data.width * 0.5, win->disp_data.height * 0.83, ALLEGRO_ALIGN_CENTER, "INFO");

	al_flip_display();
}

void draw_info(window *win, game_t *game)
{
	system("sort -nr -o resources/score_high.txt resources/score.txt");

	FILE *score = fopen("resources/score_high.txt", "r");
	char line[256];
	int i = 0;
	char text[50];

	ALLEGRO_BITMAP *bg_menu = al_load_bitmap("resources/imgs/bg_menu.png");
	al_draw_bitmap(bg_menu, 0, 0, 0);

	al_draw_text(win->fonts->medium, BRANCO, win->disp_data.width * 0.5, win->disp_data.height * 0.1, ALLEGRO_ALIGN_CENTER, "RECORDES");

	ALLEGRO_BITMAP *button = al_load_bitmap("resources/imgs/button.png");
	al_draw_bitmap(button, (win->disp_data.width - al_get_bitmap_width(button)) * 0.5, win->disp_data.height * 0.2, 0);

	al_draw_text(win->fonts->medium, PRETO, win->disp_data.width * 0.5, win->disp_data.height * 0.23, ALLEGRO_ALIGN_CENTER, "MENU");

	sprintf(text, "Maior pontuacao %d", game->highscore);
	al_draw_text(win->fonts->small, BRANCO, win->disp_data.width * 0.5, win->disp_data.height * 0.45, ALLEGRO_ALIGN_CENTER, text);

	al_draw_text(win->fonts->small, BRANCO, win->disp_data.width * 0.5, win->disp_data.height * 0.5, ALLEGRO_ALIGN_CENTER, "10 melhores pontuacoes:");

	while (fgets(line, sizeof(line), score) && i < 10)
	{
		al_draw_text(win->fonts->small, BRANCO, win->disp_data.width * 0.5, win->disp_data.height * 0.55 + (30 * i), ALLEGRO_ALIGN_CENTER, line);
		printf("%s", line);
		i++;
	}

	al_flip_display();
	fclose(score);
}

void draw_gameover(window *win, game_t *game)
{
	if (al_is_event_queue_empty(win->event_queue))
	{
		char text[20];

		ALLEGRO_BITMAP *bg_menu = al_load_bitmap("resources/imgs/bg_menu.png");
		al_draw_bitmap(bg_menu, 0, 0, 0);

		al_draw_text(win->fonts->medium, BRANCO, win->disp_data.width * 0.5, win->disp_data.height * 0.2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
		sprintf(text, "Pontuacao: %d", game->score);
		al_draw_text(win->fonts->medium, BRANCO, win->disp_data.width * 0.5, win->disp_data.height * 0.4, ALLEGRO_ALIGN_CENTRE, text);

		if (game->score >= game->highscore)
		{
			al_draw_text(win->fonts->small, VERDE_ESCURO, win->disp_data.width * 0.5, win->disp_data.height * 0.5, ALLEGRO_ALIGN_CENTRE, "Nova melhor pontuacao");
		}

		new_highscore(game);

		ALLEGRO_BITMAP *button = al_load_bitmap("resources/imgs/button.png");
		al_draw_bitmap(button, (win->disp_data.width - al_get_bitmap_width(button)) * 0.5, win->disp_data.height * 0.6, 0);
		al_draw_bitmap(button, (win->disp_data.width - al_get_bitmap_width(button)) * 0.5, win->disp_data.height * 0.8, 0);

		al_draw_text(win->fonts->medium, PRETO, win->disp_data.width * 0.5, win->disp_data.height * 0.63, ALLEGRO_ALIGN_CENTER, "PLAY");
		al_draw_text(win->fonts->medium, PRETO, win->disp_data.width * 0.5, win->disp_data.height * 0.83, ALLEGRO_ALIGN_CENTER, "MENU");

		al_flip_display();
	}
}

void show_blocks(window *win, int blocks[][7], float off_set_y)
{
	float l = block_side(win->disp_data.width);
	int i, j;

	for (i = 0; i < ROW; ++i)
	{
		for (j = 0; j < COL; ++j)
		{
			if (blocks[i][j] > 0)
			{
				char text[10];
				int textoff_set_y = al_get_font_line_height(win->fonts->small) / 2;
				al_draw_filled_rectangle(block_i_x(j, l), block_i_y(i, l) + off_set_y, block_f_x(j, l), block_f_y(i, l) + off_set_y, 
				blocks[i][j] <= 7 ? ROSA_CLARO : blocks[i][j] > 7 && blocks[i][j] < 12 ? ROSA_BRILHANTE : ROSA_PROFUNDO);
				sprintf(text, "%d", blocks[i][j]);
				al_draw_text(win->fonts->small, BRANCO, block_mid_x(j, l), block_mid_y(i, l) + off_set_y - textoff_set_y, ALLEGRO_ALIGN_CENTER, text);
			}
			if (blocks[i][j] == -1)
			{
				al_draw_filled_circle(block_mid_x(j, l), block_mid_y(i, l) + off_set_y, 8, BRANCO);
				al_draw_circle(block_mid_x(j, l), block_mid_y(i, l) + off_set_y, BALL_RADIUS + 5, BRANCO, 2);
			}
			if (blocks[i][j] == -2)
			{
				al_draw_filled_circle(block_mid_x(j, l), block_mid_y(i, l) + off_set_y, 8, AMARELO);
				al_draw_circle(block_mid_x(j, l), block_mid_y(i, l) + off_set_y, BALL_RADIUS + 5, AMARELO_QUEIMADO, 2);
			}
		}
	}
}

void draw_score(window *win, game_t *game)
{
	char text[20];
	sprintf(text, "Pontuacao: %d", game->score);
	al_draw_text(win->fonts->small, BRANCO, 20, 765, ALLEGRO_ALIGN_LEFT, text);
}

void draw_count_balls(window *win, game_t *game)
{
	char text[20];
	sprintf(text, "x%d", game->balls - game->shot_balls);
	al_draw_text(win->fonts->small, BRANCO, game->shooting_x - BALL_RADIUS - 20, game->shooting_y - BALL_RADIUS - 30, ALLEGRO_ALIGN_LEFT, text);
}

void draw_wait(window *win, ball_t *ball, int blocks[][7], game_t *game)
{
	if (al_event_queue_is_empty(win->event_queue))
	{
		ALLEGRO_BITMAP *bg_game = al_load_bitmap("resources/imgs/bg_game.png");
		al_draw_bitmap(bg_game, 0, 0, 0);
		al_draw_filled_circle(ball->x, ball->y, BALL_RADIUS, BRANCO);
		show_blocks(win, blocks, 0);
		draw_score(win, game);
		draw_count_balls(win, game);
		al_flip_display();
	}
}

void draw_serving(window *win, ball_t *ball, int blocks[][COL], float off_set_y, game_t *game)
{
	if (al_event_queue_is_empty(win->event_queue))
	{
		ALLEGRO_BITMAP *bg_game = al_load_bitmap("resources/imgs/bg_game.png");
		al_draw_bitmap(bg_game, 0, 0, 0);
		al_draw_filled_circle(ball->x, ball->y, BALL_RADIUS, BRANCO);
		show_blocks(win, blocks, off_set_y);
		draw_score(win, game);
		draw_count_balls(win, game);
		al_flip_display();
	}
}

void draw_aim(window *win, ball_t *ball, float distance_x, float distance_y, float dist, int blocks[][COL], game_t *game)
{
	if (al_is_event_queue_empty(win->event_queue))
	{
		ALLEGRO_BITMAP *bg_game = al_load_bitmap("resources/imgs/bg_game.png");
		al_draw_bitmap(bg_game, 0, 0, 0);
		al_draw_filled_circle(ball->x, ball->y, BALL_RADIUS, BRANCO);
		show_blocks(win, blocks, 0);
		draw_count_balls(win, game);

		al_draw_line(ball->x, ball->y, ball->x + (BALL_RADIUS + 400) * distance_x / dist, ball->y + (BALL_RADIUS + 400) * distance_y / dist, BRANCO, 2);

		draw_score(win, game);
		al_flip_display();
	}
}

void draw_shoot(window *win, ball_t **balls, int ballsCount, int blocks[][COL], game_t *game)
{
	if (al_is_event_queue_empty(win->event_queue))
	{
		ALLEGRO_BITMAP *bg_game = al_load_bitmap("resources/imgs/bg_game.png");
		al_draw_bitmap(bg_game, 0, 0, 0);
		for (int i = 0; i < ballsCount; i++)
		{
			if (balls[i])
			{
				al_draw_filled_circle(balls[i]->x, balls[i]->y, BALL_RADIUS, BRANCO);
			}
		}
		if (game->shot_balls != game->balls)
		{
			draw_count_balls(win, game);
		}
		show_blocks(win, blocks, 0);
		draw_score(win, game);
		al_flip_display();
	}
}

void new_highscore(game_t *game)
{
	FILE *score;
	char score_string[32];

	score = fopen("resources/score.txt", "a");

	if (score == NULL)
	{
		printf("erro ao abrir arquivo");
	}

	sprintf(score_string, "%d", game->score);

	fputs(score_string, score);
	fputs("\n", score);

	fclose(score);
}

void graph_deinit(window win)
{
	al_destroy_timer(win.timer);
	al_destroy_event_queue(win.event_queue);
	al_destroy_display(win.display);

	al_destroy_font(win.fonts->big);
	al_destroy_font(win.fonts->medium);
	al_destroy_font(win.fonts->small);
	free(win.fonts);
}