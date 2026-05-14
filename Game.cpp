#include "Game.h"

#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

#include <string>

// =====================================================
// CONSTRUCTOR
// =====================================================

Game::Game()
{
    currentTurn = 'W';

    selected = false;

    selectedRow = -1;
    selectedCol = -1;

    check = false;
    checkmate = false;

    promotionActive = false;

    promoRow = -1;
    promoCol = -1;

    lastFromRow = -1;
    lastFromCol = -1;

    lastToRow = -1;
    lastToCol = -1;

    whiteTime = 600.0f;
    blackTime = 600.0f;

    setupBoard();
}

// =====================================================
// SETUP BOARD
// =====================================================

void Game::setupBoard()
{
    // BLACK

    board.setPiece(0, 0, new Rook('B', 0, 0));
    board.setPiece(0, 1, new Knight('B', 0, 1));
    board.setPiece(0, 2, new Bishop('B', 0, 2));
    board.setPiece(0, 3, new Queen('B', 0, 3));
    board.setPiece(0, 4, new King('B', 0, 4));
    board.setPiece(0, 5, new Bishop('B', 0, 5));
    board.setPiece(0, 6, new Knight('B', 0, 6));
    board.setPiece(0, 7, new Rook('B', 0, 7));

    for (int c = 0; c < 8; c++)
    {
        board.setPiece(1, c, new Pawn('B', 1, c));
    }

    // WHITE

    board.setPiece(7, 0, new Rook('W', 7, 0));
    board.setPiece(7, 1, new Knight('W', 7, 1));
    board.setPiece(7, 2, new Bishop('W', 7, 2));
    board.setPiece(7, 3, new Queen('W', 7, 3));
    board.setPiece(7, 4, new King('W', 7, 4));
    board.setPiece(7, 5, new Bishop('W', 7, 5));
    board.setPiece(7, 6, new Knight('W', 7, 6));
    board.setPiece(7, 7, new Rook('W', 7, 7));

    for (int c = 0; c < 8; c++)
    {
        board.setPiece(6, c, new Pawn('W', 6, c));
    }
}

// =====================================================
// HANDLE CLICK
// =====================================================

void Game::handleClick(int row, int col)
{
    // =========================
    // SELECT PIECE
    // =========================

    if (!selected)
    {
        Piece* p = board.getPiece(row, col);

        if (p && p->getColor() == currentTurn)
        {
            selected = true;

            selectedRow = row;
            selectedCol = col;

            calculateLegalMoves(row, col);
        }

        return;
    }

    // =========================
    // TRY MOVE
    // =========================

    for (auto& move : legalMoves)
    {
        if (move.first == row &&
            move.second == col)
        {
            makeMove(
                selectedRow,
                selectedCol,
                row,
                col
            );

            break;
        }
    }

    selected = false;

    legalMoves.clear();
}

// =====================================================
// CALCULATE LEGAL MOVES
// =====================================================

void Game::calculateLegalMoves(int row, int col)
{
    legalMoves.clear();

    Piece* piece = board.getPiece(row, col);

    if (!piece)
        return;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Piece* target =
                board.getPiece(r, c);

            if (target &&
                target->getColor() ==
                piece->getColor())
            {
                continue;
            }

            if (!piece->isValidMove(r, c, board))
            {
                continue;
            }

            // =========================
            // SIMULATE MOVE
            // =========================

            Piece* captured =
                board.getPiece(r, c);

            board.setPiece(r, c, piece);
            board.setPiece(row, col, nullptr);

            int oldRow = piece->getRow();
            int oldCol = piece->getCol();

            piece->setPosition(r, c);

            bool illegal =
                board.isCheck(piece->getColor());

            // =========================
            // UNDO MOVE
            // =========================

            board.setPiece(row, col, piece);
            board.setPiece(r, c, captured);

            piece->setPosition(oldRow, oldCol);

            if (!illegal)
            {
                legalMoves.push_back({ r, c });
            }
        }
    }
}

// =====================================================
// MAKE MOVE
// =====================================================

