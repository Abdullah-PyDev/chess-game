#pragma once
#include "raylib.h"
#include "Game.h"
#include <string>

class ChessUI
{
private:

    // =====================================================
    // WINDOW
    // =====================================================

    static const int TILE = 80;

    static const int BOARD_SIZE = 8;

    static const int SCREEN_WIDTH =
        TILE * BOARD_SIZE + 250;

    static const int SCREEN_HEIGHT =
        TILE * BOARD_SIZE;

    // =====================================================
    // GAME
    // =====================================================

    Game game;

    bool selected;

    int selectedRow;
    int selectedCol;

    bool gameOver;

    std::string status;

public:

    // =====================================================
    // CONSTRUCTOR
    // =====================================================

    ChessUI();

    // =====================================================
    // MAIN LOOP
    // =====================================================

    void run();

private:

    // =====================================================
    // INPUT
    // =====================================================

    void handleInput();

    // =====================================================
    // DRAWING
    // =====================================================

    void draw();

    void drawBoard();

    void drawPieces();

    void drawUI();

    void drawCheckHighlight();

    void drawSelection();

    void drawCoordinates();
};