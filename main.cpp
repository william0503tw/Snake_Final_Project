#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <deque>
using namespace std;

//變數宣告 Variables declarations
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#define x_size 52 //size of ground (include boundary) 50 x 25
#define y_size 27
enum {GROUND,FOOD,WALL,HEAD,BODY,OBSTACLE}; //index -> 0,1,2,3,4
enum {UP,DOWN,LEFT,RIGHT,INIT}; //direction -> 0,1,2,3,4
bool gameOver = false;
int direction = INIT;
int FOOD_COUNT = 3 ;

//class宣告 class definition
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
    void setScore(int s);
    void drawScore();

    //角色資訊 character info
    int x = 25 ,y = 12; //Snake head's (X,Y) coordinate
    int score = 0;
    int onTouch(int x, int y); //tells what thing does the snake's head touched
    int length = 3;
    deque<int> body_x = {25,25,25}; //init position
    deque<int> body_y = {13,14,15}; //init position
    int speed = 900; //990 full speed

    //character movement
    void updateSnake(int direction);
};

//Global Function Prototypes
void readInput(void* id);
void resetColor();
void clearScreen();
void gotoXY(int x, int y);
void setWindowSize(int width, int height);
void ShowConsoleCursor(bool showFlag);
//

//start of class function definition
void snake::indexObstacle(int y , int x_start , int x_finish){
    for(int i = x_start ; i <= x_finish ; i++ ){
        ground[i][y] = OBSTACLE;
    }
}
void snake::initPlayground()
{
    //In this function, it will generate random location food, obstacles,
    //infill GROUND
    for(int i = 1 ; i <= x_size-1 ; i++){
        for(int j = 1 ; j <= y_size-1 ; j++) {
            ground[i][j] = GROUND;
        }
    }
    //build WALL
    for(int i = 0 ; i <= y_size-1 ; i++){
        ground[0][i] = WALL ;
    }


    for(int i = 0 ; i <= y_size-1 ; i++){
        ground[51][i] = WALL ;
    }


    for(int i = 0 ; i <= x_size-1 ; i++){
        ground[i][0] = WALL ;
    }
    for(int i = 0 ; i <= x_size-1 ; i++){
        ground[i][26] = WALL ;
    }

    //obstacles
    indexObstacle(5,6,13);
    indexObstacle(6,6,8);
    indexObstacle(7,4,8);
    indexObstacle(20,15,17);
    indexObstacle(21,15,22);
    indexObstacle(22,17,24);
    indexObstacle(3,44,46);
    indexObstacle(4,38,46);
    indexObstacle(5,38,40);
    indexObstacle(6,34,40);
    indexObstacle(17,35,37);
    indexObstacle(17,41,43);
    indexObstacle(18,36,36);
    indexObstacle(18,42,42);
    indexObstacle(20,35,35);
    indexObstacle(20,43,43);
    indexObstacle(21,36,36);
    indexObstacle(21,42,42);
    indexObstacle(22,37,41);
    indexObstacle(13,12,17);
    indexObstacle(14,10,19);
    indexObstacle(15,12,17);

    //food
    int temp ;
    srand((unsigned int)time(NULL));
    for(int i = 1 ; i <= FOOD_COUNT ; i++){
        if(temp != rand()){
            int x_ran = rand() % (x_size - 2) + 1 ;
            int y_ran = rand() % (y_size - 2) + 2 ;
            if(ground[x_ran][y_ran] != OBSTACLE && ground[x_ran][y_ran] != WALL){
                ground[x_ran][y_ran] = FOOD ;
            }else{
                ground[x_ran][y_ran] = OBSTACLE ;
                i--;
            }
        }else{
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
void snake::setScore(int s) {
    cout << "\b \b";
    gotoXY(28,-4);
    cout << score ;
    gotoXY(x,y);
}
void snake::drawScore(){
    cout << string(15,' ') << "////////////////////" << endl;
    cout << string(15,' ') << setfill(' ') << "//" << setw(18) << setfill(' ')  << "//" << endl;
    cout << string(15,' ') <<"//" << "    " << "Score: " << score  << "    "  << "//" << endl;
    cout << string(15,' ') <<"//" << setw(18) << setfill(' ')  << "//" << endl;
    cout << string(15,' ') <<"////////////////////" << endl;
    cout << endl ;
}
void snake::drawGround()
{
    clearScreen();
    drawScore();
    for(int i = 0 ; i <= y_size-1 ; i++){
        for(int j = 0 ; j <= x_size-1 ; j++){
            if(ground[j][i] == WALL){
                SetConsoleTextAttribute(hConsole, 11);
                cout << "+"   ;
            }else if(ground[j][i] == GROUND){
                SetConsoleTextAttribute(hConsole, 7);
                cout << " " ;
            }else if(ground[j][i] == FOOD){
                SetConsoleTextAttribute(hConsole, 6);
                cout << "&" ;
            }else if(ground[j][i] == OBSTACLE){
                SetConsoleTextAttribute(hConsole, 7);
                cout << "#" ;
            }
        }
        cout << endl ;
    }
    SetConsoleTextAttribute(hConsole, 10);
    gotoXY(x,y);
    cout << "@" ;
    resetColor();
}
int snake::onTouch(int x, int y) {
    return ground[x][y] ;
}
void snake::updateSnake(int direction){
    if( (x < x_size-1 && x > 0) && (y < y_size-1 && y > 0) ){
        switch(direction){
            case UP :
                cout <<"\b \b";
                y--;
                gotoXY(x,y);
                SetConsoleTextAttribute(hConsole, 10);
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);
                break;

            case DOWN :
                cout <<"\b \b";
                y++;
                gotoXY(x,y);
                SetConsoleTextAttribute(hConsole, 10);
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);

                break;

            case LEFT:
                cout <<"\b \b";
                x--;
                gotoXY(x,y);
                SetConsoleTextAttribute(hConsole, 10);
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);

                break;

            case RIGHT:
                cout <<"\b \b";
                x++;
                gotoXY(x,y);
                SetConsoleTextAttribute(hConsole, 10);
                cout << "@" ;
                resetColor();
                Sleep(1000 - speed);

                break;

        }

    }
}
void snake::recreateFood(){
    newRandom:
    int new_x = rand() % (x_size - 2) + 2 ; //2~50
    int new_y = rand() % (y_size - 2)  ; //2~25
    if(ground[new_x][new_y] != OBSTACLE && ground[new_x][new_y] != WALL){
        ground[new_x][new_y] = FOOD;
        gotoXY(new_x,new_y);
        SetConsoleTextAttribute(hConsole, 6);
        cout << '&' ;
        resetColor();
        gotoXY(x,y);
    }else{
        srand(rand());
        goto newRandom;
    }

}
// End of class definition


