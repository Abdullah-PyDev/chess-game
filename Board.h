#pragma once
#include "Piece.h"

class Board
{
private:
    Piece* grid[8][8];

public:
    Board();

    Piece* getPiece(int row, int col) const;
    void   setPiece(int row, int col, Piece* piece);

    bool isInsideBoard(int row, int col) const;

    // returns true if the move succeeded, also handles castling rook move
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);

    bool isCheck(char kingColor);
    bool isCheckmate(char kingColor);
    bool canEscapeCheck(char kingColor);

    ~Board();
};