#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"

void randomizeChessboard(chessboard * chessboard)
{
	srand((unsigned int)time(NULL));

	int chessRank[8] = { 0 };

	chessRank[2 * (rand() % 4)] = bBishop;
	chessRank[2 * (rand() % 4) + 1] = bBishop;

	int kingPosition = rand() % 4 + 1;
	int i = 0;
	int emptySquaresCounter = 0;
	while (i < 8)
	{
		if (chessRank[i] == 0)
		{
			if (emptySquaresCounter == kingPosition)
			{
				chessRank[i] = bKing;
				kingPosition = i;
				chessboard->wKing_posX = i;
				chessboard->bKing_posX = i;
				break;
			}
			emptySquaresCounter++;
		}
		i++;
	}

	int leftSideEmptySquares = 0;
	int rightSideEmptySquares = 0;
	i = 0;
	while (i < 8)
	{
		if (chessRank[i] == 0) leftSideEmptySquares++;
		else if (i == kingPosition) break;
		i++;
	}
	while (i < 8)
	{
		if (chessRank[i] == 0) rightSideEmptySquares++;
		i++;
	}

	int leftRookPosition = rand() % leftSideEmptySquares;
	int rightRookPosition = rand() % rightSideEmptySquares;
	i = 0;
	emptySquaresCounter = 0;
	while (i < kingPosition)
	{
		if (chessRank[i] == 0)
		{
			if (emptySquaresCounter == leftRookPosition)
			{
				chessRank[i] = bRook;
				break;
			}
			emptySquaresCounter++;
		}
		i++;
	}
	i = kingPosition + 1;
	emptySquaresCounter = 0;
	while (i < 8)
	{
		if (chessRank[i] == 0)
		{
			if (emptySquaresCounter == rightRookPosition)
			{
				chessRank[i] = bRook;
				break;
			}
			emptySquaresCounter++;
		}
		i++;
	}

	int queenPosition = rand() % 3;
	i = 0;
	emptySquaresCounter = 0;
	while (i < 8)
	{
		if (chessRank[i] == 0)
		{
			if (emptySquaresCounter == queenPosition)
			{
				chessRank[i] = bQueen;
				break;
			}
			emptySquaresCounter++;
		}
		i++;
	}

	i = 0;
	while (i < 8)
	{
		if (chessRank[i] == 0) chessRank[i] = bKnight;
		i++;
	}

	i = 0;
	while (i<8)
	{
		chessboard->chessboard[0][i] = chessRank[i];
		chessboard->chessboard[7][i] = chessRank[i] + 1;
		i++;
	}
}

void initializeChessboard(chessboard * chessboard)
{
	chessboard->chessboard[0][0] = bRook; chessboard->chessboard[0][1] = bKnight; chessboard->chessboard[0][2] = bBishop; chessboard->chessboard[0][3] = bQueen; chessboard->chessboard[0][4] = bKing; chessboard->chessboard[0][5] = bBishop; chessboard->chessboard[0][6] = bKnight; chessboard->chessboard[0][7] = bRook;
	for (int i = 0; i < 8; i++) chessboard->chessboard[1][i] = bPawn;
	for (int i = 0; i < 8; i++) for (int j = 2; j < 6; j++) chessboard->chessboard[j][i] = empty;
	for (int i = 0; i < 8; i++) chessboard->chessboard[6][i] = wPawn;
	chessboard->chessboard[7][0] = wRook; chessboard->chessboard[7][1] = wKnight; chessboard->chessboard[7][2] = wBishop; chessboard->chessboard[7][3] = wQueen; chessboard->chessboard[7][4] = wKing; chessboard->chessboard[7][5] = wBishop; chessboard->chessboard[7][6] = wKnight; chessboard->chessboard[7][7] = wRook;
	chessboard->wKing_posX = 4;
	chessboard->wKing_posY = 7;
	chessboard->bKing_posX = 4;
	chessboard->bKing_posY = 0;
	chessboard->blackCanCastleKingside = true;
	chessboard->blackCanCastleQueenside = true;
	chessboard->whiteCanCastleKingside = true;
	chessboard->whiteCanCastleQueenside = true;
	chessboard->lastWhiteDoublePawnMove = -1;
	chessboard->lastBlackDoublePawnMove = -1;
	chessboard->lastExceptionMove = noExceptionMoves;
	chessboard->whoseMove = whiteToMove;
}

