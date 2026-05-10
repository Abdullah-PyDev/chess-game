#include "Board.h"
#include "Queen.h"
#include <cmath>

Queen::Queen(char c, int row, int col)
    : Piece(c, row, col) {
}

char Queen::getSymbol() {
    // FIX: correct symbol
    return (color == 'W') ? 'Q' : 'q';
}

bool Queen::isValidMove(int toRow, int toCol, const Board& board) const {

    // same original condition (kept)
    if (!(abs(toRow - x) == abs(toCol - y)) && (!(x == toRow || y == toCol)))
        return false;

    // 2. Determine movement direction (kept style, only safer handling)
    int rowStep = 0;
    if (toRow > x)
        rowStep = 1;
    else if (toRow < x)
        rowStep = -1;

    int colStep = 0;
    if (toCol > y)
        colStep = 1;
    else if (toCol < y)
        colStep = -1;

    // check path (unchanged logic)
    int r = x + rowStep;
    int c = y + colStep;

    while (r != toRow || c != toCol) {

        if (board.getPiece(r, c) != nullptr)
            return false;

        r += rowStep;
        c += colStep;
    }

    Piece* destination = board.getPiece(toRow, toCol);

    if (destination == nullptr || destination->getColor() != color)
        return true;

    return false;
}