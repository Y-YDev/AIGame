#include "MinMaxClassique.h"
#include <iostream>
#include <ctime>
using namespace std;

MinMaxClassique::MinMaxClassique(bool isP1, int maxDepth) {
    this->isP1 = isP1;
    this->maxDepth = maxDepth;
    this->pool = PoolBoard(maxDepth + 5);
}
int MinMaxClassique::play(Board* board) {
    std::clock_t start;
    start = std::clock();

    int maxResult = 0;
    int index = -1;

    for (int i = 0; i < board->currentSize / 2; i++) {
        Board* playNext = pool.pollBoard(board);

        if (!playNext->correctMove(convertIndex(isP1, i))) {
            pool.pushBoard(playNext);
            continue;
        }
        //l'ia fait tout les coup possible 1 par 1
        playNext->playMove(convertIndex(isP1, i));
        //l'ia fait jouer les autre coup en partant de l'inverse
        int result = minMaxValue(playNext, !this->isP1, 1);

        //On retient le coup le plus avantageux
        if (index == -1) {
            maxResult = result;
            index = i;
        }
        else if (isP1 && result > maxResult) {
            maxResult = result;
            index = i;
        }
        else if (!isP1 && result < maxResult) {
            maxResult = result;
            index = i;
        }
        pool.pushBoard(playNext);
    }
    cout << "----------------------------------" << endl;
    cout<< "max result = " << maxResult << endl;
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return convertIndex(isP1, index);//Case 1 d'index 0
}

int MinMaxClassique::minMaxValue(Board* board, bool rushMax, int depth) {

    if (board->endPosition()) {
        if (board->scoreP1 == board->scoreP2) return 0;
        else if (board->scoreP1 > board->scoreP2) return VALMAX;
        else return -VALMAX;
    }
    if (depth == maxDepth) {
        return evaluation(board);
    }
    int nbPlay = board->currentSize / 2;
    if (rushMax) {
        int max = -VALMAX;
        int current;
        for (int i = 0; i < nbPlay; i++) {
            if (board->correctMove(convertIndex(rushMax, i))) {
                Board* nextPos = pool.pollBoard(board);
                nextPos->playMove(convertIndex(rushMax, i));
                current = minMaxValue(nextPos, !rushMax, depth + 1);
                if (max < current) {
                    max = current;
                }
                pool.pushBoard(nextPos);
            }
        }
        return max;
    }
    else {
        int min = VALMAX;
        int current;
        for (int i = 0; i < nbPlay; i++) {
            if (board->correctMove(convertIndex(rushMax, i))) {
                Board* nextPos = pool.pollBoard(board);
                nextPos->playMove(convertIndex(rushMax, i));
                current = minMaxValue(nextPos, !rushMax, depth + 1);
                if (min > current) {
                    min = current;
                }
                pool.pushBoard(nextPos);
            }
        }
        return min;
    }

    return 0;
}

int MinMaxClassique::evaluation(Board* board) {
    if (board->scoreP1 > 48) return VALMAX + board->scoreP1 - board->scoreP2;
    if (board->scoreP2 > 48) return -VALMAX + board->scoreP1 - board->scoreP2;
    return board->scoreP1 - board->scoreP2;
}

int MinMaxClassique::convertIndex(bool isP1, int i) { 
    if (isP1) {
        return i * 2;
    }
    else {
        return i * 2 + 1;
    }
}