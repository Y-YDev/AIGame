#pragma once
#include "Board.h"
#include "PoolBoard.h"

class DynamicAI
{
public:
	DynamicAI();
	DynamicAI(bool, int);
	int play(Board* board);
	void computeDynDepth(Board* board);
	int minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta);
private:

	int evaluation(Board* board);
	int convertIndex(bool isP1, int i);

	bool isP1 = false;
	const int VALMAX = 96;
	int winValue = 1000000;

	int maxDepth;
	int capeMax = 22;//Profondeur maximale.
	int minDepthReduce = 15;//Profondeur mini avec tableau réduit.
	PoolBoard pool;
	int turn = 0;
	long lastTurnTime = 500;
};
