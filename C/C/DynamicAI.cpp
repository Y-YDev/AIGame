#include "DynamicAI.h"

#include <iostream>
#include <ctime>
using namespace std;

DynamicAI::DynamicAI(bool isP1, int maxDepth) {
    this->isP1 = isP1;
    this->maxDepth = maxDepth;
    this->pool = PoolBoard(maxDepth + 100);
}
int DynamicAI::play(Board* board) {
    std::clock_t start;
    start = std::clock();

    turn++;
    if (turn == 1) {
        if (isP1) return convertIndex(isP1, 1);
        return convertIndex(isP1, 6);
    }

    computeDynDepth(board);

    int maxResult = 0;
    int index = -1;
    int alpha = -VALMAX;
    int beta = VALMAX;

    for (int i = 0; i < board->currentSize / 2; i++) {
        Board* playNext = pool.pollBoard(board);

        if (!playNext->correctMove(convertIndex(isP1, i))) {
            pool.pushBoard(playNext);
            continue;
        }
        //l'ia fait tout les coup possible 1 par 1
        playNext->playMove(convertIndex(isP1, i));
        //l'ia fait jouer les autre coup en partant de l'inverse
        int result = minMaxValue(playNext, !this->isP1, 1, alpha, beta);

        //On retient le coup le plus avantageux
        if (index == -1) {
            maxResult = result;
            index = i;
        }
        else if (isP1 && result > maxResult) {
            maxResult = result;
            index = i;
            alpha = result;
        }
        else if (!isP1 && result < maxResult) {
            maxResult = result;
            index = i;
            beta = result;
        }
        pool.pushBoard(playNext);
    }
    std::clock_t end;
    end = std::clock();
    lastTurnTime = (end - start) / (double)(CLOCKS_PER_SEC / 1000);

    cout << "----------------------------------" << endl;
    cout << "max depth = " << maxDepth << endl;
    cout << "max result = " << maxResult << endl;
    std::cout << "Time: " << (end - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return convertIndex(isP1, index);//Case 1 d'index 0
}

int DynamicAI::minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta) {

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
                current = minMaxValue(nextPos, !rushMax, depth + 1, alpha, beta);
                if (max < current) {
                    max = current;
                }
                pool.pushBoard(nextPos);
                if (alpha < current) {
                    alpha = current;
                    if (alpha >= beta) {
                        return alpha;
                    }
                }
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
                current = minMaxValue(nextPos, !rushMax, depth + 1, alpha, beta);
                if (min > current) {
                    min = current;
                }
                pool.pushBoard(nextPos);
                if (beta >= current) {
                    beta = current;
                    if (beta <= alpha) {
                        return beta;
                    }
                }
            }
        }
        return min;
    }

    return 0;
}
/*
int DynamicAI::evaluation(Board* board) {
    if (board->scoreP1 > 48) return VALMAX + board->scoreP1 - board->scoreP2;
    if (board->scoreP2 > 48) return -VALMAX + board->scoreP1 - board->scoreP2;
    return board->scoreP1 - board->scoreP2;
}*/
int DynamicAI::evaluation(Board* board) {
    if (board->scoreP1 > 48) return VALMAX;
    if (board->scoreP2 > 48) return -VALMAX;
    int score = (board->scoreP1 - board->scoreP2) * 1000;

    if (!board->isReduce) {
        for (int i = 0; i < board->currentSize; i++) {
            if (i % 4 < 2)
                score += board->cells[i];
            else
                score -= board->cells[i];
        }
    }

    return score;
}


int DynamicAI::convertIndex(bool isP1, int i) {
    if (isP1) {
        return i * 2;
    }
    else {
        return i * 2 + 1;
    }
}

void DynamicAI::computeDynDepth(Board* board) {
    if (lastTurnTime < 120) {
        maxDepth++;

        if (lastTurnTime < 10) {//TOOO FAST
            maxDepth++;
        }
    }
    
    else if (lastTurnTime > 1700) {
        maxDepth--;
    }
    if (capeMax < maxDepth) {
        maxDepth = capeMax;
    }
    if (board->isReduce && maxDepth < minDepthReduce) {
        maxDepth = minDepthReduce;
    }
}