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
            if (IsKeyPressed(KEY_RIGHT) && (snake.speed.x == 0) && allowMove)
            {
                snake.speed = (Vector2){SQUARE_SIZE, 0};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && (snake.speed.x == 0) && allowMove)
            {
                snake.speed = (Vector2){-SQUARE_SIZE, 0};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_UP) && (snake.speed.y == 0) && allowMove)
            {
                snake.speed = (Vector2){0, -SQUARE_SIZE};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && (snake.speed.y == 0) && allowMove)
            {
                snake.speed = (Vector2){0, SQUARE_SIZE};
                allowMove = false;
            }

            //Snake movement

            if ((framesCounter % 15) == 0)
            {

                snake.position.x += snake.speed.x;
                snake.position.y += snake.speed.y;
                allowMove = true;
                fruit.radius*=0.98;
                snake.radius=snake.radius<8?8:snake.radius*0.99;
                if(fruit.radius<snake.radius/5){
                    fruit.active=false;
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

            //Fruit position calculation
            if (!fruit.active)
            {
                fruit.active = true;
                fruit.position = (Vector2){
                    GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
                    GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                fruit.radius = GetRandomValue(snake.radius / 2, snake.radius * 2);

                while ((fruit.position.x == snake.position.x) &&
                       (fruit.position.y == snake.position.y))
                {
                    fruit.position = (Vector2){
                        GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2,
                        GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                }
            }

        }

        //Collision
        if ((snake.position.x < (fruit.position.x + fruit.radius) &&
             (snake.position.x + snake.radius) > fruit.position.x) &&
            (snake.position.y < (fruit.position.y + fruit.radius) &&
             (snake.position.y + snake.radius) > fruit.position.y))
        {
            if(snake.radius>fruit.radius){
                snake.radius = sqrt(pow(snake.radius, 2) + pow(fruit.radius, 2));
                fruit.active = false;
            }
            else{
                float old_snake_size=pow(snake.radius,2);
                
                snake.radius = sqrt(pow(snake.radius,2)-pow(snake.radius,4)/pow(fruit.radius,2));
                fruit.radius=sqrt(pow(fruit.radius,2)+sqrt(old_snake_size-pow(snake.radius,2)));
            }
            //snake[counterTail].position = snakePosition[counterTail - 1];
            //counterTail += 1;
            
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
