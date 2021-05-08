#include "game.h"

Game::Game() {
}

//Initialize game variables
void Game::InitGame(void)
{
    framesCounter = 0;
    gameOver = false;
    pause = false;
    //fruit = Food{ 0 };
    //snake[0] = { 0 };
    snakePosition = { 0 };
    allowMove = false;
    offset = { 0 };
    counterTail = 0;

    //camera = {0};

    counterTail = 1;
    allowMove = false;

    offset.x = screenWidth%SQUARE_SIZE;
    offset.y = screenHeight%SQUARE_SIZE;


        snake.position = (Vector2){ offset.x/2, offset.y/2 };
        snake.size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
        snake.radius = 10.0f;
        snake.speed = (Vector2){ SQUARE_SIZE, 0 };


        snake.color = DARKBLUE;




        snakePosition = (Vector2){ 0.0f, 0.0f };


    fruit.size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
    fruit.color = SKYBLUE;
    fruit.active = false;
    fruit.radius=10.0f;

    camera.position = (Vector3){0.0f, 10.0f, 10.0f};
    camera.target = (Vector3){1.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}