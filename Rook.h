#pragma once
#include "Piece.h"

class Rook : public Piece
{
public:
    Rook(char c, int row, int col);

    bool isValidMove(int toRow, int toCol, const Board& board) const override;
    bool canAttack(int row, int col, const Board& board) override;
    char getSymbol() const override;
};