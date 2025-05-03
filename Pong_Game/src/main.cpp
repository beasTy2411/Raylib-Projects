#include<raylib.h>

class Ball{
    public: 
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }
    void update()
    {
        x += speed_x;
        y += speed_y;

        if(y + radius >=GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        if(x + radius >= GetScreenWidth() || x - radius <= 0)
        {
            speed_x *= -1;
        }
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
        DrawRectangle(x,y,width,height,WHITE);
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

    Color CustomColor = {100,150,100,255};
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
        
        //3. Drawing
        ClearBackground(CustomColor);
        BeginDrawing();
        DrawLine(ScreenWidth/2, 0, ScreenWidth/2, ScreenHeight ,WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}