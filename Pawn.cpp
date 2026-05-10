#include "Pawn.h"
#include "Board.h"

// Constructor: pass color and starting position to base Piece
Pawn::Pawn(char c, int row, int col) : Piece(c, row, col) {}

// 'P' for white, 'p' for black
char Pawn::getSymbol() {
    return (color == 'W') ? 'P' : 'p';
}

// Pawn movement logic:
bool Pawn::isValidMove(int toRow, int toCol, const Board& board) const {

    // ---------- WHITE PAWN ----------
    if (color == 'W') {
        // Forward move: 1 step up, same column, target must be empty
        if (toRow == x - 1 && toCol == y && board.getPiece(toRow, toCol) == nullptr) {
            return true;
        }
        // Forward move: 1 step up, same column, target must be empty
        if (toRow == x - 2 && toCol == y && board.getPiece(toRow, toCol) == nullptr) {
            return true;
        }
        // Diagonal capture: 1 step up, 1 column left or right, enemy piece present
        if (toRow == x - 1 && (toCol == y + 1 || toCol == y - 1)) {
            Piece* target = board.getPiece(toRow, toCol);
            if (target != nullptr && target->getColor() != color) {
                return true;
            }
        }
    }

   //black pawn
    else {

        // Forward move: 1 step down, same column, target must be empty
        if (toRow == x + 1 && toCol == y && board.getPiece(toRow, toCol) == nullptr) {
            return true;
        }
        if (toRow == x + 2 && toCol == y && board.getPiece(toRow, toCol) == nullptr) {
            return true;
        }

        // Diagonal capture: 1 step down, 1 column left or right, enemy piece present
        if (toRow == x + 1 && (toCol == y + 1 || toCol == y - 1)) {
            Piece* target = board.getPiece(toRow, toCol);
            if (target != nullptr && target->getColor() != color) {
                return true;
            }
        }
    }

    return false;
}
