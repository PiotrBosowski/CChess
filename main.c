#include "structures.h"
#include "frontend.h"
#include "backend.h"

int main()
{ 
	chessboard chessboard;
	initializeChessboard(&chessboard);

	settings settings;
	loadSettings(&settings);

	return launchGUI(settings, &chessboard);
}