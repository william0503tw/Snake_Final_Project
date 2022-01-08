#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <deque>
#include <stdio.h>
#include <time.h>
using namespace std;


/* 變數宣告 Variables declarations */
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

/* size of ground (include boundary) 50 x 25 */
const int x_size = 52;
#define y_size 27

enum {GROUND,FOOD,WALL,HEAD,BODY,OBSTACLE,SPECIAL_FOOD,PORTAL}; //index -> 0,1,2,3,4,5,6
enum {UP,DOWN,LEFT,RIGHT,INIT}; //direction -> 0,1,2,3,4
bool gameOver = false;
int direction = INIT;
int FOOD_COUNT = 3 ;
char answer;
bool obstacle_on = true;
bool obstacle_random = true;
bool playagain = false;


/* class definition */
class snake
{
public:

    //場地 ground
    void initPlayground();
    void drawGround();
    void indexObstacle(int y , int x_start , int x_finish);
    int ground[x_size][y_size];

    //遊戲邏輯 game logic
    void recreateFood();
    void addSpecialFood();
    void setScore(int s);
    void drawScore();

    //角色資訊 character info
    int x = 25 ,y = 12; //蛇頭的初始位置座標
    int onTouch(int x, int y); //判斷蛇頭的座標
    int score = 0;
    int length = 3;
    int skin;
    bool touchFood = false;
    deque<int> body_x = {25,25,25}; //init position
    deque<int> body_y = {13,14,15}; //init position
    int speed = 860; //990 full speeds
    int prev_x = 0 , prev_y = 0;
    char BODY_TYPE = 'A'; //蛇初始身體符號為A
    int BODY_COLOR ;  //蛇初始身體顏色為綠色

    void drawBody(); //畫出蛇
    void updateSnake(int direction); //更新蛇頭座標
};

/* Global Function Prototypes */
void printsnake();
void readInput(void* id);
void color(int color_number);
void resetColor();
void gotoXY(int x, int y);
void setWindowSize(int width, int height);
void ShowConsoleCursor(bool showFlag);
//void playagain();
void drawset();
//////////////////////////////





/* start of class function definition */
void snake::indexObstacle(int y , int x_start , int x_finish)
{
    for(int i = x_start ; i <= x_finish ; i++ ){
        ground[i][y] = OBSTACLE;
    }
}


//建立初始畫面的各個座標
void snake::initPlayground()
{
    //場地座標 infill GROUND
    for(int i = 1 ; i <= x_size-1 ; i++)
    {
        for(int j = 1 ; j <= y_size-1 ; j++)
        {
            ground[i][j] = GROUND;
        }
    }

    //牆壁座標 build WALL
    for(int i = 0 ; i <= y_size-1 ; i++)
    {
        ground[0][i] = WALL ;
    }
    for(int i = 0 ; i <= y_size-1 ; i++)
    {
        ground[51][i] = WALL ;
    }
    for(int i = 0 ; i <= x_size-1 ; i++)
    {
        ground[i][0] = WALL ;
    }
    for(int i = 0 ; i <= x_size-1 ; i++)
    {
        ground[i][26] = WALL ;
    }

    //不隨機障礙物座標
    if (obstacle_on == true && obstacle_random == false)
    {
        indexObstacle(5,6,13);    indexObstacle(6,6,8);     indexObstacle(7,4,8);
        indexObstacle(20,15,17);  indexObstacle(21,15,22);  indexObstacle(22,17,24);
        indexObstacle(3,44,46);   indexObstacle(4,38,46);   indexObstacle(5,38,40);
        indexObstacle(6,34,40);   indexObstacle(17,35,37);  indexObstacle(17,41,43);
        indexObstacle(18,36,36);  indexObstacle(18,42,42);  indexObstacle(20,35,35);
        indexObstacle(20,43,43);  indexObstacle(21,36,36);  indexObstacle(21,42,42);
        indexObstacle(22,37,41);  indexObstacle(13,12,17);  indexObstacle(14,10,19);  indexObstacle(15,12,17);
    }

    //隨機建立食物座標 food
    int temp ;
    srand((unsigned int)time(NULL));
    for(int i = 1 ; i <= FOOD_COUNT ; i++)
    {
        if(temp != rand())
        {
            int x_ran = rand() % (x_size - 2) + 1 ; // x = 1~50
            int y_ran = rand() % (y_size - 2) + 2 ; // y = 2~25
            if(ground[x_ran][y_ran] != OBSTACLE && ground[x_ran][y_ran] != WALL) //座標非障礙物且非牆壁
            {
                ground[x_ran][y_ran] = FOOD ;
            }
            else
            {
                ground[x_ran][y_ran] = OBSTACLE ;
                i--;
            }
        }else
        {
            srand(0);
            int x_ran = rand() % (x_size - 1) + 2 ;
            int y_ran = rand() % (y_size - 1) + 2 ;
            if(ground[x_ran][y_ran] != OBSTACLE && ground[x_ran][y_ran] != WALL){
                ground[x_ran][y_ran] = FOOD ;
            }else{
                ground[x_ran][y_ran] = OBSTACLE ;
                i--;
            }
        }
        temp = rand();
    }
}


