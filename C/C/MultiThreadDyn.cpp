#include "MultiThreadDyn.h"
#include <thread>
#include <iostream>
#include <ctime>
using namespace std;

MultiThreadDyn::MultiThreadDyn(bool isP1, int maxDepth) {
    this->isP1 = isP1;
    this->maxDepth = maxDepth;
    for (int i = 0; i < 12; i++) {
        allPool[i] = new PoolBoard(maxDepth + 25);
    }

}
int MultiThreadDyn::play(Board* board) {
    std::clock_t start;
    start = std::clock();

    computeDynDepth(board);

    int maxResult = 0;
    int alpha = -VALMAX;
    int beta = VALMAX;
    thread threadList[12];
    //m_thread = std::thread(&MinMaxMultiThread::threadPlay,this,board,0,alpha,beta);

    for (int i = 0; i < board->currentSize / 2; i++) {
        threadList[i] = thread(&MultiThreadDyn::threadPlay, this, board, i, alpha, beta, allPool[i]);

    }
    for (int i = 0; i < board->currentSize / 2; i++) {
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

void MultiThreadDyn::threadPlay(Board* board, int index, int alpha, int beta, PoolBoard* pool) {
    Board* playNext = pool->pollBoard(board);


    if (!playNext->correctMove(convertIndex(isP1, index))) {
        pool->pushBoard(playNext);
        if (isP1) listScore[index] = -VALMAX - 1;
        else listScore[index] = VALMAX + 1;
        return;
    }
    //l'ia fait tout les coup possible 1 par 1
    playNext->playMove(convertIndex(isP1, index));
    //l'ia fait jouer les autre coup en partant de l'inverse
    listScore[index] = minMaxValue(playNext, !this->isP1, 1, alpha, beta, pool);
    pool->pushBoard(playNext);
    return;
}

int MultiThreadDyn::minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta, PoolBoard* pool) {

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
                Board* nextPos = pool->pollBoard(board);
                nextPos->playMove(convertIndex(rushMax, i));
                current = minMaxValue(nextPos, !rushMax, depth + 1, alpha, beta, pool);
                if (max < current) {
                    max = current;
                }
                pool->pushBoard(nextPos);
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
                Board* nextPos = pool->pollBoard(board);
                nextPos->playMove(convertIndex(rushMax, i));
                current = minMaxValue(nextPos, !rushMax, depth + 1, alpha, beta, pool);
                if (min > current) {
                    min = current;
                }
                pool->pushBoard(nextPos);
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

int MultiThreadDyn::evaluation(Board* board) {
    if (board->scoreP1 > 48) return VALMAX + board->scoreP1 - board->scoreP2;
    if (board->scoreP2 > 48) return -VALMAX + board->scoreP1 - board->scoreP2;
    return board->scoreP1 - board->scoreP2;
}

int MultiThreadDyn::convertIndex(bool isP1, int i) {
    if (isP1) {
        return i * 2;
    }
    else {
        return i * 2 + 1;
    }
}

void MultiThreadDyn::computeDynDepth(Board* board) {
    if (lastTurnTime < 180) {
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