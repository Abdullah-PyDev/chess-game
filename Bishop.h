#pragma once

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(char c, int row, int col);

    bool isValidMove(int toRow, int toCol, const Board& board) const override;

    char getSymbol() override;
};