//更新分數
void snake::setScore(int s)
{
    cout << "\b \b";        //清空分數
    gotoXY(20,-4);          //游標到分數的位置
    cout << score ;        //更新分數
    gotoXY(34,-4);         //游標到速度的位置
    cout << speed - 850;  //更新速度
    gotoXY(x,y);          //游標回到蛇頭位置
}


//畫出分數
void snake::drawScore()
{
    int i;
    if (skin == 1) i = 143; //深灰
    if (skin == 2) i = 42;  //綠色
    if (skin == 3) i = 25;  //藍色
    if (skin == 4) i = 'D'; //紅色
    cout << string(7,' '); color(i); cout << "///////////////////////////////////" << endl; resetColor();
    cout << string(7,' '); color(i); cout << "//"; resetColor(); cout << "                               "; color(i); cout << "//" << endl; resetColor();
    cout << string(7,' '); color(i); cout << "//"; resetColor(); cout << setw(11) << "Score: " << score << setw(13) << "Speed: " << setw(6) << left << (speed - 850); color(i); cout << "//" << endl; resetColor();
    cout << string(7,' '); color(i); cout << "//"; resetColor(); cout << "                               "; color(i); cout << "//" << endl; resetColor();
    cout << string(7,' '); color(i); cout << "///////////////////////////////////" << endl; resetColor();
    cout << endl;
}


