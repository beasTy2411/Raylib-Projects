
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include "game.h"

// Initialize graphics
void GraphicsInit(void);

// Unloads Textures from memory
void GraphicsUnload(void);

// Draws Player icons and board
void DrawBoard(GridCell board[ROWS][COLS]);

// Draw Game status message
void DrawStatus(const char *msg);

// Draw Title text
void DrawTitle(const char *title);

#endif