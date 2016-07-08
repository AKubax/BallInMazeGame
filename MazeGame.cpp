#include "TXLib.h"
#include <cstdio>
#include <thread>

using std::cout;



enum Direction{
    NOWHERE = 0, UP, DOWN, LEFT, RIGHT
};

enum Status{
    LOST = 0, WON, RACING
};

struct State{                      //Just for sending data
    volatile int ball1X = 0, ball1Y = 0;
    volatile int ball2X = 0, ball2Y = 0;
    volatile Direction dir1 = NOWHERE, dir2 = NOWHERE;
    volatile Status status1 = RACING, status2 = RACING;
};

State state;

class Ball{
    public:
        int x = 0, y = 0;
        Direction dir = NOWHERE;

        void draw(){
            txTransparentBlt (txDC(), x - 14, y - 14, 28, 28, myHDC, 0, 0, TX_WHITE);
        }

    protected:
        HDC myHDC = txLoadImage ("Resources/Ball.bmp");
};

class PlayerBall : public Ball{
    public:
        Direction dir = NOWHERE;

        void updateDirection(){
            if(GetAsyncKeyState(keyUp)) dir = UP;
            if(GetAsyncKeyState(keyLeft)) dir = LEFT;
            if(GetAsyncKeyState(keyDown)) dir = DOWN;
            if(GetAsyncKeyState(keyRight)) dir = RIGHT;

            dir = NOWHERE;
        }
}


class GameManager{
    Ball balls[2];


}



//PROTOTYPES

//END OF PROTOTYPES


int main(){
    txCreateWindow (800, 450);
    Ball b;
    b.x = 400;
    b.y = 225;
    b.draw();
}

