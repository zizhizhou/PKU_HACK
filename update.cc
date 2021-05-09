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
            int speed = 5;
            if (snake.buff == Dirt)
            {
                snake.buff = Normal;
                speed = 1;
                snake.buffTime = 100;
            }
            else if (snake.buffTime > 0)
            {
                snake.buffTime -= 1;
                speed = 1;
            }
            //Player control
            double camereLenth = 625;
            Vector3 rule;
            if (IsKeyDown(KEY_RIGHT) && allowMove)
            {
                camera->up.z+=0.1;
                camera->up.y+=0.1;
                UpdateCamera(camera);
                // double tmp_rand1 = asin(camera.position.z / 15);
                // camera.position.z = 15 * sin(tmp_rand1 + 0.01);
                // camera.position.x = sqrt(camereLenth - pow(camera.position.y, 2)) * cos(tmp_rand1 + 0.01);
                //snake.position.x += speed;
            }
            if (IsKeyDown(KEY_LEFT) && allowMove)
            {
                 camera->up.z-=0.1;
                camera->up.y-=0.1;
                UpdateCamera(camera);
                // double tmp_rand = atan(camera.position.z / camera.position.x);
                // camera.position.z = sqrt(camereLenth - pow(camera.position.y, 2)) * sin(tmp_rand - 0.01);
                // camera.position.x = sqrt(camereLenth - pow(camera.position.y, 2)) * cos(tmp_rand - 0.01);
                //snake.position.x -= speed;
            }
            if (IsKeyDown(KEY_UP) && allowMove)
            {
                camera->up.x+=0.1;
                UpdateCamera(camera);
                //camera->up.z+=0.1;
                // double tmp_rand = atan(camera->position.y / camera->position.x);

                // camera->position.y = sqrt(camereLenth - pow(camera->position.z, 2)) * sin(tmp_rand - 0.01);
                // camera->position.x = sqrt(camereLenth - pow(camera->position.z, 2)) * cos(tmp_rand - 0.01);
                // //snake.position.y -= speed;
                // rule.x=camera->position.x;

            }
            if (IsKeyDown(KEY_DOWN) && allowMove)
            {
                camera->up.x-=0.1;
                UpdateCamera(camera);
                //camera->up.z-=0.1;
                // double tmp_rand = atan(camera->position.y / camera->position.x);

                // camera->position.y = sqrt(camereLenth - pow(camera->position.z, 2)) * sin(tmp_rand + 0.01);
                // camera->position.x = sqrt(camereLenth - pow(camera->position.z, 2)) * cos(tmp_rand + 0.01);
            }
            
            //Snake movement
            //Wall behaviour

            // if ((snake.position.x) > 10 ||
            //     (snake.position.z) > 10 ||
            //     (snake.position.x < -10) || (snake.position.z < -10))
            // {
            //     snake.position.y -= 0.5;
            //     if (snake.position.y < -10)
            //         gameOver = true;
            // }
            // else{
            //     snake.position =
            //     Vector3{snake.position.x + fabs(camera->position.x - 15) * (camera->position.y - 20.0) / 50, 0.0,
            //             snake.position.z + fabs(camera->position.z - 0.0) * (camera->position.y - 0) / 50};
            // }
            if ((framesCounter % 15) == 0)
            {

                snake.radius = snake.radius < 0.5 ? 0.5 : snake.radius * 0.99;
                for (int i = 0; i < dripNums; i++)
                {
                    fruit[i].radius *= 0.98;

                    if (fruit[i].radius < snake.radius / 5)
                    {
                        fruit[i].active = false;
                    }
                }
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
                if (fruit[i].active && CheckCollisionCircles(fruit[i].position, fruit[i].radius, Vector2{snake.position.x, snake.position.y}, snake.radius))
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
                if (buff[i].active && CheckCollisionCircleRec(Vector2{snake.position.x, snake.position.y}, snake.radius, buff[i].rec))
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
    //camera.fovy = 45.0f - (framesCounter * 0.05f);
}
