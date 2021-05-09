#ifndef __SNAKE__
#define __SNAKE__

#include "raylib.h"
#include <math.h>
#include <string>
#include <random>
using namespace std;

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif




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
    Vector2 size;
    bool active;
    Color color;
    float radius;
    Vector2 position;
} Food;

typedef struct Rain {
    Vector2 position;
    float radius;
    Vector2 speed;
    Color color;
    bool active;
} Rain;

typedef struct Buff{
    Obstacle species;
    Rectangle rec;
    bool active;
    Color color;
    float radius;
} Buff;

class Game {
public:
    static const int screenWidth = 1200;
    static const int screenHeight = 800;
    static const int MaxWidth = 4800;
    static const int MaxHeight = 3200;
    static const int SQUARE_SIZE = 40;

    static const int leftx = -MaxWidth/2+screenWidth/2;
    static const int rightx = MaxWidth/2+screenWidth/2;
    static const int topy = -MaxHeight/2+screenHeight/2;
    static const int bottomy = MaxHeight/2+screenHeight/2;
    int cleftx=0;
    int crightx=0;
    int ctopy=0;
    int cbottomy=0;
    int CameraWidth=0;
    int CameraHeight=0;


    static const int EDGECOUNT = 8;
    static const int G = 1;
    static const int A = 1;
    static const int MAXSPEED = 5;
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
    int mark = 0;
    //Num = 0 Wind Probabilty = 0.1
    //Num = 1 Sunny Probability = 0.2


    Vector2 snakePosition;
    bool allowMove;
    Vector2 offset;
    int counterTail;
    
    int RAIN_NUM = 10;
    int DRIP_MAX_NUMS = 256;
    int BUFF_MAX_NUMS = 256;

    int difficult_level = 0;
    int BASIC_SIZE = 10;
    int BASIC_SPEED = 20;
    int diff_rate = 1;

    int mycount=0;



    vector<Food> fruit;
    vector<Buff> buff;
    vector<Rain> rain;

    Snake snake;


    Color rain_color = GOLD;

    
    Camera2D camera;

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
    void cameramove(void);
    void drawtext(void);
    Vector2 getcameralt(void);
    void drawgrid(void);
    void drawbuff(void);
    void init_Rain(void);
    void init_Fruit(void);
    void updatecamera(void);
};

#endif
