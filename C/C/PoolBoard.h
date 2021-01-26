#pragma once
#include "Board.h"

#ifndef POOLBOARD_H
#define POOLBOARD_H 
class PoolBoard
{
public :
	PoolBoard();
	PoolBoard(int);
	Board* pollBoard(Board* toCopy);
	void pushBoard(Board* board);

private:
	
	Board** allBoard;
	int currentIndex;

};
#endif
