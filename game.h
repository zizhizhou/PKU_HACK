#ifndef __DRIP__
#define __DRIP__

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

//Drip is controled by the player
typedef struct Drip{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Vector2 acc;
    Color color;
    Obstacle buff;
    float radius;
} Drip;


typedef struct Food {
    Vector2 size;
    bool active;
    Color color;
    float radius;
    Vector2 position;
} Food;

//Drip should go away from Rain
typedef struct Rain {
    Vector2 position;
    float radius;
    Vector2 speed;
    Color color;
    bool active;
} Rain;

//Buff is some obstacles
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
    char *BackgroundTexture = "Images/BackgroundTexture.png";//Background 
    char *dropSound = "Sounds/Drop.wav";//Combine
    char *windSound = "Sounds/Wind.wav";//Wind
    char *rainSound = "Sounds/Rain.wav";//Rain
    char *sunSound = "Sounds/Sun.wav";//Sunny
    char *hotSound = "Sounds/Hot.wav";//Hot
    char *breakSound = "Sounds/DropBreak.wav";//Break
    char *endSound = "Sounds/End.wav";//End
    char *bgmMusic = "Sounds/Music.mp3";//BGM
    
    //Define sound time
    int windTime;
    int rainTime;
    int sunTime;
    int hotTime;
    int bgmTime;
    int PlaySoundContinus(char* filename, int playtime, int totaltime);//Play continously                                              
    void PlaySoundInstant(char* filename);//Play instantly
    void DrawBackground(Texture2D texture, Rectangle target, int state);
    GameResource(){
        windTime = 0;
        rainTime = 0;
        sunTime = 0;
        hotTime = 0;
        bgmTime = 0;
    };
    ~GameResource(){};
};
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

    //Define Score: score = time + eated_food_size ** 2 * 0.5
    int score;
    int framesCounter;
    bool gameOver;
    bool pause;
    int foodNums;
    int buffNums;

    //Define Weather Event
    bool Event_active;
    int Event_time;
    Event Weather[10];
    int acc_rate;
    int mark = 0;

    Vector2 offset;
    
    //Define items number
    int RAIN_NUM = 10;
    int DRIP_MAX_NUMS = 256;
    int BUFF_MAX_NUMS = 256;

    //Define difficulty
    int difficult_level = 0;
    int BASIC_SIZE = 10;
    int BASIC_SPEED = 20;
    int diff_rate = 1;

    int mycount=0;

    vector<Food> food;
    vector<Buff> buff;
    vector<Rain> rain;

    Drip drip;

    Color rain_color = GRAY;
    
    Camera2D camera;

    GameResource resource;
    Texture2D backgroundTexture;

    Game();
    ~Game();

    void InitGame(void);         // Initialize game
    void UpdateGame(void);       // Update game (one frame)
    void DrawGame(void);         // Draw game (one frame)
    void UnloadGame(void);       // Unload game
    void UpdateDrawFrame(void);  // Update and Draw (one frame)

    void calcedge(Vector2 center,float radius,Vector2 speed);
    void drawdrop(void);
    void updatedrip(void);
    void cameramove(void);
    void drawtext(void);
    Vector2 getcameralt(void);
    void drawgrid(void);
    void drawbuff(void);
    void init_Rain(void);       //Initialize Rain position and size
    void init_Food(void);       //Initialize Food position and size
    float ComputeCollision(float r1,float r2,float* great, float* small);       //Compute size when drip meet food
    void updatecamera(void);
};

#endif
