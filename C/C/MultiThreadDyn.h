#pragma once
#include "Board.h"
#include "PoolBoard.h"
#include <thread>

class MultiThreadDyn
{
public:
	MultiThreadDyn();
	MultiThreadDyn(bool, int);
	int play(Board* board);
	int minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta, PoolBoard* pool);
private:
	void threadPlay(Board* board, int index, int alpha, int beta, PoolBoard* pool);
	void computeDynDepth(Board* board);
	int evaluation(Board* board);
	int convertIndex(bool isP1, int i);
	bool isP1 = false;

	PoolBoard* allPool[12];
	int listScore[12];

	const int VALMAX = 100000;
	int winValue = 1000000;

	int maxDepth;
	int capeMax = 22;//Profondeur maximale.
	int minDepthReduce = 17;//Profondeur mini avec tableau réduit.
	PoolBoard pool;
	int turn = 0;
	long lastTurnTime = 500;

	std::thread m_thread;
};


