#pragma once
#define bool _Bool
#define true 1
#define false 0
#define _CRT_SECURE_NO_WARNINGS

enum pieceTypeEnum
{
	empty = 0, bKing = 1, wKing = 2, bQueen = 3, wQueen = 4, bRook = 5, wRook = 6, bBishop = 7, wBishop = 8, bKnight = 9, wKnight = 10, bPawn = 11, wPawn = 12
};

enum whoseMoveEnum
{
	whiteToMove = 0, blackToMove = 1
};
enum exceptionMovesEnum
{
	noExceptionMoves = 0, whiteCastleKingside, whiteCastleQueenside, blackCastleKingside, blackCastleQueenside,  enPassantCapture, promotion
};

typedef struct settings
{
	int windowSize;
	int darkR, darkG, darkB;
	int lightR, lightG, lightB;
	int soundSet;
}settings;

typedef struct chessboard
{
	int chessboard[8][8];
	int sourceX;
	int sourceY;
	int targetX;
	int targetY;
	int wKing_posX;
	int wKing_posY;
	int bKing_posX;
	int bKing_posY;
	int lastWhiteDoublePawnMove;
	int lastBlackDoublePawnMove;
	int lastExceptionMove;
	bool whiteCanCastleKingside;
	bool whiteCanCastleQueenside;
	bool blackCanCastleKingside;
	bool blackCanCastleQueenside;
	bool wasItCapture;
	bool whoseMove;
}chessboard;
