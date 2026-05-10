#pragma once
class Board;
class Piece {
protected:
    char color;
    int x, y;

public:
    Piece(char c, int row, int col);

    virtual bool isValidMove(int toRow, int toCol, const Board& board) const = 0;

    virtual char getSymbol() const = 0;
    virtual bool canAttack(int row, int col, const Board& board) = 0;
    char getColor() const;

    void setPosition(int newX, int newY);
    
    int getRow() const;
    int getCol() const;
};
