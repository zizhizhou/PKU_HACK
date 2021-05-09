#include "game.h"
#include <math.h>
//Update game per frame...
#define MAX_SPEED 500
void Game::UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P'))
            pause = !pause;
        if (!pause)
        {
            int speed = 5;
            if (drip.buff == Dirt)
            {
                drip.buff = Normal;
                speed = 1;
                drip.buffTime = 100;
            }
            else if (drip.buffTime > 0)
            {
                drip.buffTime -= 1;
                speed = 1;
            }
            //Player control
            double camereLenth = 625;
            Vector3 rule;
            if (IsKeyDown(KEY_RIGHT) && allowMove)
            {
                double tmp_rand = atan(camera->position.z / camera->position.y);

                camera->position.z = sqrt(camereLenth - pow(camera->position.x, 2)) * sin(tmp_rand + 0.01);
                camera->position.y = sqrt(camereLenth - pow(camera->position.x, 2)) * cos(tmp_rand + 0.01);
                //camera.position.z
                // double tmp_rand1 = asin(camera.position.z / 15);
                // camera.position.z = 15 * sin(tmp_rand1 + 0.01);
                // camera.position.x = sqrt(camereLenth - pow(camera.position.y, 2)) * cos(tmp_rand1 + 0.01);
                //drip.position.x += speed;
            }
            if (IsKeyDown(KEY_LEFT) && allowMove)
            {
                double tmp_rand = atan(camera->position.z / camera->position.y);

                camera->position.z = sqrt(camereLenth - pow(camera->position.x, 2)) * sin(tmp_rand - 0.01);
                camera->position.y = sqrt(camereLenth - pow(camera->position.x, 2)) * cos(tmp_rand - 0.01);
                // double tmp_rand = atan(camera.position.z / camera.position.x);
                // camera.position.z = sqrt(camereLenth - pow(camera.position.y, 2)) * sin(tmp_rand - 0.01);
                // camera.position.x = sqrt(camereLenth - pow(camera.position.y, 2)) * cos(tmp_rand - 0.01);
                //drip.position.x -= speed;
            }
            if (IsKeyDown(KEY_UP) && allowMove)
            {
                //camera->up.z+=0.1;
                camera->fovy -= 0.1f;
                double tmp_rand = atan(camera->position.y / camera->position.x);

                camera->position.y = sqrt(camereLenth - pow(camera->position.z, 2)) * sin(tmp_rand - 0.01);
                camera->position.x = sqrt(camereLenth - pow(camera->position.z, 2)) * cos(tmp_rand - 0.01);
                // //drip.position.y -= speed;
                // rule.x=camera->position.x;
            }
            if (IsKeyDown(KEY_DOWN) && allowMove)
            {
                camera->fovy += 0.1f;
                //camera->up.z-=0.1;
                double tmp_rand = atan(camera->position.y / camera->position.x);

                camera->position.y = sqrt(camereLenth - pow(camera->position.z, 2)) * sin(tmp_rand + 0.01);
                camera->position.x = sqrt(camereLenth - pow(camera->position.z, 2)) * cos(tmp_rand + 0.01);
            }

            //Snake movement
            //Wall behaviour

            if ((drip.position.x) > 10 ||
                (drip.position.z) > 10 ||
                (drip.position.x < -10) || (drip.position.z < -10))
            {
                drip.position.x += 0.0001 * drip.speed.x;
                drip.position.y += 0.0001 * drip.speed.y;
                drip.position.z += 0.0001 * drip.speed.z;
                drip.position.y -= 0.5;
                if (drip.position.y < -10)
                    gameOver = true;
            }
            else
            {
                Vector3 acc = Vector3{20 * (20 - camera->position.x) / camera->position.y, 0.0f, -20 * (camera->position.z) / camera->position.y};
                if (drip.speed.x * acc.x < 0)
                {
                    acc.x *= 5;
                }
                if (drip.speed.y * acc.y < 0)
                {
                    acc.y *= 5;
                }
                if (drip.speed.z * acc.z < 0)
                {
                    acc.z *= 5;
                }
                if (drip.speed.x > MAX_SPEED)
                {
                    drip.speed.x = MAX_SPEED;
                }
                if (drip.speed.x < -MAX_SPEED)
                {
                    drip.speed.x = -MAX_SPEED;
                }
                if (drip.speed.y > MAX_SPEED)
                {
                    drip.speed.y = MAX_SPEED;
                }
                if (drip.speed.y < -MAX_SPEED)
                {
                    drip.speed.y = -MAX_SPEED;
                }
                if (drip.speed.z > MAX_SPEED)
                {
                    drip.speed.z = MAX_SPEED;
                }
                if (drip.speed.z < -MAX_SPEED)
                {
                    drip.speed.z = -MAX_SPEED;
                }

                drip.speed.x = drip.speed.x + acc.x;
                drip.speed.y = drip.speed.y + acc.y;
                drip.speed.z = drip.speed.z + acc.z;
                drip.position.x += 0.0001 * drip.speed.x;
                drip.position.y += 0.0001 * drip.speed.y;
                drip.position.z += 0.0001 * drip.speed.z;
            }
            if ((framesCounter % 15) == 0)
            {

                drip.radius = drip.radius < 0.5 ? 0.5 : drip.radius * 0.99;
                for (int i = 0; i < dripNums; i++)
                {
                    water[i].radius *= 0.98;

                    if (water[i].radius < drip.radius / 5)
                    {
                        water[i].active = false;
                    }
                }
            }

            /*             //Collision with yourself
            for (int i = 1; i < counterTail; i++)
            {
                if ((drip[0].position.x == drip[i].position.x) && 
                        (drip[0].position.y == drip[i].position.y)) 
                    gameOver = true;
            } */
            for (int i = 0; i < dripNums + buffNums; i++) //玩家水滴碰撞检测
            {
                if (water[i].active && CheckCollisionSpheres(water[i].position, water[i].radius, drip.position, drip.radius))
                {
                    if (drip.radius > water[i].radius)
                    {
                        drip.radius = sqrt(pow(drip.radius, 2) + pow(water[i].radius, 2));
                        water[i].active = false;
                    }
                    else
                    {
                        float old_drip_size = pow(drip.radius, 2);

                        drip.radius = sqrt(pow(drip.radius, 2) - pow(drip.radius, 4) / pow(water[i].radius, 2));
                        water[i].radius = sqrt(pow(water[i].radius, 2) + sqrt(old_drip_size - pow(drip.radius, 2)));
                    }
                    //drip[counterTail].position = dripPosition[counterTail - 1];
                    //counterTail += 1;
                }
                // if (buff[i].active && CheckCollisionCircleRec(Vector2{drip.position.x, drip.position.z}, drip.radius, buff[i].rec))
                // {
                //     drip.buff = buff[i].species;
                //     buff[i].active = false;
                // }
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
                // while (CheckCollisionCircles(water[i].position, 2*water[i].radius, drip.position, drip.radius))
                // {
                //     water[i].position = (Vector2){
                //         GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
                //         GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                // }
            }
        }

        int temp_num = GetRandomValue(0, 15);
        dripNums = dripNums < temp_num ? temp_num : dripNums; //新水滴
        for (int i = 0; i < dripNums; i++)
        {
            if (!water[i].active)
            {

                water[i].active = true;
                water[i].position = (Vector3){
                    GetRandomValue(-10, 10),
                    0.0,
                    GetRandomValue(-10, 10)};
                water[i].radius = GetRandomValue(drip.radius / 200, drip.radius * 110) / 100.0;

                // while (CheckCollisionCircles(water[i].position, 2 * water[i].radius, drip.position, drip.radius))
                // {
                //     water[i].position = (Vector2){
                //         GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
                //         GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                // }
            }
        }

        //Collision
        for (int i = 0; i < dripNums; i++) //各种物体之间的碰撞检测
        {
            // if (CheckCollisionCircles(water[i].position, water[i].radius, drip.position, drip.radius))
            // {
            //     if (drip.radius > water[i].radius)
            //     {
            //         drip.radius = sqrt(pow(drip.radius, 2) + pow(water[i].radius, 2));
            //         water[i].active = false;
            //     }
            //     else
            //     {
            //         float old_drip_size = pow(drip.radius, 2);

            //         drip.radius = sqrt(pow(drip.radius, 2) - pow(drip.radius, 4) / pow(water[i].radius, 2));
            //         water[i].radius = sqrt(pow(water[i].radius, 2) + sqrt(old_drip_size - pow(drip.radius, 2)));
            //     }
            //     //drip[counterTail].position = dripPosition[counterTail - 1];
            //     //counterTail += 1;
            // }
            for (int j = 0; j < dripNums; j++)
            {
                if (i != j && water[i].active && water[j].active && CheckCollisionSpheres(water[i].position, water[i].radius, water[j].position, water[j].radius))
                {
                    water[i].radius = sqrt(pow(water[i].radius, 2) + pow(water[j].radius, 2));
                    water[j].active = false;
                }
            }
            // for (int j = 0; j < buffNums; j++)
            // {
            //     if (water[i].active && buff[j].active && CheckCollisionCircleRec(water[i].position, water[i].radius, buff[j].rec))
            //     {
            //         buff[j].active = false;
            //     }
            // }
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
