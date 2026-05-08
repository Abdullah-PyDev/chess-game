//Ahsan
#include"Knight.h"
#include"Board.h"
#include"cmath"
// Constructor for Knight
Knight::Knight(char c, int row, int col) : Piece(c, row, col) 
{ }
bool Knight::isValidMove(int toRow, int toCol, const Board& board) const
{   
	// x is the starting row
	//y is the starting column
	//using abs function so that magnitude of distance is always +ve

	int rowDiff = abs(toRow - x);
	int colDiff = abs(toCol - y);

	if (!board.isInsideBoard(toRow, toCol))
	{
		return false;
	}

	// if row diff is 2 and col diff is 1 or row diff is 1 and col diff is 2 then it is a valid move for knight
	// return false if the move does not match the knight's movement pattern

	if((rowDiff !=2 || colDiff !=1) && (rowDiff !=1 || colDiff !=2))
	{
		return false;
	}

	//pointer to the piece at the destination
	//check if final position is occupied by a piece of the same color
	//check if the destination is empty or occupied by an opponent's piece
	Piece* destination = board.getPiece(toRow, toCol);

	if (destination == nullptr || destination->getColor() != color)
	{	
		//Piece captured or moved to an empty square
		return true;
	}
	return false; // Destination occupied by same color piece

}
char Knight::getSymbol()
{
	// Returning 'N' for white knight and 'n' for black knight
	if (color == 'W')
	{
		return 'N';
	}
	return 'n';
}