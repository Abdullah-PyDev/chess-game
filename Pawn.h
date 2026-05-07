#pragma once

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(char c, int row, int col);

    bool isValidMove(int toRow, int toCol, const Board& board) const override;

    char getSymbol() override;
};