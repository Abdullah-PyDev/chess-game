#include "Board.h"
#include "Queen.h"
#include <cmath>

Queen::Queen(char c, int row, int col)
    : Piece(c, row, col) {
}

char Queen::getSymbol() const {
    // FIX: correct symbol
    return (color == 'W') ? 'Q' : 'q';
}
bool Queen::canAttack(int r, int c, const Board& board)
{
    // Horizontal / Vertical (like rook)
    if (r == x || c == y)
    {
        int dr = (r == x) ? 0 : (r > x ? 1 : -1);
        int dc = (c == y) ? 0 : (c > y ? 1 : -1);

        int i = x + dr;
        int j = y + dc;

        while (i != r || j != c)
        {
            if (board.getPiece(i, j) != nullptr)
                return false;

            i += dr;
            j += dc;
        }

        return true;
    }

    // Diagonal (like bishop)
    if (abs(r - x) == abs(c - y))
    {
        int dr = (r > x) ? 1 : -1;
        int dc = (c > y) ? 1 : -1;

        int i = x + dr;
        int j = y + dc;

        while (i != r && j != c)
        {
            if (board.getPiece(i, j) != nullptr)
                return false;

            i += dr;
            j += dc;
        }

        return true;
    }

    return false;
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