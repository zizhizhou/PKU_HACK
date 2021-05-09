#include "game.h"
#define DROPCHANGETIME 60   //变化时间
/*需要添加两个有注释的变量，每次更新物体大小时改变nextSize，而非size
在触发事件后，等待

*/
typedef struct Drop{
    vector2 position;
    Color color;
    int size;
    int nextSize;//将变化的大小
    int changeRestTime;  //变化剩余时间
    Drop(){
        nextSize = size;
        changeRestTime = DROPCHANGETIME;
    };
};
//水滴变化
void DropChange(Drop* drop){
        if(drop->nextSize == 0){    //被吸收
            drop->color.a = drop->changeRestTime/DROPCHANGETIME * a;//透明
            drop->size += (1 - drop->changeRestTime/DROPCHANGETIME) //变小 
                            * (drop->nextSize - drop->size);
            drop->changeRestTime--; //变化剩余时间减少
        }
        else{   //主动吸收
            drop->size += (1 - drop->changeRestTime/DROPCHANGETIME) //变大 
                            * (drop->nextSize - drop->size);
            drop->changeRestTime--; //变化剩余时间减少
        }
}
//绘制水滴，返回值为是否结束变化
bool DrawDrop(Drop* drop){
    if(drop->size != drop->nextSize)    //与下一时刻大小不同，需要变化
        DropChange(drop);
    DrawCircleV(drop->position, drop->size, drop->color);   //绘制水滴
    if (drop->changeRestTime <= 0){ //变化剩余时间为0,变化结束返回真，并重置计时
        drop->changeRestTime = DROPCHANGETIME;
        return true;
    }
    else 
        return false;
}