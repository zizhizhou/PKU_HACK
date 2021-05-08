#include "game.h"

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

        //Draw snake

            DrawCircleV(snake.position, snake.radius, snake.color);

        //Draw fruit to pick
        DrawCircleV(fruit.position, fruit.radius, fruit.color);

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

