#include "game.h"

static GridCell board[ROWS][COLS];
static int currentPlayer, moveCount;

// Initalize the game
void GameInit(void)
{
    // Set value in grid cell to empty
    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            board[r][c].player = EMPTY;
        }
    }
    // sets current player as PLAYER_X
    currentPlayer = PLAYER_X;
    // sets move count to 0
    moveCount = 0;
}

// Records the move in grid cell
int GameMakeMove(int row, int col)
{
    if (board[row][col].player != EMPTY)
    {
        return 0;
    }
    board[row][col].player = currentPlayer;
    moveCount++;
    return 1;
}

// Check win conditions
int GameCheckWin(int player)
{
    for (int i = 0; i < ROWS; i++)
        if ((board[i][0].player == player && board[i][1].player == player && board[i][2].player == player) ||
            (board[0][i].player == player && board[1][i].player == player && board[2][i].player == player))
            return 1;
    if ((board[0][0].player == player && board[1][1].player == player && board[2][2].player == player) ||
        (board[0][2].player == player && board[1][1].player == player && board[2][0].player == player))
        return 1;
    return 0;
}

// Check if game is a draw
int GameIsDraw(void)
{
    return moveCount >= 9;
}

// Switches Player turnds
void GameSwitchTurn(void)
{
    currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

// Returns current player
int GameGetCurrentTurn(void)
{
    return currentPlayer;
}

// Return game board values
GridCell (*GameGetBoard(void))[COLS]
{
    return board;
}
