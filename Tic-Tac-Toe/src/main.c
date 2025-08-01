#include "raylib.h"
#include "game.h"
#include "graphics.h"
#include "input.h"
#include <stdio.h>

// Define your application states
typedef enum
{
    STATE_MENU,
    STATE_GAME,
    STATE_EXIT
} AppState;

int main(void)
{
    InitWindow(800, 800, "Tic-Tac-Toe");
    SetTargetFPS(60);

    AppState appState = STATE_MENU;
    int winner = 0;         // 0=in progress, 1=X wins, 2=O wins, 3=draw
    char message[60] = "";  // Generic message string
    GridCell(*board)[COLS]; // Pointer to game board

    GameInit();
    GraphicsInit();

    while (!WindowShouldClose() && appState != STATE_EXIT)
    {
        switch (appState)
        {
        case STATE_MENU:
        {
            BeginDrawing();
            ClearBackground(LIGHTGRAY);

            const char *title = "Tic-Tac-Toe";
            const char *startInstr = "Press ENTER to Start";
            const char *exitInstr = "Press ESC to Exit";

            int tw = MeasureText(title, 84);      // text with of title
            int sw = MeasureText(startInstr, 30); // text width of start instruction
            int ew = MeasureText(exitInstr, 30);  // text width of exit instruction

            DrawText(title, 400 - tw / 2, 200, 84, DARKGRAY);
            DrawText(startInstr, 400 - sw / 2, 400, 30, DARKGRAY);
            DrawText(exitInstr, 400 - ew / 2, 450, 30, DARKGRAY);

            EndDrawing();

            if (IsKeyPressed(KEY_ENTER))
            {
                appState = STATE_GAME;
                GameInit();
                winner = 0;
                message[0] = '\0';
                board = GameGetBoard();
            }
            else if (IsKeyPressed(KEY_ESCAPE))
            {
                appState = STATE_EXIT;
            }
            break;
        }

        case STATE_GAME:
        {
            if (winner == 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                CellPos click = GetCellClicked();
                if (click.row != -1 && click.col != -1)
                {
                    if (GameMakeMove(click.row, click.col))
                    {
                        if (GameCheckWin(GameGetCurrentTurn()))
                        {
                            winner = GameGetCurrentTurn();
                            snprintf(message, sizeof(message), "Player %c Wins! Press R to Restart.", (winner == PLAYER_X ? 'X' : 'O'));
                        }
                        else if (GameIsDraw())
                        {
                            winner = 3;
                            snprintf(message, sizeof(message), "It's a Draw! Press R to Restart.");
                        }
                        else
                        {
                            GameSwitchTurn();
                            message[0] = '\0'; // Clear message after valid move without game over
                        }
                    }
                    else
                    {
                        snprintf(message, sizeof(message), "Position Not Empty");
                    }
                }
            }

            if (winner != 0 && IsKeyPressed(KEY_R))
            {
                GameInit();
                winner = 0;
                message[0] = '\0';
            }

            BeginDrawing();
            ClearBackground(LIGHTGRAY);

            DrawTitle("Tic-Tac-Toe");
            DrawBoard(board);
            DrawStatus(message);

            if (winner == 0)
            {
                const char *turnMsg = GameGetCurrentTurn() == PLAYER_X ? "Turn: Player X" : "Turn: Player O";
                int tw = MeasureText(turnMsg, 32);
                DrawText(turnMsg, 400 - tw / 2, 700, 32, DARKGRAY);
            }
            else if (winner == PLAYER_X)
            {
                const char *winMsg = "Player X Wins! Press R to Restart.";
                int tw = MeasureText(winMsg, 40);
                DrawText(winMsg, 400 - tw / 2, 720, 40, DARKGRAY);
            }
            else if (winner == PLAYER_O)
            {
                const char *winMsg = "Player O Wins! Press R to Restart.";
                int tw = MeasureText(winMsg, 40);
                DrawText(winMsg, 400 - tw / 2, 720, 40, DARKGRAY);
            }
            else if (winner == 3) // draw
            {
                const char *drawMsg = "It's a Draw! Press R to Restart.";
                int tw = MeasureText(drawMsg, 40);
                DrawText(drawMsg, 400 - tw / 2, 720, 40, DARKGRAY);
            }

            EndDrawing();
            break;
        }

        default:
            break;
        }
    }
    GraphicsUnload();
    CloseWindow();
    return 0;
}
