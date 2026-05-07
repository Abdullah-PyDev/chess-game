#pragma once

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(char c, int row, int col);

    bool isValidMove(int toRow, int toCol, const Board& board) const override;

    char getSymbol() override;
};
