#include "Board.h"
#include "Piece.h"
#include <iostream>

// Constructor initializes all grid cells to nullptr
Board::Board() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            grid[row][col] = nullptr;
        }
    }
}

// Destructor deletes all pieces on the board to free memory
Board::~Board() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (grid[row][col] != nullptr) {
                delete grid[row][col];
                grid[row][col] = nullptr;
            }
        }
    }
}

// Returns the piece at (row, col), or nullptr if the square is empty
Piece* Board::getPiece(int row, int col) const {
    if (!isInsideBoard(row, col))
        return nullptr;
    return grid[row][col];
}

// Places a piece at (row, col) directly (used for board setup)
void Board::setPiece(int row, int col, Piece* piece) {
    if (!isInsideBoard(row, col)) 
        return;
    grid[row][col] = piece;
}

// Checks whether (row, col) is within the 8x8 board boundaries
bool Board::isInsideBoard(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// Attempts to move a piece, Returns true if the move was successful, false otherwise

bool Board::movePiece(int Source_row, int source_col, int dest_row, int dest_col) {
    if (!isInsideBoard(Source_row, source_col) || !isInsideBoard(dest_row, dest_col)) { // Checking that both squares are on the board
        return false;
    }

    Piece* piece = grid[Source_row][source_col];

    // No piece at source square
    if (piece == nullptr) {
        return false;
    }

    // Can't capture your own piece
    Piece* target = grid[dest_row][dest_col];
    if (target != nullptr && target->getColor() == piece->getColor()) {
        return false;
    }

    // Checking Valid Move
    if (!piece->isValidMove(dest_row, dest_col, *this)) {
        return false;
    }

    // Capture: delete the target piece if present
    if (target != nullptr) {
        delete target;
    }

    // Move Performance
    grid[dest_row][dest_col] = piece;
    grid[Source_row][source_col] = nullptr;
    piece->setPosition(dest_row, dest_col);

    return true;
}
bool Board::isCheck(char kingColor)
{
    // Find the king's position
	int kingRow = -1, kingCol = -1;       // Initialize to invalid position

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            Piece* piece = grid[row][col];

            if (piece != nullptr)
            {
                if (piece->getColor() == kingColor)
                {
                    if (piece->getSymbol() == 'K' || piece->getSymbol() == 'k')
                    {
                        kingRow = row;
                        kingCol = col;
                        break;
                    }
                }
            }
        }
        if (kingRow != -1) 
        {
            break; // King found, exit loop
        }
    }
    if (kingRow == -1)
    {
        // King not found
        return false;
    }
    // Check if any opponent piece can move to the king's position
    char opponentColor;

    if (kingColor == 'W')
    {
        opponentColor = 'B';
    }
    else
    {
        opponentColor = 'W';
    }
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            Piece* piece = grid[row][col];
            if (piece != nullptr && piece->getColor() == opponentColor)
            {
                if (piece->isValidMove(kingRow, kingCol, *this))
                {
                    return true; // King is in check
                }
            }
        }
    }
    return false; // King is not in check
}