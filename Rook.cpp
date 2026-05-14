#include "Rook.h"
#include "Board.h"
#include <cmath>

Rook::Rook(char c, int row, int col) : Piece(c, row, col)
{
}

char Rook::getSymbol() const
{
    return (color == 'W') ? 'R' : 'r';
}

bool Rook::canAttack(int r, int c, const Board& board)
{
    if (r != x && c != y)
        return false;

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

bool Rook::isValidMove(int toRow, int toCol, const Board& board) const
{
    if (x != toRow && y != toCol)
        return false;

    int rowStep = 0;
    if (toRow > x) rowStep = 1;
    else if (toRow < x) rowStep = -1;

    int colStep = 0;
    if (toCol > y) colStep = 1;
    else if (toCol < y) colStep = -1;

    int r = x + rowStep;
    int c = y + colStep;

    while (r != toRow || c != toCol)
    {
        if (board.getPiece(r, c) != nullptr)
            return false;

        r += rowStep;
        c += colStep;
    }

    Piece* dest = board.getPiece(toRow, toCol);
    return dest == nullptr || dest->getColor() != color;
}