#include "input.h"
#include <raylib.h>

#define GRID_OFFSET 130
#define CELL_SIZE 180
#define ROWS 3
#define COLS 3

// Returns cell position
CellPos GetCellClicked(void)
{
    CellPos pos = {-1, -1};
    Vector2 mouse = GetMousePosition();
    if (mouse.x >= GRID_OFFSET && mouse.x < GRID_OFFSET + COLS * CELL_SIZE &&
        mouse.y >= GRID_OFFSET && mouse.y < GRID_OFFSET + ROWS * CELL_SIZE)
    {
        pos.col = (mouse.x - GRID_OFFSET) / CELL_SIZE;
        pos.row = (mouse.y - GRID_OFFSET) / CELL_SIZE;
    }
    return pos;
}
