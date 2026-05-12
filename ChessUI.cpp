#include "raylib.h"
#include "Game.h"
#include <vector>
#include <string>
#include <cstdio>
#include"Queen.h"
#include"Bishop.h"
#include"King.h"
#include"Rook.h"
#include"Knight.h"

void PromotePawn(Board& board, char color, int r, int c, int choice)
{
    Piece* old = board.getPiece(r, c);

    if (!old)
        return;

    delete old;

    Piece* newPiece = nullptr;

    if (choice == 1)
    {
        newPiece = new Queen(color, r, c);
    }
    else if (choice == 2)
    {
        newPiece = new Rook(color, r, c);
    }
    else if (choice == 3)
    {
        newPiece = new Bishop(color, r, c);
    }
    else if (choice == 4)
    {
        newPiece = new Knight(color, r, c);
    }

    board.setPiece(r, c, newPiece);
}
// =====================================================
// CONFIG
// =====================================================
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
// LAST MOVE
// =====================================================
int lastFromRow = -1;
int lastFromCol = -1;
int lastToRow = -1;
int lastToCol = -1;
// PROMOTION STATE
// =====================================================
bool promotionActive = false;
int promoRow = -1;
int promoCol = -1;
char promotionColor = 'W';

// =====================================================
// MOVE HISTORY
// =====================================================
std::vector<std::string> moveHistory;

// =====================================================
// CHESS CLOCK
// =====================================================
float whiteTime = 600.0f;
float blackTime = 600.0f;

bool gameOverOnTime = false;
std::string timeWinner = "";

// =====================================================
// BOARD THEMES
// =====================================================
struct BoardTheme
{
    Color light;
    Color dark;
};

std::vector<BoardTheme> themes =
{
    { Color{240,217,181,255}, Color{181,136,99,255} }, // Classic
    { Color{235,235,208,255}, Color{119,148,85,255} }, // Green
    { Color{229,228,200,255}, Color{60,95,135,255} },  // Blue
    { Color{255,230,200,255}, Color{140,90,60,255} },  // Brown
    { Color{220,220,220,255}, Color{80,80,80,255} }    // Gray
};

