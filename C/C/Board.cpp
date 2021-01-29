#include "Board.h"
#include <iostream>

using namespace std;





Board::Board() {}
Board::Board(Board* board) {//Copie du board par constructeur
    for (int i = 0; i < board->currentSize; i++) {
        cells[i] = board->cells[i];
    }
    this->totalSeed = board->totalSeed;
    this->P1Turn = board->P1Turn;
    this->scoreP1 = board->scoreP1;
    this->scoreP2 = board->scoreP2;
    this->isReduce = board->isReduce;
    this->currentSize = board->currentSize;
}

void Board::initBoard() {
    currentSize = START_SIZE;
    totalSeed = 4 * 24;//96 graines

    for (int i = 0; i < 24; i++) {//Start avec 4 graines
        cells[i] = 4;
    }

    scoreP2 = 0;
    scoreP1 = 0;
    P1Turn = true;
}

//Donne le nombre de graines sur les cellules du joueur P1 ou P2
int Board::nBSeedsOnCells(bool P1) {
    int seeds = 0;
    for (int i = 0; i < currentSize; i++) {
        if ((i % 2 == 0) == P1) {
            seeds += cells[i];
        }
    }
    return seeds;
}

bool Board::endPosition() {

    if (nBSeedsOnCells(P1Turn) == 0) {//Starving rule
        return true;
    }

    if (totalSeed < 8) {//Teacher rule
        return true;
    }

    return false;
}

//Conversion d'index
int Board::getIndex(int idx) {
    idx = idx % currentSize;

    if (idx < 0) {
        return currentSize + idx;
    }
    else {
        return idx;
    }
}

void Board::changeBoard() {
    currentSize = END_SIZE;
    isReduce = true;

    for (int i = 0; i < 12; i++) {//Merge des cases (Teacher rule)
        cells[i] = cells[i * 2] + cells[i * 2 + 1];
    }
}

void Board::printBoard() {
    for (int i = 0; i < currentSize / 2; i++) {
        if (i % 2 == 0) cout << "#";
        cout << cells[i] << " | ";
    }
    cout << endl;

    for (int i = currentSize - 1; i >= currentSize / 2; i--) {
        if (i % 2 == 0) cout << "#";
        cout << cells[i] << " | ";
    }
    cout << endl;
}

//Copy d'un board
void Board::copy(Board* board) {
    for (int i = 0; i < board->currentSize; i++) {
        cells[i] = board->cells[i];
    }
    this->totalSeed = board->totalSeed;
    this->P1Turn = board->P1Turn;
    this->scoreP1 = board->scoreP1;
    this->scoreP2 = board->scoreP2;
    this->isReduce = board->isReduce;
    this->currentSize = board->currentSize;
}

void Board::playMove(int move) {
    int initialIdx = move;
    //on retire les graine de la case
    int seeds = cells[move];
    cells[move] = 0;

    //On sème
    while (seeds != 0) {
        move = getIndex(move + 1);
        if (move != initialIdx) { //Loop rule
            cells[move]++;
            seeds--;
        }
    }

    //Prise
    while (cells[move] == 2 || cells[move] == 3) {
        if (P1Turn) {
            scoreP1 += cells[move];
        }
        else {
            scoreP2 += cells[move];
        }
        totalSeed -= cells[move];
        cells[move] = 0;
        move = getIndex(move - 1);
    }

    //Starving end
    if (nBSeedsOnCells(!P1Turn) == 0) {
        if (P1Turn) {//P1 take P2 starve
            scoreP1 += totalSeed;
        }
        else {
            scoreP2 += totalSeed;
        }
        totalSeed = 0;
    }

    if (!isReduce && totalSeed <= 48) {//Teacher rule
        changeBoard();
        if (nBSeedsOnCells(!P1Turn) == 0) {//Starving end
            if (P1Turn) {//P1 take P2 starve
                scoreP1 += totalSeed;
            }
            else {
                scoreP2 += totalSeed;
            }
            totalSeed = 0;
        }
    }
    P1Turn = !P1Turn;
}

bool Board::correctMove(int move) {

    if ((move % 2 == 0) != P1Turn) {
        return false;//Joue sur la case de l'autre.
    }

    if (cells[move] == 0) {
        return false;//Il joue sur une case vide.
    }

    return true;
}


