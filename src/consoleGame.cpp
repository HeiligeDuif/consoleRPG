#include "consoleGame.hpp"

int main()
{
    initscr();
    mousemask(BUTTON1_CLICKED, nullptr);
    keypad(stdscr, TRUE);
    cbreak();
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
    }
    else {
        printw("No color supported :(");
    }
    gameManager manager;
    setupAndUtility util(manager);
    manager.startGame();
    endwin();
    return 0;
}