//畫出場地
void snake::drawGround()
{
    system("CLS"); //清空屏幕
    drawScore();   //畫出分數

    //畫出牆壁、食物、障礙物
    for(int i = 0 ; i <= y_size-1 ; i++)
    {
        for(int j = 0 ; j <= x_size-1 ; j++)
        {
            if(ground[j][i] == WALL)
            {
                if (skin == 1) color(143); //深灰
                if (skin == 2) color(42);  //綠色
                if (skin == 3) color(25);  //藍色
                if (skin == 4) color('D'); //紅色
                cout << "-" ;
            }else if(ground[j][i] == FOOD)
            {
                if (skin == 1) color(8);   //灰色
                if (skin == 2) color(10);  //綠色
                if (skin == 3) color(9);   //藍色
                if (skin == 4) color(12);  //紅色
                cout << "&" ;
            }else if(ground[j][i] == OBSTACLE && obstacle_on == true && obstacle_random == false)
            {
                color(7);
                cout << "#";
            }else if(ground[j][i] == GROUND)
            {
                color(7);
                cout << " " ;
            }
        }
        cout << endl ;
    }


    //畫出傳送門
    int new_x = rand() % (x_size-4)+1 ; // x = 1~48
    int new_y = rand() % (y_size-4)+1 ; // y = 1~23

    if (skin == 1) color(120);  //白色
    if (skin == 2) color('g');  //黃色
    if (skin == 3) color('V');  //紫色
    if (skin == 4) color(42);   //綠色

    //左邊界上的傳送門
    ground[0][new_y] = PORTAL;
    ground[0][new_y + 1] = PORTAL;
    ground[0][new_y + 2] = PORTAL;
    ground[0][new_y + 3] = PORTAL;
    gotoXY(0, new_y);      cout << "O";
    gotoXY(0, new_y + 1);  cout << "O";
    gotoXY(0, new_y + 2);  cout << "O";
    gotoXY(0, new_y + 3);  cout << "O";

    //右邊界上的傳送門
    ground[51][new_y] = PORTAL;
    ground[51][new_y + 1] = PORTAL;
    ground[51][new_y + 2] = PORTAL;
    ground[51][new_y + 3] = PORTAL;
    gotoXY(51, new_y);     cout << "O";
    gotoXY(51, new_y + 1); cout << "O";
    gotoXY(51, new_y + 2); cout << "O";
    gotoXY(51, new_y + 3); cout << "O";

    //上邊界上的傳送門
    ground[new_x][0] = PORTAL;
    ground[new_x + 1][0] = PORTAL;
    ground[new_x + 2][0] = PORTAL;
    ground[new_x + 3][0] = PORTAL;
    gotoXY(new_x, 0);      cout << "O";
    gotoXY(new_x + 1, 0);  cout << "O";
    gotoXY(new_x + 2, 0);  cout << "O";
    gotoXY(new_x + 3, 0);  cout << "O";

    //下邊界上的傳送門
    ground[new_x][26] = PORTAL;
    ground[new_x + 1][26] = PORTAL;
    ground[new_x + 2][26] = PORTAL;
    ground[new_x + 3][26] = PORTAL;
    gotoXY(new_x, 26);     cout << "O";
    gotoXY(new_x + 1, 26); cout << "O";
    gotoXY(new_x + 2, 26); cout << "O";
    gotoXY(new_x + 3, 26); cout << "O";

    resetColor();
    gotoXY(x,y);

    //畫出蛇頭
    if (skin == 1) color(15);   //白色
    if (skin == 2) color(175);  //綠色
    if (skin == 3) color(159);  //藍色
    if (skin == 4) color(79);   //紅色
    gotoXY(x,y);
    ground[x][y] = HEAD;
    ground[x][y+1] = BODY;
    ground[x][y+2] = BODY;
    ground[x][y+3] = BODY;
    cout << "@" ;
    resetColor();


    //畫出隨機障礙物
    if (obstacle_on == true && obstacle_random == true)
    {
        for (int i=1; i<=12; i++)
        {
            newRandom1:
            int new_x = rand() % (x_size-10)+3 ; // x = 3~42
            int new_y = rand() % (y_size-5)+3 ; // y = 3~22

            if(ground[new_x][new_y] == GROUND && ground[new_x+1][new_y] == GROUND && ground[new_x+2][new_y] == GROUND && ground[new_x+3][new_y] == GROUND && ground[new_x+4][new_y] == GROUND && ground[new_x+5][new_y] == GROUND)
            {
                ground[new_x][new_y] = OBSTACLE;    gotoXY(new_x,new_y);   cout << "#";
                ground[new_x+1][new_y] = OBSTACLE;  gotoXY(new_x+1,new_y); cout << "#";
                ground[new_x+2][new_y] = OBSTACLE;  gotoXY(new_x+2,new_y); cout << "#";
                ground[new_x+3][new_y] = OBSTACLE;  gotoXY(new_x+3,new_y); cout << "#";
                ground[new_x+4][new_y] = OBSTACLE;  gotoXY(new_x+4,new_y); cout << "#";
                ground[new_x+5][new_y] = OBSTACLE;  gotoXY(new_x+5,new_y); cout << "#";
            }
            else if (ground[new_x][new_y] == GROUND && ground[new_x+1][new_y] == GROUND && ground[new_x+2][new_y] == GROUND && ground[new_x+3][new_y] == GROUND && ground[new_x+4][new_y] == GROUND && ground[new_x+5][new_y] != GROUND)
            {
                ground[new_x][new_y] = OBSTACLE;    gotoXY(new_x,new_y);   cout << "#";
                ground[new_x+1][new_y] = OBSTACLE;  gotoXY(new_x+1,new_y); cout << "#";
                ground[new_x+2][new_y] = OBSTACLE;  gotoXY(new_x+2,new_y); cout << "#";
                ground[new_x+3][new_y] = OBSTACLE;  gotoXY(new_x+3,new_y); cout << "#";
                ground[new_x+4][new_y] = OBSTACLE;  gotoXY(new_x+4,new_y); cout << "#";
            }
            else if (ground[new_x][new_y] == GROUND && ground[new_x+1][new_y] == GROUND && ground[new_x+2][new_y] == GROUND && ground[new_x+3][new_y] == GROUND && ground[new_x+4][new_y] != GROUND && ground[new_x+5][new_y] != GROUND)
            {
                ground[new_x][new_y] = OBSTACLE;    gotoXY(new_x,new_y);   cout << "#";
                ground[new_x+1][new_y] = OBSTACLE;  gotoXY(new_x+1,new_y); cout << "#";
                ground[new_x+2][new_y] = OBSTACLE;  gotoXY(new_x+2,new_y); cout << "#";
                ground[new_x+3][new_y] = OBSTACLE;  gotoXY(new_x+3,new_y); cout << "#";
            }
            else
            {
                srand(rand());
                goto newRandom1;
            }
            gotoXY(x,y);
        }
    }
}


