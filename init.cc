#include "game.h"

Game::Game()
{
}

//Initialize game variables
void Game::InitGame(void)
{
    framesCounter = 0;
    gameOver = false;
    pause = false;
    dripNums = 0;
    buffNums=0;
    //fruit = Food{ 0 };
    //snake[0] = { 0 };
    snakePosition = {0};
    allowMove = false;
    offset = {0};
    counterTail = 0;

    //camera = {0};

    counterTail = 1;
    allowMove = true;

    offset.x = screenWidth % SQUARE_SIZE;
    offset.y = screenHeight % SQUARE_SIZE;

    drip.position = Vector3{0, 0,0};
    drip.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    drip.radius = 0.5;
    drip.speed = (Vector3){0, 0,0};
    drip.buff=Normal;
    drip.color = DARKBLUE;

    snakePosition = (Vector3){0.0f, 0.0f,0.0f};

    for (int i = 0; i < DRIP_MAX_NUMS; i++)
    {
        water[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
        water[i].color = SKYBLUE;
        water[i].active = false;
        water[i].radius = 10.0f;
    }
    for (int i = 0; i < BUFF_MAX_NUMS; i++)
    {
        buff[i].species=Dirt;
        buff[i].rec.height=SQUARE_SIZE;
        buff[i].rec.width=SQUARE_SIZE;
        buff[i].color = BLACK;
        buff[i].active = false;
        buff[i].radius = 10.0f;
    }
    camera->position = (Vector3){ 20.0f, 14.0f, 0.0f };
    camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera->up = (Vector3){ -1.0f, 0.0f, 0.0f };
    camera->fovy = 45.0f;
    camera->projection = CAMERA_PERSPECTIVE;
    SetCameraMode(*camera, CAMERA_FREE);
    // camera.position = (Vector3){0.0f, 10.0f, 10.0f};
    // camera.target = (Vector3){1.0f, 0.0f, 0.0f};
    // camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    // camera.fovy = 45.0f;
    // camera.projection = CAMERA_PERSPECTIVE;
}
