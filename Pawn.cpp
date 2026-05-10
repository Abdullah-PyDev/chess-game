#include "Pawn.h"
#include "Board.h"

// Constructor
Pawn::Pawn(char c, int row, int col) : Piece(c, row, col) {}

char Pawn::getSymbol() {
    return (color == 'W') ? 'P' : 'p';
}

// Pawn movement logic:
bool Pawn::isValidMove(int toRow, int toCol, const Board& board) const {

    //  WHITE PAWN 
    if (color == 'W') {
        // First Move of White Pawn 
        if (x == 6) {
            if (toRow == x - 2 && toCol == y) {
                if (board.getPiece(toRow, toCol) == nullptr) {
                    if (board.getPiece(x - 1, y) == nullptr) {
                        return true;
                    }
                }
            }
        }

        // Forward move of White Pawn
        if (toRow == x - 1 && toCol == y && board.getPiece(toRow, toCol) == nullptr) {
                if (board.getPiece(toRow, toCol) == nullptr) {
                    return true;
                }   
        }

        // Diagonal capture
        if (toRow == x - 1 && (toCol == y + 1 || toCol == y - 1)) {
            Piece* target = board.getPiece(toRow, toCol);

            if (target != nullptr && target->getColor() != color) {
                return true;
            }
        }
    }

    // BLACK PAWN 
    else {

        // Forward Double Move of Black Pawn
      
        if (x == 1) {
            if (toRow == x + 2 && toCol == y) {
                if (board.getPiece(toRow, toCol) == nullptr) {
                    if (board.getPiece(x + 1, y) == nullptr) {
                        return true;
                    }
                }
            }
        }
        // Forward Move 
        if (toRow == x + 1 && toCol == y && board.getPiece(toRow, toCol) == nullptr) {
            if (board.getPiece(toRow, toCol) == nullptr) {
                return true;
            }
        }

        // Diagonal capture
        if (toRow == x + 1 && (toCol == y + 1 || toCol == y - 1)) {
            Piece* target = board.getPiece(toRow, toCol);
            if (target != nullptr && target->getColor() != color) {
                return true;
            }
        }
    }

    return false;
}
