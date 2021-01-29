#pragma once
#include "Board.h"

#include <thread>

class MultiDynEval
{
public:
	MultiDynEval();
	MultiDynEval(bool, int);
	int play(Board* board,int lastTurn);
	int minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta);
private:
	void threadPlay(Board* board, int index, int alpha, int beta);
	void computeDynDepth(Board* board);
	int evaluation(Board* board);
	int convertIndex(bool isP1, int i);
	bool isP1 = false;
	int turn = 0;

	int listScore[12];

	const int VALMAX = 100000;
	int winValue = 1000000;

	int maxDepth;
	int capeMax = 22;//Profondeur maximale.
	int minDepthReduce = 16;//Profondeur mini avec tableau réduit.

	long lastTurnTime = 500;
	std::thread threadList[12];
	bool finish[12];
};