#include"King.h"
#include"Board.h"
#include<cmath>

// Constructor for King

King::King(char c, int row, int col) : Piece(c, row, col)
{ }
bool King::canAttack(int r, int c, const Board& board)
{
	return abs(r - x) <= 1 && abs(c - y) <= 1;
}


bool King::isValidMove(int toRow, int toCol, const Board& board) const
{
	// x is the starting row
	//y is the starting column 

	int rowDiff = abs(toRow - x);
	int colDiff = abs(toCol - y);

	// King can move one square in any direction, so both row and column differences must be 1
	if (!board.isInsideBoard(toRow, toCol))
	{
		return false;
	}
	if (rowDiff > 1 || colDiff > 1)
	{
		return false;
	}
	// Pointer to the piece at the destination 
	Piece* destination = board.getPiece(toRow, toCol);

	if (destination == nullptr || destination->getColor() != color)
	{
		//Piece captured or moved to an empty square
		return true;
	}
	return false; // Destination occupied by same color piece

}
char King::getSymbol() const
{
	// Returning 'K' for white king and 'k' for black king

	if (color == 'W')
		return 'K';

	return 'k';
}
