#include "game.h"
//音效播放
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
        resource.bgmTime = resource.PlayBGM(resource.bgmMusic, resource.bgmTime);//游戏未停止播放背景音乐        
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
            const char sunny_message[128] = "What a sunny day!";
            DrawText(sunny_message, 400, 10, 40, RED);
            //播放晴朗音效
            resource.sunTime = resource.PlaySoundContinus(resource.sunSound, resource.sunTime);
        }
        //默认其他时刻为雨天播放雨天音效
        if (!Event_active)
            resource.rainTime = resource.PlaySoundContinus(resource.rainSound,resource.rainTime);
        //Draw snake

            DrawCircleV(snake.position, snake.radius, snake.color);

        //Draw fruit to pick
        for(int i=0;i<dripNums;i++){
            if(fruit[i].active)
                DrawCircleV(fruit[i].position, fruit[i].radius, fruit[i].color);
        }
        //Draw rain as obstacles
        for (int i = 0; i < RAIN_NUM; i++)
        {
            DrawCircleV(rain[i].position, sqrt(rain[i].size), rain[i].color);
        }
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

