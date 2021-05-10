#include "game.h"

int
main(void)
{
    Game *g = new Game();
    InitAudioDevice(); //初始化音频设备
    InitWindow(g->screenWidth, g->screenHeight, "sample game: snake");
    g->InitGame();

#if defined(PLATFORM_WEB)

    //emscripten_set_main_loop(UpdateDrawFrame, 60, 1);

#else

    SetTargetFPS(60);

    //Main game loop
    //Detect window close button or ESC key
    while (!WindowShouldClose()) 
    {
        g->UpdateGame();
        g->DrawGame();
    }

#endif

    //Close window and OpenGL context
    CloseAudioDevice();//关闭音频设备
    CloseWindow();

    return 0;
}
