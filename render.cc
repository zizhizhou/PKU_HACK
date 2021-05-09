#include "game.h"
#include <iostream>
using namespace std;

vector<Vector2>edges(Game::EDGECOUNT+1);
vector<Vector2>controls(Game::EDGECOUNT);
int keypoint=-1;



//Draw game per frame...
void Game::DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    this->cameramove();


    BeginMode2D(camera);

    if (!gameOver)
    {
        //Draw grid lines
        drawgrid();
        
        

        //Draw drops
        drawdrop();


        //Draw buffs
        drawbuff();

        //Draw text
        drawtext();
        

        if (pause) 
        {
            DrawText("GAME PAUSED", camera.target.x-MeasureText("GAME PAUSED",40/camera.zoom)/2, camera.target.y-40/camera.zoom, 40/camera.zoom, GRAY);
        }
        
    }
    else
    {
        DrawText("PRESS [ENTER] TO PLAY AGAIN", camera.target.x-MeasureText("PRESS [ENTER] TO PLAY AGAIN",40/camera.zoom)/2, camera.target.y-40/camera.zoom, 40/camera.zoom, GRAY);
    }

    EndMode2D();
    EndDrawing();
}


void Game::drawdrop(){



    for(int i=0;i<dripNums;i++){
        if(!fruit[i].active)continue;
        if(fruit[i].radius<=5)continue;
        //if(abs(fruit[i].position.x-snake.position.x)>(camera.offset.x/camera.zoom)||abs(fruit[i].position.y-snake.position.y)>(camera.offset.y/camera.zoom))continue;
        this->calcedge(fruit[i].position, fruit[i].radius, (Vector2){0,0});
        for(int j=0;j<EDGECOUNT;j++)DrawLineBezierQuad(edges[j],edges[j+1],controls[j],fruit[i].radius/5,fruit[i].color);
        if(FILLING)DrawCircleV(fruit[i].position,fruit[i].radius,(Color){0,121,241,100});
    }

    for (int i = 0; i < RAIN_NUM; i++){
        if(rain[i].radius<=5)continue;

        if(abs(rain[i].position.x-snake.position.x)>(camera.offset.x/camera.zoom)||abs(rain[i].position.y-snake.position.y)>(camera.offset.y/camera.zoom))continue;

        this->calcedge(rain[i].position,rain[i].radius,rain[i].speed);
        
        DrawCircleV(rain[i].position,rain[i].radius,(Color){230,41,55,100});
        if(FILLING){
            if(keypoint!=-1){
                if(keypoint==0){
                    DrawTriangle(edges[0],edges[1],edges[EDGECOUNT-1],WHITE);
                    DrawTriangle(edges[0],edges[1],edges[EDGECOUNT-1],(Color){230,41,55,100});
                }
                else{
                    DrawTriangle(edges[keypoint],edges[keypoint+1],edges[keypoint-1],WHITE);
                    DrawTriangle(edges[keypoint],edges[keypoint+1],edges[keypoint-1],(Color){230,41,55,100});
                } 
            }
        }
        for(int j=0;j<EDGECOUNT;j++)DrawLineBezierQuad(edges[j],edges[j+1],controls[j],rain[i].radius/5,rain[i].color);
    }

    this->calcedge(snake.position,snake.radius,snake.speed);
    
    if(FILLING){
        DrawCircleV(snake.position,snake.radius,(Color){0,82,172,100});
        if(keypoint!=-1){
            if(keypoint==0){
                DrawTriangle(edges[0],edges[1],edges[EDGECOUNT-1],WHITE);
                DrawTriangle(edges[0],edges[1],edges[EDGECOUNT-1],(Color){0,82,172,100});
            }
            else{
                DrawTriangle(edges[keypoint],edges[keypoint+1],edges[keypoint-1],WHITE);
                DrawTriangle(edges[keypoint],edges[keypoint+1],edges[keypoint-1],(Color){0,82,172,100});
            } 
        }
    }

    
    for(int i=0;i<EDGECOUNT;i++)DrawLineBezierQuad(edges[i],edges[i+1],controls[i],snake.radius/5,snake.color);


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

void Game::cameramove(void){
 
    camera.zoom += ((float)GetMouseWheelMove()*0.05f);
    if(snake.radius>=0.5*SQUARE_SIZE&&camera.zoom>=2.0f)camera.zoom/=1.01f;
    if(snake.radius>=1.5*SQUARE_SIZE&&camera.zoom>=1.0f)camera.zoom/=1.01f;
    if(snake.radius<1.5*SQUARE_SIZE&&camera.zoom<=2.0f)camera.zoom*=1.01f;
    if(snake.radius<0.5*SQUARE_SIZE&&camera.zoom<=4.0f)camera.zoom*=1.01f;
    

    if (camera.zoom > 3.0f) camera.zoom = 3.0f;
    else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

    // Camera reset (zoom and rotation)
    if (IsKeyPressed(KEY_R))
    {
        camera.zoom = 0.25f;
        camera.rotation = 0.0f;
    }

    if(snake.position.x-camera.offset.x/camera.zoom>=leftx&&snake.position.x+camera.offset.x/camera.zoom<=rightx)camera.target.x=snake.position.x;
    if(snake.position.y-camera.offset.y/camera.zoom>=topy&&snake.position.y+camera.offset.y/camera.zoom<=bottomy)camera.target.y=snake.position.y;
}

void Game::drawtext(){

    Vector2 lt=getcameralt();
    string temp_score = "SCORE="+to_string(score);
    DrawText(temp_score.c_str(), lt.x, lt.y, 40/camera.zoom, BLACK);

    

    if (Weather[0].active && Event_active)
    {   
        DrawText("Wind is coming!", lt.x+camera.offset.x*2/camera.zoom-MeasureText("Wind is coming!",40/camera.zoom)-40/camera.zoom, lt.y, 40/camera.zoom, DARKBLUE);
    }
    if (Weather[1].active && Event_active)
    {
        DrawText("It's so hot!", lt.x+camera.offset.x*2/camera.zoom-MeasureText("It's so hot!",40/camera.zoom)-40/camera.zoom, lt.y, 40/camera.zoom, RED);
    }
    if (Weather[2].active && Event_active)
    {
        DrawText("What a sunny day!", lt.x+camera.offset.x*2/camera.zoom-MeasureText("What a sunny day!",40/camera.zoom)-40/camera.zoom, lt.y, 40/camera.zoom, RED);
    }
}

Vector2 Game::getcameralt(void){
    Vector2 lt;
    lt.x=camera.target.x-camera.offset.x/camera.zoom;
    lt.y=camera.target.y-camera.offset.y/camera.zoom;
    return lt;
}


void Game::drawgrid(void){
    for (int i = 0; i < MaxWidth/SQUARE_SIZE + 1; i++)
    {
        Vector2 p1 = {SQUARE_SIZE*i+leftx, topy};
        Vector2 p2 = {SQUARE_SIZE*i+leftx , bottomy};
        DrawLineV(p1, p2, LIGHTGRAY);
    }

    for (int i = 0; i < MaxHeight/SQUARE_SIZE + 1; i++)
    {
        Vector2 p1 = {leftx, SQUARE_SIZE*i+topy};
        Vector2 p2 = {rightx, SQUARE_SIZE*i+topy };
        DrawLineV(p1, p2, LIGHTGRAY);
    }
}

void Game::drawbuff(void){
    for(int i=0;i<buffNums;i++){
        if(buff[i].active)
            DrawRectangleRec(buff[i].rec,buff[i].color);
    }
}