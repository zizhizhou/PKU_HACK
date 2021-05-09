#include "game.h"
#include <iostream>
using namespace std;

vector<Vector2>edges(Game::EDGECOUNT+1);
vector<Vector2>controls(Game::EDGECOUNT);
int keypoint=-1;

int GameResource::PlaySoundContinus(char *filename, int playtime)
{
    if ((playtime%150) == 0){
        Sound s = LoadSound(filename);
        PlaySound(s);
    }
    playtime++;
    return playtime;    
}
int GameResource::PlayBGM(char *filename, int playtime)
{
    if ((playtime%18300) == 0){
        Sound s = LoadSound(filename);
        PlaySound(s);
    }
    playtime++;
    return playtime;    
}
void GameResource::PlaySoundInstant(char *filename)
{
        Sound s = LoadSound(filename);
        PlaySound(s);
}



//Draw game per frame...
void Game::DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!gameOver)
    {
        //Draw grid lines
        for (int i = 0; i < screenWidth/SQUARE_SIZE + 1; i++)
        {
            Vector2 p1 = {SQUARE_SIZE*i + offset.x/2, offset.y/2};
            Vector2 p2 = {SQUARE_SIZE*i + offset.x/2, screenHeight - offset.y/2};
            DrawLineV(p1, p2, LIGHTGRAY);
        }

        for (int i = 0; i < screenHeight/SQUARE_SIZE + 1; i++)
        {
            Vector2 p1 = {offset.x/2, SQUARE_SIZE*i + offset.y/2};
            Vector2 p2 = {screenWidth - offset.x/2, SQUARE_SIZE*i + offset.y/2};
            DrawLineV(p1, p2, LIGHTGRAY);
        }
        char text_score[128] = "SCORE=";
        std::string temp_text = std::to_string(score);
        for (int i = 0;i < sizeof(temp_text);i++)
        {
            text_score[i+6]=temp_text[i];
        }
        DrawText(text_score, 10, 10, 40, BLACK);

        if (Weather[0].active && Event_active)
        {   const char wind_message[128] = "Wind is coming!";
            DrawText(wind_message, 500, 10, 40, DARKBLUE);
            //播放风声
            resource.windTime = resource.PlaySoundContinus(resource.windSound, resource.windTime); 
        }
        if (Weather[1].active && Event_active)
        {
            const char hot_message[128] = "It's so hot!";
            DrawText(hot_message, 400, 10, 40, DARKPURPLE);
        }
        if (Weather[2].active && Event_active)
        {
            const char sunny_message[128] = "What a sunny day!";
            DrawText(sunny_message, 400, 10, 40, RED);
            //播放晴朗音效
            resource.sunTime = resource.PlaySoundContinus(resource.sunSound, resource.sunTime);
        }
        //默认其他时刻为雨天播放雨天音效
        if (!Event_active)
            resource.rainTime = resource.PlaySoundContinus(resource.rainSound,resource.rainTime);

        //Draw drops
        drawdrop();
        

        if (pause) 
        {
            char msgPause[128] = "GAME PAUSED";
            float x = screenWidth/2 - MeasureText(msgPause, 40)/2;
            float y = screenHeight/2 - 40;
            DrawText(msgPause, x, y, 40, GRAY);
        }
        for(int i=0;i<buffNums;i++){
            if(buff[i].active)
                DrawRectangleRec(buff[i].rec,buff[i].color);
        }

        if (pause) 
        {
            char msgPause[128] = "GAME PAUSED";
            float x = screenWidth/2 - MeasureText(msgPause, 40)/2;
            float y = screenHeight/2 - 40;
            DrawText(msgPause, x, y, 40, GRAY);
        }
    }
    else
    {
        char msgAgain[128] = "PRESS [ENTER] TO PLAY AGAIN";
        float x = GetScreenWidth()/2 - MeasureText(msgAgain, 20)/2;
        float y = GetScreenHeight()/2 - 50;
        DrawText(msgAgain, x, y, 20, GRAY);
    }


    EndDrawing();
}