//回傳蛇的位置座標
int snake::onTouch(int x, int y)
{
    return ground[x][y] ;
}


//畫出蛇
void snake::drawBody()
{
    if(direction != INIT)
    {
        prev_x = x ;
        prev_y = y ;

        //清空畫面上一次畫出的蛇身
        if(body_x.size() == body_y.size() && body_x.size() == length)
        {
            for(int i = 0 ; i <= body_x.size() - 1 ; i++)
            {
                ground[body_x[i]][body_y[i]] = GROUND ;
                gotoXY(body_x[i],body_y[i]);
                cout <<  " ";
                resetColor();
            }
        }

        //更新蛇身座標
        body_x.push_back(prev_x);
        body_y.push_back(prev_y);

        if(!touchFood)
        {
            body_x.pop_front();
            body_y.pop_front();

        }
        touchFood = false;
    }


    //畫出現在的蛇身
    if(body_x.size() == body_y.size() && body_x.size() == length)
    {
        for(int i = 0 ; i <= body_x.size() - 1 ; i++)
        {
            ground[body_x[i]][body_y[i]] = BODY ;
            gotoXY(body_x[i],body_y[i]);
            color(BODY_COLOR);
            cout << BODY_TYPE ;
            resetColor();
        }
    }
};


//根據user的輸入，更新蛇的座標
void snake::updateSnake(int direction)
{
    if( (x <= x_size-1 && x >= 0) && (y <= y_size-1 && y >= 0) )
    {
        switch(direction)
        {
            case UP :
                gotoXY(x,y);
                cout <<" "; //清除蛇頭
                drawBody();
                y--;
                gotoXY(x,y);    //游標更新
                if (skin == 1)color(15);    //白色
                if (skin == 2) color(175);  //綠色
                if (skin == 3) color(159);  //藍色
                if (skin == 4) color(79);   //紅色
                cout << "@" ;   //重新畫出蛇
                resetColor();
                Sleep(1000 - speed);  //暫停程式(1000-speed)毫秒
                break;

            case DOWN :
                gotoXY(x,y);
                cout <<" ";
                drawBody();
                y++;
                gotoXY(x,y);
                if (skin == 1)color(15);    //白色
                if (skin == 2) color(175);  //綠色
                if (skin == 3) color(159);  //藍色
                if (skin == 4) color(79);   //紅色
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);
                break;

            case LEFT:
                gotoXY(x,y);
                cout <<" ";
                drawBody();
                x--;
                gotoXY(x,y);
                if (skin == 1)color(15);    //白色
                if (skin == 2) color(175);  //綠色
                if (skin == 3) color(159);  //藍色
                if (skin == 4) color(79);   //紅色
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);
                break;

            case RIGHT:
                gotoXY(x,y);
                cout <<" ";
                drawBody();
                x++;
                gotoXY(x,y);
                if (skin == 1)color(15);    //白色
                if (skin == 2) color(175);  //綠色
                if (skin == 3) color(159);  //藍色
                if (skin == 4) color(79);   //紅色
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);
                break;

            case 10: //碰到上傳送門
                gotoXY (x,y);
                cout << " ";
                y = 25;
                gotoXY(x, y);
                if (skin == 1) color(15);   //白色
                if (skin == 2) color(175);  //綠色
                if (skin == 3) color(159);  //藍色
                if (skin == 4) color(79);   //紅色
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);
                break;

            case 11: //碰到下傳送門
                gotoXY (x,y);
                cout << " ";
                y = 1;
                gotoXY(x, y);
                if (skin == 1) color(15);   //白色
                if (skin == 2) color(175);  //綠色
                if (skin == 3) color(159);  //藍色
                if (skin == 4) color(79);   //紅色
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);
                break;

            case 12: //碰到左傳送門
                gotoXY (x,y);
                cout << " ";
                x = 50;
                gotoXY(x, y);
                if (skin == 1) color(15);   //白色
                if (skin == 2) color(175);  //綠色
                if (skin == 3) color(159);  //藍色
                if (skin == 4) color(79);   //紅色
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);
                break;

            case 13: //碰到右傳送門
                gotoXY (x,y);
                cout << " ";
                x = 1;
                gotoXY(x, y);
                if (skin == 1) color(15);    //白色
                if (skin == 2) color(175);  //綠色
                if (skin == 3) color(159);  //藍色
                if (skin == 4) color(79);   //紅色
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);
                break;

        }
    }
}


