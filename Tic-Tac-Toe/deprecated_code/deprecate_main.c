#include <stdio.h>
#include <raylib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define CELL_SIZE 180
#define PLAYER_X 1
#define PLAYER_O 2
#define EMPTY 0
#define COLS 3
#define ROWS 3
#define GRID_OFFSET 130

typedef struct
{
    int row;
    int col;
} cellPos;

typedef struct
{
    int player;
    Vector2 position;
} Grid;

int winner = 0; // 0 = in progress, 1 = X wins, 2 = 0 wins, 3 = Draw
int moves = 0;  // Count moves to detect draw
int current_turn = PLAYER_X;
char title[20];
char message[50];
Grid grid[ROWS][COLS];
Image x_image, o_image;
Texture x_texture, o_texture;

void gridInitialize();
void gridDraw();
void loadImages();
void xDraw(Vector2 position);
void oDraw(Vector2 position);
void checkPlayer(int row, int col);
int isPlayerEmpty(int row, int col);
int isValidCell(cellPos pos);
void Reset();

void gridInitialize()
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            grid[row][col].player = EMPTY;
            grid[row][col].position.x = GRID_OFFSET + col * CELL_SIZE;
            grid[row][col].position.y = GRID_OFFSET + row * CELL_SIZE;
        }
    }
}

void gridDraw()
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            DrawRectangle(grid[row][col].position.x, grid[row][col].position.y, CELL_SIZE, CELL_SIZE, DARKGRAY);
            DrawRectangleLines(grid[row][col].position.x, grid[row][col].position.y, CELL_SIZE, CELL_SIZE, BLACK);
        }
    }
}

void loadImages()
{
    x_image = LoadImage("graphics/x_image.png");
    ImageResize(&x_image, x_image.width / 14, x_image.height / 14);
    x_texture = LoadTextureFromImage(x_image);
    o_image = LoadImage("graphics/o_image.png");
    ImageResize(&o_image, o_image.width / 6.7f, o_image.height / 6.7f);
    o_texture = LoadTextureFromImage(o_image);
    UnloadImage(o_image);
    UnloadImage(x_image);
}

void xDraw(Vector2 position)
{
    DrawTextureV(x_texture, position, WHITE);
}

void oDraw(Vector2 position)
{
    DrawTextureV(o_texture, position, WHITE);
}

int checkWin(int player)
{
    // For rows
    for (int row = 0; row < ROWS; row++)
    {
        if (grid[row][0].player == player &&
            grid[row][1].player == player &&
            grid[row][2].player == player)
        {
            return 1;
        }
    }

    // For col
    for (int col = 0; col < COLS; col++)
    {
        if (grid[0][col].player == player &&
            grid[1][col].player == player &&
            grid[2][col].player == player)
        {
            return 1;
        }
    }

    // For diagonal
    if (grid[0][0].player == player &&
        grid[1][1].player == player &&
        grid[2][2].player == player)
    {
        return 1;
    }

    if (grid[0][2].player == player &&
        grid[1][1].player == player &&
        grid[2][0].player == player)
    {
        return 1;
    }
    return 0;
}

void checkPlayer(int row, int col)
{

    if (isPlayerEmpty(row, col))
    {
        sprintf(message, "Position Not Empty");
        int text_width = MeasureText(message, 48);
        DrawText(message, SCREEN_WIDTH / 2 - text_width / 2, 740, 48, DARKGRAY);
    }
    else
    {
        printf("Player turn %d", current_turn);
        if (current_turn == PLAYER_X)
        {
            // printf("Clicked X cell: row %d, column %d\n", row, col);
            grid[row][col].player = PLAYER_X;
            current_turn = PLAYER_O;
        }
        else if (current_turn == PLAYER_O)
        {
            // printf("Clicked O cell: row %d, column %d\n", row, col);
            grid[row][col].player = PLAYER_O;
            current_turn = PLAYER_X;
        }
        moves++;
        if (checkWin(PLAYER_X))
            winner = 1;
        else if (checkWin(PLAYER_O))
            winner = 2;
        else if (winner == 0 && moves == 9)
            winner = 3;
    }
}

int isPlayerEmpty(int row, int col)
{
    if (grid[row][col].player != EMPTY)
    {
        return 1;
    }
    return 0;
}

cellPos cellClicked()
{
    cellPos clicked_cell_func = {-1, -1};
    Vector2 mouse = GetMousePosition();
    if (mouse.x >= GRID_OFFSET && mouse.x < GRID_OFFSET + COLS * CELL_SIZE &&
        mouse.y >= GRID_OFFSET && mouse.y < GRID_OFFSET + ROWS * CELL_SIZE)
    {
        clicked_cell_func.col = (mouse.x - GRID_OFFSET) / CELL_SIZE;
        clicked_cell_func.row = (mouse.y - GRID_OFFSET) / CELL_SIZE;
    }
    return clicked_cell_func;
}

int isValidCell(cellPos pos)
{
    return pos.row >= 0 && pos.row < ROWS && pos.col >= 0 && pos.col < COLS;
}

void Reset()
{
    gridInitialize();
    winner = 0;
    moves = 0;
    current_turn = PLAYER_X;
    strcpy(message, "");
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic-Tac-Toe");
    SetTargetFPS(60);

    gridInitialize();
    loadImages();

    int row = 0;
    int col = 0;

    sprintf(title, "Tic-Tac-Toe");
    int title_width = MeasureText(title, 64);
    int win_message_width;
    cellPos clicked_cell;

    while (WindowShouldClose() == false)
    {
        const char *turnText = current_turn == PLAYER_X ? "Turn: Player X" : "Turn: Player O";

        if (winner == 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            clicked_cell = cellClicked();
            if (isValidCell(clicked_cell))
            {
                checkPlayer(clicked_cell.row, clicked_cell.col);
            }
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        gridDraw();

        DrawText(title, SCREEN_WIDTH / 2 - title_width / 2, GRID_OFFSET / 2 - 32, 64, DARKGRAY);

        for (row = 0; row < ROWS; row++)
        {
            for (col = 0; col < COLS; col++)
            {
                if (grid[row][col].player == PLAYER_X)
                    xDraw(grid[row][col].position);
                else if (grid[row][col].player == PLAYER_O)
                    oDraw(grid[row][col].position);
            }
        }

        if (winner == 1)
        {
            const char *msg = "Player X Wins! Press R to Restart.";
            DrawText(msg, SCREEN_WIDTH / 2 - MeasureText(msg, 40) / 2, 720, 40, BLACK);
        }
        else if (winner == 2)
        {
            const char *msg = "Player O Wins! Press R to Restart.";
            DrawText(msg, SCREEN_WIDTH / 2 - MeasureText(msg, 40) / 2, 720, 40, BLACK);
        }
        else if (winner == 3)
        {
            const char *msg = "It's a Draw! Press R to Restart.";
            DrawText(msg, SCREEN_WIDTH / 2 - MeasureText(msg, 40) / 2, 720, 40, BLACK);
        }
        else
        {
            // Only show turn message if game is ongoing
            DrawText(turnText, SCREEN_WIDTH / 2 - MeasureText(turnText, 32) / 2, 700, 32, DARKGRAY);
        }

        win_message_width = MeasureText(message, 40);
        DrawText(message, SCREEN_WIDTH / 2 - win_message_width / 2, 750, 40, DARKGRAY);

        if (winner != 0 && IsKeyPressed(KEY_R))
        {
            Reset();
        }

        EndDrawing();
    }
    UnloadTexture(x_texture);
    UnloadTexture(o_texture);
    CloseWindow();
    return 0;
}