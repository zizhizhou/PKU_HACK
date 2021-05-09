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

    int score;

    int framesCounter;
    bool gameOver;
    bool pause;
    int dripNums;
    int buffNums;

    GameResource resource;//游戏资源

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
};

#endif
