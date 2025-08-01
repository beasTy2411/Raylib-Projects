#include "graphics.h"

static Texture x_texture, o_texture;

// Initializes graphics for the game
void GraphicsInit(void)
{
    Image x_img = LoadImage("graphics/x_image.png");
    ImageResize(&x_img, 180, 180);
    x_texture = LoadTextureFromImage(x_img);

    Image o_img = LoadImage("graphics/o_image.png");
    ImageResize(&o_img, 180, 180);
    o_texture = LoadTextureFromImage(o_img);

    UnloadImage(x_img);
    UnloadImage(o_img);
}

// Unload textures from memory
void GraphicsUnload(void)
{
    UnloadTexture(x_texture);
    UnloadTexture(o_texture);
}

// Draw player icons
void DrawBoard(GridCell board[ROWS][COLS])
{
    const int offset = 130, cell = 180;
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
        {
            int x = offset + c * cell, y = offset + r * cell;
            DrawRectangle(x, y, cell, cell, GRAY);
            // DrawRectangleLines(x, y, cell, cell, BLACK);
            DrawRectangleLinesEx((Rectangle){x, y, cell, cell}, 4, DARKGRAY);
            if (board[r][c].player == PLAYER_X)
                DrawTextureV(x_texture, (Vector2){x, y}, WHITE);
            else if (board[r][c].player == PLAYER_O)
                DrawTextureV(o_texture, (Vector2){x, y}, WHITE);
        }
}

// Draw status text
void DrawStatus(const char *msg)
{
    int width = MeasureText(msg, 40);
    DrawText(msg, 400 - width / 2, 720, 40, BLACK);
}

// Draw title text
void DrawTitle(const char *title)
{
    int width = MeasureText(title, 64);
    DrawText(title, 400 - width / 2, 64, 64, DARKGRAY);
}
