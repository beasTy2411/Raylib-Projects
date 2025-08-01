#ifndef GAME_H
#define GAME_H

#define ROWS 3
#define COLS 3
#define PLAYER_X 1
#define PLAYER_O 2
#define EMPTY 0

// Cell structure to keep the information of player in the cell
typedef struct
{
    int player;
} GridCell;

// Initalize the game
void GameInit(void);

// Stores the player value in the grid cell
int GameMakeMove(int row, int col);

// Check the win conditions
int GameCheckWin(int player);

// Checks draw conditions
int GameIsDraw(void);

// Swtiches turns between PLAYER_X and PLAYER_O
void GameSwitchTurn(void);

// Returns current player
int GameGetCurrentTurn(void);

// Return the game board values
GridCell (*GameGetBoard(void))[COLS];

#endif