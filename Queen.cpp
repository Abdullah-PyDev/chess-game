#include "Board.h"
#include"Queen.h"
#include <cmath>

Queen::Queen(char c, int row, int col)
    : Piece(c, row, col) {
}

char Queen::getSymbol() {
    return (color == 'W') ? 'B' : 'b';
}

bool Queen::isValidMove(int toRow, int toCol, const Board& board) const {

    //usign abs for making sure magnitude of distance is +ve
    //conditon for a correct diagonal is row = col
    
    if (!(abs(toRow - x) == abs(toCol - y)) && (!(x == toRow || y == toCol)))
        return false;

    // 2. Determine movement direction
    //rows
    int rowStep = 0;
    if (toRow > x)
        rowStep = 1;
    else
        rowStep = -1;
    //col
    int colStep = 0;
    if (toCol > y)
        colStep = 1;
    else
        colStep = -1;

    // check if there are any pieces in the path from source to destination
    int r = x + rowStep;
    int c = y + colStep;
    // checks every box along the path to see if it has any piece or not
    //avoids jump
    while(r != toRow || c != toCol){

        // Piece blocking path
        if (board.getPiece(r, c) != nullptr)
            return false;

        r += rowStep;
        c += colStep;
    }

    //check if final destination is empty or not or it has any piece
    //makes a pointer to the final destination piece on the board
    Piece* destination = board.getPiece(toRow, toCol);

    // returns true if final destination is empty
    if (destination == nullptr || destination->getColor() != color)
        //captured or final destination is empty
        return true;
    return false;


}