bool checkCordsCorrectness(chessboard * chessboard)
{
	if (chessboard->targetX >= 0 && chessboard->targetX <= 7 && chessboard->targetY >= 0 && chessboard->targetY <= 7) return true;
	else return false;
}

bool checkSourcePieceColor(chessboard * chessboard)
{
	if (chessboard->chessboard[chessboard->sourceY][chessboard->sourceX] % 2 == chessboard->whoseMove) return true;
	else return false;
}

bool checkTargetSquare(chessboard * chessboard)
{
	if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] == empty)
	{
		chessboard->wasItCapture = false;
		return true;
	}
	else if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] % 2 != chessboard->whoseMove)
	{
		chessboard->wasItCapture = true;
		return true;
	}
	return false;
}

bool checkNextPosition(chessboard * tempboard)
{
	if ((tempboard->wKing_posX - tempboard->bKing_posX)*(tempboard->wKing_posX - tempboard->bKing_posX) < 4 && (tempboard->wKing_posY - tempboard->bKing_posY)*(tempboard->wKing_posY - tempboard->bKing_posY) < 4) return false; //king

	int King_posY, King_posX;
	if (tempboard->whoseMove == whiteToMove)
	{
		if (tempboard->wKing_posY > 1)
		{
			if (tempboard->wKing_posX >= 1) if (tempboard->chessboard[tempboard->wKing_posY - 1][tempboard->wKing_posX - 1] == bPawn) return false;
			if (tempboard->wKing_posX <= 6) if (tempboard->chessboard[tempboard->wKing_posY - 1][tempboard->wKing_posX + 1] == bPawn) return false;
		}
		King_posY = tempboard->wKing_posY;
		King_posX = tempboard->wKing_posX;
	}
	else
	{
		if (tempboard->bKing_posY < 6)
		{
			if (tempboard->bKing_posX >= 1) if (tempboard->chessboard[tempboard->bKing_posY + 1][tempboard->bKing_posX - 1] == wPawn) return false;
			if (tempboard->bKing_posX <= 6) if (tempboard->chessboard[tempboard->bKing_posY + 1][tempboard->bKing_posX + 1] == wPawn) return false;
		}
		King_posY = tempboard->bKing_posY;
		King_posX = tempboard->bKing_posX;
	}

	if (King_posY >= 2)//knight
	{
		if (King_posX <= 6) if (tempboard->chessboard[King_posY - 2][King_posX + 1] == bKnight + tempboard->whoseMove) return false;
		if (King_posX >= 1) if (tempboard->chessboard[King_posY - 2][King_posX - 1] == bKnight + tempboard->whoseMove) return false;
	}
	if (King_posY >= 1)
	{
		if (King_posX <= 5) if (tempboard->chessboard[King_posY - 1][King_posX + 2] == bKnight + tempboard->whoseMove) return false;
		if (King_posX >= 2) if (tempboard->chessboard[King_posY - 1][King_posX - 2] == bKnight + tempboard->whoseMove) return false;
	}
	if (King_posY <= 6)
	{
		if (King_posX <= 5) if (tempboard->chessboard[King_posY + 1][King_posX + 2] == bKnight + tempboard->whoseMove) return false;
		if (King_posX >= 2) if (tempboard->chessboard[King_posY + 1][King_posX - 2] == bKnight + tempboard->whoseMove) return false;
	}
	if (King_posY <= 5)
	{
		if (King_posX <= 6) if (tempboard->chessboard[King_posY + 2][King_posX + 1] == bKnight + tempboard->whoseMove) return false;
		if (King_posX >= 1) if (tempboard->chessboard[King_posY + 2][King_posX - 1] == bKnight + tempboard->whoseMove) return false;
	}

	bool line1isSafe = false, line2isSafe = false, line3isSafe = false, line4isSafe = false;
	for (int i = 1; i < 8; i++)							   //lines
	{
		if (!line1isSafe)
		{
			if (King_posX + i < 8)
			{
				if (tempboard->chessboard[King_posY][King_posX + i] != 0)
				{
					if (tempboard->chessboard[King_posY][King_posX + i] == bRook + tempboard->whoseMove || tempboard->chessboard[King_posY][King_posX + i] == bQueen + tempboard->whoseMove) return false;
					else line1isSafe = true;
				}
			}
		}
		if (!line2isSafe)
		{
			if (King_posX - i >= 0)
			{
				if (tempboard->chessboard[King_posY][King_posX - i] != 0)
				{
					if (tempboard->chessboard[King_posY][King_posX - i] == bRook + tempboard->whoseMove || tempboard->chessboard[King_posY][King_posX - i] == bQueen + tempboard->whoseMove) return false;
					else line2isSafe = true;
				}
			}
		}
		if (!line3isSafe)
		{
			if (King_posY + i < 8)
			{
				if (tempboard->chessboard[King_posY + i][King_posX] != 0)
				{
					if (tempboard->chessboard[King_posY + i][King_posX] == bRook + tempboard->whoseMove || tempboard->chessboard[King_posY + i][King_posX] == bQueen + tempboard->whoseMove) return false;
					else line3isSafe = true;
				}
			}
		}
		if (!line4isSafe)
		{
			if (King_posY - i < 8)
			{
				if (tempboard->chessboard[King_posY - i][King_posX] != 0)
				{
					if (tempboard->chessboard[King_posY - i][King_posX] == bRook + tempboard->whoseMove || tempboard->chessboard[King_posY - i][King_posX] == bQueen + tempboard->whoseMove) return false;
					else line4isSafe = true;
				}
			}
		}
	}

	line1isSafe = line2isSafe = line3isSafe = line4isSafe = false;
	for (int i = 1; i < 8; i++)							//diagonals
	{
		if (!line1isSafe)
		{
			if (King_posY + i < 8 && King_posX + i < 8)
			{
				if (tempboard->chessboard[King_posY + i][King_posX + i] != 0)
				{
					if (tempboard->chessboard[King_posY + i][King_posX + i] == bBishop + tempboard->whoseMove || tempboard->chessboard[King_posY + i][King_posX + i] == bQueen + tempboard->whoseMove) return false;
					else line1isSafe = true;
				}
			}
		}
		if (!line2isSafe)
		{
			if (King_posY + i < 8 && King_posX - i >= 0)
			{
				if (tempboard->chessboard[King_posY + i][King_posX - i] != 0)
				{
					if (tempboard->chessboard[King_posY + i][King_posX - i] == bBishop + tempboard->whoseMove || tempboard->chessboard[King_posY + i][King_posX - i] == bQueen + tempboard->whoseMove) return false;
					else line2isSafe = true;
				}
			}
		}
		if (!line3isSafe)
		{
			if (King_posY - i >= 0 && King_posX + i < 8)
			{
				if (tempboard->chessboard[King_posY - i][King_posX + i] != 0)
				{
					if (tempboard->chessboard[King_posY - i][King_posX + i] == bBishop + tempboard->whoseMove || tempboard->chessboard[King_posY - i][King_posX + i] == bQueen + tempboard->whoseMove) return false;
					else line3isSafe = true;
				}
			}
		}
		if (!line4isSafe)
		{
			if (King_posY - i >= 0 && King_posX - i >= 0)
			{
				if (tempboard->chessboard[King_posY - i][King_posX - i] != 0)
				{
					if (tempboard->chessboard[King_posY - i][King_posX - i] == bBishop + tempboard->whoseMove || tempboard->chessboard[King_posY - i][King_posX - i] == bQueen + tempboard->whoseMove) return false;
					else line4isSafe = true;
				}
			}
		}
	}
	return true;
}

