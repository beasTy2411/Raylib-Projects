#pragma once

#include <raylib.h>

typedef struct Bird
{
    Texture2D bird_texture;
    float scale;
    int x, y;
    float velocity;
    float angle;
    bool is_alive;
    int gravity;
} Bird;

extern float targetAngle;
extern Vector2 position;
extern Vector2 origin;
extern Bird bird;
extern Sound jumpSound;

void Bird_Init();
void Bird_Draw();
void Bird_Update();
void Bird_Reset();
void Bird_Collided();
void Bird_Unload();
