#include <raylib.h>

using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green= Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

class Ball{
    public: 
    float x, y;
    int speed_x, speed_y;
    int radius;
    
    void Draw()
    {
        DrawCircle(x, y, radius, Yellow);
    }
    void update()
    {
        x += speed_x;
        y += speed_y;

        if(y + radius >=GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        if(x + radius >= GetScreenWidth())
        {
            cpu_score++;
            ResetBall();
        }
        if(x - radius <= 0)
        {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        y = GetScreenHeight()/2;
        x = GetScreenWidth()/2;
        
        int speed_choice[2] = {1, -1};
        speed_x *= speed_choice[GetRandomValue(0,1)];
        speed_y *= speed_choice[GetRandomValue(0,1)];
    }
};

class Paddle{

    protected:

    void LimiteMovement()
    {
        if(y <= 0)
        {
            y = 0;
        }
        if(y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

    public: 

    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width,height}, 0.8, 0,WHITE);
    }
    void Update()
    {
        if(IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
        LimiteMovement();

    }
};

class CPUPaddle: public Paddle{
    public: 

    void Update(int ball_y)
    {
        if(y + height/2 > ball_y)
        {
            y = y - speed;
        }
        if(y + height/2 < ball_y)
        {
            y = y + speed;
        }
        LimiteMovement();
    }
};

Ball ball;
Paddle player;
CPUPaddle cpu;

int main()
{   
    const int ScreenWidth = 1280;
    const int ScreenHeight = 800;
    InitWindow(ScreenWidth, ScreenHeight,"My First Raylib Game");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = ScreenWidth/2;
    ball.y = ScreenHeight/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = ScreenWidth - player.width -10;
    player.y = ScreenHeight/2 - player.height/2;
    player.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = ScreenHeight/2 - cpu.height/2;
    cpu.speed = 6;

    //GameLoop
    while(WindowShouldClose() == false)
    {
        //Updating
        ball.update();
        player.Update();
        cpu.Update(ball.y);

        //Checking for Collision
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speed_x *= -1;
        }

        //3. Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(ScreenWidth/2,  0, ScreenWidth/2, ScreenHeight, Green);
        DrawCircle(ScreenWidth/2, ScreenHeight/2, 200, Light_Green);
        BeginDrawing();
        DrawLine(ScreenWidth/2, 0, ScreenWidth/2, ScreenHeight ,WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), ScreenWidth/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * ScreenWidth/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}