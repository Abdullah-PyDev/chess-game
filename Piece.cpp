#include "Piece.h"

Piece::Piece(char c, int row, int col)
{
    color = c;
    x = row;
    y = col;
    hasMoved = false;
}

char Piece::getColor() const
{
    return color;
}

void Piece::setPosition(int newX, int newY)
{
    x = newX;
    y = newY;
}

int Piece::getRow() const
{
    return x;
}

int Piece::getCol() const
{
    return y;
}

bool Piece::getHasMoved() const
{
    return hasMoved;
}

void Piece::markMoved()
{
    hasMoved = true;
}