#include "consoleGame.hpp"

int main()
{
    gameManager manager;
    setupAndUtility util(manager);
    manager.startGame();
}