//判斷user的輸入是否與蛇現在的行進方向相反
int oppositeDirection(int direction, int input)
{
    if (direction == LEFT && input == RIGHT)  return 1;
    if (direction == RIGHT && input == LEFT)  return 1;
    if (direction == UP && input == DOWN)     return 1;
    if (direction == DOWN && input == UP)     return 1;

    return 0;
}


//蛇吃到食物後，新增另一個食物的位置
void snake::recreateFood()
{
    newRandom:

    int new_x = rand() % (x_size - 2) + 2 ; // x = 2~50
    int new_y = rand() % (y_size - 2)  ;    // y = 2~25

    if(ground[new_x][new_y] != OBSTACLE && ground[new_x][new_y] != WALL && ground[new_x][new_y] != PORTAL)
    {
        ground[new_x][new_y] = FOOD;
        gotoXY(new_x,new_y);
        if (skin == 1) color(8);   //灰色
        if (skin == 2) color(10);  //綠色
        if (skin == 3) color(9);   //藍色
        if (skin == 4) color(12);  //紅色
        cout << '&' ;
        resetColor();
        gotoXY(x,y);
    }
    else
    {
        srand(rand());
        goto newRandom;
    }
}


//新增特殊食物
void snake::addSpecialFood()
{
    newRandom_1:

    int random_x = rand() % (x_size - 2) + 2 ; // x = 2~50
    int random_y = rand() % (y_size - 2)  ;    // y = 2~25

    if(ground[random_x][random_y] != OBSTACLE && ground[random_x][random_y] != WALL && ground[random_x][random_y] != FOOD && ground[random_x][random_y] != PORTAL)
    {
        ground[random_x][random_y] = SPECIAL_FOOD;
        gotoXY(random_x,random_y);
        SetConsoleTextAttribute(hConsole, 14);
        cout << 'S' ;
        resetColor();
        gotoXY(x,y);
    }
    else
    {
        srand(time(NULL));
        goto newRandom_1;
    }
}
// End of class definition




