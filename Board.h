#pragma once
#include "Piece.h"

class Board {
private:
    Piece* grid[8][8];

public:
    Board();
    ~Board();

    // Returns piece at given position
    Piece* getPiece(int row, int col) const;

    // Moves piece if move is valid
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);

    // Checks board boundaries
    bool isInsideBoard(int row, int col) const;

    // Place a piece manually on board
    void setPiece(int row, int col, Piece* piece);
};