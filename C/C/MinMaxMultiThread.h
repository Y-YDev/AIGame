#pragma once
#include "Board.h"
#include "PoolBoard.h"
#include <thread>

class MinMaxMultiThread
{
public:
	MinMaxMultiThread();
	MinMaxMultiThread(bool, int);
	int play(Board* board);
	int minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta, PoolBoard* pool);
private:
	void threadPlay(Board* board,int index, int alpha, int beta,PoolBoard* pool);
	int evaluation(Board* board);
	int convertIndex(bool isP1, int i);
	bool isP1 = false;
	const int VALMAX = 96;
	int maxDepth;
	PoolBoard* allPool[12];
	int listScore[12];

	std::thread m_thread;
};


