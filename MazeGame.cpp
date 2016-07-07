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

    private:
        static HDC myHDC = txLoadImage ("Resources/Ball.bmp");
        static HDC enemyHDC = txLoadImage ("Resources/EnemyBall.bmp");

        void updateDirection(){
            if(GetAsyncKeyState(keyUp)) dir = UP;
            if(GetAsyncKeyState(keyLeft)) dir = LEFT;
            if(GetAsyncKeyState(keyDown)) dir = DOWN;
            if(GetAsyncKeyState(keyRight)) dir = RIGHT;

            dir = NOWHERE;
        }

}



//PROTOTYPES

//END OF PROTOTYPES

void startServer(void*){
    Game game;
    game.start();
}

void startPeople(void*){
    Player p1("Resources/Ball.bmp", "Resources/EnemyBall.bmp", "Resources/Maze.bmp", 1, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);
    Player p2("Resources/Ball.bmp", "Resources/EnemyBall.bmp", "Resources/Maze.bmp", 2, VK_NUMPAD8, VK_NUMPAD2, VK_NUMPAD4, VK_NUMPAD6);
    p1.start();
    p2.start();
}

int main(){
    _beginthread(startServer, 0, NULL);
    Sleep(100);
    _beginthread(startPeople, 0, NULL);
    Sleep(100000);
}

