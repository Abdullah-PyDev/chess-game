#pragma once

#include "Board.h"
#include <vector>
#include <string>

class Game
{
private:

    Board board;

    char currentTurn;

    bool selected;
    int selectedRow;
    int selectedCol;

    bool check;
    bool checkmate;

    bool promotionActive;
    int promoRow;
    int promoCol;
    char promotionColor;

    int lastFromRow;
    int lastFromCol;
    int lastToRow;
    int lastToCol;

    float whiteTime;
    float blackTime;

    std::vector<std::string> moveHistory;

    std::vector<std::pair<int, int>> legalMoves;
    std::vector<char> capturedByWhite;
    std::vector<char> capturedByBlack;

public:

    Game();

    void setupBoard();
    void reset();

    void handleClick(int row, int col);

    void calculateLegalMoves(int row, int col);

    bool makeMove(int fromRow, int fromCol,
        int toRow, int toCol);

    void switchTurn();

    void updateClock(float dt);

    void promotePawn(int choice);

    // getters

    Board& getBoard();

    char getCurrentTurn() const;

    bool isCheck() const;

    bool isCheckmate() const;

    bool isPromotionActive() const;

    int getPromoRow() const;
    int getPromoCol() const;

    int getLastFromRow() const;
    int getLastFromCol() const;
    int getLastToRow() const;
    int getLastToCol() const;

    float getWhiteTime() const;
    float getBlackTime() const;

    const std::vector<std::string>&
        getMoveHistory() const;

    const std::vector<std::pair<int, int>>&
        getLegalMoves() const;

    const std::vector<char>& getCapturedByWhite() const;
    const std::vector<char>& getCapturedByBlack() const;

    int getCapturedPointsWhite() const;
    int getCapturedPointsBlack() const;
};