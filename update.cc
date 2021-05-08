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
            if (IsKeyDown(KEY_RIGHT) && allowMove)
            {
                snake.position.x += 5;
            }
            if (IsKeyDown(KEY_LEFT) && allowMove)
            {
                snake.position.x -= 5;
            }
            if (IsKeyDown(KEY_UP) && allowMove)
            {
                snake.position.y -= 5;
            }
            if (IsKeyDown(KEY_DOWN) && allowMove)
            {
                snake.position.y += 5;
            }

            //Snake movement

            if ((framesCounter % 15) == 0)
            {

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
            for (int i = 0; i < dripNums + buffNums; i++) //玩家水滴碰撞检测
            {
                if (fruit[i].active && CheckCollisionCircles(fruit[i].position, fruit[i].radius, snake.position, snake.radius))
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
                if (buff[i].active && CheckCollisionCircleRec(snake.position, snake.radius, buff[i].rec))
                {
                    snake.buff = buff[i].species;
                    buff[i].active = false;
                }
            }
            //Fruit position calculation
            int temp_Buff_num = GetRandomValue(0, 5);
            buffNums = buffNums < temp_Buff_num ? temp_Buff_num : buffNums; //新障碍物
            for (int i = 0; i < dripNums; i++)
            {
                if (!buff[i].active)
                {

                    buff[i].active = true;
                    buff[i].rec.x = GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2;
                    buff[i].rec.y = GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2;
                    buff[i].rec.width = GetRandomValue(10, 20),
                    buff[i].rec.height = (10, 20);
                }
                buff[i].species = GetRandomValue(1, 2) == 1 ? Dirt : Insect;
                // while (CheckCollisionCircles(fruit[i].position, 2*fruit[i].radius, snake.position, snake.radius))
                // {
                //     fruit[i].position = (Vector2){
                //         GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
                //         GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                // }
            }
        }

        int temp_num = GetRandomValue(0, 10);
        dripNums = dripNums < temp_num ? temp_num : dripNums; //新水滴
        for (int i = 0; i < dripNums; i++)
        {
            if (!fruit[i].active)
            {

                fruit[i].active = true;
                fruit[i].position = (Vector2){
                    GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
                    GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                fruit[i].radius = GetRandomValue(snake.radius / 2, snake.radius * 2);

                // while (CheckCollisionCircles(fruit[i].position, 2 * fruit[i].radius, snake.position, snake.radius))
                // {
                //     fruit[i].position = (Vector2){
                //         GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
                //         GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                // }
            }
        }

        //Collision
        for (int i = 0; i < dripNums; i++) //各种物体之间的碰撞检测
        {
            // if (CheckCollisionCircles(fruit[i].position, fruit[i].radius, snake.position, snake.radius))
            // {
            //     if (snake.radius > fruit[i].radius)
            //     {
            //         snake.radius = sqrt(pow(snake.radius, 2) + pow(fruit[i].radius, 2));
            //         fruit[i].active = false;
            //     }
            //     else
            //     {
            //         float old_snake_size = pow(snake.radius, 2);

            //         snake.radius = sqrt(pow(snake.radius, 2) - pow(snake.radius, 4) / pow(fruit[i].radius, 2));
            //         fruit[i].radius = sqrt(pow(fruit[i].radius, 2) + sqrt(old_snake_size - pow(snake.radius, 2)));
            //     }
            //     //snake[counterTail].position = snakePosition[counterTail - 1];
            //     //counterTail += 1;
            // }
            for (int j = 0; j < dripNums; j++)
            {
                if (i != j && fruit[i].active && fruit[j].active && CheckCollisionCircles(fruit[i].position, fruit[i].radius, fruit[j].position, fruit[j].radius))
                {
                    fruit[i].radius = sqrt(pow(fruit[i].radius, 2) + pow(fruit[j].radius, 2));
                    fruit[j].active = false;
                }
            }
            for (int j = 0; j < buffNums; j++)
            {
                if (fruit[i].active && buff[j].active && CheckCollisionCircleRec(fruit[i].position, fruit[i].radius, buff[j].rec))
                {
                    buff[j].active = false;
                }
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
