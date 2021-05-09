#include "game.h"

//Draw game per frame...
void Game::DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!gameOver)
    {
        //Draw grid lines



            //DrawCircleV(snake.position, snake.radius, snake.color);

        //Draw fruit to pick
        for(int i=0;i<dripNums;i++){
            if(fruit[i].active)
                DrawCircleV(fruit[i].position, fruit[i].radius, fruit[i].color);
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

    BeginMode3D(*camera);
    // for (int i = 0; i < screenWidth/SQUARE_SIZE + 1; i++)
    // {
    //     Vector3 p1 = {SQUARE_SIZE*i + offset.x/2, offset.y/2,0.0};
    //     Vector3 p2 = {SQUARE_SIZE*i + offset.x/2, screenHeight - offset.y/2,0.0};
    //     DrawLine3D(p1, p2, LIGHTGRAY);
    // }

    // for (int i = 0; i < screenHeight/SQUARE_SIZE + 1; i++)
    // {
    //     Vector3 p1 = {offset.x/2, SQUARE_SIZE*i + offset.y/2,0.0};
    //     Vector3 p2 = {screenWidth - offset.x/2, SQUARE_SIZE*i + offset.y/2,0.0};
    //     DrawLine3D(p1, p2, LIGHTGRAY);
    // }
    //Draw snake
    DrawSphere(snake.position,snake.radius, snake.color);
    //DrawCircle3D(snake.position,snake.radius,Vector3{0,1,0},90,snake.color);
    DrawGrid(20, 1.0f);
    //DrawCube((Vector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, RED);
    // DrawCubeWires((Vector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, GOLD);
    // DrawCubeWires((Vector3){-4.0f, 0.0f, -2.0f}, 3.0f, 6.0f, 2.0f, MAROON);
    // DrawSphere((Vector3){-1.0f, 0.0f, -2.0f}, 1.0f, GREEN);
    // DrawSphereWires((Vector3){1.0f, 0.0f, 2.0f}, 2.0f, 16, 16, LIME);
    // DrawCylinder((Vector3){4.0f, 0.0f, -2.0f}, 1.0f, 2.0f, 3.0f, 4, SKYBLUE);
    // DrawCylinderWires((Vector3){4.0f, 0.0f, -2.0f}, 1.0f, 2.0f, 3.0f, 4, DARKBLUE);
    // DrawCylinderWires((Vector3){4.5f, -1.0f, 2.0f}, 1.0f, 1.0f, 2.0f, 6, BROWN);
    // DrawCylinder((Vector3){1.0f, 0.0f, -4.0f}, 0.0f, 1.5f, 3.0f, 8, GOLD);
    // DrawCylinderWires((Vector3){1.0f, 0.0f, -4.0f}, 0.0f, 1.5f, 3.0f, 8, PINK);
    // DrawGrid(10, 1.0f); //Draw a grid
    EndMode3D();
    EndDrawing();
}

