#include "raylib.h"
#include "Game.h"
#include <vector>
#include <string>
#include <cstdio>

const int TILE = 80;
const int BOARD = TILE * 8;
const int PANEL = 320;
const int SCREEN_W = BOARD + PANEL;
const int SCREEN_H = 860;

// =====================================================
// TEXTURES
// =====================================================

Texture2D pieceTex[12];

// =====================================================
// SOUNDS
// =====================================================

Sound moveSound;
Sound captureSound;
Sound checkSound;
Sound checkmateSound;

// =====================================================
// THEMES
// =====================================================

struct BoardTheme
{
    Color light;
    Color dark;
};

std::vector<BoardTheme> themes =
{
    { Color{240,217,181,255}, Color{181,136,99,255} },
    { Color{235,235,208,255}, Color{119,148,85,255} },
    { Color{229,228,200,255}, Color{60,95,135,255} },
    { Color{255,230,200,255}, Color{140,90,60,255} },
    { Color{220,220,220,255}, Color{80,80,80,255} }
};

int currentTheme = 0;

// =====================================================
// CLOCK
// =====================================================

float whiteTime = 600.0f;
float blackTime = 600.0f;

bool gameOverOnTime = false;
std::string timeWinner = "";

// =====================================================
// LOAD PIECES
// =====================================================

void LoadPieces()
{
    pieceTex[0] = LoadTexture("Assets/PNG/wK.png");
    pieceTex[1] = LoadTexture("Assets/PNG/wQ.png");
    pieceTex[2] = LoadTexture("Assets/PNG/wR.png");
    pieceTex[3] = LoadTexture("Assets/PNG/wB.png");
    pieceTex[4] = LoadTexture("Assets/PNG/wN.png");
    pieceTex[5] = LoadTexture("Assets/PNG/wP.png");

    pieceTex[6] = LoadTexture("Assets/PNG/bK.png");
    pieceTex[7] = LoadTexture("Assets/PNG/bQ.png");
    pieceTex[8] = LoadTexture("Assets/PNG/bR.png");
    pieceTex[9] = LoadTexture("Assets/PNG/bB.png");
    pieceTex[10] = LoadTexture("Assets/PNG/bN.png");
    pieceTex[11] = LoadTexture("Assets/PNG/bP.png");
}

// =====================================================
// GET PIECE INDEX
// =====================================================

int GetPieceIndex(Piece* p)
{
    char s = p->getSymbol();

    switch (s)
    {
    case 'K': return 0;
    case 'Q': return 1;
    case 'R': return 2;
    case 'B': return 3;
    case 'N': return 4;
    case 'P': return 5;

    case 'k': return 6;
    case 'q': return 7;
    case 'r': return 8;
    case 'b': return 9;
    case 'n': return 10;
    case 'p': return 11;
    }

    return -1;
}

// =====================================================
// MAIN
// =====================================================

