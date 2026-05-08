#include "Piece.h"

Piece::Piece(char c, int row, int col) {
    color = c;
    x = row;
    y = col;
}

char Piece::getColor() {
    return color;
}

void Piece::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

int Piece::getRow() const {
    return x; 
}
int Piece::getCol() const {
    return y; 
}
