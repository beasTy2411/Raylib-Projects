#pragma once

#include <raylib.h>
#include "bird.h"

typedef struct PipePair
{
    Texture2D *texture;
    Vector2 position;
    float invertedY;
    bool is_active;
    bool point_awarded;
} PipePair;

extern PipePair pipes[10];
extern PipePair pipe;
extern int gapY;
extern float spawnTimer;
extern const float spawnInterval;
extern Texture2D text;
extern Texture2D invertedTexture;
extern Vector2 invertedPosition;
extern Rectangle src;
extern Rectangle groundBbox;
extern Rectangle birdBbox;
extern bool pipeSpawned;
extern char string[32];
extern Sound scored;

void Pipe_Init();
int Pipe_Update(int xShift);
bool Check_Collision(Bird *bird, Texture2D *groundTexture);
void Pipe_Render();
void Pipe_Reset();
void Pipe_Unload();
