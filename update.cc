#include "game.h"
#include <math.h>
//Update game per frame...
void Game::UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P'))
            pause = !pause;
        if (!pause)
        {
            //Player control
            if (IsKeyPressed(KEY_RIGHT) && (snake.speed.x == 0) && allowMove)
            {
                snake.speed = (Vector2){SQUARE_SIZE, 0};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && (snake.speed.x == 0) && allowMove)
            {
                snake.speed = (Vector2){-SQUARE_SIZE, 0};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_UP) && (snake.speed.y == 0) && allowMove)
            {
                snake.speed = (Vector2){0, -SQUARE_SIZE};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && (snake.speed.y == 0) && allowMove)
            {
                snake.speed = (Vector2){0, SQUARE_SIZE};
                allowMove = false;
            }

            //Snake movement

            if ((framesCounter % 15) == 0)
            {

                snake.position.x += snake.speed.x;
                snake.position.y += snake.speed.y;
                allowMove = true;
                snake.radius = snake.radius < 8 ? 8 : snake.radius * 0.99;
                for (int i = 0; i < dripNums; i++)
                {
                    fruit[i].radius *= 0.98;

                    if (fruit[i].radius < snake.radius / 5)
                    {
                        fruit[i].active = false;
                    }
                }
            }

            //Wall behaviour
            if (((snake.position.x) > (screenWidth - offset.x)) ||
                ((snake.position.y) > (screenHeight - offset.y)) ||
                (snake.position.x < 0) || (snake.position.y < 0))
            {
                gameOver = true;
            }

            /*             //Collision with yourself
            for (int i = 1; i < counterTail; i++)
            {
                if ((snake[0].position.x == snake[i].position.x) && 
                        (snake[0].position.y == snake[i].position.y)) 
                    gameOver = true;
            } */

            //Fruit position calculation

            int temp_num = GetRandomValue(0, 10);
            dripNums = dripNums < temp_num ? temp_num : dripNums;
            for (int i = 0; i < dripNums; i++)
            {
                if (!fruit[i].active)
                {

                    fruit[i].active = true;
                    fruit[i].position = (Vector2){
                        GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
                        GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                    fruit[i].radius = GetRandomValue(snake.radius / 2, snake.radius * 2);

                    while ((fruit[i].position.x == snake.position.x) &&
                           (fruit[i].position.y == snake.position.y))
                    {
                        fruit[i].position = (Vector2){
                            GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
                            GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                    }
                }
            }
        }

        //Collision
        for (int i = 0; i < dripNums; i++)
        {
            if ((snake.position.x < (fruit[i].position.x + fruit[i].radius) &&
                 (snake.position.x + snake.radius) > fruit[i].position.x) &&
                (snake.position.y < (fruit[i].position.y + fruit[i].radius) &&
                 (snake.position.y + snake.radius) > fruit[i].position.y))
            {
                if (snake.radius > fruit[i].radius)
                {
                    snake.radius = sqrt(pow(snake.radius, 2) + pow(fruit[i].radius, 2));
                    fruit[i].active = false;
                }
                else
                {
                    float old_snake_size = pow(snake.radius, 2);

                    snake.radius = sqrt(pow(snake.radius, 2) - pow(snake.radius, 4) / pow(fruit[i].radius, 2));
                    fruit[i].radius = sqrt(pow(fruit[i].radius, 2) + sqrt(old_snake_size - pow(snake.radius, 2)));
                }
                //snake[counterTail].position = snakePosition[counterTail - 1];
                //counterTail += 1;
            }
        }
        framesCounter++;
    }

    

    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }
    camera.fovy = 45.0f - (framesCounter * 0.05f);
}
