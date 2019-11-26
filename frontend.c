#define _CRT_SECURE_NO_WARNINGS
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include "structures.h"
#include "backend.h"

void loadSettings(settings * settings)
{
	FILE * input;
	input = fopen("./settings/settings.txt", "r");
	if (input)
	{
		fscanf(input, "%d", &(settings->windowSize));
		if (settings->windowSize < 120 || settings->windowSize > 1000) settings->windowSize = 800;
		else if (settings->windowSize % 8) settings->windowSize -= settings->windowSize % 8;
		fscanf(input, "%d", &(settings->soundSet));
		if (settings->soundSet < 1 || settings->windowSize > 2) settings->soundSet = 1;
		fscanf(input, "%d %d %d", &(settings->darkR), &(settings->darkG), &(settings->darkB));
		settings->darkR %= 256;
		settings->darkG %= 256;
		settings->darkB %= 256;
		fscanf(input, "%d %d %d", &(settings->lightR), &(settings->lightG), &(settings->lightB));
		settings->lightR %= 256;
		settings->lightG %= 256;
		settings->lightB %= 256;
		fclose(input);
	}
	else
	{
		settings->windowSize = 800;
		settings->soundSet = 1;
		settings->darkR = 40;
		settings->darkG = 40;
		settings->darkB = 40;
		settings->lightR = 166;
		settings->lightG = 128;
		settings->lightB = 100;
	}
}

bool initializeGUI(void)
{
	if (!al_init()) return 0;
	if (!al_install_mouse()) return 0;
	if (!al_init_image_addon()) return 0;
	if (!al_init_primitives_addon()) return 0;
	if (!al_install_audio()) return 0;
	if (!al_init_acodec_addon()) return 0;
	if (!al_reserve_samples(2)) return 0;
	return 1;
}

bool initializeWindows(ALLEGRO_DISPLAY ** display, int windowSize)
{
	if ((*display = al_create_display(windowSize, windowSize)) == NULL) return false;
	al_set_window_title(*display, "C_Chess");
	al_set_app_name("C_Chess");
	ALLEGRO_BITMAP * appIcon;
	if (appIcon = al_load_bitmap("./graphics/appIcon.png"))
	{
		al_set_display_icon(*display, appIcon);
		al_destroy_bitmap(appIcon);
	}
	return true;
}

bool initializeGraphics(ALLEGRO_BITMAP ** menuScreen, ALLEGRO_BITMAP * piecesArray[13])
{
	if (!(*menuScreen = al_load_bitmap("./graphics/menuScreen.png"))) return false;
	if (!(piecesArray[1] = al_load_bitmap("./graphics/blackKing.png"))) return false;
	if (!(piecesArray[2] = al_load_bitmap("./graphics/whiteKing.png"))) return false;
	if (!(piecesArray[3] = al_load_bitmap("./graphics/blackQueen.png"))) return false;
	if (!(piecesArray[4] = al_load_bitmap("./graphics/whiteQueen.png"))) return false;
	if (!(piecesArray[5] = al_load_bitmap("./graphics/blackRook.png"))) return false;
	if (!(piecesArray[6] = al_load_bitmap("./graphics/whiteRook.png"))) return false;
	if (!(piecesArray[7] = al_load_bitmap("./graphics/blackBishop.png"))) return false;
	if (!(piecesArray[8] = al_load_bitmap("./graphics/whiteBishop.png"))) return false;
	if (!(piecesArray[9] = al_load_bitmap("./graphics/blackKnight.png"))) return false;
	if (!(piecesArray[10] = al_load_bitmap("./graphics/whiteKnight.png"))) return false;
	if (!(piecesArray[11] = al_load_bitmap("./graphics/blackPawn.png"))) return false;
	if (!(piecesArray[12] = al_load_bitmap("./graphics/whitePawn.png"))) return false;
	return true;
}

bool initializeSounds(ALLEGRO_SAMPLE * sample[2], int soundSet)
{
	switch (soundSet)
	{
	case 2:
	{
		if (!(sample[0] = al_load_sample("./sounds/moveSound2.ogg"))) return false;
		if (!(sample[1] = al_load_sample("./sounds/captureSound2.ogg"))) return false;
		break;
	}
	default:
	{
		if (!(sample[0] = al_load_sample("./sounds/moveSound1.ogg"))) return false;
		if (!(sample[1] = al_load_sample("./sounds/captureSound1.ogg"))) return false;
		break;
	}
	}
	return true;
}

void runMenu(ALLEGRO_DISPLAY * display, ALLEGRO_BITMAP * menuScreen, ALLEGRO_SAMPLE * sample[2], int windowSize)
{
	ALLEGRO_TIMER * timer;
	timer = al_create_timer(1.0 / 60);

	ALLEGRO_EVENT_QUEUE * menuQueue;
	menuQueue = al_create_event_queue();
	al_register_event_source(menuQueue, al_get_timer_event_source(timer));
	al_register_event_source(menuQueue, al_get_mouse_event_source());
	al_register_event_source(menuQueue, al_get_display_event_source(display));
	
	ALLEGRO_EVENT event;
	ALLEGRO_MOUSE_STATE state;
	bool need_menu_redraw = true;

	al_start_timer(timer);

	while (1)
	{
		al_wait_for_event(menuQueue, &event);
		
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			exit(0);
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			al_get_mouse_state(&state);
			if (state.x > 383 * (double)windowSize / 1136 && state.x < 631 * (double)windowSize / 1136.0 && state.y > 430 * (double)windowSize / 1136 && state.y < 555 * (double)windowSize / 1136)
			{
				al_play_sample(sample[0], 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				break;
			}
			if (state.x > 383 * (double)windowSize / 1136 && state.x < 635 * (double)windowSize / 1136.0 && state.y > 589 * (double)windowSize / 1136 && state.y < 707 * (double)windowSize / 1136)
			{
				al_play_sample(sample[0], 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				break;
			}
			if (state.x > 72 * (double)windowSize / 1136 && state.x < 329 * (double)windowSize / 1136.0 && state.y > 1021 * (double)windowSize / 1136 && state.y < 1136 * (double)windowSize / 1136)
			{
				exit(0);
			}
		}

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			need_menu_redraw = true;
		}

		if (need_menu_redraw && al_is_event_queue_empty(menuQueue))
		{
			al_draw_scaled_bitmap(menuScreen, 0, 0, 1136, 1136, 0, 0, windowSize, windowSize, 0);
			al_flip_display();
			need_menu_redraw = false;
		}
	}
	al_destroy_event_queue(menuQueue);
	al_destroy_timer(timer);
}

