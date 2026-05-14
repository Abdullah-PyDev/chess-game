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

Texture2D pieceTex[12];

Sound moveSound;
Sound captureSound;
Sound checkSound;
Sound checkmateSound;
Sound promotionSound;

struct BoardTheme
{
    Color light;
    Color dark;
};

BoardTheme themes[] =
{
    { {240,217,181,255}, {181,136,99,255} }, // Classic
    { {235,235,208,255}, {119,148,85,255} }, // Green
    { {229,228,200,255}, {60,95,135,255}  }, // Blue
    { {255,230,200,255}, {140,90,60,255}  }, // Brown
    { {220,220,220,255}, {80,80,80,255}   }  // Gray
};

int currentTheme = 0;

// 10 minutes per player
float whiteTime = 600.0f;
float blackTime = 600.0f;
bool  gameOverOnTime = false;

std::string timeWinner = "";

void LoadPieces()
{
    const char* paths[12] =
    {
        "Assets/PNG/wK.png",
        "Assets/PNG/wQ.png",
        "Assets/PNG/wR.png",
        "Assets/PNG/wB.png",
        "Assets/PNG/wN.png",
        "Assets/PNG/wP.png",

        "Assets/PNG/bK.png",
        "Assets/PNG/bQ.png",
        "Assets/PNG/bR.png",
        "Assets/PNG/bB.png",
        "Assets/PNG/bN.png",
        "Assets/PNG/bP.png"
    };

    for (int i = 0; i < 12; i++)
    {
        pieceTex[i] = LoadTexture(paths[i]);
    }
}

