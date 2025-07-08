#include "pipe.h"

#include <stdio.h>

PipePair pipes[10];
PipePair pipe;
float spawnTimer = 0.0f;
const float spawnInterval = 3.0f;
Texture2D text;
Texture2D invertedTexture;
Vector2 invertedPosition;
bool pipeSpawned;
int gapY = 150;
Rectangle src = {0};

Rectangle groundBbox = {0};
Rectangle birdBbox = {0};
char string[32] = {0};

void Pipe_Init()
{
    Image image = LoadImage("assets/pipe.png");
    ImageResize(&image, image.width * 0.5, image.height * 0.5);
    text = LoadTextureFromImage(image);
    pipe.texture = &text;
    invertedTexture = text;
    UnloadImage(image);

    src = (Rectangle){0.f, 0.f, (float)invertedTexture.width, -(float)invertedTexture.height};

    for (int i = 0; i < 10; i++)
    {
        pipes[i].is_active = false;
        pipes[i].point_awarded = false;
        pipes[i].texture = &text;
    }
}

int Pipe_Update(int xShift)
{
    int pointScored = 0;
    spawnTimer += GetFrameTime();
    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0;
        pipeSpawned = false;
        for (int i = 0; i < 10; i++)
        {
            if (!pipes[i].is_active)
            {
                pipes[i].is_active = true;
                pipes[i].position.x = GetScreenWidth();
                pipes[i].invertedY = GetRandomValue(75, GetScreenHeight() / 2) - invertedTexture.height;
                pipes[i].position.y = pipes[i].invertedY + invertedTexture.height + gapY;
                pipeSpawned = true;
                break;
            }
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (pipes[i].is_active)
        {
            pipes[i].position.x = pipes[i].position.x - (xShift * GetFrameTime());

            if (!pipes[i].point_awarded && (pipes[i].position.x + pipes[i].texture->width) < (bird.x + bird.bird_texture.width))
            {
                pipes[i].point_awarded = true;
                // printf("crossed\n");
                pointScored++;
            }

            if (pipes[i].position.x < -pipes[i].texture->width)
            {
                pipes[i].is_active = false;
                pipes[i].point_awarded = false;
            }
        }
    }
    return pointScored;
}

void Pipe_Render()
{

    for (int i = 0; i < 10; i++)
    {
        if (pipes[i].is_active)
        {
            DrawTexture(*pipes[i].texture, pipes[i].position.x, pipes[i].position.y, RAYWHITE);
            DrawTextureRec(invertedTexture, src, (Vector2){pipes[i].position.x, pipes[i].invertedY}, RAYWHITE);

            // // DEBUG LINES
            // Rectangle bottomBbox = {
            //     pipes[i].position.x + 10,
            //     pipes[i].position.y + 5,
            //     pipes[i].texture->width - 20,
            //     pipes[i].texture->height - 5};

            // Rectangle invertedBbox = {
            //     pipes[i].position.x + 10,
            //     pipes[i].invertedY - 5,
            //     pipes[i].texture->width - 20,
            //     pipes[i].texture->height};

            // // Filled semi-transparent rectangles
            // DrawRectangleRec(bottomBbox, Fade(RED, 0.3f));
            // DrawRectangleRec(invertedBbox, Fade(RED, 0.3f));

            // // Optional: outline on top
            // DrawRectangleLinesEx(bottomBbox, 2, RED);
            // DrawRectangleLinesEx(invertedBbox, 2, RED);
        }
    }
}

bool Check_Collision(Bird *bird, Texture2D *groundTexture)
{
    birdBbox = (Rectangle){bird->x + 5, bird->y + 5, bird->bird_texture.width - 10, bird->bird_texture.height - 10};
    groundBbox = (Rectangle){0, 650, groundTexture->width, groundTexture->height};

    if (CheckCollisionRecs(birdBbox, groundBbox))
    {
        return true;
    }

    for (int i = 0; i < 10; i++)
    {
        if (pipes[i].is_active)
        {
            // Bottom pipe bounding box
            Rectangle bottomBbox = (Rectangle){
                pipes[i].position.x + 5,
                pipes[i].position.y + 5,
                pipes[i].texture->width - 5,
                pipes[i].texture->height - 5};

            // Inverted pipe bounding box
            Rectangle invertedBbox = (Rectangle){
                pipes[i].position.x + 5,
                pipes[i].invertedY - 5,
                pipes[i].texture->width - 5,
                pipes[i].texture->height};

            // Check collisions
            if (CheckCollisionRecs(birdBbox, bottomBbox) || CheckCollisionRecs(birdBbox, invertedBbox))
            {
                return true;
            }
        }
    }
    return false;
}

void Pipe_Reset()
{
    for (int i = 0; i < 10; i++)
    {
        pipes[i].is_active = false;
        spawnTimer = 0;
    }
}

void Pipe_Unload()
{
    UnloadTexture(text);
    UnloadTexture(invertedTexture);
}