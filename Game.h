#pragma once
#include "Board.h"

class Game {
private:
    /*Board board;
    char currentTurn;*/

public:
    Game();
    Board board;
    int lastFromRow = -1;
    int lastFromCol = -1;
    int lastToRow = -1;
    int lastToCol = -1;
    char currentTurn;

    void setupBoard();

    void displayBoard();

    void start();
   
};