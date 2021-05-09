#ifndef __SNAKE__
#define __SNAKE__

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif
#define DRIP_MAX_NUMS  256
#define BUFF_MAX_NUMS  256
#define SNAKE_LENGTH   256
#define SQUARE_SIZE     31
enum Obstacle{Normal,Dirt,Insect};
typedef struct Drip {//
    Vector3 position;
    Vector2 size;
    Vector3 speed;
    Color color;
    Obstacle buff;
    int buffTime;
    float radius;
} Drip;

typedef struct Water {
    Vector3 position;
    Vector2 size;
    bool active;
    Color color;
    float radius;
} Water;

typedef struct Buff{
    Vector3 position;
    Obstacle species;
    Rectangle rec;
    bool active;
    Color color;
    float radius;
} Buff;

class Game {
public:
    static const int screenWidth = 800;
    static const int screenHeight = 450;

    int framesCounter;
    bool gameOver;
    bool pause;
    int dripNums;
    int buffNums;

    Water water[DRIP_MAX_NUMS];
    Buff buff[BUFF_MAX_NUMS];
    Drip drip;
    Vector3 snakePosition;
    bool allowMove;
    Vector2 offset;
    int counterTail;

    Camera *camera=new Camera();

    Game();
    ~Game();

    void InitGame(void);         // Initialize game
    void UpdateGame(void);       // Update game (one frame)
    void DrawGame(void);         // Draw game (one frame)
    void UnloadGame(void);       // Unload game
    void UpdateDrawFrame(void);  // Update and Draw (one frame)
};

#endif
