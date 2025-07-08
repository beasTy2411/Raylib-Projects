#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h>

#include "bird.h"
#include "pipe.h"
#include "highscore.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main()
{
    int groundScroll = 0;
    int groundScrollSpeed = 5;
    int groundLoopingPoint = 392;
    int backgroundScroll = 0;
    int backgroundScrollSpeed = 2;
    int backgroundLoopingPoint = 1280;
    int pipeScrollSpeed = 200;
    float titleBounceTime = 0.f;
    float cloudScroll = 0;
    float cloudScrollSpeed = 1.0f;
    int score = 0;
    int highScore = 0;
    const char *highScoreFile = "highscore.flb ";

    bool collided = false;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "New Window");
    SetTargetFPS(60);
    Image background = LoadImage("assets/background.jpg");
    Texture2D backgroundTexture = LoadTextureFromImage(background);
    UnloadImage(background);

    Image ground = LoadImage("assets/ground.png");
    Texture2D groundTexture = LoadTextureFromImage(ground);
    UnloadImage(ground);

    Image cloudImage = LoadImage("assets/clouds.png");
    ImageResize(&cloudImage, cloudImage.width * .7, cloudImage.height * 0.7);
    Texture2D cloudTexture = LoadTextureFromImage(cloudImage);
    UnloadImage(cloudImage);

    highScore = LoadHighScore(highScoreFile);
    char *gameState = "MENU";

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        if (!strcmp("MENU", gameState))
        {
            titleBounceTime += GetFrameTime();
            float bounceOffset = sinf(titleBounceTime * 2.0f) * 10.0f; // Oscillate ±10 pixels
            cloudScroll = fmodf(cloudScroll + cloudScrollSpeed, SCREEN_WIDTH);

            ClearBackground(SKYBLUE);

            DrawTexture(cloudTexture, -cloudScroll, 0, WHITE);
            DrawTexture(cloudTexture, -cloudScroll + SCREEN_WIDTH, 0, WHITE);

            DrawTexture(cloudTexture, -cloudScroll + 250, 100, WHITE);
            DrawTexture(cloudTexture, -cloudScroll + SCREEN_WIDTH + 250, 100, WHITE);

            // Draw semi-transparent panel
            DrawRectangle(SCREEN_WIDTH / 2 - 300, 150, 600, 300, Fade(RAYWHITE, 0.8f));

            // Title with shadow
            DrawText("REVERSE FLY", SCREEN_WIDTH / 2 - MeasureText("REVERSE FLY", 70) / 2 + 3, 170 + (int)bounceOffset + 3, 70, DARKGRAY);
            DrawText("REVERSE FLY", SCREEN_WIDTH / 2 - MeasureText("REVERSE FLY", 70) / 2, 170 + (int)bounceOffset, 70, MAROON);

            DrawText("Press ENTER to Play", SCREEN_WIDTH / 2 - MeasureText("Press ENTER to Play", 30) / 2, 300, 30, DARKBLUE);
            DrawText("Press I for Instructions", SCREEN_WIDTH / 2 - MeasureText("Press I for Instructions", 30) / 2, 350, 30, DARKBLUE);
            DrawText("Press Q to Quit", SCREEN_WIDTH / 2 - MeasureText("Press Q to Quit", 30) / 2, 400, 30, DARKBLUE);

            char highScoreText[32];
            sprintf(highScoreText, "High Score: %d", highScore);
            DrawText(highScoreText, SCREEN_WIDTH / 2 - MeasureText(highScoreText, 24) / 2, SCREEN_HEIGHT - 60, 24, RAYWHITE);

            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
            {
                DrawText("MENU", 320, 100, 48, RED);
                collided = false;
                groundScrollSpeed = 5;
                backgroundScrollSpeed = 2;
                score = 0;
                Bird_Init();
                Pipe_Init();
                // Bird_Reset();
                // Pipe_Reset();
                gameState = "PLAY";
            }
            else if (IsKeyPressed(KEY_I))
            {
                gameState = "INSTRUCTIONS";
            }
            else if (IsKeyPressed(KEY_Q))
            {
                break; // Exit game
            }
        }

        else if (!strcmp("INSTRUCTIONS", gameState))
        {
            ClearBackground(SKYBLUE);

            // Draw semi-transparent instruction box
            DrawRectangle(SCREEN_WIDTH / 2 - 350, 100, 700, 450, Fade(RAYWHITE, 0.85f));

            DrawText("INSTRUCTIONS", SCREEN_WIDTH / 2 - MeasureText("INSTRUCTIONS", 50) / 2, 130, 50, MAROON);

            DrawText("Press UP to dive", SCREEN_WIDTH / 2 - MeasureText("Press UP to dive", 30) / 2, 220, 30, DARKGRAY);
            DrawText("Press P to pause the game", SCREEN_WIDTH / 2 - MeasureText("Press P to pause the game", 30) / 2, 270, 30, DARKGRAY);
            DrawText("Avoid hitting pipes and ground!", SCREEN_WIDTH / 2 - MeasureText("Avoid hitting pipes and ground!", 30) / 2, 320, 30, DARKGRAY);

            DrawText("Press BACKSPACE to return to Menu", SCREEN_WIDTH / 2 - MeasureText("Press BACKSPACE to return to Menu", 25) / 2, 420, 25, DARKBLUE);

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                gameState = "MENU";
            }
        }

        else if (!strcmp("PLAY", gameState))
        {
            if (collided)
            {
                groundScrollSpeed = 0;
                backgroundScrollSpeed = 0;

                if (score > highScore)
                {
                    highScore = score;
                    SaveHighScore(highScoreFile, score);
                }
                Bird_Collided();
            }
            else
            {
                Bird_Update();
                score += Pipe_Update(pipeScrollSpeed);
                collided = Check_Collision(&bird, &groundTexture);

                groundScroll = (groundScroll + groundScrollSpeed) % groundLoopingPoint;
                backgroundScroll = (backgroundScroll + backgroundScrollSpeed) % backgroundLoopingPoint;
            }

            ClearBackground(WHITE);
            DrawTexture(backgroundTexture, -backgroundScroll, 0, WHITE);
            DrawTexture(backgroundTexture, -backgroundScroll + backgroundLoopingPoint, 0, WHITE);
            Pipe_Render();
            DrawTexture(groundTexture, -groundScroll, 0, WHITE);
            DrawTexture(groundTexture, -groundScroll + groundLoopingPoint + 196, 0, WHITE);
            Bird_Draw();

            char scoreText[16];
            sprintf(scoreText, "Score: %d", score);
            DrawText(scoreText, SCREEN_WIDTH / 2 - MeasureText(scoreText, 40) / 2, 30, 40, WHITE);

            // Debug collision state statements
            // Rectangle groundBbox = (Rectangle){0, 650, groundTexture.width, groundTexture.height};
            // DrawRectangleLinesEx(groundBbox, 2, RED);

            //  sprintf(string, "Collided: %d", collided);
            //  DrawText(string, 10, 50, 25, RED);

            if (collided)
            {
                DrawText("GAME\nOVER", GetScreenWidth() / 2 - MeasureText("GAME\nOVER", 72) / 2, GetScreenHeight() / 2 - MeasureText("GAME\nOVER", 72) / 2, 72, WHITE);
            }

            if (!collided && IsKeyPressed(KEY_P))
            {
                gameState = "PAUSE";
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                gameState = "MENU";
            }
        }

        else if (!strcmp("PAUSE", gameState))
        {
            // Draw current game visuals "frozen"
            ClearBackground(WHITE);
            DrawTexture(backgroundTexture, -backgroundScroll, 0, WHITE);
            DrawTexture(backgroundTexture, -backgroundScroll + backgroundLoopingPoint, 0, WHITE);
            Pipe_Render();
            DrawTexture(groundTexture, -groundScroll, 0, WHITE);
            DrawTexture(groundTexture, -groundScroll + groundLoopingPoint + 196, 0, WHITE);
            Bird_Draw();

            char scoreText[16];
            sprintf(scoreText, "Score: %d", score);
            DrawText(scoreText, SCREEN_WIDTH / 2 - MeasureText(scoreText, 40) / 2, 30, 40, WHITE);

            DrawText("PAUSED", SCREEN_WIDTH / 2 - MeasureText("PAUSED", 48) / 2, SCREEN_HEIGHT / 2 - 50, 48, WHITE);
            DrawText("Press P to Resume or BACKSPACE for MENU", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 + 20, 24, WHITE);

            if (IsKeyPressed(KEY_P))
            {
                gameState = "PLAY";
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                gameState = "MENU";
            }
        }

        EndDrawing();
    }
    UnloadTexture(backgroundTexture);
    UnloadTexture(cloudTexture);
    UnloadTexture(groundTexture);
    Bird_Unload();
    Pipe_Unload();
    CloseWindow();
    return 0;
}