void copyPosition(chessboard * sourceboard, chessboard * tempboard)
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			tempboard->chessboard[j][i] = sourceboard->chessboard[j][i];

	tempboard->sourceX = sourceboard->sourceX;
	tempboard->sourceY = sourceboard->sourceY;
	tempboard->targetX = sourceboard->targetX;
	tempboard->targetY = sourceboard->targetY;
	tempboard->wKing_posX = sourceboard->wKing_posX;
	tempboard->wKing_posY = sourceboard->wKing_posY;
	tempboard->bKing_posX = sourceboard->bKing_posX;
	tempboard->bKing_posY = sourceboard->bKing_posY;
	tempboard->lastWhiteDoublePawnMove = sourceboard->lastWhiteDoublePawnMove;
	tempboard->lastBlackDoublePawnMove = sourceboard->lastBlackDoublePawnMove;
	tempboard->lastExceptionMove = sourceboard->lastExceptionMove;
	tempboard->whiteCanCastleKingside = sourceboard->whiteCanCastleKingside;
	tempboard->whiteCanCastleQueenside = sourceboard->whiteCanCastleQueenside;
	tempboard->blackCanCastleKingside = sourceboard->blackCanCastleKingside;
	tempboard->blackCanCastleQueenside = sourceboard->blackCanCastleQueenside;
	tempboard->wasItCapture = sourceboard->wasItCapture;
	tempboard->whoseMove = sourceboard->whoseMove;
}

