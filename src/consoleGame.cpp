#include "consoleGame.hpp"

int main()
{
    initscr();
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
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