//main function
int main()
{
    setWindowSize(600,600);     //遊戲視窗大小
    ShowConsoleCursor(false);  //隱藏游標

    snake Snake;

    system("CLS");

    printsnake();
    if (getch())  system("CLS");

    system("CLS");
    drawset();
    gotoXY(8,0);
    color(6);
    cout << "  Color?  (1=white  2=green  3=blue  4=red) : ";
    cin >> Snake.skin;
    gotoXY(8,1);
    cout << "  Speed?  Please enter a multiple of 10 (10~130) : ";
    cin >> Snake.speed;
    gotoXY(8,2);
    cout << "  Obstacles? (1=Yes  0=No) : ";
    cin >> obstacle_on;
    if (obstacle_on == true)
    {
        gotoXY(8,3);
        cout << "  Random obstacle? (1=Yes  0=No) : ";
        cin >> obstacle_random;
    }

    if (Snake.skin == 1) Snake.BODY_COLOR = 15;   //白色
    else if (Snake.skin == 2) Snake.BODY_COLOR = 175;  //綠色
    else if (Snake.skin == 3) Snake.BODY_COLOR = 159;  //藍色
    else if (Snake.skin == 4) Snake.BODY_COLOR = 79;   //紅色
    else
    {
        Snake.skin = 1;
        Snake.BODY_COLOR = 15;
    }

    if (Snake.speed == 10) Snake.speed = 860;
    else if (Snake.speed == 20) Snake.speed = 870;
    else if (Snake.speed == 30) Snake.speed = 880;
    else if (Snake.speed == 40) Snake.speed = 890;
    else if (Snake.speed == 50) Snake.speed = 900;
    else if (Snake.speed == 60) Snake.speed = 910;
    else if (Snake.speed == 70) Snake.speed = 920;
    else if (Snake.speed == 80) Snake.speed = 930;
    else if (Snake.speed == 90) Snake.speed = 940;
    else if (Snake.speed == 100) Snake.speed = 950;
    else if (Snake.speed == 110) Snake.speed = 960;
    else if (Snake.speed == 120) Snake.speed = 970;
    else if (Snake.speed == 130) Snake.speed = 980;
    else if (Snake.speed == 140) Snake.speed = 990;
    else Snake.speed = 860;

    system("CLS");

    Snake.initPlayground();  //設定出場地、牆壁、障礙物的座標
    Snake.drawGround();      //畫出場地、牆壁、障礙物
    Snake.drawBody();        //畫出蛇
    _beginthread(readInput,0,(void*)0);

    int food_count = 0 ;
    int special_food_count = 0 ;
    int old_dir = -1;
    gameOver = false;

    while(!gameOver)
    {
        //若user的輸入與蛇現在的行進方向相反，則維持原本的方向行進
        if ( oppositeDirection(direction, old_dir) == 1 )
        {
            Snake.updateSnake(old_dir);
        }
            //若user的輸入不與蛇現在行進的方向相反，則根據使用者的input來移動蛇
        else
        {
            old_dir = direction;
            Snake.updateSnake(direction);
        }

        //蛇碰到障礙物、牆壁、蛇身 => GAMEOVER
        if(Snake.onTouch(Snake.x,Snake.y) == OBSTACLE || Snake.onTouch(Snake.x,Snake.y) == WALL || Snake.onTouch(Snake.x,Snake.y) == BODY)
        {
            gameOver = true;
            Beep(880,250);
            Beep(988,250);
            Beep(880,250);
            Beep(784,250);
            Beep(880,250);
            Beep(784,250);
            Beep(740,250);
        }

            //蛇吃到食物 => 分數++、長度++、新增另一個食物
        else if(Snake.onTouch(Snake.x,Snake.y) == FOOD)
        {
            Snake.ground[Snake.x][Snake.y] = GROUND;
            Snake.setScore(Snake.score++);
            Snake.touchFood = true;
            Snake.length++;
            Snake.recreateFood();
            food_count++;
            special_food_count++;
            Beep(880,50);
            Beep(988,40);
        }

            //蛇吃到特殊食物 => 分數++、長度++、改變身體符號及顏色
        else if(Snake.onTouch(Snake.x,Snake.y) == SPECIAL_FOOD)
        {
            Snake.ground[Snake.x][Snake.y] = GROUND;
            Snake.setScore(Snake.score++);
            Snake.touchFood = true;
            Snake.length++;
            food_count++;
            special_food_count++;
            Snake.BODY_TYPE += (rand() % 2) + 1;
            srand(rand());
            Snake.BODY_COLOR = (rand() % 25) + 1;
            Beep(880,50);
            Beep(1330,40);
        }

            //蛇碰到傳送門 => 傳送
        else if(Snake.onTouch(Snake.x,Snake.y) == PORTAL )
        {
            if (Snake.y == 0)
            {
                Snake.updateSnake(10);
                Snake.ground[Snake.x][0] = PORTAL;
                gotoXY (Snake.x, 0);
                if (Snake.skin == 1) color(120);  //白色
                if (Snake.skin == 2) color('g');  //黃色
                if (Snake.skin == 3) color('V');  //紫色
                if (Snake.skin == 4) color(42);   //綠色
                cout << "O";
                resetColor();
                Beep(523,35);
            }
            else if (Snake.y == 26)
            {
                Snake.updateSnake(11);
                Snake.ground[Snake.x][26] = PORTAL;
                gotoXY (Snake.x, 26);
                if (Snake.skin == 1) color(120);  //白色
                if (Snake.skin == 2) color('g');  //黃色
                if (Snake.skin == 3) color('V');  //紫色
                if (Snake.skin == 4) color(42);   //綠色
                cout << "O";
                resetColor();
                Beep(523,35);
            }
            else if (Snake.x == 0)
            {
                Snake.updateSnake(12);
                Snake.ground[0][Snake.y] = PORTAL;
                gotoXY (0, Snake.y);
                if (Snake.skin == 1) color(120);  //白色
                if (Snake.skin == 2) color('g');  //黃色
                if (Snake.skin == 3) color('V');  //紫色
                if (Snake.skin == 4) color(42);   //綠色
                cout << "O";
                resetColor();
                Beep(523,35);
            }
            else if (Snake.x == 51)
            {
                Snake.updateSnake(13);
                Snake.ground[51][Snake.y] = PORTAL;
                gotoXY (51, Snake.y);
                if (Snake.skin == 1) color(120);  //白色
                if (Snake.skin == 2) color('g');  //黃色
                if (Snake.skin == 3) color('V');  //紫色
                if (Snake.skin == 4) color(42);   //綠色
                cout << "O";
                resetColor();
                Beep(523,35);
            }
        }

        //當分數 = 4 的倍數時 => 加速蛇的移動
        if(Snake.score % 4 == 0 && Snake.score != 0 && food_count == 4 && Snake.speed <= 990)
        {
            Snake.setScore(Snake.speed += 10);
            food_count = 0 ;
        }

        //當分數 = 6 的倍數 => 新增特殊食物
        if ((Snake.score) % 6 == 0 && Snake.score != 0 && special_food_count == 6)
        {
            Snake.addSpecialFood();
            special_food_count = 0;
            Beep(1760,50);
            Beep(880,40);
        }
    }


    //遊戲結束時，顯示分數
    if(gameOver == true)
    {
        system("cls");
        gotoXY (0,-6);
        cout << "=========================================" << endl;
        cout << "||                                     ||" << endl;
        cout << "||                                     ||" << endl;
        cout << "||                                     ||" << endl;
        cout << "||                                     ||" << endl;
        cout << "||                                     ||" << endl;
        cout << "||                                     ||" << endl;
        cout << "=========================================" << endl;

        gotoXY(11,-3);  cout << "Such A Loser XD " << endl ;
        gotoXY(10,-2);  cout << "Your score is : " << Snake.score << endl;
        gotoXY(11,1);   cout << endl;
        system("pause");
    }

    return 0;
}

