#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
    Knight(char c, int row, int col);

    bool isValidMove(int newX, int newY) override;

    char getSymbol() override;
};