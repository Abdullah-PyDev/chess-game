#pragma once

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(char c, int row, int col);

    bool isValidMove(int newX, int newY) override;

    char getSymbol() override;
};