//初始遊戲介面
void printsnake()
{
    color(9);
    cout << endl;

    cout <<"      ___           ___           ___           ___           ___" << endl ;
    cout << "     /\\  \\         /\\__\\         /\\  \\         /\\__\\         /\\  \\"    << endl ;
    cout << "    /::\\  \\       /::|  |       /::\\  \\       /:/  /        /::\\  \\"   << endl ;
    cout << "   /:/\\ \\  \\     /:|:|  |      /:/\\:\\  \\     /:/__/        /:/\\:\\  \\" << endl ;
    cout << "   _\\:\\~\\ \\  \\   /:/|:|  |__   /::\\~\\:\\  \\   /::\\__\\____   /::\\~\\:\\  \\" << endl ;
    cout << " /\\ \\:\\ \\ \\__\\ /:/ |:| /\\__\\ /:/\\:\\ \\:\\__\\ /:/\\:::::\\__\\ /:/\\:\\ \\:\\__\\" << endl ;
    cout << " \\:\\ \\:\\ \\/__/ \\/__|:|/:/  / \\/__\\:\\/:/  / \\/_|:|~~|~    \\:\\~\\:\\ \\/__/" << endl ;
    cout << "  \\:\\ \\:\\__\\       |:/:/  /       \\::/  /     |:|  |      \\:\\ \\:\\__\\" << endl ;
    cout << "   \\:\\/:/  /       |::/  /        /:/  /      |:|  |       \\:\\ \\/__/" << endl ;
    cout << "    \\::/  /        /:/  /        /:/  /       |:|  |        \\:\\__\\" << endl ;
    cout << "     \\/__/         \\/__/         \\/__/         \\|__|         \\/__/" << endl;


            color(3);
    cout << endl << setw(43) << "press to continue..." << endl;
    resetColor();
}