int currentTheme = 0;

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
    captureSound = LoadSound("Assets/Sounds/anime-ahh.mp3");
    checkSound = LoadSound("Assets/Sounds/gun-load_abJphmJ.mp3");
    checkmateSound = LoadSound("Assets/Sounds/checkmate.mp3");

    Game game;

    std::vector<std::pair<int, int>> legalMoves;

    bool selected = false;
    Piece* selectedPiece = nullptr;

    int sr = -1;
    int sc = -1;

    bool check = false;
    bool checkmate = false;

    // =====================================================
    // LEGAL MOVES
    // =====================================================
    auto GetMoves = [&](Piece* p, int r, int c)
        {
            legalMoves.clear();

            for (int tr = 0; tr < 8; tr++)
            {
                for (int tc = 0; tc < 8; tc++)
                {
                    Piece* target = game.board.getPiece(tr, tc);

                    if (target &&
                        target->getColor() == p->getColor())
                    {
                        continue;
                    }

                    if (!p->isValidMove(tr, tc, game.board))
                    {
                        continue;
                    }

                    // simulate move
                    Piece* captured = game.board.getPiece(tr, tc);

                    game.board.setPiece(tr, tc, p);
                    game.board.setPiece(r, c, nullptr);

                    int oldR = p->getRow();
                    int oldC = p->getCol();

                    p->setPosition(tr, tc);

                    bool illegal =
                        game.board.isCheck(p->getColor());

                    // undo
                    game.board.setPiece(r, c, p);
                    game.board.setPiece(tr, tc, captured);

                    p->setPosition(oldR, oldC);

                    if (!illegal)
                    {
                        legalMoves.push_back({ tr, tc });
                    }
                }
            }
        };

    // =====================================================
    // GAME LOOP
    // =====================================================
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // =====================================================
        // CHANGE THEME
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
        if (!checkmate && !gameOverOnTime)
        {
            if (game.currentTurn == 'W')
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

        Vector2 mouse = GetMousePosition();
        
        if (promotionActive)
        {
            if (IsKeyPressed(KEY_ONE))
            {
                PromotePawn(game.board, promotionColor, promoRow, promoCol, 1);
                promotionActive = false;
            }

            if (IsKeyPressed(KEY_TWO))
            {
                PromotePawn(game.board, promotionColor, promoRow, promoCol, 2);
                promotionActive = false;
            }

            if (IsKeyPressed(KEY_THREE))
            {
                PromotePawn(game.board, promotionColor, promoRow, promoCol, 3);
                promotionActive = false;
            }

            if (IsKeyPressed(KEY_FOUR))
            {
                PromotePawn(game.board, promotionColor, promoRow, promoCol, 4);
                promotionActive = false;
            }
        }
        // =====================================================
        // GAME OVER
        // =====================================================
        if (checkmate || gameOverOnTime)
        {
            BeginDrawing();

            ClearBackground(BLACK);

            if (checkmate)
            {
                DrawText(
                    "CHECKMATE!",
                    BOARD / 2 - 120,
                    250,
                    40,
                    RED
                );
                DrawText(
                    (game.currentTurn == 'W')
                    ? "BLACK WINS"
                    : "WHITE WINS",
                    BOARD / 2 - 120,
                    320,
                    35,
                    WHITE
                );
            }
            else
            {
                DrawText(
                    timeWinner.c_str(),
                    BOARD / 2 - 170,
                    300,
                    35,
                    GOLD
                );
            }

            EndDrawing();
            continue;
        }

        // =====================================================
        // INPUT
        // =====================================================
        if (!promotionActive && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int c = mouse.x / TILE;
            int r = mouse.y / TILE;

            if (c >= 0 && c < 8 && r >= 0 && r < 8)
            {
                // =============================================
                // SELECT
                // =============================================
                if (!selected)
                {
                    Piece* p = game.board.getPiece(r, c);

                    if (p &&p->getColor() == game.currentTurn)
                    {
                        selected = true;
                        selectedPiece = p;
                        sr = r;
                        sc = c;
                        GetMoves(p, r, c);
                    }
                }

                // =============================================
                // MOVE
                // =============================================
                else
                {
                    bool ok = false;

                    for (auto& m : legalMoves)
                    {
                        if (m.first == r &&
                            m.second == c)
                        {
                            ok = true;
                        }
                    }

                    if (ok)
                    {
                        Piece* target = game.board.getPiece(r, c);
                        // move
                        game.board.movePiece(sr,sc,r,c);
                        //pawn promotion
                        Piece* moved = game.board.getPiece(r, c);

                        if (moved)
                        {
                            char s = moved->getSymbol();

                            if ((s == 'P' && r == 0) || (s == 'p' && r == 7))
                            {
                                promotionActive = true;
                                promoRow = r;
                                promoCol = c;
                                promotionColor = moved->getColor();
                            }
                        }
                        // move history
                        char fromFile = 'a' + sc;
                        char toFile = 'a' + c;

                        int fromRank = 8 - sr;
                        int toRank = 8 - r;

                        std::string moveText = "";

                        moveText += fromFile;
                        moveText += std::to_string(fromRank);
                        moveText += " -> ";
                        moveText += toFile;
                        moveText += std::to_string(toRank);

                        moveHistory.push_back(moveText);

                        // last move
                        lastFromRow = sr;
                        lastFromCol = sc;

                        lastToRow = r;
                        lastToCol = c;

                        // sounds
                        if (target)
                            PlaySound(captureSound);
                        else
                            PlaySound(moveSound);

                        // switch turn
                        game.currentTurn =
                            (game.currentTurn == 'W')
                            ? 'B'
                            : 'W';

                        char enemy =
                            game.currentTurn;

                        check =
                            game.board.isCheck(enemy);

                        checkmate =
                            game.board.isCheckmate(enemy);

                        if (check)
                            PlaySound(checkSound);

                        if (checkmate)
                            PlaySound(checkmateSound);
                    }

                    selected = false;
                    selectedPiece = nullptr;

                    legalMoves.clear();
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
        if (lastFromRow != -1)
        {
            DrawRectangle(
                lastFromCol * TILE,
                lastFromRow * TILE,
                TILE,
                TILE,
                Color{ 255,255,0,80 }
            );

            DrawRectangle(
                lastToCol * TILE,
                lastToRow * TILE,
                TILE,
                TILE,
                Color{ 255,215,0,120 }
            );
        }

        // =====================================================
        // CHECK HIGHLIGHT
        // =====================================================
        if (check)
        {
            for (int r = 0; r < 8; r++)
            {
                for (int c = 0; c < 8; c++)
                {
                    Piece* p =
                        game.board.getPiece(r, c);

                    if (!p)
                        continue;

                    if (p->getColor() == game.currentTurn)
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
        for (auto& m : legalMoves)
        {
            Piece* target =
                game.board.getPiece(
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
                    game.board.getPiece(r, c);

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
        // PANEL
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
            (game.currentTurn == 'W')
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
        if (check)
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
        DrawText("MOVE HISTORY",BOARD + 20,110,24,GOLD);

        int historyY = 145;

        for (int i = 0;i < moveHistory.size();i++)
        {
            std::string text =
                std::to_string(i + 1)
                + ". "
                + moveHistory[i];

            DrawText(text.c_str(),BOARD + 20,historyY,20,WHITE);

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

        sprintf_s(whiteClock,"White: %02d:%02d",whiteMin,whiteSec);

        sprintf_s(blackClock,"Black: %02d:%02d",blackMin,blackSec);

        DrawRectangle(BOARD + 20,520,240,50,(game.currentTurn == 'W')? Color{ 70,70,70,255 }: Color{ 40,40,40,255 });

        DrawRectangle(BOARD + 20,590,240,50,(game.currentTurn == 'B')? Color{ 70,70,70,255 }: Color{ 40,40,40,255 });

        DrawText(whiteClock,BOARD + 35,533,28,WHITE);

        DrawText(blackClock,BOARD + 35,603,28,WHITE);

        // =====================================================
        // THEMES
        // =====================================================
        DrawText("BOARD THEMES",BOARD + 20,690,24,GOLD);

        DrawText("1 - Classic",BOARD + 20,730,20,WHITE);

        DrawText("2 - Green",BOARD + 20,760,20,WHITE);

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

        DrawText("5 - Gray",BOARD + 160,760,20,WHITE);
        if (promotionActive)
        {
            DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Color{ 0,0,0,180 });

            DrawText("PAWN PROMOTION",
                BOARD / 2 - 140, 200, 40, YELLOW);

            DrawText("Press: 1=Queen  2=Rook  3=Bishop  4=Knight",
                BOARD / 2 - 260, 280, 25, WHITE);
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