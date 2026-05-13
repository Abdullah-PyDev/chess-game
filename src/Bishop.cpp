#include "Bishop.h"
#include "Board.h"
#include <cmath>

Bishop::Bishop(char c, int row, int col)
    : Piece(c, row, col) {
}

char Bishop::getSymbol()  const{
    return (color == 'W') ? 'B' : 'b';
}
bool Bishop::canAttack(int r, int c, const Board& board)
{
    if (abs(r - x) != abs(c - y))
        return false;

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


bool Bishop::isValidMove(int toRow, int toCol, const Board& board) const {

    //usign abs for making sure magnitude of distance is +ve
    //conditon for a correct diagonal is row = col
    //(2,2) -> (3,3) : row (3,2) == col(3-2) Valid
    if (abs(toRow - x) != abs(toCol - y))
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
    while (r != toRow && c != toCol) {
          
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
