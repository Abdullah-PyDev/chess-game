#pragma once
#include "Board.h"

class Game {
private:
    Board board;
    char currentTurn;

public:
    Game();

    void setupBoard();

    void displayBoard();

    void start();
};