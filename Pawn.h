#pragma once

#include "Piece.h"
// pawn class inhrits Piece
class Pawn : public Piece {
public:
    Pawn(char c, int row, int col);
    
    bool isValidMove(int toRow, int toCol, const Board& board) const override;
    bool canAttack(int row, int col, const Board& board) override;
    char getSymbol() const override;
};
