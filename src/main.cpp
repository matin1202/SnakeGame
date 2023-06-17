#include <vector>
#include "game.h"
#include <ncurses.h>

using namespace std;

int main(){
    int ch, bt = 1;
    Game m = Game();
    while(1){
        initscr();
        clear();
        raw();
        keypad(stdscr, TRUE);
        noecho();
        curs_set(0); 
        mvprintw(12, 29, "SNAKE GAME");
        mvprintw(15, 34, "Play : Press \'p\'");
        mvprintw(16, 34, "Exit : Press \'e\'");
        while(1){
            ch = getch();
            if(ch == 'p'){
                bt = 1;
                break;
            }
            else if(ch=='e'){
                bt = 2;
                break;
            }
        }
        if(bt == 1){
            m = Game();
            m.refreshScreen();
        }
        if(bt == 2){
            endwin();
            return 0;
        }
    }
    return 0;
}