void runGame(ALLEGRO_DISPLAY * display, ALLEGRO_SAMPLE * sample[2], settings settings, chessboard * chessboard, ALLEGRO_BITMAP * piecesArray[13])
{
	ALLEGRO_TIMER * timer;
	timer = al_create_timer(1.0 / 60);

	ALLEGRO_EVENT_QUEUE * queue;
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	ALLEGRO_EVENT event;
	ALLEGRO_MOUSE_STATE state;
	int squareSize = settings.windowSize / 8;
	int grabbedPiece_y = -squareSize, grabbedPiece_x = -squareSize;
	bool need_redraw = true;
	bool piece_was_released = false;
	bool piece_was_grabbed = false;
	
	al_start_timer(timer);

	while (1)
	{
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			break;
		}

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			need_redraw = true;
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			al_get_mouse_state(&state);
			if (state.buttons & 1)
			{
				grabbedPiece_x = state.x;
				grabbedPiece_y = state.y;
				if ((chessboard->chessboard[grabbedPiece_y / squareSize][grabbedPiece_x / squareSize] != empty) && (chessboard->chessboard[grabbedPiece_y / squareSize][grabbedPiece_x / squareSize] % 2 == chessboard->whoseMove))
				{
					piece_was_grabbed = true;
				}
				else
				{
					grabbedPiece_x = -squareSize;
					grabbedPiece_y = -squareSize;
				}
			}
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && piece_was_grabbed)
		{
			piece_was_grabbed = false;
			piece_was_released = true;
		}

		al_get_mouse_state(&state);
		if (need_redraw && al_is_event_queue_empty(queue))
		{
			al_clear_to_color(al_map_rgb(settings.lightR, settings.lightG, settings.lightB));

			for (int j = 0; j < 8; j++)
				for (int i = 0; i < 8; i++)
				{
					if ((i + j) % 2) al_draw_filled_rectangle(i*squareSize, j*squareSize, (i + 1)*squareSize, (j + 1)*squareSize, al_map_rgb(settings.darkR, settings.darkG, settings.darkB));
					if (i == (grabbedPiece_x / squareSize) && j == (grabbedPiece_y / squareSize)) continue;
					if (chessboard->chessboard[j][i]) al_draw_scaled_bitmap(piecesArray[chessboard->chessboard[j][i]], 0, 0, 142, 142, i*squareSize, j*squareSize, squareSize, squareSize, 0);
				}
			if (piece_was_grabbed)
			{
				al_draw_scaled_bitmap(piecesArray[chessboard->chessboard[grabbedPiece_y / squareSize][grabbedPiece_x / squareSize]], 0, 0, 142, 142, state.x - grabbedPiece_x % squareSize - (int)(0.08*squareSize), state.y - grabbedPiece_y % squareSize - (int)(0.16*squareSize), squareSize*1.16, squareSize*1.16, 0);
			}
			if (piece_was_released)
			{
				chessboard->sourceX = grabbedPiece_x / squareSize;
				chessboard->sourceY = grabbedPiece_y / squareSize;
				chessboard->targetX = (state.x - grabbedPiece_x % squareSize + (int)(0.5*squareSize)) / squareSize;
				chessboard->targetY = (state.y - grabbedPiece_y % squareSize + (int)(0.5*squareSize)) / squareSize;
				if (makeMoveIfPossible(chessboard))
				{
					if (chessboard->wasItCapture == true)
						al_play_sample(sample[1], 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					else al_play_sample(sample[0], 0.9, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				grabbedPiece_x = -squareSize;
				grabbedPiece_y = -squareSize;
				piece_was_released = false;
			}
			al_flip_display();
			need_redraw = false;
		}
	}
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
}

bool launchGUI(settings settings, chessboard * chessboard)
{
	if(!initializeGUI()) return -1;

	ALLEGRO_DISPLAY * display;
	if (initializeWindows(&display, settings.windowSize) == false) return -1;
	ALLEGRO_BITMAP * menuScreen; ALLEGRO_BITMAP * piecesArray[13];
	if (initializeGraphics(&menuScreen, piecesArray) == false) return -1;
	ALLEGRO_SAMPLE * sample[2];
	if (initializeSounds(sample, settings.soundSet) == false) return -1;
	
	runMenu(display, menuScreen, sample, settings.windowSize);
	
	runGame(display, sample, settings, chessboard, piecesArray);
	
	al_uninstall_mouse();
	al_destroy_display(display);
	al_destroy_bitmap(menuScreen);
	for (int i = 1; i < 13; i++)
		al_destroy_bitmap(piecesArray[i]);
	for (int i = 0; i < 2; i++)
		al_destroy_sample(sample[i]);

	return 0;
}

