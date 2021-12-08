#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <future>
using namespace std;

//Variables declarations
#define x_size 52 //size of ground (include boundary) 50 x 25
#define y_size 27
enum {GROUND,FOOD,WALL,HEAD,BODY,OBSTACLE}; //index -> 0,1,2,3,4
enum {UP,DOWN,LEFT,RIGHT,STAY}; //direction -> 0,1,2,3,4
bool gameOver = false;
int direction = STAY;
//

//class definition
class snake
{
private:
    int score = 0;
    int ground[x_size][y_size];
    int length;
    int x = 25 ,y = 15; //Snake head's (X,Y) coordinate // init place at (5,5)
    int speed = 850; //950 full speed

public:
    //ground
    void initPlayground();
    void drawGround();

    //score
    void setScore(int s);
    void drawScore();

    //game logic


    //character info.
    int getLength(); //return snake's length
    void setLength();
    void setHeadPos(int x ,int y);
    int onTouch(int x, int y); //tells what thing does the snake's head touched


    //character movement
    void moveDirection(int direction);
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
void snake::initPlayground(){
    //In this function, it will generate random location food, obstacles,
    //infill GROUND
    for(int x = 1 ; x <= x_size-1 ; x++){
        for(int y = 1 ; y <= y_size-1 ; y++) {
            ground[x][y] = GROUND;
        }
    }
    //build WALL
    for(int y = 0 ; y <= y_size ; y++){
        ground[0][y] = WALL ;
    }
    for(int y = 0 ; y <= y_size ; y++){
        ground[51][y] = WALL ;
    }
    for(int x = 0 ; x <= x_size ; x++){
        ground[x][0] = WALL ;
    }
    for(int x = 0 ; x <= x_size ; x++){
        ground[x][26] = WALL ;
    }

    int temp ;
    srand((unsigned int)time(NULL));
    for(int i = 1 ; i <= 7 ; i++){
        if(temp != rand()){
            int x = rand() % (x_size - 2) + 1 ;
            int y = rand() % (y_size - 2) + 1 ;
            ground[x][y] = FOOD ;
        }else{
            srand(0);
            int x = rand() % (x_size - 2) + 1 ;
            int y = rand() % (y_size - 2) + 1 ;
            ground[x][y] = FOOD ;
        }
        temp = rand();
    }
}
void snake::setScore(int s) {
    this->score = s ;
}
void snake::drawScore(){
    cout << string(15,' ') << "////////////////////" << endl;
    cout << string(15,' ') << setfill(' ') << "//" << setw(18) << setfill(' ')  << "//" << endl;
    cout << string(15,' ') <<"//" << "    " << "Score: " << score  << "    "  << "//" << endl;
    cout << string(15,' ') <<"//" << setw(18) << setfill(' ')  << "//" << endl;
    cout << string(15,' ') <<"////////////////////" << endl;
    cout << endl ;
}
void snake::drawGround() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    clearScreen();
    drawScore();
    for(int i = 0 ; i <= y_size-1 ; i++){
        for(int j = 0 ; j <= x_size-1 ; j++){
            if(ground[j][i] == WALL){
                SetConsoleTextAttribute(hConsole, 11);
                cout << (char)219   ;
            }else if(ground[j][i] == GROUND){
                SetConsoleTextAttribute(hConsole, 7);
                cout << " " ;
            }else if(ground[j][i] == FOOD){
                cout << "&" ;
            }
        }
        cout << endl ;
    }
    resetColor(); //reset Color
    gotoXY(x,y); cout << (char)2;
}
int snake::onTouch(int x, int y) {
    return ground[x][y] ;
}
int snake::getLength() {
    return this->length;
}
void snake::setHeadPos(int X, int Y){
    //set "Head" position
    x = X;
    y = Y;
}
void snake::moveDirection(int direction){
    if( (x < 49 && x > 0) && (y < 24 && y > 0) && ground[x][y] != OBSTACLE){
        switch(direction){
            case UP :
                cout <<"\b \b";
                y--;
                gotoXY(x,y);
                cout << (char)2 ;
                Sleep(1000 - speed);
                break;

            case DOWN :
                cout <<"\b \b";
                y++;
                gotoXY(x,y);
                cout << (char)2 ;
                Sleep(1000 - speed);
                break;

            case LEFT:
                cout <<"\b \b";
                x--;
                gotoXY(x,y);
                cout << (char)2 ;
                Sleep(1000 - speed);
                break;

            case RIGHT:
                cout <<"\b \b";
                x++;
                gotoXY(x,y);
                cout << (char)2 ;
                Sleep(1000 - speed);
                break;
        }
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

    while(!gameOver){
        Snake.moveDirection(direction);
    }






    //gotoXY(0,33);
    system("pause");
    return 0;
}

void resetColor(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}
void gotoXY(int x, int y)
{
    HANDLE hStdOut;
    COORD coord;
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;
    coord.X = x + 1;
    coord.Y = y + 7;
    SetConsoleCursorPosition(hStdOut, coord);
}
void clearScreen()
{
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