void makeMove(chessboard * chessboard)
{
	chessboard->chessboard[chessboard->targetY][chessboard->targetX] = chessboard->chessboard[chessboard->sourceY][chessboard->sourceX];
	chessboard->chessboard[chessboard->sourceY][chessboard->sourceX] = 0;
	switch (chessboard->chessboard[chessboard->targetY][chessboard->targetX])
	{
	case wKing:
	{
		chessboard->whiteCanCastleKingside = false;
		chessboard->whiteCanCastleQueenside = false;
		chessboard->wKing_posX = chessboard->targetX;
		chessboard->wKing_posY = chessboard->targetY;
		break;
	}
	case bKing:
	{
		chessboard->blackCanCastleKingside = false;
		chessboard->blackCanCastleQueenside = false;
		chessboard->bKing_posX = chessboard->targetX;
		chessboard->bKing_posY = chessboard->targetY;
		break;
	}
	case wRook:
	{
		if (chessboard->sourceX == 0) chessboard->whiteCanCastleQueenside = false; else
		if (chessboard->sourceX == 7) chessboard->whiteCanCastleKingside = false;
		break;
	}
	case bRook:
	{
		if (chessboard->sourceX == 0) chessboard->blackCanCastleQueenside = false; else
		if (chessboard->sourceX == 7) chessboard->blackCanCastleKingside = false;
		break;
	}
	}
}

void handleException(chessboard * chessboard)
{
	if (chessboard->lastExceptionMove == enPassantCapture)
	{
		chessboard->chessboard[chessboard->sourceY][chessboard->targetX] = 0;
	}
	if (chessboard->lastExceptionMove == promotion)
	{
		if (chessboard->targetY == 0) chessboard->chessboard[chessboard->targetY][chessboard->targetX] = wQueen;
		if (chessboard->targetY == 7) chessboard->chessboard[chessboard->targetY][chessboard->targetX] = bQueen;
	}
	if (chessboard->lastExceptionMove == whiteCastleKingside)
	{
		chessboard->chessboard[7][7] = 0;
		chessboard->chessboard[7][5] = wRook;
	}
	if (chessboard->lastExceptionMove == whiteCastleQueenside)
	{
		chessboard->chessboard[7][0] = 0;
		chessboard->chessboard[7][3] = wRook;
	}
	if (chessboard->lastExceptionMove == blackCastleKingside)
	{
		chessboard->chessboard[0][7] = 0;
		chessboard->chessboard[0][5] = bRook;
	}
	if (chessboard->lastExceptionMove == blackCastleQueenside)
	{
		chessboard->chessboard[0][0] = 0;
		chessboard->chessboard[0][3] = bRook;
	}

	if (chessboard->whoseMove == whiteToMove) chessboard->lastBlackDoublePawnMove = -1;
	else /*chessboard->whoseMove == blackToMove*/ chessboard->lastWhiteDoublePawnMove = -1;

	chessboard->lastExceptionMove = noExceptionMoves;
}

