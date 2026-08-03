#include "consoleGame.hpp"

int main()
{
    setlocale(LC_ALL, "");
    initscr();

#ifdef PDC_WIDE
    printw("PDC_WIDE is active\n");
#endif
#ifdef PDC_FORCE_UTF8
    printw("PDC_FORCE_UTF8 is active\n");
#endif

    mousemask(BUTTON1_CLICKED, nullptr);
    keypad(stdscr, TRUE);
    cbreak();
    if (has_colors()) {
        start_color();
        int colorID = 0;
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; j++) {
                init_pair(colorID + 1, i, j);
                colorID++;
            };   
        }
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