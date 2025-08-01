#ifndef INPUT_H
#define INPUT_H

// structure to store col and row values
typedef struct
{
    int row;
    int col;
} CellPos;

// Returns clicked cell position
CellPos GetCellClicked(void);

#endif
