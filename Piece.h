#pragma once

class Piece {
protected:
    char color;
    int x, y;

public:
    Piece(char c, int row, int col);

    virtual bool isValidMove(int newX, int newY) = 0;

    virtual char getSymbol() = 0;

    char getColor();

    void setPosition(int newX, int newY);
};