int main()
{
    InitWindow(SCREEN_W, SCREEN_H, "Ultimate Chess");

    SetTargetFPS(60);

    InitAudioDevice();

    LoadPieces();

    moveSound = LoadSound("Assets/Sounds/move.mp3");
    captureSound = LoadSound("Assets/Sounds/Check.mp3");
    checkSound = LoadSound("Assets/Sounds/gun-load_abJphmJ.mp3");
    checkmateSound = LoadSound("Assets/Sounds/checkmate.mp3");

    Game game;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // =====================================================
        // THEMES
        // =====================================================

        if (IsKeyPressed(KEY_ONE))
            currentTheme = 0;

        if (IsKeyPressed(KEY_TWO))
            currentTheme = 1;

        if (IsKeyPressed(KEY_THREE))
            currentTheme = 2;

        if (IsKeyPressed(KEY_FOUR))
            currentTheme = 3;

        if (IsKeyPressed(KEY_FIVE))
            currentTheme = 4;

        // =====================================================
        // CLOCK
        // =====================================================

        if (!game.isCheckmate() && !gameOverOnTime)
        {
            if (game.getCurrentTurn() == 'W')
                whiteTime -= dt;
            else
                blackTime -= dt;

            if (whiteTime <= 0)
            {
                whiteTime = 0;
                gameOverOnTime = true;
                timeWinner = "BLACK WINS ON TIME!";
            }

            if (blackTime <= 0)
            {
                blackTime = 0;
                gameOverOnTime = true;
                timeWinner = "WHITE WINS ON TIME!";
            }
        }

        // =====================================================
        // PROMOTION
        // =====================================================

        if (game.isPromotionActive())
        {
            if (IsKeyPressed(KEY_ONE))
                game.promotePawn(1);

            if (IsKeyPressed(KEY_TWO))
                game.promotePawn(2);

            if (IsKeyPressed(KEY_THREE))
                game.promotePawn(3);

            if (IsKeyPressed(KEY_FOUR))
                game.promotePawn(4);
        }

        // =====================================================
// INPUT
// =====================================================

        if (!game.isPromotionActive() &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();

            int col = mouse.x / TILE;
            int row = mouse.y / TILE;

            if (row >= 0 && row < 8 &&
                col >= 0 && col < 8)
            {
                int oldHistorySize = game.getMoveHistory().size();
                bool oldCheck = game.isCheck();
                bool oldMate = game.isCheckmate();

                // 🔥 IMPORTANT: capture BEFORE move
                Piece* beforeMoveTarget =
                    game.getBoard().getPiece(row, col);

                game.handleClick(row, col);

                bool moveMade =
                    game.getMoveHistory().size() > oldHistorySize;

                if (moveMade)
                {
                    // =========================
                    // 🔊 SIMPLE SOUND SYSTEM
                    // =========================

                    // MOVE / CAPTURE
                    if (beforeMoveTarget != nullptr)
                    {
                        PlaySound(captureSound);
                    }
                    else
                    {
                        PlaySound(moveSound);
                    }

                    // CHECK SOUND
                    if (!oldCheck && game.isCheck())
                    {
                        PlaySound(checkSound);
                    }

                    // CHECKMATE SOUND
                    if (!oldMate && game.isCheckmate())
                    {
                        PlaySound(checkmateSound);
                    }
                }
            }
        }

        // =====================================================
        // DRAW
        // =====================================================

        BeginDrawing();

        ClearBackground({ 20,18,16,255 });

        // =====================================================
        // BOARD
        // =====================================================

        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                Color col =
                    ((r + c) % 2 == 0)
                    ? themes[currentTheme].light
                    : themes[currentTheme].dark;

                DrawRectangle(
                    c * TILE,
                    r * TILE,
                    TILE,
                    TILE,
                    col
                );
            }
        }

        // =====================================================
        // LAST MOVE
        // =====================================================

        if (game.getLastFromRow() != -1)
        {
            DrawRectangle(
                game.getLastFromCol() * TILE,
                game.getLastFromRow() * TILE,
                TILE,
                TILE,
                Color{ 255,255,0,80 }
            );

            DrawRectangle(
                game.getLastToCol() * TILE,
                game.getLastToRow() * TILE,
                TILE,
                TILE,
                Color{ 255,215,0,120 }
            );
        }

        // =====================================================
        // CHECK HIGHLIGHT
        // =====================================================

        if (game.isCheck())
        {
            for (int r = 0; r < 8; r++)
            {
                for (int c = 0; c < 8; c++)
                {
                    Piece* p =
                        game.getBoard().getPiece(r, c);

                    if (!p)
                        continue;

                    if (p->getColor() ==
                        game.getCurrentTurn())
                    {
                        char s = p->getSymbol();

                        if (s == 'K' || s == 'k')
                        {
                            DrawRectangle(
                                c * TILE,
                                r * TILE,
                                TILE,
                                TILE,
                                Color{ 255,0,0,120 }
                            );
                        }
                    }
                }
            }
        }

        // =====================================================
        // LEGAL MOVES
        // =====================================================

        for (auto& m : game.getLegalMoves())
        {
            Piece* target =
                game.getBoard().getPiece(
                    m.first,
                    m.second
                );

            if (target)
            {
                DrawCircleLines(
                    m.second * TILE + 40,
                    m.first * TILE + 40,
                    22,
                    RED
                );
            }
            else
            {
                DrawCircle(
                    m.second * TILE + 40,
                    m.first * TILE + 40,
                    12,
                    Color{ 40,180,60,160 }
                );
            }
        }

        // =====================================================
        // PIECES
        // =====================================================

        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                Piece* p =
                    game.getBoard().getPiece(r, c);

                if (!p)
                    continue;

                int id = GetPieceIndex(p);

                if (id < 0)
                    continue;

                DrawTextureEx(
                    pieceTex[id],
                    {
                        c * TILE + 5.0f,
                        r * TILE + 5.0f
                    },
                    0.0f,
                    0.6f,
                    WHITE
                );
            }
        }

        // =====================================================
        // SIDE PANEL
        // =====================================================

        DrawRectangle(
            BOARD,
            0,
            PANEL,
            SCREEN_H,
            Color{ 30,30,30,255 }
        );

        // =====================================================
        // TURN
        // =====================================================

        DrawText(
            (game.getCurrentTurn() == 'W')
            ? "WHITE TURN"
            : "BLACK TURN",
            BOARD + 20,
            20,
            30,
            GOLD
        );

        // =====================================================
        // CHECK
        // =====================================================

        if (game.isCheck())
        {
            DrawText(
                "CHECK!",
                BOARD + 20,
                60,
                30,
                RED
            );
        }

        // =====================================================
        // MOVE HISTORY
        // =====================================================

        DrawText(
            "MOVE HISTORY",
            BOARD + 20,
            110,
            24,
            GOLD
        );

        int historyY = 145;

        const auto& history =
            game.getMoveHistory();

        for (int i = 0; i < history.size(); i++)
        {
            std::string text =
                std::to_string(i + 1)
                + ". "
                + history[i];

            DrawText(
                text.c_str(),
                BOARD + 20,
                historyY,
                20,
                WHITE
            );

            historyY += 28;

            if (historyY > 470)
                break;
        }

        // =====================================================
        // CLOCKS
        // =====================================================

        int whiteMin = (int)whiteTime / 60;
        int whiteSec = (int)whiteTime % 60;

        int blackMin = (int)blackTime / 60;
        int blackSec = (int)blackTime % 60;

        char whiteClock[32];
        char blackClock[32];

        sprintf_s(
            whiteClock,
            "White: %02d:%02d",
            whiteMin,
            whiteSec
        );

        sprintf_s(
            blackClock,
            "Black: %02d:%02d",
            blackMin,
            blackSec
        );

        DrawRectangle(
            BOARD + 20,
            520,
            240,
            50,
            (game.getCurrentTurn() == 'W')
            ? Color{ 70,70,70,255 }
            : Color{ 40,40,40,255 }
        );

        DrawRectangle(
            BOARD + 20,
            590,
            240,
            50,
            (game.getCurrentTurn() == 'B')
            ? Color{ 70,70,70,255 }
            : Color{ 40,40,40,255 }
        );

        DrawText(
            whiteClock,
            BOARD + 35,
            533,
            28,
            WHITE
        );

        DrawText(
            blackClock,
            BOARD + 35,
            603,
            28,
            WHITE
        );

        // =====================================================
        // THEMES
        // =====================================================

        DrawText(
            "BOARD THEMES",
            BOARD + 20,
            690,
            24,
            GOLD
        );

        DrawText(
            "1 - Classic",
            BOARD + 20,
            730,
            20,
            WHITE
        );

        DrawText(
            "2 - Green",
            BOARD + 20,
            760,
            20,
            WHITE
        );

        DrawText(
            "3 - Blue",
            BOARD + 20,
            790,
            20,
            WHITE
        );

        DrawText(
            "4 - Brown",
            BOARD + 160,
            730,
            20,
            WHITE
        );

        DrawText(
            "5 - Gray",
            BOARD + 160,
            760,
            20,
            WHITE
        );

        // =====================================================
        // PROMOTION UI
        // =====================================================

        if (game.isPromotionActive())
        {
            DrawRectangle(
                0,
                0,
                SCREEN_W,
                SCREEN_H,
                Color{ 0,0,0,180 }
            );

            DrawText(
                "PAWN PROMOTION",
                BOARD / 2 - 140,
                200,
                40,
                YELLOW
            );

            DrawText(
                "1=Queen  2=Rook  3=Bishop  4=Knight",
                BOARD / 2 - 260,
                280,
                25,
                WHITE
            );
        }

        // =====================================================
        // GAME OVER
        // =====================================================

        if (game.isCheckmate())
        {
            DrawRectangle(
                0,
                0,
                SCREEN_W,
                SCREEN_H,
                Color{ 0,0,0,200 }
            );

            DrawText("CHECKMATE!", BOARD / 2 - 120, 250, 40, RED);

            DrawText(
                (game.getCurrentTurn() == 'W') ? "BLACK WINS" : "WHITE WINS", BOARD / 2 - 120, 320, 35, WHITE
            );
        }

        if (gameOverOnTime)
        {
            DrawRectangle(
                0,
                0,
                SCREEN_W,
                SCREEN_H,
                Color{ 0,0,0,200 }
            );

            DrawText(timeWinner.c_str(), BOARD / 2 - 170, 300, 35, GOLD);
        }

        EndDrawing();
    }

    // =====================================================
    // CLEANUP
    // =====================================================

    for (int i = 0; i < 12; i++)
    {
        UnloadTexture(pieceTex[i]);
    }

    UnloadSound(moveSound);
    UnloadSound(captureSound);
    UnloadSound(checkSound);
    UnloadSound(checkmateSound);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}