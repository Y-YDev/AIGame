#include "BestIA.h"
#include <thread>
#include <iostream>
#include <ctime>
using namespace std;

BestIA::BestIA(bool isP1, int maxDepth) {
    this->isP1 = isP1;
    this->maxDepth = maxDepth;//Profondeur de départ
}

int BestIA::play(Board* board, int lastTurn) {
    turn++;
    std::clock_t start;
    start = std::clock();
    if (turn == 1) {
        if (isP1) return 6; //Meilleur coup pour P1 (3) 
        else {
            if (lastTurn % 4 < 2) { //Meilleur coup pour P2 en fonction du coup de P1
                return ((lastTurn + 9) % 24);
            }
            else {
                return ((lastTurn + 19) % 24);
            }
        }
        //else return convertIndex(isP1, 8); //if 3 -> 22 if 1 -> 10 TODO(best value)
    }
    if (turn > 3) {//Profondeur dynamique
        computeDynDepth(board);
    }

    int maxResult = 0;
    int alpha = -VALMAX * 2;
    int beta = VALMAX * 2;
    //m_thread = std::thread(&MinMaxMultiThread::threadPlay,this,board,0,alpha,beta);

    for (int i = 0; i < board->currentSize / 2; i++) {//Création des thread
        threadList[i] = thread(&BestIA::threadPlay, this, board, i, alpha, beta);
    }
    for (int i = 0; i < board->currentSize / 2; i++) { //Attente de la réponse de tout les threads
        threadList[i].join();
    }
    int index = 0;
    if (isP1) {//Récupération du score max
        maxResult = listScore[0];
        for (int i = 1; i < board->currentSize / 2; i++) {
            if (maxResult < listScore[i]) {
                maxResult = listScore[i];
                index = i;
            }
            if (maxResult == listScore[i] && i%4 <2) {
                maxResult = listScore[i];
                index = i;
            }
        }
    }
    else
    {//Récupération du score min
        maxResult = listScore[0];
        for (int i = 1; i < board->currentSize / 2; i++) {
            if (maxResult > listScore[i]) {
                maxResult = listScore[i];
                index = i;
            }
            if (maxResult == listScore[i] && i % 4 >= 2) {
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

    return convertIndex(isP1, index);//Ex : Case 1 d'index 0
}

void BestIA::threadPlay(Board* board, int index, int alpha, int beta) {
    Board playNext = Board(board);
    finish[index] = false;//Tableau de booléen pour savoir quel thread à finis


    if (!playNext.correctMove(convertIndex(isP1, index))) {//Move incorrecte sur cette case
        if (isP1) listScore[index] = -VALMAX * 2;
        else listScore[index] = VALMAX * 2;
        finish[index] = true;
        return;
    }
    //l'ia fait tout les coup possible 1 par 1
    playNext.playMove(convertIndex(isP1, index));

    //Minmax classique
    listScore[index] = minMaxValue(&playNext, !this->isP1, 1, alpha, beta);
    finish[index] = true;
    return;
}

int BestIA::minMaxValue(Board* board, bool rushMax, int depth, int alpha, int beta) {

    if (board->endPosition()) {//Position finale -> evaluation
        if (board->scoreP1 == board->scoreP2) return 0;
        else if (board->scoreP1 > board->scoreP2) return VALMAX + board->scoreP1 - board->scoreP2;
        else return -VALMAX + board->scoreP1 - board->scoreP2;
    }
    if (depth == maxDepth) {//Si on atteind la profondeur maximale
        return evaluation(board);
    }
    int nbPlay = board->currentSize / 2;

    if (rushMax) {
        int max = -VALMAX * 2;
        int current;
        for (int i = 0; i < nbPlay; i++) {

            if (board->correctMove(convertIndex(rushMax, i))) {

                Board nextPos = Board(board);//Position suivante
                nextPos.playMove(convertIndex(rushMax, i));
                //MinMax 
                current = minMaxValue(&nextPos, !rushMax, depth + 1, alpha, beta);

                if (max < current) {
                    max = current;
                }
                if (alpha < current) { //Coupe Alpha Beta
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

                Board nextPos = Board(board);//Position suivante
                nextPos.playMove(convertIndex(rushMax, i));
                current = minMaxValue(&nextPos, !rushMax, depth + 1, alpha, beta);

                if (min > current) {
                    min = current;

                }

                if (beta >= current) {//Coupe Alpha Beta
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

int BestIA::evaluation(Board* board) {//Evaluation naive.
    if (board->scoreP1 > 48) return VALMAX + board->scoreP1 - board->scoreP2;
    if (board->scoreP2 > 48) return -VALMAX + board->scoreP1 - board->scoreP2;
    return board->scoreP1 - board->scoreP2;
}

int BestIA::convertIndex(bool isP1, int i) {
    if (isP1) {
        return i * 2;
    }
    else {
        return i * 2 + 1;
    }
}

void BestIA::computeDynDepth(Board* board) {
    if (lastTurnTime < 450) {//TOO FAST
        maxDepth++;

        if (lastTurnTime < 10) {//REALLY TOOO FAST
            maxDepth++;
        }
    }

    else if (lastTurnTime > 1300) {//TOO SLOW
        maxDepth--;
    }
    if (capeMax < maxDepth) {
        maxDepth = capeMax;
    }
    if (board->isReduce && maxDepth < minDepthReduce) {//Min deep for reduce board
        maxDepth = minDepthReduce;
    }
}