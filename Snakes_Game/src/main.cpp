#include<raylib.h>
#include<iostream>
#include<deque>
#include<raymath.h>

using namespace std;

int cellSize = 20;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for(unsigned int i = 0; i< deque.size(); i++)
    {
        if(Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
    public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9},Vector2{4,9}};
    Vector2 direction = {1,0};
    bool addSegment  = false;

    void Draw()
    {
        for(unsigned int i =0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset+x*cellSize, offset+y*cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 4, BLACK);
        }
    }

    void Update()
    {   
        body.push_front(Vector2Add(body[0], direction));
        if(addSegment == true)
        {
            addSegment = false;
        }
        else
        {
            body.pop_back();
        }
        
    }

    void Reset()
    {
        body = {Vector2{6,9}, Vector2{5,9},Vector2{4,9}};
        direction = {1,0};
    }

};

class Food
{
    public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("assets/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    void Draw()
    {   
        DrawTexture(texture,offset + position.x*cellSize, offset + position.y*cellSize, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float X = GetRandomValue(0, cellCount-1);
        float Y = GetRandomValue(0, cellCount -1);
        return Vector2{X, Y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
        
        Vector2 position = GenerateRandomCell();
        while(ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        return position;
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

};

class Game
{
    public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;

    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("sounds/eat.mp3");
        wallSound = LoadSound("sounds/wall.mp3");
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }
    void Update()
    {   
        if(running)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }
    void CheckCollisionWithFood()
    {
        if(Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges()
    {
        if(snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            GameOver();
        }
    }

    void GameOver()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }

    void CheckCollisionWithTail()
    {
        deque<Vector2> headlessbody = snake.body;
        headlessbody.pop_front();
        if(ElementInDeque(snake.body[0], headlessbody))
        {
            GameOver();
        }
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }
};

int main()
{   
    Color digital_green = Color{176,212,93,150};
    InitWindow(2*offset+cellSize*cellCount, 2*offset+cellCount*cellSize, "Snake 2D");
    SetTargetFPS(60);

    Game game = Game();



    while (!WindowShouldClose())
    {   
        BeginDrawing();

        //Updating
        if(eventTriggered(0.2))
        {
            game.Update();
        }  

        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0,-1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0,1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1,0};
            game.running = true;
        }

        //Drawing
        ClearBackground(digital_green);
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset -5, (float)cellSize*cellCount+10, (float)cellSize*cellCount+10},5, BLACK);
        DrawText("Retro Snake", offset - 5, 20, 40, BLACK);
        DrawText(TextFormat("Score: %i",game.score),offset - 5, offset + cellSize*cellCount+10 , 40, BLACK);
        game.Draw();

        EndDrawing();
    }
    
    return 0;
}