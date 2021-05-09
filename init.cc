#include "game.h"

Game::Game()
{
}

//Initialize game variables
void Game::InitGame(void)
{
    SetWindowSize(screenWidth,screenHeight);
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

    for (int i = 0;i < 10;i++)
    {
        Weather[i].active = false;
    }

    Event_time = 0;
    Event_active = false;
    acc_rate = 1;

    for (int i = 0; i < RAIN_NUM; i++)
    {   
        Rain temp = {
            .position = (Vector2){GetRandomValue(0, (screenWidth/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.x/2,  -20},
            .radius = (BASIC_SIZE) * double(GetRandomValue(2,10))/double(10),
            .speed = (Vector2){ 0, double(GetRandomValue(2,10))/double(10) * BASIC_SPEED * 2},
            .color = rain_color,
            .active = false
        };
        rain.push_back(temp);
    }




    offset.x = MaxWidth % SQUARE_SIZE;
    offset.y = MaxHeight % SQUARE_SIZE;

    snake.position = (Vector2){screenWidth / 2, screenHeight  / 2};
    snake.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    snake.radius = 10.0f;
    snake.speed = (Vector2){0, 0};
    snake.acc=(Vector2){0,0};
    snake.buff=Normal;
    snake.color = DARKBLUE;



    for (int i = 0; i < DRIP_MAX_NUMS; i++)
    {
        Food temp = {
            .size = (Vector2){SQUARE_SIZE, SQUARE_SIZE},
            .active = false,
            .color = SKYBLUE,
            .radius = 10.0f
        };
        fruit.push_back(temp);
    }
    Rectangle rec_temp{.x = 0, .y = 0, .width = SQUARE_SIZE, .height = SQUARE_SIZE};
    for (int i = 0; i < BUFF_MAX_NUMS; i++)
    {   
        Buff temp = {
            .species=Dirt,
            .rec = rec_temp,
            .active = false,
            .color = BLACK,
            .radius = 10.0f
        };
        buff.push_back(temp);
    }

    camera.target = (Vector2){ snake.position.x, snake.position.y  };
    camera.offset = (Vector2){ screenWidth/2.0f,screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;
    updatecamera();
}
