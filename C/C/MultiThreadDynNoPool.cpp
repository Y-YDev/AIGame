#include "MultiThreadDynNoPool.h"
#include <thread>
#include <iostream>
#include <ctime>
using namespace std;

MultiThreadDynNoPool::MultiThreadDynNoPool(bool isP1, int maxDepth) {
    this->isP1 = isP1;
    this->maxDepth = maxDepth;
}
int MultiThreadDynNoPool::play(Board* board) {
    std::clock_t start;
    start = std::clock();

    computeDynDepth(board);

    int maxResult = 0;
    int alpha = -VALMAX;
    int beta = VALMAX;
    //m_thread = std::thread(&MinMaxMultiThread::threadPlay,this,board,0,alpha,beta);

    for (int i = 0; i < board->currentSize / 2; i++) {
        threadList[i] = thread(&MultiThreadDynNoPool::threadPlay, this, board, i, alpha, beta);

    }
    for (int i = 0; i < board->currentSize / 2; i++) { //TODO finish test 1s resend thread with depth -1 
        threadList[i].join();
    }
    int index = 0;
    if (isP1) {
        maxResult = listScore[0];
        for (int i = 1; i < board->currentSize / 2; i++) {
            if (maxResult < listScore[i]) {
                maxResult = listScore[i];
                index = i;
            }
        }
    }
    else
    {
        maxResult = listScore[0];
        for (int i = 1; i < board->currentSize / 2; i++) {
            if (maxResult > listScore[i]) {
                maxResult = listScore[i];
                index = i;
            }
        }
    }
    cout << "----------------------------------" << endl;
    cout << "max result = " << maxResult << endl;
    cout << "max depth = " << maxDepth << endl;
    lastTurnTime = (std::clock() - start);
    std::cout << "Time: " << lastTurnTime << " ms" << std::endl;
    return convertIndex(isP1, index);//Case 1 d'index 0
}

void MultiThreadDynNoPool::threadPlay(Board* board, int index, int alpha, int beta) {
    Board playNext = Board(board);
    finish[index] = false;


    if (!playNext.correctMove(convertIndex(isP1, index))) {
        if (isP1) listScore[index] = -VALMAX * 2;
        else listScore[index] = VALMAX * 2;
        finish[index] = true;
        return;
    }
    //l'ia fait tout les coup possible 1 par 1
    playNext.playMove(convertIndex(isP1, index));
    //l'ia fait jouer les autre coup en partant de l'inverse
    listScore[index] = minMaxValue(&playNext, !this->isP1, 1, alpha, beta);
    finish[index] = true;
    return;
}

int MultiThreadDynNoPool::minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta) {

    if (board->endPosition()) {
        if (board->scoreP1 == board->scoreP2) return 0;
        else if (board->scoreP1 > board->scoreP2) return VALMAX + 100 + board->scoreP1 - board->scoreP2;
        else return -VALMAX - 100 + board->scoreP1 - board->scoreP2;
    }
    if (depth == maxDepth) {
        return evaluation(board);
    }
    int nbPlay = board->currentSize / 2;
    if (rushMax) {
        int max = -VALMAX*2;
        int current;
        for (int i = 0; i < nbPlay; i++) {
            if (board->correctMove(convertIndex(rushMax, i))) {
                Board nextPos = Board(board);
                nextPos.playMove(convertIndex(rushMax, i));
                current = minMaxValue(&nextPos, !rushMax, depth + 1, alpha, beta);
                if (max < current) {
                    max = current;
                    if (max >= VALMAX) {
                        return max;
                    }
                }
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
        int min = VALMAX*2;
        int current;
        for (int i = 0; i < nbPlay; i++) {
            if (board->correctMove(convertIndex(rushMax, i))) {
                Board nextPos = Board(board);
                nextPos.playMove(convertIndex(rushMax, i));
                current = minMaxValue(&nextPos, !rushMax, depth + 1, alpha, beta);
                if (min > current) {
                    min = current;
                    if (min <= -VALMAX) {
                        return min;
                    }
                }

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

int MultiThreadDynNoPool::evaluation(Board* board) {
    if (board->scoreP1 > 48) return VALMAX + board->scoreP1 - board->scoreP2;
    if (board->scoreP2 > 48) return -VALMAX + board->scoreP1 - board->scoreP2;
    return board->scoreP1 - board->scoreP2;
}

int MultiThreadDynNoPool::convertIndex(bool isP1, int i) {
    if (isP1) {
        return i * 2;
    }
    else {
        return i * 2 + 1;
    }
}

void MultiThreadDynNoPool::computeDynDepth(Board* board) {
    if (lastTurnTime < 140) {
        maxDepth++;

        if (lastTurnTime < 10) {//TOOO FAST
            maxDepth++;
        }
    }

    else if (lastTurnTime > 1100) {
        maxDepth--;
    }
    if (capeMax < maxDepth) {
        maxDepth = capeMax;
    }
    if (board->isReduce && maxDepth < minDepthReduce) {
        maxDepth = minDepthReduce;
    }
}