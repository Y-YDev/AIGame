#include "MinMaxMultiThread.h"
#include <thread>
#include <iostream>
#include <ctime>
using namespace std;

MinMaxMultiThread::MinMaxMultiThread(bool isP1, int maxDepth) {
    this->isP1 = isP1;
    this->maxDepth = maxDepth;
    for (int i = 0; i < 12; i++) {
        allPool[i] = new PoolBoard(maxDepth + 5);
    }

}
int MinMaxMultiThread::play(Board* board) {
    std::clock_t start;
    start = std::clock();

    int maxResult = 0;
    int alpha = -VALMAX;
    int beta = VALMAX;
    thread threadList[12];
    //m_thread = std::thread(&MinMaxMultiThread::threadPlay,this,board,0,alpha,beta);

    for (int i = 0; i < board->currentSize / 2; i++) {
        threadList[i] = thread(&MinMaxMultiThread::threadPlay, this, board, i, alpha, beta,allPool[i]);
        
    }
    for (int i = 0; i < board->currentSize / 2; i++) {
        threadList[i].join();
    }
    int index = 0;
    if (isP1) {
        maxResult = listScore[0];
        for (int i = 1; i < board->currentSize / 2;i++) {
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
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return convertIndex(isP1, index);//Case 1 d'index 0
}

void MinMaxMultiThread::threadPlay(Board* board,int index, int alpha, int beta, PoolBoard* pool) {
    Board* playNext = pool->pollBoard(board);


    if (!playNext->correctMove(convertIndex(isP1, index))) {
        pool->pushBoard(playNext);
        if (isP1) listScore[index] = -VALMAX-1;
        else listScore[index] = VALMAX+1;
        return;
    }
    //l'ia fait tout les coup possible 1 par 1
    playNext->playMove(convertIndex(isP1, index));
    //l'ia fait jouer les autre coup en partant de l'inverse
    listScore[index] = minMaxValue(playNext, !this->isP1, 1, alpha, beta,pool);
    pool->pushBoard(playNext);
    return;
}

int MinMaxMultiThread::minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta, PoolBoard* pool) {

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

int MinMaxMultiThread::evaluation(Board* board) {
    if (board->scoreP1 > 48) return VALMAX + board->scoreP1 - board->scoreP2;
    if (board->scoreP2 > 48) return -VALMAX + board->scoreP1 - board->scoreP2;
    return board->scoreP1 - board->scoreP2;
}

int MinMaxMultiThread::convertIndex(bool isP1, int i) {
    if (isP1) {
        return i * 2;
    }
    else {
        return i * 2 + 1;
    }
}
