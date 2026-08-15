#pragma once
#include "Board.h"

class Game {
private:
    Board board;
    char currentTurn;
    int enPassantRow;
    int enPassantCol;

public:
    Game();

    void setupBoard();

    void displayBoard();

    void start();
};
