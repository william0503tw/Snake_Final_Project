#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;


#define x_size 52 //size of ground (include boundary) 50 x 25
#define y_size 27
enum {GROUND,FOOD,WALL,SNAKE}; //index -> 0,1,2,3


class snake
{
private:
    int score = 0 ;
    int ground[x_size][y_size];

public:
    void initPlayground();
    void drawScore();
    void updateScreen();
    int onTouch();
};

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
}

void snake::drawScore(){
    cout << string(40,' ') << "////////////////////" << endl;
    cout << string(40,' ') << setfill(' ') << "//" << setw(18) << setfill(' ')  << "//" << endl;
    cout << string(40,' ') <<"//" << "    " << "Score: " << score  << "    "  << "//" << endl;
    cout << string(40,' ') <<"//" << setw(18) << setfill(' ')  << "//" << endl;
    cout << string(40,' ') <<"////////////////////" << endl;
    cout << endl ;
}

void snake::updateScreen() {
    snake::drawScore();
    for(int i = 0 ; i <= y_size-1 ; i++){
        for(int j = 0 ; j <= x_size-1 ; j++){
            //cout << ground[j][i] ;
            if(ground[j][i] == WALL){
                cout << (char)219 << (char)219 ;
            }else if(ground[j][i] == GROUND){
                cout << "  " ;
            }
        }
        cout << endl ;
    }
}

int snake::onTouch() {


    return 1 ;
}


int main() {

    snake Snake;
    Snake.initPlayground();
    Snake.updateScreen();
    system("pause");
    return 0;
}