bool Game::makeMove(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol)
{
    Piece* target =
        board.getPiece(toRow, toCol);

    bool moved =
        board.movePiece(
            fromRow,
            fromCol,
            toRow,
            toCol
        );

    if (!moved)
        return false;

    // =========================
    // LAST MOVE
    // =========================

    lastFromRow = fromRow;
    lastFromCol = fromCol;

    lastToRow = toRow;
    lastToCol = toCol;

    // =========================
    // MOVE HISTORY
    // =========================

    char fromFile = 'a' + fromCol;
    char toFile = 'a' + toCol;

    int fromRank = 8 - fromRow;
    int toRank = 8 - toRow;

    std::string moveText;

    moveText += fromFile;
    moveText += std::to_string(fromRank);

    moveText += " -> ";

    moveText += toFile;
    moveText += std::to_string(toRank);

    moveHistory.push_back(moveText);

    // =========================
    // PROMOTION
    // =========================

    Piece* movedPiece =
        board.getPiece(toRow, toCol);

    if (movedPiece)
    {
        char s = movedPiece->getSymbol();

        if ((s == 'P' && toRow == 0) ||
            (s == 'p' && toRow == 7))
        {
            promotionActive = true;

            promoRow = toRow;
            promoCol = toCol;

            promotionColor =
                movedPiece->getColor();
        }
    }

    // =========================
    // SWITCH TURN
    // =========================

    switchTurn();

    check =
        board.isCheck(currentTurn);

    checkmate =
        board.isCheckmate(currentTurn);

    return true;
}

// =====================================================
// SWITCH TURN
// =====================================================

void Game::switchTurn()
{
    currentTurn =
        (currentTurn == 'W')
        ? 'B'
        : 'W';
}

// =====================================================
// UPDATE CLOCK
// =====================================================

void Game::updateClock(float dt)
{
    if (checkmate)
        return;

    if (currentTurn == 'W')
    {
        whiteTime -= dt;

        if (whiteTime < 0)
            whiteTime = 0;
    }
    else
    {
        blackTime -= dt;

        if (blackTime < 0)
            blackTime = 0;
    }
}

// =====================================================
// PROMOTE PAWN
// =====================================================

void Game::promotePawn(int choice)
{
    Piece* old =
        board.getPiece(
            promoRow,
            promoCol
        );

    if (!old)
        return;

    delete old;

    Piece* newPiece = nullptr;

    if (choice == 1)
    {
        newPiece =
            new Queen(
                promotionColor,
                promoRow,
                promoCol
            );
    }

    else if (choice == 2)
    {
        newPiece =
            new Rook(
                promotionColor,
                promoRow,
                promoCol
            );
    }

    else if (choice == 3)
    {
        newPiece =
            new Bishop(
                promotionColor,
                promoRow,
                promoCol
            );
    }

    else if (choice == 4)
    {
        newPiece =
            new Knight(
                promotionColor,
                promoRow,
                promoCol
            );
    }

    board.setPiece(
        promoRow,
        promoCol,
        newPiece
    );

    promotionActive = false;
}

// =====================================================
// GETTERS
// =====================================================

Board& Game::getBoard()
{
    return board;
}

char Game::getCurrentTurn() const
{
    return currentTurn;
}

bool Game::isCheck() const
{
    return check;
}

bool Game::isCheckmate() const
{
    return checkmate;
}

bool Game::isPromotionActive() const
{
    return promotionActive;
}

int Game::getPromoRow() const
{
    return promoRow;
}

int Game::getPromoCol() const
{
    return promoCol;
}

int Game::getLastFromRow() const
{
    return lastFromRow;
}

int Game::getLastFromCol() const
{
    return lastFromCol;
}

int Game::getLastToRow() const
{
    return lastToRow;
}

int Game::getLastToCol() const
{
    return lastToCol;
}

float Game::getWhiteTime() const
{
    return whiteTime;
}

float Game::getBlackTime() const
{
    return blackTime;
}

const std::vector<std::string>&
Game::getMoveHistory() const
{
    return moveHistory;
}

const std::vector<std::pair<int, int>>&
Game::getLegalMoves() const
{
    return legalMoves;
}