void Game::drawdrop(){

    this->calcedge(snake.position,snake.radius,snake.speed);
    for(int i=0;i<EDGECOUNT;i++)DrawLineBezierQuad(edges[i],edges[i+1],controls[i],snake.radius/5,snake.color);
    if(FILLING){
        DrawCircleV(snake.position,snake.radius,(Color){0,82,172,100});
        if(keypoint!=-1){
            if(keypoint==0)DrawTriangle(edges[0],edges[1],edges[EDGECOUNT-1],(Color){0,82,172,100});
            else DrawTriangle(edges[keypoint],edges[keypoint+1],edges[keypoint-1],(Color){0,82,172,100});
        }
    }


    for(int i=0;i<dripNums;i++){
        if(!fruit[i].active)continue;
        this->calcedge(fruit[i].position, fruit[i].radius, (Vector2){0,0});
        for(int j=0;j<EDGECOUNT;j++)DrawLineBezierQuad(edges[j],edges[j+1],controls[j],fruit[i].radius/5,fruit[i].color);
        if(FILLING)DrawCircleV(fruit[i].position,fruit[i].radius,(Color){0,121,241,100});
    }

    for (int i = 0; i < RAIN_NUM; i++){

        this->calcedge(rain[i].position,rain[i].radius,rain[i].speed);
        for(int j=0;j<EDGECOUNT;j++)DrawLineBezierQuad(edges[j],edges[j+1],controls[j],rain[i].radius/5,rain[i].color);
        DrawCircleV(rain[i].position,rain[i].radius,(Color){230,41,55,100});
        if(FILLING){
            if(keypoint!=-1){
                if(keypoint==0)DrawTriangle(edges[0],edges[1],edges[EDGECOUNT-1],(Color){230,41,55,100});
                else DrawTriangle(edges[keypoint],edges[keypoint+1],edges[keypoint-1],(Color){230,41,55,100});
            }
        }
    }
}

void Game::calcedge(Vector2 center,float radius,Vector2 speed){


    //计算边缘顶点位置
    for(int i=0;i<=EDGECOUNT;i++){
        Vector2 pos={center.x+radius*cos(PI/4*i),center.y-radius*sin(PI/4*i)};
        edges[i]=pos;
    }


    keypoint=-1;
    if(speed.x>0&&speed.y>0){edges[3].x-=min(speed.x,radius)/2;edges[3].y-=min(speed.y,radius)/2;keypoint=3;}
    else if(speed.x>0&&speed.y<0){edges[5].x-=min(speed.x,radius)/2;edges[5].y-=max(speed.y,-radius)/2;keypoint=5;}
    else if(speed.x<0&&speed.y>0){edges[1].x-=max(speed.x,-radius)/2;edges[1].y-=min(speed.y,radius)/2;keypoint=1;}
    else if(speed.x<0&&speed.y<0){edges[7].x-=max(speed.x,-radius)/2;edges[7].y-=max(speed.y,-radius)/2;keypoint=7;}
    else if(speed.x>0){edges[4].x-=min(speed.x,radius);keypoint=4;}
    else if(speed.x<0){edges[0].x-=max(speed.x,-radius);edges[EDGECOUNT].x-=max(speed.x,-radius);keypoint=0;}
    else if(speed.y>0){edges[2].y-=min(speed.y,radius);keypoint=2;}
    else if(speed.y<0){edges[6].y-=max(speed.y,-radius);keypoint=6;}
    


    //计算贝塞尔控制点

    int error_count=0;
    while(1){
        float a=GetRandomValue(60,120)/180.0*PI;
        float l=(rand()%30+20)/100.0*radius;
        controls[0]=(Vector2){edges[0].x+l*cos(a),edges[0].y-l*sin(a)};


        for(int i=1;i<EDGECOUNT-1;i++){
            float k=(controls[i-1].y-edges[i].y)/(edges[i].x-controls[i-1].x);
            float dx=(rand()%30+20)/100.0*radius;
            float dy=dx*k;

            if(abs(dy)>radius/2){
                dy=dy>0? radius/2:-radius/2;
                dx=dy/k;
            }

            if(controls[i-1].x>edges[i].x){
                controls[i]=(Vector2){edges[i].x-dx,edges[i].y+dy};   
            }
            else{
                controls[i]=(Vector2){edges[i].x+dx,edges[i].y-dy};
            }
        }
        float k0=-(controls[0].y-edges[0].y)/(edges[0].x-controls[0].x);
        float b0=edges[0].y-k0*edges[0].x;
        float kl=-(controls[EDGECOUNT-2].y-edges[EDGECOUNT-1].y)/(edges[EDGECOUNT-1].x-controls[EDGECOUNT-2].x);
        float bl=edges[EDGECOUNT-1].y-kl*edges[EDGECOUNT-1].x;


        float xl=(bl-b0)/(k0-kl);
        float yl=k0*xl+b0;
        controls[EDGECOUNT-1]=(Vector2){xl,yl};

        if(abs(xl-edges[0].x)<=radius/2&&abs(yl-edges[0].y)<=radius/2)break;

        if((abs(xl-edges[0].x)>radius/2||abs(yl-edges[0].y)>radius/2)&&error_count>3){
            float dx=(rand()%30+20)/100.0*radius;
            float dy=dx*kl;
            if(abs(dy)>radius/2){
                dy=dy>0? radius/2:-radius/2;
                dx=dy/kl;
            }
            if(controls[EDGECOUNT-2].x>edges[EDGECOUNT-1].x){
                controls[EDGECOUNT-1]=(Vector2){edges[EDGECOUNT-1].x-dx,edges[EDGECOUNT-1].y+dy};   
            }
            else{
                controls[EDGECOUNT-1]=(Vector2){edges[EDGECOUNT-1].x+dx,edges[EDGECOUNT-1].y-dy};
            }
            break;
        }
        
        error_count++;

    }    
}

