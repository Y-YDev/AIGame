#include "PoolBoard.h"

PoolBoard::PoolBoard() {}

PoolBoard::PoolBoard(int size) {
	allBoard = new Board*[size];
	for (int i = 0; i < size; i++) {
		allBoard[i] = new Board();
	}
	currentIndex = -1;
}

Board* PoolBoard::pollBoard(Board* toCopy) {
	currentIndex++;
	allBoard[currentIndex]->copy(toCopy);
	return allBoard[currentIndex];
}
void PoolBoard::pushBoard(Board* board) {
	allBoard[currentIndex]= board;
	currentIndex--;
}