bool checkKingsSafety(chessboard * chessboard)
{
	struct chessboard tempboard;
	copyPosition(chessboard, &tempboard);
	if (tempboard.lastExceptionMove == whiteCastleKingside)
	{
		tempboard.sourceX = 4;
		tempboard.targetX = 5;
		tempboard.wKing_posX = 5;
		makeMove(&tempboard);
		if (!checkNextPosition(&tempboard)) return false;
		tempboard.sourceX = 5;
		tempboard.targetX = 6;
		tempboard.wKing_posX = 6;
		makeMove(&tempboard);
		return checkNextPosition(&tempboard);
	}
	if (tempboard.lastExceptionMove == whiteCastleQueenside)
	{
		tempboard.sourceX = 4;
		tempboard.targetX = 3;
		tempboard.wKing_posX = 3;
		makeMove(&tempboard);
		if (!checkNextPosition(&tempboard)) return false;
		tempboard.sourceX = 3;
		tempboard.targetX = 2;
		tempboard.wKing_posX = 2;
		makeMove(&tempboard);
		return checkNextPosition(&tempboard);
	}
	if (tempboard.lastExceptionMove == blackCastleKingside)
	{
		tempboard.sourceX = 4;
		tempboard.targetX = 5;
		tempboard.bKing_posX = 5;
		makeMove(&tempboard);
		if (!checkNextPosition(&tempboard)) return false;
		tempboard.sourceX = 5;
		tempboard.targetX = 6;
		tempboard.bKing_posX = 6;
		makeMove(&tempboard);
		return checkNextPosition(&tempboard);
	}
	if (tempboard.lastExceptionMove == blackCastleQueenside)
	{
		tempboard.sourceX = 4;
		tempboard.targetX = 3;
		tempboard.bKing_posX = 3;
		makeMove(&tempboard);
		if (!checkNextPosition(&tempboard)) return false;
		tempboard.sourceX = 3;
		tempboard.targetX = 2;
		tempboard.bKing_posX = 2;
		makeMove(&tempboard);
		return checkNextPosition(&tempboard);
	}
	makeMove(&tempboard);
	handleException(&tempboard);
	return checkNextPosition(&tempboard);
}

