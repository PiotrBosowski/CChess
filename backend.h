#pragma once
#include "structures.h"

void randomizeChessboard(chessboard * chessboard);
void initializeChessboard(chessboard * chessboard);
bool checkCordsCorrectness(chessboard * chessboard);
bool checkSourcePieceColor(chessboard * chessboard);
bool checkTargetSquare(chessboard * chessboard);
bool checkNextPosition(chessboard * tempboard);
chessboard * copyPosition(chessboard * sourceboard);
bool checkKingsSafety(chessboard * chessboard);
bool checkKingMovement(chessboard * chessboard);
bool checkQueenMovement(chessboard * chessboard);
bool checkRookMovement(chessboard * chessboard);
bool checkBishopMovement(chessboard * chessboard);
bool checkKnightMovement(chessboard * chessboard);
bool checkPawnMovement(chessboard * chessboard);
bool checkPieceMovementAndCollision(chessboard * chessboard);
void handleException(chessboard * chessboard);
void changeTurn(chessboard * chessboard);
void makeMove(chessboard * chessboard);
void writeMoveToFile(chessboard * chessboard);
bool makeMoveIfPossible(chessboard * chessboard);
