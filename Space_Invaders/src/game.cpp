#include "game.hpp"
#include <iostream>
#include <fstream>
Game::Game()
{
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.4f);
    SetSoundVolume(explosionSound, 0.5f);
    InitGame();
}

Game::~Game()
{
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::Draw()
{
    spaceship.Draw();
    for (auto &laser : spaceship.lasers)
    {
        laser.Draw();
    }

    for (auto &obstacle : obstacles)
    {
        obstacle.Draw();
    }

    for (auto &alien : aliens)
    {
        alien.Draw();
    }

    for (auto &alien : alienLasers)
    {
        alien.Draw();
    }

    mysteryship.Draw();
}

void Game::Update()
{
    if (run)
    {
        double currentTime = GetTime();
        if (currentTime - timeLastSpawn > mysteryshipSpawnInterval)
        {
            mysteryship.Spawn();
            timeLastSpawn = currentTime;
            mysteryshipSpawnInterval = GetRandomValue(10, 20);
        }

        for (auto &laser : spaceship.lasers)
        {
            laser.Update();
        }

        MoveAliens();

        AlienShootLaser();

        for (auto &laser : alienLasers)
        {
            laser.Update();
        }

        mysteryship.Update();

        DeleteInactiveLasers();

        CheckForCollisions();
    }
    else if (IsKeyDown(KEY_ENTER))
    {
        Reset();
        InitGame();
    }
}

void Game::HandleInput()
{
    if (run)
    {
        if (IsKeyDown(KEY_LEFT))
        {
            spaceship.MoveLeft();
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            spaceship.MoveRight();
        }
        if (IsKeyDown(KEY_SPACE))
        {
            spaceship.FireLaser();
        }
    }
}

void Game::DeleteInactiveLasers()
{
    for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
    {
        if (!it->active)
        {
            it = spaceship.lasers.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = alienLasers.begin(); it != alienLasers.end();)
    {
        if (!it->active)
        {
            it = alienLasers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size();
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;

    for (int i = 0; i < 4; i++)
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 200)}));
    }

    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 11; col++)
        {
            int alienType;
            if (row == 0)
            {
                alienType = 1;
            }
            else if (row == 1 || row == 2)
            {
                alienType = 2;
            }
            else
            {
                alienType = 3;
            }
            float x = 75 + col * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}

void Game::MoveAliens()
{
    for (auto &alien : aliens)
    {
        if (alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25)
        {
            aliensDirection = -1;
            MoveDownAliens(4);
        }
        if (alien.position.x < 25)
        {
            aliensDirection = 1;
            MoveDownAliens(4);
        }
        alien.Update(aliensDirection);
    }
}

void Game::MoveDownAliens(int distance)
{
    for (auto &alien : aliens)
    {
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser()
{
    double currentTime = GetTime();
    if (currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien &alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width / 2,
                                     alien.position.y + alien.alienImages[alien.type - 1].height},
                                    6));
        timeLastAlienFired = GetTime();
    }
}

void Game::CheckForCollisions()
{
    // Spaceship Lasers
    for (auto &laser : spaceship.lasers)
    {
        auto it = aliens.begin();
        while (it != aliens.end())
        {
            if (CheckCollisionRecs(it->getRect(), laser.getRect()))
            {
                PlaySound(explosionSound);
                if (it->type == 1)
                {
                    score += 100;
                }
                else if (it->type == 2)
                {
                    score += 200;
                }
                else if (it->type == 3)
                {
                    score += 300;
                }
                checkForHighScore();

                it = aliens.erase(it);
                laser.active = false;
            }
            else
            {
                ++it;
            }
        }

        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                {
                    ++it;
                }
            }
        }

        if (CheckCollisionRecs(mysteryship.getRect(), laser.getRect()))
        {
            mysteryship.alive = false;
            laser.active = false;
            score += 500;
            checkForHighScore();
            PlaySound(explosionSound);
        }
    }

    // Alien lasers
    for (auto &laser : alienLasers)
    {
        if (CheckCollisionRecs(laser.getRect(), spaceship.getRect()))
        {
            laser.active = false;
            lives--;
            if (lives == 0)
            {
                GameOver();
            }
        }

        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    // Alien Collision with Obstacles
    for (auto &alien : aliens)
    {
        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), alien.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }

        if (CheckCollisionRecs(alien.getRect(), spaceship.getRect()))
        {
            GameOver();
        }
    }
}

void Game::GameOver()
{
    run = false;
    std::cout << "GameOver" << std::endl;
}

void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0;
    timeLastSpawn = 0;
    mysteryshipSpawnInterval = GetRandomValue(10, 20);
    lives = 3;
    run = true;
    score = 0;
    highScore = LoadHighScoreFromFile();
}

void Game::checkForHighScore()
{
    if (score > highScore)
    {
        highScore = score;
        SaveHighScoreToFile(highScore);
    }
}

void Game::SaveHighScoreToFile(int highscore)
{
    std::ofstream highscoreFile("highscore.txt");

    if (highscoreFile.is_open())
    {
        highscoreFile << highScore;
        highscoreFile.close();
    }
    else
    {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}

int Game::LoadHighScoreFromFile()
{
    int loadedHighScore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open())
    {
        highscoreFile >> loadedHighScore;
        highscoreFile.close();
    }
    else
    {
        std::cerr << "Failed to load the highscore from the file" << std::endl;
    }
    return loadedHighScore;
}
