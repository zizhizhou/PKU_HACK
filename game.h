#ifndef __SNAKE__
#define __SNAKE__

#include "raylib.h"
#include <math.h>
#include <string>
#include <random>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define SNAKE_LENGTH   256
#define SQUARE_SIZE     31



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

//游戏资源,在game类中实例化resource
class GameResource{
public:
    char *dropSound = "Sounds/Drop.wav";//水滴合并
    char *windSound = "Sounds/Wind.wav";//风声
    char *rainSound = "Sounds/Rain.wav";//雨声
    char *sunSound = "Sounds/Sun.wav";//晴天声
    char *breakSound = "Sounds/DropBreak.wav";//水滴破裂
    char *endSound = "Sounds/End.wav";//游戏结束
    char *bgmMusic = "Sounds/Music.wav";//背景音乐
    int windTime;//风声时间
    int rainTime;//雨声时间
    int sunTime;//晴天声时间
    int bgmTime;//背景音乐时间
    int PlaySoundContinus(char* filename, int playtime);//连续播放，如天气效果，仅在render中该天气部分添加
                                                        //playtime = PlaySoundContinus(char* filename, int playtime)
    int PlayBGM(char* filename, int playtime);//背景音乐播放，在init和render中添加
                                              //playtime = PlayBGM(char* filename, int playtime)
    void PlaySoundInstant(char* filename);//即时播放，在update中触发时添加
    GameResource(){
        windTime = 0;
        rainTime = 0;
        sunTime = 0;
        bgmTime = 0;
    };
    ~GameResource(){};

};

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

    int RAIN_NUM = 10;
    int DRIP_MAX_NUMS = 256;
    int BUFF_MAX_NUMS = 256;

    int difficult_level = 0;
    int BASIC_SIZE = 10;
    int BASIC_SPEED = 20;
    int diff_rate = 1;

    //Define Events
    bool Event_active;
    int Event_time;
    Event Weather[10];
    int acc_rate;
    int mark = 0;
    //Num = 0 Wind Probabilty = 0.1
    //Num = 1 Sunny Probability = 0.2

    std::vector<Food> fruit;
    std::vector<Buff> buff;
    std::vector<Rain> rain;
    Snake snake;
    Vector2 snakePosition;
    bool allowMove;
    Vector2 offset;
    int counterTail;

    Color rain_color = GOLD;

    Camera camera;
    GameResource resource;//游戏资源


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
    void init_Rain(void);
    void init_Fruit(void);
};

#endif
