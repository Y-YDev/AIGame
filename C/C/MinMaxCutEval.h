#pragma once
#include "Board.h"
#include "PoolBoard.h"

class MinMaxCutEval
{
public:
	MinMaxCutEval();
	MinMaxCutEval(bool, int);
	int play(Board* board);
	int minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta);
private:

	int evaluation(Board* board);
	int evaluationEnd(Board* board);
	int convertIndex(bool isP1, int i);
	bool isP1 = false;
	const int VALMAX = 100000;
	int maxDepth;
	PoolBoard pool;
};
