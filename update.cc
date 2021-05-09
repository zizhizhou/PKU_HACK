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
            updatecamera();
            updatedrip();

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
                acc_rate = 0.95;
            }


            //drip movement

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
                    if(Weather[1].active = false)
                    {
                        Weather[1].active = false;
                        acc_rate = 1;
                    }
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
  
                drip.radius = drip.radius * 0.99 * acc_rate * diff_rate;
                if(drip.radius < 5)
                {
                    resource.PlaySoundInstant(resource.endSound);
                    gameOver = true;
                }
                for (int i = 0; i < foodNums; i++)
                {
                    food[i].radius *= (0.97 * acc_rate );
                }
            }

            //Wall behaviour
            if (((drip.position.x) > rightx) ||
                ((drip.position.y) > bottomy) ||
                (drip.position.x < leftx) || (drip.position.y < topy))
            {
                resource.PlaySoundInstant(resource.endSound);
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
                if (CheckCollisionCircles(rain[i].position, rain[i].radius,drip.position,drip.radius))
                {
                    resource.PlaySoundInstant(resource.breakSound);
                    float alpha = sqrt((1 - (rain[i].speed.y*rain[i].radius/(4*BASIC_SIZE*BASIC_SPEED))));
                    //define aplha = sqrt((1-(rain_speed*rain_radius)/(MAX_SPEED*MAX_SIZE))
                    
                    //Collision direction different, loss different
                    if(rain[i].speed.y > 0)
                    {
                        drip.radius *= (0.99*alpha);
                    }
                    else if(rain[i].speed.y < 0)
                    {
                        drip.radius *= (1 - (1 - alpha)*0.99);
                    }
                    else
                    {
                        drip.radius *= alpha;
                    }
                    
                }    
            }
            for (int i = 0; i < foodNums + buffNums; i++) //Check drip and food collision
            {
                if (food[i].active && CheckCollisionCircles(food[i].position, food[i].radius, drip.position, drip.radius))
                {
                    if (drip.radius > food[i].radius)
                    {
                        resource.PlaySoundInstant(resource.dropSound);
                        drip.radius = sqrt(pow(drip.radius, 2) + pow(food[i].radius, 2));
                        food[i].active = false;
                        score += food[i].radius*food[i].radius*0.5;
                        drip.radius = drip.radius < 30 ? drip.radius : 30;
                    }
                    else
                    {
                        resource.PlaySoundInstant(resource.breakSound);
                        float temp_r1, temp_r2;
                        ComputeCollision(food[i].radius, drip.radius, &temp_r1, &temp_r2);
                        drip.radius = temp_r2;
                        food[i].radius = temp_r1;
                        if (drip.radius < 5)
                        {
                            gameOver = true;
                            resource.PlaySoundInstant(resource.endSound);
                        }
                    }
                }
                if (buff[i].active && CheckCollisionCircleRec(drip.position, drip.radius, buff[i].rec))
                {
                    resource.PlaySoundInstant(resource.breakSound);
                    drip.buff = buff[i].species;
                    drip.radius *= 0.85;
                    buff[i].active = false;
                }
            }


            //Buff position calculation
            int temp_Buff_num = GetRandomValue(0, 5);
            buffNums = buffNums < temp_Buff_num ? temp_Buff_num : buffNums; //New Obstacles
            for (int i = 0; i < foodNums; i++)
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
                if((rain[i].position.y >cbottomy+CameraHeight/2)|| (rain[i].position.x > crightx+CameraWidth/2))
                {
                    rain[i].radius = BASIC_SIZE * 2 * double(GetRandomValue(1,10))/double(10) ;
                    rain[i].speed = (Vector2){ 0, double(GetRandomValue(2,10))/double(10) * BASIC_SIZE * 2};
                    rain[i].position = (Vector2){GetRandomValue(0, (2*CameraWidth/SQUARE_SIZE) - 1)*SQUARE_SIZE-CameraWidth/2+cleftx, ctopy-CameraHeight/2};
                }
            }

        }

        //food position calculation

        int temp_num = GetRandomValue(0, 10);
        foodNums = foodNums < temp_num ? temp_num : foodNums; //New food
        for (int i = 0; i < foodNums; i++)
        {
            if (!food[i].active)
            {

                food[i].active = true;
                food[i].position = (Vector2){
                    GetRandomValue(0, (2*CameraWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE-CameraWidth/2 + cleftx,
                    GetRandomValue(0, (2*CameraHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE-CameraHeight/2 + ctopy};
                food[i].radius = GetRandomValue(drip.radius / 2, drip.radius * 1.5);
                food[i].radius = food[i].radius < 40 ? food[i].radius : 40;

            }
            else{
                if(food[i].radius<=5)food[i].active=false;
                if(food[i].position.x<cleftx-CameraWidth/2||food[i].position.x>crightx+CameraWidth/2||food[i].position.y<ctopy-CameraHeight/2||food[i].position.y>cbottomy+CameraHeight/2){
                    food[i].active=false;
                }
            }
        }

        for (int i = 0; i < foodNums; i++) //Check conflict between items
        {
            for (int j = 0; j < foodNums; j++)
            {
                if (i != j && food[i].active && food[j].active && CheckCollisionCircles(food[i].position, food[i].radius, food[j].position, food[j].radius))
                {
                    food[i].radius = sqrt(pow(food[i].radius, 2) + pow(food[j].radius, 2));
                    food[j].active = false;

                }
            }
            for (int j = 0; j < buffNums; j++)
            {
                if (food[i].active && buff[j].active && CheckCollisionCircleRec(food[i].position, food[i].radius, buff[j].rec))
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

        //Update difficult level  (remain to be improved)
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

        //Avoid death at birth
        if (drip.radius < 5 && score > 10)
        {    
            gameOver = true;
            resource.PlaySoundInstant(resource.endSound);
    
        framesCounter++;
        
        if(framesCounter % 5 ==0)
        {
            //Event active, additional buff
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
            init_Food();
            diff_rate = 1;
            score = 0;
            InitGame();
            gameOver = false;
            BASIC_SIZE=10;
    
        }
    }
}


void Game::updatedrip(void){


    drip.acc.x=0;drip.acc.y=0;
    if(drip.speed.x>0)drip.acc.x=-G;
    else if(drip.speed.x<0) drip.acc.x=G;
    if(drip.speed.y>0)drip.acc.y=-G;
    else if(drip.speed.y<0) drip.acc.y=G;
    

    //Player control
    if (IsKeyDown(KEY_RIGHT))
    {
        drip.acc = (Vector2){ A,0};
    }
    if (IsKeyDown(KEY_LEFT))
    {
        drip.acc = (Vector2){ -A, 0 };
    }
    if (IsKeyDown(KEY_UP))
    {
        drip.acc = (Vector2){ 0, -A };
    }
    if (IsKeyDown(KEY_DOWN))
    {
        drip.acc = (Vector2){ 0, A };
    }

    
    if(drip.speed.x>0&&drip.speed.x+drip.acc.x<0){
        drip.speed.x=0;
    }
    else if(drip.speed.x<0&&drip.speed.x+drip.acc.x>0){
        drip.speed.x=0;
    }
    else if (drip.speed.x+drip.acc.x>MAXSPEED){
        drip.speed.x=MAXSPEED;
    }
    else if(drip.speed.x+drip.acc.x<-MAXSPEED){
        drip.speed.x=-MAXSPEED;
    }
    else{
        drip.speed.x+=drip.acc.x;
    }

    if(drip.speed.y>0&&drip.speed.y+drip.acc.y<0){
        drip.speed.y=0;
    }
    else if(drip.speed.y<0&&drip.speed.y+drip.acc.y>0){
        drip.speed.y=0;
    }
    else if(drip.speed.y+drip.acc.y>MAXSPEED){
        drip.speed.y=MAXSPEED;
    }
    else if(drip.speed.y+drip.acc.y<-MAXSPEED){
        drip.speed.y=-MAXSPEED;
    }
    else{
        drip.speed.y+=drip.acc.y;
    }

    drip.position.x += drip.speed.x;
    drip.position.y += drip.speed.y;

}

void Game::init_Rain(void){
    for (int i = 0;i < RAIN_NUM;i++)
    {
        rain[i].radius = BASIC_SIZE * 2 * double(GetRandomValue(1,10))/double(10) ;
        rain[i].speed = (Vector2){ 0, (!Weather[2].active)*double(GetRandomValue(2,10))/double(10) * BASIC_SIZE * 2};
        rain[i].position = (Vector2){GetRandomValue(0, (2*camera.offset.x/SQUARE_SIZE) - 1)*SQUARE_SIZE+cleftx, ctopy-SQUARE_SIZE};
    }
}

void Game::init_Food(void){
    for (int i = 0;i < foodNums; i++)
    {
        food[i].active = false;
    }
}

void Game::updatecamera(void){
    cleftx=camera.target.x-camera.offset.x/camera.zoom;
    crightx=camera.target.x+camera.offset.x/camera.zoom;
    ctopy=camera.target.y-camera.offset.y/camera.zoom;
    cbottomy=camera.target.y+camera.offset.y/camera.zoom;
    CameraWidth=crightx-cleftx;
    CameraHeight=cbottomy-ctopy;
}

float Game::ComputeCollision(float r1, float r2, float* great, float* small)
{
    //r_min = sqrt(r_min**2*(1-alpha)),r_max = sqrt(r_min**2*alpha + r_max**2)
    //Set alpha = 0.92 here
    //Confirm r1**2 + r2**2 = Constant
    if(r1 < r2)
    {
        float temp_r1 = r1;
        r1 = sqrt(r1 * r1 * 0.92);
        r2 = sqrt(temp_r1 * temp_r1 * 0.08 + r2 * r2); 
        *great = r2;
        *small = r1;
    }
    else
    {
        float temp_r2 = r2;
        r2 = sqrt(r2 * r2 * 0.92);
        r1 = sqrt(r1 * r1 + 0.08 * temp_r2 * temp_r2);
        *great = r1;
        *small = r2; 
    }
    
}