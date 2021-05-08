#ifndef __SNAKE__
#define __SNAKE__

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif
#define DRIP_MAX_NUMS  256
#define SNAKE_LENGTH   256
#define SQUARE_SIZE     31

typedef struct Snake {//
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
    float radius;
} Snake;

typedef struct Food {
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
    float radius;
} Food;



class Game {
public:
    static const int screenWidth = 800;
    static const int screenHeight = 450;

    int framesCounter;
    bool gameOver;
    bool pause;
    int dripNums;

    Food fruit[SNAKE_LENGTH];
    Snake snake;
    Vector2 snakePosition;
    bool allowMove;
    Vector2 offset;
    int counterTail;

    Camera camera;

    Game();
    ~Game();

    void InitGame(void);         // Initialize game
    void UpdateGame(void);       // Update game (one frame)
    void DrawGame(void);         // Draw game (one frame)
    void UnloadGame(void);       // Unload game
    void UpdateDrawFrame(void);  // Update and Draw (one frame)
};

#endif
