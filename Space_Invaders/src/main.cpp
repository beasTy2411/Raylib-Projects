#include "raylib.h"
#include "game.hpp"
#include <string>

#define SCREEN_WIDTH 750
#define SCREEN_HEIGHT 700
#define offset 50
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

std::string FormatWithLeadingZeroes(int number, int width)
{
    std::string numberText = std::to_string(number);
    int leadingZeroes = width - numberText.length();
    return numberText = std::string(leadingZeroes, '0') + numberText;
}

int main(void)
{
    Color grey = {29, 29, 27, 255};
    Color yellow = {243, 216, 63, 255};
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH + offset, SCREEN_HEIGHT + 2 * offset, "Space Invader");
    InitAudioDevice();

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    Game game;

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        //  Draw
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLinesEx({10, 10, 780, 780}, 0.18f, 20, 2, yellow);
        DrawLineEx({25, 730}, {775, 730}, 3, yellow);
        if (game.run)
        {
            DrawTextEx(font, "LEVEL 01", {570, 740}, 34, 2, yellow);
        }
        else
        {
            DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, yellow);
        }

        float x = 50;
        for (int i = 0; i < game.lives; i++)
        {
            DrawTextureV(spaceshipImage, {x, 745}, WHITE);
            x += 50;
        }

        DrawTextEx(font, "SCORE", {50, 15}, 34, 2, yellow);
        std::string scoreText = FormatWithLeadingZeroes(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 40}, 32, 2, yellow);
        DrawTextEx(font, "HIGH SCORE", {517, 15}, 32, 2, yellow);
        std::string highScoreText = FormatWithLeadingZeroes(game.highScore, 5);
        DrawTextEx(font, highScoreText.c_str(), {655, 40}, 34, 2, yellow);

        game.Draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseAudioDevice();
    UnloadTexture(spaceshipImage);
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window
    //--------------------------------------------------------------------------------------

    return 0;
}