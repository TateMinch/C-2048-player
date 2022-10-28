#ifndef __PLAYER_H
#define __PLAYER_H
#include <list>
#include "board.h"

class Player
{
	Board board;
public:
	Player ();
	ValidMove makeMove(const Board);
	double calculateScore(const Board, ValidMove);
	double generateScore(const Board, int, int);
	double calculateMoveScore(const Board, int, int);
	double calculateFinalScore(const Board);
private:
	int weights[4][4];
};

#endif