void drawset()
{
    color(14);
    cout << endl << endl << endl;
    cout<< "       --------------------------------------------------------" << endl;
    cout << "       |                                                      |" << endl;
    cout << "       |                                                      |" << endl;
    cout << "       |                                                      |" << endl;
    cout << "       |                                                      |" << endl;
    cout << "       |                                                      |" << endl;
    cout << "       |                                                      |" << endl;
    cout << "       |                                                      |" << endl;
    cout << "       |                                                      |" << endl;
    cout<< "       --------------------------------------------------------" << endl;
    resetColor();
}

//設定顏色
void color(int color_number)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color_number);
}


//將顏色改回白色
void resetColor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}


//移動游標位置
void gotoXY(int x, int y)
{
    HANDLE hStdOut;
    COORD coord;
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);    //從一個標準設備中取得句炳
    if (hStdOut == INVALID_HANDLE_VALUE) return; //調用失敗則返回
    coord.X = x ;
    coord.Y = y + 6;
    SetConsoleCursorPosition(hStdOut, coord);  //設定游標位置
}


//設定視窗大小
void setWindowSize(int width, int height)
{
    HWND console = GetConsoleWindow();      //取得視窗的句炳
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);   //取得RECT的指定尺寸(console讀入width、&ConsoleRect讀入height)

    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, width, height, TRUE);  //改變指定視窗的位置和大小
}


//判斷使用者的input
void readInput(void* id)
{
    char c;

    do
    {
        c = getch(); //從鍵盤讀入任意字元
        switch(c)
        {
            case 'w' : case 'W':  direction = UP;    break;
            case 'a' : case 'A':  direction = LEFT;  break;
            case 's' : case 'S':  direction = DOWN;  break ;
            case 'd' : case 'D':  direction = RIGHT; break ;
        }
    }while( (c != 'q') || c != (char)27);

    _endthread(); //终止由 _beginthread 所創建的線程

    return;
}


//隱藏游標
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

