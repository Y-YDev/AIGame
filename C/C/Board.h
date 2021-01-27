#pragma once
#ifndef BOARD_H
#define BOARD_H 
const int START_SIZE = 24;
const int END_SIZE = 12;
class Board {

	public:
		int cells[24];
		int totalSeed;
		int currentSize;
		bool isReduce = false;

		int scoreP1;
		int scoreP2;
		bool P1Turn = true;

		Board();
		Board(Board* board);

		void initBoard();

		int nBSeedsOnCells(bool P1);

		bool endPosition();

		int getIndex(int idx);

		void changeBoard();

		void printBoard();
		void copy(Board* board);

		void playMove(int move);

		bool correctMove(int move);



};
#endif