bool checkKingMovement(chessboard * chessboard)
{
	if ((chessboard->targetY - chessboard->sourceY)*(chessboard->targetY - chessboard->sourceY) + (chessboard->targetX - chessboard->sourceX)*(chessboard->targetX - chessboard->sourceX) > 2)
	{
		if (chessboard->whoseMove == whiteToMove)
		{
			if (chessboard->whiteCanCastleKingside)
			{
				if (chessboard->targetY == 7 && chessboard->targetX == 6)
				{
					if (chessboard->chessboard[7][5] == empty && chessboard->chessboard[7][6] == empty)
					{
						chessboard->lastExceptionMove = whiteCastleKingside;
						return true;
					}
				}
			}
			if (chessboard->whiteCanCastleQueenside)
			{
				if (chessboard->targetY == 7 && chessboard->targetX == 2)
				{
					if (chessboard->chessboard[7][1] == empty && chessboard->chessboard[7][2] == empty && chessboard->chessboard[7][3] == empty)
					{
						chessboard->lastExceptionMove = whiteCastleQueenside;
						return true;
					}
				}
			}
		}
		else if (chessboard->whoseMove == blackToMove)
		{
			if (chessboard->blackCanCastleKingside)
			{
				if (chessboard->targetY == 0 && chessboard->targetX == 6)
				{
					if (chessboard->chessboard[0][5] == empty && chessboard->chessboard[0][6] == empty)
					{
						chessboard->lastExceptionMove = blackCastleKingside;
						return true;
					}
				}
			}
			if (chessboard->blackCanCastleQueenside)
			{
				if (chessboard->targetY == 0 && chessboard->targetX == 2)
				{
					if (chessboard->chessboard[0][1] == empty && chessboard->chessboard[0][2] == empty && chessboard->chessboard[0][3] == empty)
					{
						chessboard->lastExceptionMove = blackCastleQueenside;
						return true;
					}
				}
			}
		}
	}
	else return true;
	return  false;
}
bool checkQueenMovement(chessboard * chessboard)
{
	if (chessboard->sourceX == chessboard->targetX)//pionowo
	{
		if (chessboard->sourceY > chessboard->targetY) //do przodu
		{
			for (int i = chessboard->sourceY - 1; i > chessboard->targetY; i--)
			{
				if (chessboard->chessboard[i][chessboard->sourceX] != 0) return false;
			}
			return true;
		}
		if (chessboard->sourceY < chessboard->targetY)//do tylu
		{
			for (int i = chessboard->sourceY + 1; i < chessboard->targetY; i++)
			{
				if (chessboard->chessboard[i][chessboard->sourceX] != 0) return false;
			}
			return true;
		}
	}
	if (chessboard->sourceY == chessboard->targetY)//poziomo
	{
		if (chessboard->sourceX > chessboard->targetX)//w lewo
		{
			for (int i = chessboard->sourceX - 1; i > chessboard->targetX; i--)
			{
				if (chessboard->chessboard[chessboard->sourceY][i] != 0) return false;
			}
			return true;
		}
		if (chessboard->sourceX < chessboard->targetX)//w prawo
		{
			for (int i = chessboard->sourceX + 1; i < chessboard->targetX; i++)
			{
				if (chessboard->chessboard[chessboard->sourceY][i] != 0) return false;
			}
			return true;
		}
	}
	//po skosie
	if (abs(chessboard->sourceX - chessboard->targetX) == abs(chessboard->sourceY - chessboard->targetY))
	{
		if (chessboard->sourceX < chessboard->targetX) //w prawo
		{
			if (chessboard->sourceY < chessboard->targetY) //w dol
			{
				for (int i = 1; chessboard->sourceX + i < chessboard->targetX; i++)
				{
					if (chessboard->chessboard[chessboard->sourceY + i][chessboard->sourceX + i] != 0) return false;
				}
				return true;
			}
			if (chessboard->sourceY > chessboard->targetY) //w gore
			{
				for (int i = 1; chessboard->sourceX + i < chessboard->targetX; i++)
				{
					if (chessboard->chessboard[chessboard->sourceY - i][chessboard->sourceX + i] != 0) return false;
				}
				return true;
			}
		}
		if (chessboard->sourceX > chessboard->targetX) //w lewo
		{
			if (chessboard->sourceY < chessboard->targetY) //w dol
			{
				for (int i = 1; chessboard->sourceX - i > chessboard->targetX; i++)
				{
					if (chessboard->chessboard[chessboard->sourceY + i][chessboard->sourceX - i] != 0) return false;
				}
				return true;
			}
			if (chessboard->sourceY > chessboard->targetY) //w gore
			{
				for (int i = 1; chessboard->sourceX - i > chessboard->targetX; i++)
				{
					if (chessboard->chessboard[chessboard->sourceY - i][chessboard->sourceX - i] != 0) return false;
				}
				return true;
			}
		}
	}
	return false;
}
bool checkRookMovement(chessboard * chessboard)
{
	if (chessboard->sourceX == chessboard->targetX)//pionowo
	{
		if (chessboard->sourceY > chessboard->targetY) //do przodu
		{
			for (int i = chessboard->sourceY - 1; i > chessboard->targetY; i--)
			{
				if (chessboard->chessboard[i][chessboard->sourceX] != 0) return false;
			}
			return true;
		}
		if (chessboard->sourceY < chessboard->targetY)//do tylu
		{
			for (int i = chessboard->sourceY + 1; i < chessboard->targetY; i++)
			{
				if (chessboard->chessboard[i][chessboard->sourceX] != 0) return false;
			}
			return true;
		}
	}
	if (chessboard->sourceY == chessboard->targetY)//poziomo
	{
		if (chessboard->sourceX > chessboard->targetX)//w lewo
		{
			for (int i = chessboard->sourceX - 1; i > chessboard->targetX; i--)
			{
				if (chessboard->chessboard[chessboard->sourceY][i] != 0) return false;
			}
			return true;
		}
		if (chessboard->sourceX < chessboard->targetX)//w prawo
		{
			for (int i = chessboard->sourceX + 1; i < chessboard->targetX; i++)
			{
				if (chessboard->chessboard[chessboard->sourceY][i] != 0) return false;
			}
			return true;
		}
	}
	return false;
}
bool checkBishopMovement(chessboard * chessboard)
{
	if (abs(chessboard->sourceX - chessboard->targetX) == abs(chessboard->sourceY - chessboard->targetY))
	{
		if (chessboard->sourceX < chessboard->targetX) //w prawo
		{
			if (chessboard->sourceY < chessboard->targetY) //w dol
			{
				for (int i = 1; chessboard->sourceX + i < chessboard->targetX; i++)
				{
					if (chessboard->chessboard[chessboard->sourceY + i][chessboard->sourceX + i] != 0) return false;
				}
				return true;
			}
			if (chessboard->sourceY > chessboard->targetY) //w gore
			{
				for (int i = 1; chessboard->sourceX + i < chessboard->targetX; i++)
				{
					if (chessboard->chessboard[chessboard->sourceY - i][chessboard->sourceX + i] != 0) return false;
				}
				return true;
			}
		}
		if (chessboard->sourceX > chessboard->targetX) //w lewo
		{
			if (chessboard->sourceY < chessboard->targetY) //w dol
			{
				for (int i = 1; chessboard->sourceX - i > chessboard->targetX; i++)
				{
					if (chessboard->chessboard[chessboard->sourceY + i][chessboard->sourceX - i] != 0) return false;
				}
				return true;
			}
			if (chessboard->sourceY > chessboard->targetY) //w gore
			{
				for (int i = 1; chessboard->sourceX - i > chessboard->targetX; i++)
				{
					if (chessboard->chessboard[chessboard->sourceY - i][chessboard->sourceX - i] != 0) return false;
				}
				return true;
			}
		}
	}
	return false;
}
bool checkKnightMovement(chessboard * chessboard)
{
	if ((chessboard->targetY - chessboard->sourceY)*(chessboard->targetY - chessboard->sourceY) + (chessboard->targetX - chessboard->sourceX)*(chessboard->targetX - chessboard->sourceX) == 5) return true;
	return false;
}
bool checkPawnMovement(chessboard * chessboard)
{
	if (chessboard->chessboard[chessboard->sourceY][chessboard->sourceX] == bPawn)
	{
		if(chessboard->targetY == 7)
		{
			chessboard->lastExceptionMove = promotion;
		}
		if (chessboard->targetX == chessboard->sourceX)
		{
			if (chessboard->targetY == chessboard->sourceY + 1)
			{
				if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] == 0) return true;
			}
			if (chessboard->targetY == chessboard->sourceY + 2)
			{
				if (chessboard->sourceY == 1 && chessboard->chessboard[chessboard->sourceY + 1][chessboard->sourceX] == 0)
				{
					if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] == 0)
					{
						chessboard->lastBlackDoublePawnMove = chessboard->sourceX;
						return true;
					}
				}
			}
		}
		else if (chessboard->targetX == chessboard->sourceX + 1 || chessboard->targetX == chessboard->sourceX - 1)
		{
			if (chessboard->targetY == chessboard->sourceY + 1)
			{
				if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] != 0)
				{
					if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] % 2 == 0) return true;
				}
				else if (chessboard->sourceY == 4)
				{
					if (chessboard->lastWhiteDoublePawnMove == chessboard->targetX)
					{
						chessboard->lastExceptionMove = enPassantCapture; //capturing en passant
						return true;
					}
				}
			}
		}
	}
	if (chessboard->chessboard[chessboard->sourceY][chessboard->sourceX] == wPawn)
	{
		if (chessboard->targetY == 0)
		{
			chessboard->lastExceptionMove = promotion;
		}
		if (chessboard->targetX == chessboard->sourceX)
		{
			if (chessboard->targetY == chessboard->sourceY - 1)
			{
				if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] == 0) return true;
			}
			if (chessboard->targetY == chessboard->sourceY - 2)
			{
				if (chessboard->sourceY == 6 && chessboard->chessboard[chessboard->sourceY - 1][chessboard->sourceX] == 0)
				{
					if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] == 0)
					{
						chessboard->lastWhiteDoublePawnMove = chessboard->sourceX;
						return true;
					}
				}
			}
		}
		else if (chessboard->targetX == chessboard->sourceX + 1 || chessboard->targetX == chessboard->sourceX - 1)
		{
			if (chessboard->targetY == chessboard->sourceY - 1)
			{
				if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] != 0)
				{
					if (chessboard->chessboard[chessboard->targetY][chessboard->targetX] % 2 == 1) return true;
				}
				else if (chessboard->sourceY == 3)
				{
					if (chessboard->lastBlackDoublePawnMove == chessboard->targetX)
					{
						chessboard->lastExceptionMove = enPassantCapture; //capturing en passant
						return true;
					}
				}
			}
		}
	}
	return false;
}
bool checkPieceMovementAndCollision(chessboard * chessboard)
{
	switch (chessboard->chessboard[chessboard->sourceY][chessboard->sourceX])
	{
	case wKing:
	{
		return checkKingMovement(chessboard);
	}
	case bKing:
	{
		return checkKingMovement(chessboard);
	}
	case wQueen:
	{
		return checkQueenMovement(chessboard);
	}
	case bQueen:
	{
		return checkQueenMovement(chessboard);
	}
	case wRook:
	{
		return checkRookMovement(chessboard);
	}
	case bRook:
	{
		return checkRookMovement(chessboard);
	}
	case wBishop:
	{
		return checkBishopMovement(chessboard);
	}
	case bBishop:
	{
		return checkBishopMovement(chessboard);
	}
	case wKnight:
	{
		return checkKnightMovement(chessboard);
	}
	case bKnight:
	{
		return checkKnightMovement(chessboard);
	}
	case wPawn:
	{
		return checkPawnMovement(chessboard);
	}
	case bPawn:
	{
		return checkPawnMovement(chessboard);
	}
	}
}

void changeTurn(chessboard * chessboard)
{
	chessboard->whoseMove = (chessboard->whoseMove == whiteToMove) ? blackToMove : whiteToMove;
}

void writeMoveToFile(chessboard * chessboard)
{
	FILE * output;
	output = fopen("./games/lastmoves.txt", "a");
	if (output)
	{
		fprintf(output, "%c%d -> %c%d\n", 65 + chessboard->sourceX, 8 - chessboard->sourceY, 65 + chessboard->targetX, 8 - chessboard->targetY);
		fclose(output);
	}
}

bool makeMoveIfPossible(chessboard * chessboard)
{
	if (checkCordsCorrectness(chessboard) == false) return false;
	if (checkSourcePieceColor(chessboard) == false) return false;
	if (checkTargetSquare(chessboard) == false) return false;
	if (checkPieceMovementAndCollision(chessboard) == false) return false;
	if (checkKingsSafety(chessboard) == false) return false;
	
	writeMoveToFile(chessboard);
	makeMove(chessboard);
	handleException(chessboard);
	changeTurn(chessboard);

	return true;
}
