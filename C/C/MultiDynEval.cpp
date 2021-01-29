#include "MultiDynEval.h"
#include <thread>
#include <iostream>
#include <ctime>
using namespace std;

MultiDynEval::MultiDynEval(bool isP1, int maxDepth) {
    this->isP1 = isP1;
    this->maxDepth = maxDepth;
}
int MultiDynEval::play(Board* board,int lastTurn) {
    turn++;
    std::clock_t start;
    start = std::clock();
    if (turn == 1) {
        if (isP1) return 6; //best 3
        else {
            if (lastTurn % 4 < 2) {
                return ((lastTurn + 9) % 24);
            }
            else {
                return ((lastTurn + 19) % 24);
            }
        }
        //else return convertIndex(isP1, 8); //if 3 -> 22 if 1 -> 10 TODO(best value)
    }

    computeDynDepth(board);

    int maxResult = 0;
    int alpha = -VALMAX*2;
    int beta = VALMAX*2;
    //m_thread = std::thread(&MinMaxMultiThread::threadPlay,this,board,0,alpha,beta);

    for (int i = 0; i < board->currentSize / 2; i++) {
        threadList[i] = thread(&MultiDynEval::threadPlay, this, board, i, alpha, beta);

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

void MultiDynEval::threadPlay(Board* board, int index, int alpha, int beta) {
    Board playNext = Board(board);
    finish[index] = false;
    //cout << "thread start : " << index << endl;

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
    //cout << "thread end : " << index << endl;
    return;
}

int MultiDynEval::minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta) {

    if (board->endPosition()) {
        if (board->scoreP1 == board->scoreP2) return 0;
        else if (board->scoreP1 > board->scoreP2) return VALMAX + board->scoreP1 - board->scoreP2;
        else return -VALMAX + board->scoreP1 - board->scoreP2;
    }
    if (depth == maxDepth) {
        return evaluation(board);
    }
    int nbPlay = board->currentSize / 2;
    if (rushMax) {
        int max = -VALMAX * 2;
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
        int min = VALMAX * 2;
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

int MultiDynEval::evaluation(Board* board) {
    if (board->scoreP1 > 48) return VALMAX + board->scoreP1 - board->scoreP2;
    if (board->scoreP2 > 48) return -VALMAX + board->scoreP1 - board->scoreP2;
    int score = (board->scoreP1 - board->scoreP2) * 1000;

    if (!board->isReduce) {
        for (int i = 0; i < board->currentSize; i++) {
            if (i % 4 < 2) {
                score += board->cells[i];
            }
            else {
                score -= board->cells[i];
            }
        }
    }
    return score;
}

int MultiDynEval::convertIndex(bool isP1, int i) {
    if (isP1) {
        return i * 2;
    }
    else {
        return i * 2 + 1;
    }
}

void MultiDynEval::computeDynDepth(Board* board) {
    if (lastTurnTime < 400) {
        maxDepth++;

        if (lastTurnTime < 40) {//TOOO FAST
            maxDepth++;
        }
    }

    else if (lastTurnTime > 1300) {
        maxDepth--;
    }
    if (capeMax < maxDepth) {
        maxDepth = capeMax;
    }
    if (board->isReduce && maxDepth < minDepthReduce) {
        maxDepth = minDepthReduce;
    }
}