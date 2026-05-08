#include "Game.h"

#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

#include <iostream>
using namespace std;

Game::Game() : currentTurn('W')
{
}

void Game::setupBoard()
{
    // Black Pieces
    board.setPiece(0, 0, new Rook('B', 0, 0));
    board.setPiece(0, 1, new Knight('B', 0, 1));
    board.setPiece(0, 2, new Bishop('B', 0, 2));
    board.setPiece(0, 3, new Queen('B', 0, 3));
    board.setPiece(0, 4, new King('B', 0, 4));
    board.setPiece(0, 5, new Bishop('B', 0, 5));
    board.setPiece(0, 6, new Knight('B', 0, 6));
    board.setPiece(0, 7, new Rook('B', 0, 7));

    for (int col = 0; col < 8; col++)
    {
        board.setPiece(1, col, new Pawn('B', 1, col));
    }

    // White Pieces
    board.setPiece(7, 0, new Rook('W', 7, 0));
    board.setPiece(7, 1, new Knight('W', 7, 1));
    board.setPiece(7, 2, new Bishop('W', 7, 2));
    board.setPiece(7, 3, new Queen('W', 7, 3));
    board.setPiece(7, 4, new King('W', 7, 4));
    board.setPiece(7, 5, new Bishop('W', 7, 5));
    board.setPiece(7, 6, new Knight('W', 7, 6));
    board.setPiece(7, 7, new Rook('W', 7, 7));

    for (int col = 0; col < 8; col++)
    {
        board.setPiece(6, col, new Pawn('W', 6, col));
    }
}

void Game::displayBoard()
{
    cout << "\n    a   b   c   d   e   f   g   h\n";
    cout << "  +---+---+---+---+---+---+---+---+\n";

    for (int r = 0; r < 8; r++)
    {
        cout << (8 - r) << " |";

        for (int c = 0; c < 8; c++)
        {
            Piece* p = board.getPiece(r, c);

            if (p != nullptr)
            {
                cout << " " << p->getSymbol() << " |";
            }
            else
            {
                cout << "   |";
            }
        }

        cout << " " << (8 - r) << "\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }

    cout << "    a   b   c   d   e   f   g   h\n\n";
}

void Game::start()
{
    int fromRow, fromCol;
    int toRow, toCol;

	// game ends when one of the kings is checkmated
    while (!board.isCheckmate('W') && !board.isCheckmate('B'))
    {
        displayBoard();

        if (currentTurn == 'W')
            cout << "\nWhite Turn\n";
        else
            cout << "\nBlack Turn\n";

        cout << "Enter move:\n";
        cout << "From Row: ";
        cin >> fromRow;

        cout << "From Col: ";
        cin >> fromCol;

        cout << "To Row: ";
        cin >> toRow;

        cout << "To Col: ";
        cin >> toCol;

        Piece* piece = board.getPiece(fromRow, fromCol);

        // No piece selected
        if (piece == nullptr)
        {
            cout << "No piece at selected position!\n";
            continue;
        }

        // Wrong player's piece
        if (piece->getColor() != currentTurn)
        {
            cout << "Not your piece!\n";
            continue;
        }

        // Invalid move
        if (!piece->isValidMove(toRow, toCol, board))
        {
            cout << "Invalid move!\n";
            continue;
        }

        // Move piece
        board.movePiece(fromRow, fromCol, toRow, toCol);

        // Check condition
        if (board.isCheck('W'))
        {
            cout << "\nWhite King is in CHECK!\n";
        }

        if (board.isCheck('B'))
        {
            cout << "\nBlack King is in CHECK!\n";
        }

        // Switch turn
        if (currentTurn == 'W')
            currentTurn = 'B';
        else
            currentTurn = 'W';
    }
}