#include "Game.h"
#include "Board.h"
#include <iostream>
#include <sstream>
#include "MinMaxClassique.h"
#include "MinMaxCut.h"
#include "DynamicAI.h"
#include <string>


#include <Windows.h>

using namespace std;
class Game
{
public:
    Board* board;
    DynamicAI* P1;
    MinMaxCut* P2;

    Game() {
        board = new Board();
        board->initBoard();
        P1 = new DynamicAI(true, 9);
        P2 = new MinMaxCut(false, 9);
    }

    void gameLoop(){

        board->printBoard();
        while (!board->endPosition()) {


            int move;
            if (board->P1Turn) {
                move = P1->play(board);
                cout<< "P1 a joue : " << (move + 1) << endl;
            }
            else {
                move = P2->play(board);
                cout << "P2 a joue : " << (move + 1) << endl;
            }

            if (board->correctMove(move)) {
                board->playMove(move);
                board->printBoard();//Print
                cout << "SCORE P1 = " << board->scoreP1 << " | SCORE P2 = " << board->scoreP2 << endl;
                cout << endl;
            }
            else {//INCORRECT MOVE
                cout << "coup incorrect" << endl;
            }
            

        }

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