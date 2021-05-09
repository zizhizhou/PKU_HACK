#include "game.h"
#include<bits/stdc++.h>
using namespace std;
//Update game per frame...
void Game::UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P'))
            pause = !pause;
        if (!pause)
        {
            updatesnake();


            int num;

            if(Event_active == false)
            {    
                //Random Event 
                num = rand()%(1000);;
                if(framesCounter - mark > 100)
                {    
                    if (num == 0)
                    {
                        Event_active = true;
                        Weather[0].active = true;
                    }
                    if (num == 1)
                    {
                        Event_active = true;
                        Weather[1].active = true;
                    }
                    if (num == 2)
                    {
                        Event_active = true;
                        Weather[2].active = true;
                        init_Rain();
                    }
                }
                if(Event_active)
                {
                    mark = framesCounter;
                }
            }

            


            if( Weather[0].active == true)
            {   
                int direction = rand()/RAND_MAX > 0.5 ? 1 : -1;
                for (int i = 0;i < RAIN_NUM;i++)
                {
                    rain[i].speed.x = BASIC_SPEED * 0.5 * direction;
                }
            }

            if( Weather[1].active == true)
            {
                acc_rate = 1.1;
            }


            //Snake movement

            if ((framesCounter % 15) == 0)
            {
                if(Event_active)
                {
                    Event_time += 1;

                }
                if(Event_time == 10)
                {
                    mark = framesCounter;
                    Event_active = false;
                    Event_time = 0;
                    if(Weather[2].active == true)
                    {
                        Weather[2].active = false;
                        init_Rain();
                    }
                    for (int i = 0;i < 10;i++)
                    {
                        Weather[i].active = false;
                    }
                }
  
                snake.radius = snake.radius * 0.99 * acc_rate * diff_rate;
                snake.radius = snake.radius > 15 ? snake.radius : 15;
                for (int i = 0; i < dripNums; i++)
                {
                    fruit[i].radius *= (0.97 * acc_rate * diff_rate);

                    if (fruit[i].radius < snake.radius / 3)
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
            
            //Rain movement
            if ((framesCounter%15) == 0)
            {
                for (int i = 0;i < RAIN_NUM ;i++)
                {
                    rain[i].position.y += rain[i].speed.y;
                    rain[i].position.x += rain[i].speed.x;
                }
            }


            //Rain
            for(int i  = 0; i < RAIN_NUM; i++)
            {
                if (CheckCollisionCircles(rain[i].position, rain[i].radius,snake.position,snake.radius))
                {
                    resource.PlaySoundInstant(resource.breakSound);//触发破坏音效
                    float alpha = sqrt((1 - (rain[i].speed.y*rain[i].radius/(4*BASIC_SIZE*BASIC_SPEED))));
                    if(rain[i].speed.y > 0)
                    {
                        snake.radius *= (0.99*alpha);
                    }
                    else if(rain[i].speed.y < 0)
                    {
                        snake.radius *= (1 - (1 - alpha)*0.99);
                    }
                    else
                    {
                        snake.radius *= alpha;
                    }
                    
                }    
            }
            for (int i = 0; i < dripNums + buffNums; i++) //玩家水滴碰撞检测
            {
                if (fruit[i].active && CheckCollisionCircles(fruit[i].position, fruit[i].radius, snake.position, snake.radius))
                {
                    if (snake.radius > fruit[i].radius)
                    {
                        resource.PlaySoundInstant(resource.dropSound);//触发吞噬音效
                        snake.radius = sqrt(pow(snake.radius, 2) + pow(fruit[i].radius, 2));
                        fruit[i].active = false;
                        score += fruit[i].radius*fruit[i].radius;
                        snake.radius = snake.radius < 30 ? snake.radius : 30;
                    }
                    else
                    {
                        resource.PlaySoundInstant(resource.breakSound);//触发破坏音效
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
                    snake.radius *= 0.85;
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
            }
        
            //Rain position Caculation
            for (int i = 0; i < RAIN_NUM;i++)
            {
                if(rain[i].position.y > screenHeight|| rain[i].position.x > screenWidth)
                {
                    rain[i].radius = BASIC_SIZE * 2 * double(GetRandomValue(1,10))/double(10) ;
                    rain[i].speed = (Vector2){ 0, double(GetRandomValue(2,10))/double(10) * BASIC_SIZE * 2};
                    rain[i].position = (Vector2){GetRandomValue(0, (screenWidth/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.x/2,  -20};
                }
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
                fruit[i].radius = GetRandomValue(snake.radius / 2, snake.radius * 1.5);
                fruit[i].radius = fruit[i].radius < 30 ? fruit[i].radius : 30;

            }
            else{
                if(fruit[i].radius<=5)fruit[i].active=false;
            }
        }

        //Collision
        for (int i = 0; i < dripNums; i++) //各种物体之间的碰撞检测
        {
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
        for (int i = 0; i < RAIN_NUM; i++)
        {
            for (int j = 0; j< RAIN_NUM; j++)
            {
                if(i != j && rain[i].active && rain[j].active && CheckCollisionCircles(rain[i].position,rain[i].radius,rain[j].position,rain[j].radius))
                {
                    rain[i].radius = sqrt(pow(rain[i].radius, 2) + pow(rain[j].radius, 2));
                    rain[j].position =  (Vector2){GetRandomValue(0, (screenWidth/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.x/2,  -50};
                }
            }
        }
         if (score > 1000 && score < 3000 && difficult_level == 0)
        {
            difficult_level = 1;
            BASIC_SIZE *= 1.3;
            rain_color = YELLOW;
        }
        if (score > 3000 && score < 5000 && difficult_level == 1)
        {
            difficult_level = 2;
            RAIN_NUM *= 1.2;
            rain_color = ORANGE;
        }
        if (score > 5000 && difficult_level == 2)
        {
            difficult_level = 3;
            diff_rate = 0.95;
            rain_color = RED;
        }

        for(int i = 0;i < RAIN_NUM;i++)
        {
            rain[i].color = rain_color;
        }

        if (snake.radius < 10 && score > 10)
            gameOver = true;
        if (snake.radius < 5)
            gameOver = true;
        framesCounter++;
        if(framesCounter % 5 ==0)
        {
            if(Event_active)
            {
                score += 5;
            }
            else
            {
                score++;
            }
        }
    }
    
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            framesCounter = 0;
            difficult_level = 0;
            Event_active = 0;
            init_Rain();
            init_Fruit();
            diff_rate = 1;
            score = 0;
            InitGame();
            gameOver = false;
    
        }
    }
    camera.fovy = 45.0f - (framesCounter * 0.05f);
}


void Game::updatesnake(void){


    snake.acc.x=0;snake.acc.y=0;
    if(snake.speed.x>0)snake.acc.x=-G;
    else if(snake.speed.x<0) snake.acc.x=G;
    if(snake.speed.y>0)snake.acc.y=-G;
    else if(snake.speed.y<0) snake.acc.y=G;
    

    //Player control
    if (IsKeyDown(KEY_RIGHT))
    {
        snake.acc = (Vector2){ A,0};
    }
    if (IsKeyDown(KEY_LEFT))
    {
        snake.acc = (Vector2){ -A, 0 };
    }
    if (IsKeyDown(KEY_UP))
    {
        snake.acc = (Vector2){ 0, -A };
    }
    if (IsKeyDown(KEY_DOWN))
    {
        snake.acc = (Vector2){ 0, A };
    }

    
    if(snake.speed.x>0&&snake.speed.x+snake.acc.x<0){
        snake.speed.x=0;
    }
    else if(snake.speed.x<0&&snake.speed.x+snake.acc.x>0){
        snake.speed.x=0;
    }
    else if (snake.speed.x+snake.acc.x>MAXSPEED){
        snake.speed.x=MAXSPEED;
    }
    else if(snake.speed.x+snake.acc.x<-MAXSPEED){
        snake.speed.x=-MAXSPEED;
    }
    else{
        snake.speed.x+=snake.acc.x;
    }

    if(snake.speed.y>0&&snake.speed.y+snake.acc.y<0){
        snake.speed.y=0;
    }
    else if(snake.speed.y<0&&snake.speed.y+snake.acc.y>0){
        snake.speed.y=0;
    }
    else if(snake.speed.y+snake.acc.y>MAXSPEED){
        snake.speed.y=MAXSPEED;
    }
    else if(snake.speed.y+snake.acc.y<-MAXSPEED){
        snake.speed.y=-MAXSPEED;
    }
    else{
        snake.speed.y+=snake.acc.y;
    }



    snake.position.x += snake.speed.x;
    snake.position.y += snake.speed.y;

}

void Game::init_Rain(void){
    for (int i = 0;i < RAIN_NUM;i++)
    {
        rain[i].radius = BASIC_SIZE * 2 * double(GetRandomValue(1,10))/double(10) ;
        rain[i].speed = (Vector2){ 0, (!Weather[2].active)*double(GetRandomValue(2,10))/double(10) * BASIC_SIZE * 2};
        rain[i].position = (Vector2){GetRandomValue(0, (screenWidth/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.x/2,  -50};
    }
}

void Game::init_Fruit(void){
    for (int i = 0;i < dripNums; i++)
    {
        fruit[i].active = false;
    }
}