#pragma once
#include "grid.h"
#include "blocks.cpp"

class Game
{
public:
    Game();
    ~Game();
    bool gameOver;
    int score;
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    Music music;

private:
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    std::vector<Block> GetAllBlocks();
    Sound rotateSound;
    Sound clearSound;
    Block GetRandomBlock();
    void MoveBlockLeft();
    void MoveBlockRight();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UpdateScore(int linesCleared, int moveDownPoints);
};