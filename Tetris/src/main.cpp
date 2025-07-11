#include "raylib.h"
#include "game.h"
#include "colors.h"
#include <iostream>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 620

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main()
{
    // Initializing the Window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    Game game = Game();

    while (WindowShouldClose() == false)
    {
        // Event Handlings
        game.HandleInput();

        // Updates
        if (EventTriggered(0.3))
        {
            game.MoveBlockDown();
        }
        UpdateMusicStream(game.music);

        // Drawing
        BeginDrawing();
        ClearBackground(darkBlue);

        game.Draw();
        DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
        DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);
        if (game.gameOver)
        {
            DrawTextEx(font, "GAME OVER", {320, 450}, 38, 2, WHITE);
        }
        DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);

        char scoreText[10];
        sprintf(scoreText, "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);

        DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 65}, 38, 2, WHITE);
        DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);

        // End Drawing
        EndDrawing();
    }
    // Close Window
    CloseWindow();
    return 0;
}