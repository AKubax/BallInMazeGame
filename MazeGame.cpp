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

class Game{
    public:
        void start(){
            init();
            gameCycle();
        }


    private:
        int ball1X, ball1Y;
        int ball2X, ball2Y;

        Direction dir1, dir2;

        const int step = 1;

        const int startBallCenterX = 716, startBallCenterY = 879;

        bool ifBall1Out (){
            if (txGetPixel(ball1X - 14, ball1Y) == TX_WHITE) return true;
            if (txGetPixel(ball1X + 14, ball1Y) == TX_WHITE) return true;
            if (txGetPixel(ball1X, ball1Y - 12) == TX_WHITE) return true;
            if (txGetPixel(ball1X, ball1Y + 14) == TX_WHITE) return true;

            return false;
        }

        bool ifBall1AtGoal(){
            if(ball1Y < 5) return true;

            return false;
        }

        bool ifBall2Out (){
            if (txGetPixel(ball2X - 16, ball2Y) == TX_WHITE) return true;
            if (txGetPixel(ball2X + 14, ball2Y) == TX_WHITE) return true;
            if (txGetPixel(ball2X, ball2Y - 16) == TX_WHITE) return true;
            if (txGetPixel(ball2X, ball2Y + 14) == TX_WHITE) return true;

            return false;
        }

        bool ifBall2AtGoal(){
            if(ball2Y < 5) return true;

            return false;
        }

        void win(const int id){
            if(id==1){
                state.status1 = WON;
                state.status2 = LOST;
            }
            else{
                state.status1 = LOST;
                state.status2 = WON;
            }
        }

        void recieveDirection(){
            dir1 = state.dir1;
            dir2 = state.dir2;
        }

        void updateCoor(){
            switch (dir1){
                case UP:      ball1Y -= step;     break;
                case DOWN:    ball1Y += step;     break;
                case LEFT:    ball1X -= step;     break;
                case RIGHT:   ball1X += step;     break;
                case NOWHERE: break;
                default:      assert (!"Invalid dir1");      break;
            }

            switch (dir2){
                case UP:      ball2Y -= step;     break;
                case DOWN:    ball2Y += step;     break;
                case LEFT:    ball2X -= step;     break;
                case RIGHT:   ball2X += step;     break;
                case NOWHERE: break;
                default:      assert (!"Invalid dir2");       break;
            }

        }

        void sendCoor(){
            state.ball1X = ball1X;
            state.ball1Y = ball1Y;
            state.ball2X = ball2X;
            state.ball2Y = ball2Y;
        }

        void init(){
            ball1X = startBallCenterX;
            ball2X = startBallCenterX;
            ball1Y = startBallCenterY;
            ball2Y = startBallCenterY;
            sendCoor();
            txCreateWindow (1600, 900);
        }

        void gameCycle(){
            while(1){
                recieveDirection();
                updateCoor();
                sendCoor();

                cout << "1st coor   " << state.ball1X << ":" << state.ball1Y << "\n" ;
                cout << "2st coor   " << state.ball2X << ":" << state.ball2Y << "\n" ;

                if(ifBall1Out()){ win(2);    break; }
                if(ifBall2Out()){ win(1);    break; }
                if(ifBall1AtGoal()){ win(1); break; }
                if(ifBall2AtGoal()){ win(2); break; }
            }
        }

};

class Player{
    public:

        Player(const char* ballFileName, const char* enemyBallFileName, const char* mazeFileName, int id, int up, int down, int left, int right) :

            ballHDC (txLoadImage (ballFileName)),
            mazeHDC (txLoadImage (mazeFileName)),
            enemyBallHDC (txLoadImage (enemyBallFileName)),
            id (id),
            keyUp (up),
            keyDown (down),
            keyLeft (left),
            keyRight (right)
        {

        }

        ~Player(){
            txDeleteDC (ballHDC);
            txDeleteDC (mazeHDC);
            txDeleteDC (enemyBallHDC);
        }

        void start(){
            init();
            gameCycle();
        }

    private:
        HDC mazeHDC;
        HDC ballHDC;
        HDC enemyBallHDC;

        int keyUp, keyDown, keyLeft, keyRight;

        int id;

        int ballX = 0, ballY = 0;
        int enemyBallX = 0, enemyBallY = 0;

        void sendDirection(){
            if(id == 1) state.dir1 = getDirection();
            else state.dir2 = getDirection();
        }

        void getDataFromServer(){
            if(id == 1){
                ballX = state.ball1X;
                ballY = state.ball1Y;

                enemyBallX = state.ball2X;
                enemyBallY = state.ball2Y;
            }

            else{
                enemyBallX = state.ball1X;
                enemyBallY = state.ball1Y;

                ballX = state.ball2X;
                ballY = state.ball2Y;
            }
        }

        bool ifWon(){
            if(id == 1){
                if(state.status1 == WON) return true;
                else return false;
            }
            else{
                if(state.status2 == WON) return true;
                else return false;
            }
        }

        bool ifLost(){
            if(id == 1){
                if(state.status1 == LOST) return true;
                else return false;
            }
            else{
                if(state.status2 == LOST) return true;
                else return false;
            }
        }

        void drawMaze(){
            txBitBlt (txDC(), 0, 0, 1600, 900, mazeHDC, 0, 0);
        }

        void drawBall(int x, int y){
            txTransparentBlt (txDC(), x - 14, y - 14, 28, 28, ballHDC, 0, 0, TX_WHITE);
        }

        void drawEnemyBall(int x, int y){
            txTransparentBlt (txDC(), x - 14, y - 14, 28, 28, enemyBallHDC, 0, 0, TX_WHITE);
        }

        Direction getDirection(){
            if(GetAsyncKeyState(keyUp)) return UP;
            if(GetAsyncKeyState(keyLeft)) return LEFT;
            if(GetAsyncKeyState(keyDown)) return DOWN;
            if(GetAsyncKeyState(keyRight)) return RIGHT;

            return NOWHERE;
        }

        void win(){
            txClear();
            txSetColor (TX_RED);
            txTextOut(800, 450, "YOU WIN YEAAAAAAAAAAH PARTY HARD");
            txSleep(5000);
        }

        void lose(){
            txClear();
            txSetColor (TX_RED);
            txTextOut(800, 450, "YOU LOSE AHAHA");
            txSleep(5000);
        }

        void init(){
            drawMaze();

            getDataFromServer();
            drawBall(ballX, ballY);
        }

        void gameCycle(){
            while(1){
                txBegin();

                drawMaze ();
                drawBall (ballX, ballY);
                drawEnemyBall(enemyBallX, enemyBallY);

                if(ifLost()){
                    lose();
                    break;
                }
                if(ifWon()){
                    win();
                    break;
                }

                sendDirection();
                getDataFromServer();

                txSleep(0);

                txEnd();
            }
        }
};

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

