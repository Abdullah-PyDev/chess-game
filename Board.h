#pragma once
#include "Piece.h"

class Board {
private:
    Piece* grid[8][8];

public:
    Board();
    ~Board();

    Piece* getPiece(int row, int col) const;

    void setPiece(int row, int col, Piece* piece);

    bool isInsideBoard(int row, int col) const;

    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);

    bool isCheck(char kingColor);

    bool canEscapeCheck(char kingColor);

    bool isCheckmate(char kingColor);
};