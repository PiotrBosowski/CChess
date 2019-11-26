#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "structures.h"

void loadSettings(settings * settings);
bool initializeGUI(void);
bool initializeWindows(ALLEGRO_DISPLAY ** display, int windowSize);
bool initializeGraphics(ALLEGRO_BITMAP ** menuScreen, ALLEGRO_BITMAP * piecesArray[13]);
bool initializeSounds(ALLEGRO_SAMPLE * sample[2], int soundSet);
void runMenu(ALLEGRO_DISPLAY * display, ALLEGRO_BITMAP * menuScreen, ALLEGRO_SAMPLE * sample[2], int windowSize);
void runGame(ALLEGRO_DISPLAY * display, ALLEGRO_SAMPLE * sample[2], settings settings, chessboard * chessboard, ALLEGRO_BITMAP * piecesArray[13]);
bool launchGUI(settings settings, chessboard * chessboard);