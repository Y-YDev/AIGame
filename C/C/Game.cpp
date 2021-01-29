#include "Game.h"
#include "Board.h"
#include <iostream>
#include <sstream>
#include "MinMaxClassique.h"
#include "MinMaxCut.h"
#include "DynamicAI.h"
#include "MinMaxCutEval.h"
#include "MinMaxMultiThread.h"
#include "MultiThreadDyn.h"
#include "MultiThreadDynNoPool.h"
#include "MultiDynEval.h"
#include <string>
#include "BestIA.h"

#include <Windows.h>

using namespace std;
class Game
{
public:

    Board* board;
    BestIA* P1;
    BestIA* P2;
    int turn = 1;
    Game() {
        board = new Board();
        board->initBoard();//Init du plateau
        P1 = new BestIA(true, 9);
        P2 = new BestIA(false,9);
    }

    void gameLoop(){

        board->printBoard();
        int lastTurn = 0;
        while (!board->endPosition()) {


            int move;
            if (board->P1Turn) {
                move = P1->play(board, lastTurn);//Récupération du coup
                //jouer sans l'ia depuis la console
                //cin >> move;
                //move -= 1;

                cout<< "P1 a joue : " << (move + 1) << endl;
            }
            else {
                move = P2->play(board, lastTurn);//Récupération du coup
                //jouer sans l'ia depuis la console
                //cin >> move;
                //move -= 1;
                cout << "P2 a joue : " << (move + 1) << endl;

            }

            if (board->correctMove(move)) {//Si le coup est correct
                lastTurn = move;
                board->playMove(move);//On joue rééllement le coup
                board->printBoard();
                cout << "SCORE P1 = " << board->scoreP1 << " | SCORE P2 = " << board->scoreP2 << endl;
                cout << "TOUR " << turn << endl;
                cout << endl;
                turn++;
            }
            else {//INCORRECT MOVE
                cout << "coup incorrect" << endl;
            }
            

        }
        //FIN DE PARTIE
        cout<< "SCORE P1 = " << board->scoreP1 << " | SCORE P2 = " << board->scoreP2;
        cout << endl;
    }

};



int main() {

    Game game;
    game.gameLoop();
    string input;
    while (input.compare("end") != 0) {
        cin >> input;
    }
        

}