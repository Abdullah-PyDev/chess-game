#include "Game.h"

#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

#include <iostream>
using namespace std;

//white playes first
Game::Game() : currentTurn('W') {
    setupBoard();


}

void Game::setupBoard()
{
    cout << "Setup called\n";
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

void Game::start()
{
    while (true)
    {
        displayBoard();

        cout << (currentTurn == 'W' ? "\nWhite Turn\n" : "\nBlack Turn\n");

        string from, to;
        cout << "Enter move (e2 e4): ";
        cin >> from >> to;

        if (from.length() != 2 || to.length() != 2)
        {
            cout << "Invalid input!\n";
            continue;
        }

        int fromCol = from[0] - 'a';
        int toCol = to[0] - 'a';

        int fromRow = 8 - (from[1] - '0');
        int toRow = 8 - (to[1] - '0');

        Piece* piece = board.getPiece(fromRow, fromCol);

        if (!piece)
        {
            cout << "No piece!\n";
            continue;
        }

        if (piece->getColor() != currentTurn)
        {
            cout << "Not your piece!\n";
            continue;
        }

        Piece* target = board.getPiece(toRow, toCol);

        if (!piece->isValidMove(toRow, toCol, board))
        {
            cout << "Invalid move!\n";
            continue;
        }
        Piece* promotepawn = board.getPiece(toRow, toCol);
Piece* newpiece = nullptr;
if (promotepawn != nullptr && promotepawn->getSymbol() == 'P' && toRow == 0) {
    cout << "Pawn Promotion, Select Piece: " << endl;
    cout << "1. Queen" << endl;
    cout << "2. Rook" << endl;
    cout << "3. Bishop" << endl;
    cout << "4. Knight" << endl;
    int choice;
    cin >> choice;
    if (choice == 1) {
        newpiece = new Queen('W', toRow, toCol);
    }

    else if (choice == 2) {
        newpiece = new Rook('W', toRow, toCol);
    }

    else if (choice == 3) {
        newpiece = new Bishop('W', toRow, toCol);
    }
    
    else if (choice == 4) {
        newpiece = new Knight('W', toRow, toCol);
    }

    else {
        newpiece = new Queen('W', toRow, toCol);  // Default to Queen
    }
    delete promotepawn;
    board.setPiece(toRow, toCol, newpiece);
}

else if (promotepawn != nullptr && promotepawn->getSymbol() == 'p' && toRow == 7) {
    cout << "Pawn Promotion, Select Piece: " << endl;
    cout << "1. Queen" << endl;
    cout << "2. Rook" << endl;
    cout << "3. Bishop" << endl;
    cout << "4. Knight" << endl;
    int choice;
    cin >> choice;
    if (choice == 1) {
        newpiece = new Queen('B', toRow, toCol);
    }

    else if (choice == 2) {
        newpiece = new Rook('B', toRow, toCol);
    }

    else if (choice == 3) {
        newpiece = new Bishop('B', toRow, toCol);
    }

    else if (choice == 4) {
        newpiece = new Knight('B', toRow, toCol);
    }

    else {
        newpiece = new Queen('B', toRow, toCol);
    }
    delete promotepawn;
    board.setPiece(toRow, toCol, newpiece);
}
        // ===== SIMULATE MOVE =====
        board.setPiece(toRow, toCol, piece);
        board.setPiece(fromRow, fromCol, nullptr);

        int oldRow = piece->getRow();
        int oldCol = piece->getCol();
        piece->setPosition(toRow, toCol);

        // ❌ SELF CHECK PROTECTION
        if (board.isCheck(currentTurn))
        {
            board.setPiece(fromRow, fromCol, piece);
            board.setPiece(toRow, toCol, target);
            piece->setPosition(oldRow, oldCol);

            cout << "Move not allowed (puts king in check)!\n";
            continue;
        }

        // capture finalization
        

        // CHECK DISPLAY
        char enemy = (currentTurn == 'W') ? 'B' : 'W';

        if (board.isCheckmate(enemy))
        {
            displayBoard();
            cout << "\nCheckmate! " << (currentTurn == 'W' ? "White" : "Black") << " wins!\n";
            return;  // ends the game
        }
        else if (board.isCheck(enemy))
        {
            cout << "\nCheck!\n";
        }
        if (target) delete target;
        currentTurn = enemy;
    }
}