//main function
int main() {
    setWindowSize(450,600);
    ShowConsoleCursor(false);
    snake Snake;

    Snake.initPlayground();
    Snake.drawGround();
    _beginthread(readInput,0,(void*)0);

    int food_count = 0 ;
    while(!gameOver){
        Snake.updateSnake(direction);
        if(Snake.onTouch(Snake.x,Snake.y) == OBSTACLE || Snake.onTouch(Snake.x,Snake.y) == WALL || Snake.onTouch(Snake.x,Snake.y) == BODY || Snake.onTouch(Snake.x,Snake.y) == BODY){
            gameOver = true;
        }else if(Snake.onTouch(Snake.x,Snake.y) == FOOD){
            Snake.ground[Snake.x][Snake.y] = GROUND;
            Snake.setScore(Snake.score++);
            Snake.recreateFood();
            food_count++;
        }

        if(Snake.score % 5 == 0 && Snake.score != 0 && food_count == 5){
            if(Snake.speed <= 990){
                Snake.speed += 10 ;
                food_count = 0 ;
            }
        }
    }

    if(gameOver){
        clearScreen();
        cout << "Such A Loser XD" << endl ;
    }
    gotoXY(0,10);
    system("pause");
    return 0;
}

void resetColor(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}
void gotoXY(int x, int y){
    HANDLE hStdOut;
    COORD coord;
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;
    coord.X = x ;
    coord.Y = y + 6;
    SetConsoleCursorPosition(hStdOut, coord);
}
void clearScreen(){
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells and cell attributes in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
            hStdOut,		//handle to console screen buffer
            (TCHAR) ' ',	//character to write to the buffer
            cellCount,		//number of cells to write to
            homeCoords,		//coordinates of first cell
            &count			//receives the number of characters written
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
            hStdOut,			//handle to console screen buffer
            csbi.wAttributes,	//Character attributes to use
            cellCount,			//Number of cells to set attribute
            homeCoords,			//Coordinate of first cell
            &count				//receives the number of characters written
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}
void setWindowSize(int width, int height){
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);

    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, width, height, TRUE);
}
void readInput(void* id){
    char c;
    do{
        c = getch();
        switch(c){
            case 'w': direction = UP; break;
            case 'a': direction = LEFT; break;
            case 's': direction = DOWN; break ;
            case 'd': direction = RIGHT; break ;
        }
    }while( (c != 'q') || c != (char)27);
    _endthread();
    return;
    //cout << direction << endl ;
}
void ShowConsoleCursor(bool showFlag){
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
