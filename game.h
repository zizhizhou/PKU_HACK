#ifndef __SNAKE__
#define __SNAKE__

#include "raylib.h"
#include <math.h>
#include <string>
#include <random>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif
#define DRIP_MAX_NUMS  256
#define BUFF_MAX_NUMS  256
#define SNAKE_LENGTH   256
#define SQUARE_SIZE     31
#define RAIN_NUM        10

typedef struct Event{
    bool active;
    int prob;   
}Event;
enum Obstacle{Normal,Dirt,Insect};
typedef struct Snake {//
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Vector2 acc;
    Color color;
    Obstacle buff;
    float radius;
} Snake;

typedef struct Food {
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
    float radius;
} Food;

typedef struct Rain {
    Vector2 position;
    float size;
    float radius;
    Vector2 speed;
    Color color;
} Rain;

typedef struct Buff{
    Obstacle species;
    Rectangle rec;
    bool active;
    Color color;
    float radius;
} Bff;

class Game {
public:
    static const int screenWidth = 800;
    static const int screenHeight = 450;

    static const int EDGECOUNT = 8;
    static const int G = 5;
    static const int A = 2;
    static const int MAXSPEED = 20;
    static const int MINSIZE = 5;
    static const bool FILLING = true;

    int score;

    int framesCounter;
    bool gameOver;
    bool pause;
    int dripNums;
    int buffNums;

    //Define Events
    bool Event_active;
    int Event_time;
    Event Weather[10];
    int acc_rate;
    //Num = 0 Wind Probabilty = 0.1
    //Num = 1 Sunny Probability = 0.2

    Food fruit[DRIP_MAX_NUMS];
    Buff buff[BUFF_MAX_NUMS];
    Rain rain[RAIN_NUM];
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

    void calcedge(Vector2 center,float radius,Vector2 speed);
    void drawdrop(void);
    void updatesnake(void);
};

#endif
