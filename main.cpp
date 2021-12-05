#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <thread>
using namespace std;

//Variables declarations
#define x_size 52 //size of ground (include boundary) 50 x 25
#define y_size 27
enum {GROUND,FOOD,WALL,HEAD,BODY}; //index -> 0,1,2,3,4
enum {UP,DOWN,LEFT,RIGHT}; //direction -> 0,1,2,3
//


//class definition
class snake
{
private:
    int score = 0;
    int ground[x_size][y_size];
    int length;
    int pos[2] = {1,1}; //Snake head's (X,Y) coordinate // init place at (1,1)


public:
    void initPlayground();
    void drawGround();

    void setScore(int s);
    void drawScore();

    int onTouch(int x, int y); //tells what thing does the snake's head touched

    int getLength(); //return snake's length
    void setLength();
    void setHeadPos(int x ,int y);

    int direction;
};

//Global Function Prototypes
void readInput(snake Snake);
void resetColor();
void clearScreen();
//

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
    cout << string(20,' ') << "////////////////////" << endl;
    cout << string(20,' ') << setfill(' ') << "//" << setw(18) << setfill(' ')  << "//" << endl;
    cout << string(20,' ') <<"//" << "    " << "Score: " << score  << "    "  << "//" << endl;
    cout << string(20,' ') <<"//" << setw(18) << setfill(' ')  << "//" << endl;
    cout << string(20,' ') <<"////////////////////" << endl;
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
                cout << (char)2  ;
            }else if(ground[j][i] == GROUND){
                SetConsoleTextAttribute(hConsole, 7);
                cout << " " ;
            }else if(ground[j][i] == HEAD){
                SetConsoleTextAttribute(hConsole, 14);
                cout << (char)2  ;
            }else if(ground[j][i] == FOOD){
                cout << "+" ;
            }
        }
        cout << endl ;
    }
    resetColor(); //reset Color
}

int snake::onTouch(int x, int y) {
    return ground[x][y] ;
}

int snake::getLength() {
    return this->length;
}

void snake::setHeadPos(int x, int y){
    //set "Head" position
    pos[0] = x ;
    pos[1] = y ;
    ground[x][y] = HEAD ;

}
// End of class definition


//main function
int main() {
    snake Snake;
    Snake.initPlayground();

    Snake.setScore(5);
    Snake.setHeadPos(12,12);
    Snake.drawGround();

    int temp ;
    for(int i = 1 ; i <= 7 ; i++) {

    }

    system("pause");
    return 0;
}

void resetColor(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}
void readInput(snake Snake){
    char ch ;
    do
    {
        ch = getch();
        switch(ch)
        {
            case 'W':
            case 'w':
                Snake.direction = UP;
                break;
            case 'A':
            case 'a':
                Snake.direction = LEFT;
                break;
            case 's':
            case 'S':
                Snake.direction = DOWN;
                break;
            case 'D':
            case 'd':
                Snake.direction = RIGHT;
                break;
        }
    }while (ch != 'Q' && ch!='q'); ///TODO: Change to pause game
}
void gotoxy(int column, int row)
{
    HANDLE hStdOut;
    COORD coord;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    coord.X = column;
    coord.Y = row;
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