// uppercase = white, lowercase = black
int GetPieceIndex(const Piece* p)
{
    switch (p->getSymbol())
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

void FormatClock(char* buffer, const char* label, float seconds)
{
    int mins = (int)seconds / 60;
    int secs = (int)seconds % 60;

    sprintf_s(buffer, 32, "%s: %02d:%02d", label, mins, secs);
}

void DrawOverlay(Color color = { 0,0,0,200 })
{
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, color);
}

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
    promotionSound = LoadSound("Assets/Sounds/promotion.mp3");

    Game game;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // keys 1-5 switch theme, but block them during promotion
        // since those same keys are used to pick a piece
        if (!game.isPromotionActive())
        {
            if (IsKeyPressed(KEY_ONE))   currentTheme = 0;
            if (IsKeyPressed(KEY_TWO))   currentTheme = 1;
            if (IsKeyPressed(KEY_THREE)) currentTheme = 2;
            if (IsKeyPressed(KEY_FOUR))  currentTheme = 3;
            if (IsKeyPressed(KEY_FIVE))  currentTheme = 4;
        }

        if ((game.isCheckmate() || gameOverOnTime) && IsKeyPressed(KEY_R))
        {
            game.reset();

            whiteTime = 600.0f;
            blackTime = 600.0f;
            gameOverOnTime = false;
            timeWinner = "";
        }

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

        if (game.isPromotionActive())
        {
            if (IsKeyPressed(KEY_ONE)) { game.promotePawn(1); PlaySound(promotionSound); }
            if (IsKeyPressed(KEY_TWO)) { game.promotePawn(2); PlaySound(promotionSound); }
            if (IsKeyPressed(KEY_THREE)) { game.promotePawn(3); PlaySound(promotionSound); }
            if (IsKeyPressed(KEY_FOUR)) { game.promotePawn(4); PlaySound(promotionSound); }
        }

        if (!game.isPromotionActive() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();

            int col = (int)(mouse.x / TILE);
            int row = (int)(mouse.y / TILE);

            if (row >= 0 && row < 8 && col >= 0 && col < 8)
            {
                // snapshot state before the move so we can
                // figure out which sounds to play after
                int  oldHistory = (int)game.getMoveHistory().size();
                bool oldCheck = game.isCheck();
                bool oldMate = game.isCheckmate();
                bool capture = game.getBoard().getPiece(row, col) != nullptr;

                game.handleClick(row, col);

                bool moveMade = (int)game.getMoveHistory().size() > oldHistory;

                if (moveMade)
                {
                    if (capture)
                        PlaySound(captureSound);
                    else
                        PlaySound(moveSound);

                    // only play check/checkmate sounds if they
                    // just happened this move, not if they were already active
                    if (!oldCheck && game.isCheck())
                        PlaySound(checkSound);

                    if (!oldMate && game.isCheckmate())
                        PlaySound(checkmateSound);
                }
            }
        }

        BeginDrawing();

        ClearBackground({ 20,18,16,255 });

        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                Color squareColor = ((r + c) % 2 == 0)
                    ? themes[currentTheme].light
                    : themes[currentTheme].dark;

                DrawRectangle(c * TILE, r * TILE, TILE, TILE, squareColor);
            }
        }

        // highlight the squares of the last move
        if (game.getLastFromRow() != -1)
        {
            DrawRectangle(game.getLastFromCol() * TILE, game.getLastFromRow() * TILE, TILE, TILE, { 255,255,0,80 });
            DrawRectangle(game.getLastToCol() * TILE, game.getLastToRow() * TILE, TILE, TILE, { 255,215,0,120 });
        }

        // flash the king red when he's in check
        if (game.isCheck())
        {
            for (int r = 0; r < 8; r++)
            {
                for (int c = 0; c < 8; c++)
                {
                    Piece* p = game.getBoard().getPiece(r, c);

                    if (!p)
                        continue;

                    if (p->getColor() == game.getCurrentTurn())
                    {
                        char s = p->getSymbol();

                        if (s == 'K' || s == 'k')
                            DrawRectangle(c * TILE, r * TILE, TILE, TILE, { 255,0,0,120 });
                    }
                }
            }
        }

        // dot = empty square, ring = enemy piece that can be captured
        for (auto& move : game.getLegalMoves())
        {
            Piece* target = game.getBoard().getPiece(move.first, move.second);

            int x = move.second * TILE + 40;
            int y = move.first * TILE + 40;

            if (target)
                DrawRectangle(
                    move.second * TILE,
                    move.first * TILE,
                    TILE,
                    TILE,
                    Color{ 255,0,0,120 }
                );
            else
                DrawCircle(x, y, 12, { 40,180,60,160 });
        }

        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                Piece* p = game.getBoard().getPiece(r, c);

                if (!p)
                    continue;

                int id = GetPieceIndex(p);

                if (id < 0)
                    continue;

                DrawTextureEx(pieceTex[id], { c * TILE + 5.0f, r * TILE + 5.0f }, 0.0f, 0.6f, WHITE);
            }
        }

        // side panel background
        DrawRectangle(BOARD, 0, PANEL, SCREEN_H, { 30,30,30,255 });

        DrawText((game.getCurrentTurn() == 'W') ? "WHITE TURN" : "BLACK TURN", BOARD + 20, 20, 30, GOLD);

        if (game.isCheck())
            DrawText("CHECK!", BOARD + 20, 60, 30, RED);

        DrawText("MOVE HISTORY", BOARD + 20, 110, 24, GOLD);

        int historyY = 145;

        const auto& history = game.getMoveHistory();

        for (int i = 0; i < (int)history.size() && historyY <= 470; i++)
        {
            std::string move = std::to_string(i + 1) + ". " + history[i];

            DrawText(move.c_str(), BOARD + 20, historyY, 20, WHITE);

            historyY += 28;
        }

        char whiteClock[32];
        char blackClock[32];

        FormatClock(whiteClock, "White", whiteTime);
        FormatClock(blackClock, "Black", blackTime);

        // brighten the active player's clock box
        bool whiteTurn = (game.getCurrentTurn() == 'W');

        DrawRectangle(BOARD + 20, 520, 240, 50, whiteTurn ? Color{ 70,70,70,255 } : Color{ 40,40,40,255 });
        DrawRectangle(BOARD + 20, 590, 240, 50, !whiteTurn ? Color{ 70,70,70,255 } : Color{ 40,40,40,255 });

        DrawText(whiteClock, BOARD + 35, 533, 28, WHITE);
        DrawText(blackClock, BOARD + 35, 603, 28, WHITE);

        DrawText("BOARD THEMES", BOARD + 20, 690, 24, GOLD);
        DrawText("1 - Classic", BOARD + 20, 730, 20, WHITE);
        DrawText("2 - Green", BOARD + 20, 760, 20, WHITE);
        DrawText("3 - Blue", BOARD + 20, 790, 20, WHITE);
        DrawText("4 - Brown", BOARD + 160, 730, 20, WHITE);
        DrawText("5 - Gray", BOARD + 160, 760, 20, WHITE);

        if (game.isPromotionActive())
        {
            DrawOverlay({ 0,0,0,180 });

            DrawText("PAWN PROMOTION", BOARD / 2 - 140, 200, 40, YELLOW);
            DrawText("1=Queen  2=Rook  3=Bishop  4=Knight", BOARD / 2 - 260, 280, 25, WHITE);
        }

        if (game.isCheckmate())
        {
            DrawOverlay();

            DrawText("CHECKMATE!", BOARD / 2 - 120, 250, 40, RED);
            DrawText((game.getCurrentTurn() == 'W') ? "BLACK WINS" : "WHITE WINS", BOARD / 2 - 120, 320, 35, WHITE);
            DrawText("Press R to play again", BOARD / 2 - 140, 390, 25, GRAY);
        }

        if (gameOverOnTime)
        {
            DrawOverlay();

            DrawText(timeWinner.c_str(), BOARD / 2 - 170, 300, 35, GOLD);
            DrawText("Press R to play again", BOARD / 2 - 140, 360, 25, GRAY);
        }

        EndDrawing();
    }

    for (int i = 0; i < 12; i++)
        UnloadTexture(pieceTex[i]);

    UnloadSound(moveSound);
    UnloadSound(captureSound);
    UnloadSound(checkSound);
    UnloadSound(checkmateSound);
    UnloadSound(promotionSound);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}