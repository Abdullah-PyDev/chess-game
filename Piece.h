#pragma once
class Board;
class Piece {
protected:
    char color;
    int x, y;

public:
    Piece(char c, int row, int col);

    virtual bool isValidMove(int toRow, int toCol, const Board& board) const = 0;

    virtual char getSymbol() = 0;

    char getColor();

    void setPosition(int newX, int newY);
    
    int getRow() const;
    int getCol() const;
};
