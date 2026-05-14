#include "King.h"
#include "Board.h"
#include <cmath>

King::King(char c, int row, int col) : Piece(c, row, col)
{
}

char King::getSymbol() const
{
    return (color == 'W') ? 'K' : 'k';
}

bool King::canAttack(int r, int c, const Board& board)
{
    return abs(r - x) <= 1 && abs(c - y) <= 1;
}

bool King::isValidMove(int toRow, int toCol, const Board& board) const
{
    if (!board.isInsideBoard(toRow, toCol))
        return false;

    int rowDiff = abs(toRow - x);
    int colDiff = abs(toCol - y);

    // normal king move — one square in any direction
    if (rowDiff <= 1 && colDiff <= 1)
    {
        Piece* dest = board.getPiece(toRow, toCol);
        return dest == nullptr || dest->getColor() != color;
    }

    // castling — king moves exactly 2 squares sideways on his home row
    if (!hasMoved && rowDiff == 0 && colDiff == 2)
    {
        int rookCol = (toCol > y) ? 7 : 0; // kingside or queenside
        int rookRow = x;

        Piece* rook = board.getPiece(rookRow, rookCol);

        // rook must be there and must not have moved
        if (!rook || rook->getSymbol() != ((color == 'W') ? 'R' : 'r') || rook->getHasMoved())
            return false;

        // all squares between king and rook must be empty
        int step = (toCol > y) ? 1 : -1;

        for (int c = y + step; c != rookCol; c += step)
        {
            if (board.getPiece(x, c) != nullptr)
                return false;
        }

        // king cannot currently be in check
        // (passing-through-check is enforced in Game::calculateLegalMoves via simulation)
        Board& b = const_cast<Board&>(board);
        if (b.isCheck(color))
            return false;

        return true;
    }

    return false;
}