#include "bird.h"

#include "raymath.h"

float targetAngle = 0.f;
Vector2 position = {0};
Vector2 origin = {0};
Bird bird;

void Bird_Init()
{
    Image bird_image = LoadImage("assets/bird.png");
    bird.scale = 0.07;
    ImageResize(&bird_image, bird_image.width * bird.scale, bird_image.height * bird.scale);
    bird.bird_texture = LoadTextureFromImage(bird_image);
    UnloadImage(bird_image);
    bird.x = 500;
    bird.y = 350;
    bird.velocity = 0;
    bird.gravity = 500;
}

void Bird_Draw()
{
    targetAngle = Clamp(-bird.velocity * 0.1f, -30.f, 90.f);
    bird.angle = Lerp(bird.angle, targetAngle, 5.0f * GetFrameTime());
    position = (Vector2){bird.x, bird.y};
    origin = (Vector2){bird.bird_texture.width / 2.0f, bird.bird_texture.height / 2.f};

    DrawTextureEx(bird.bird_texture, position, bird.angle, 1.0f, WHITE);
    // Debug Collision Bounding box statement
    //  Rectangle birdbox = {bird.x + 5, bird.y + 5, bird.bird_texture.width - 10, bird.bird_texture.height - 10};
    //  DrawRectangleLinesEx(birdbox, 2, RED);
}

void Bird_Update()
{
    if (IsKeyPressed(KEY_UP))
    {
        bird.velocity = -300;
    }

    bird.velocity = bird.velocity + bird.gravity * GetFrameTime();
    bird.y = bird.y - bird.velocity * GetFrameTime();

    if (bird.y < 0)
    {
        bird.y = 0;
        bird.velocity = 0;
    }
}

void Bird_Reset()
{
    bird.x = 500;
    bird.y = 350;
    bird.gravity = 500;
}

void Bird_Collided()
{
    bird.velocity = 0;
    bird.gravity = 0;
}

void Bird_Unload()
{
    UnloadTexture(bird.bird_texture);
}