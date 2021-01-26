#pragma once
#include "Board.h"
#include "PoolBoard.h"

class MinMaxClassique
{
public :
	MinMaxClassique();
	MinMaxClassique(bool, int);
	int play(Board* board);
	int minMaxValue(Board* board, bool rushMax, int depth);
private :
	
	int evaluation(Board* board);
	int convertIndex(bool isP1, int i);
	bool isP1 = false;
	const int VALMAX = 96;
	int maxDepth;
	PoolBoard pool;
};

