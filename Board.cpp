#include "Board.h"
#include "Piece.h"
#include <iostream>
#include <cmath>

Board::Board()
{
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            grid[r][c] = nullptr;
}

Board::~Board()
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            delete grid[r][c];
            grid[r][c] = nullptr;
        }
    }
}

Piece* Board::getPiece(int row, int col) const
{
    if (!isInsideBoard(row, col))
        return nullptr;

    return grid[row][col];
}

void Board::setPiece(int row, int col, Piece* piece)
{
    if (!isInsideBoard(row, col))
        return;

    grid[row][col] = piece;
}

bool Board::isInsideBoard(int row, int col) const
{
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool Board::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    if (!isInsideBoard(fromRow, fromCol) || !isInsideBoard(toRow, toCol))
        return false;

    Piece* piece = grid[fromRow][fromCol];

    if (!piece)
        return false;

    // can't capture your own piece
    Piece* target = grid[toRow][toCol];
    if (target && target->getColor() == piece->getColor())
        return false;

    if (!piece->isValidMove(toRow, toCol, *this))
        return false;

    // can't capture the king directly
    if (target && (target->getSymbol() == 'K' || target->getSymbol() == 'k'))
        return false;

    // detect castling — king moving 2 squares sideways
    char sym = piece->getSymbol();
    bool isCastle = (sym == 'K' || sym == 'k') && abs(toCol - fromCol) == 2;

    // perform the king's move
    grid[toRow][toCol] = piece;
    grid[fromRow][fromCol] = nullptr;
    piece->setPosition(toRow, toCol);
    piece->markMoved();

    // move the rook to the other side of the king
    if (isCastle)
    {
        int rookFromCol = (toCol > fromCol) ? 7 : 0; // kingside : queenside
        int rookToCol = (toCol > fromCol) ? toCol - 1 : toCol + 1;

        Piece* rook = grid[fromRow][rookFromCol];

        grid[fromRow][rookToCol] = rook;
        grid[fromRow][rookFromCol] = nullptr;

        if (rook)
        {
            rook->setPosition(fromRow, rookToCol);
            rook->markMoved();
        }
    }

    // mark the rook moved on a normal rook move too
    if (sym == 'R' || sym == 'r')
        piece->markMoved();

    return true;
}

bool Board::isCheck(char kingColor)
{
    int kingRow = -1, kingCol = -1;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Piece* p = grid[r][c];

            if (p && p->getColor() == kingColor &&
                (p->getSymbol() == 'K' || p->getSymbol() == 'k'))
            {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    if (kingRow == -1)
        return false;

    char enemy = (kingColor == 'W') ? 'B' : 'W';

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Piece* p = grid[r][c];

            if (p && p->getColor() == enemy)
            {
                if (p->canAttack(kingRow, kingCol, *this))
                    return true;
            }
        }
    }

    return false;
}

bool Board::isCheckmate(char kingColor)
{
    if (!isCheck(kingColor))
        return false;

    return !canEscapeCheck(kingColor);
}

bool Board::canEscapeCheck(char kingColor)
{
    for (int fromRow = 0; fromRow < 8; fromRow++)
    {
        for (int fromCol = 0; fromCol < 8; fromCol++)
        {
            Piece* piece = grid[fromRow][fromCol];

            if (!piece || piece->getColor() != kingColor)
                continue;

            for (int toRow = 0; toRow < 8; toRow++)
            {
                for (int toCol = 0; toCol < 8; toCol++)
                {
                    Piece* target = grid[toRow][toCol];

                    if (target && target->getColor() == kingColor)
                        continue;

                    if (!piece->isValidMove(toRow, toCol, *this))
                        continue;

                    // simulate the move
                    grid[toRow][toCol] = piece;
                    grid[fromRow][fromCol] = nullptr;
                    int oldRow = piece->getRow();
                    int oldCol = piece->getCol();
                    piece->setPosition(toRow, toCol);

                    bool stillInCheck = isCheck(kingColor);

                    // undo the move
                    grid[fromRow][fromCol] = piece;
                    grid[toRow][toCol] = target;
                    piece->setPosition(oldRow, oldCol);

                    if (!stillInCheck)
                        return true;
                }
            }
        }
    }

    return false;
}