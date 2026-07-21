#include "consoleGame.hpp"
void gameManager::startGame()
{
    setupAndUtility util(*this);
    gameDataCreation gameData(*this);
    util.setConsoleOutputUTF8();
    
    gameData.loadEnemies();
    gameData.loadCharacters();
    gameData.loadLocations();
    gameData.loadAbilities();
    gameData.locationAction();

    gameData.loadItems();
    gameData.unorderedMapMaker();

    gameData.gameDataGenerator();

    gameData.setseed();

    gameData.setClass();

    std::cout << "You are a traveller from another world...\n" << "Your only purpose is to survive...\n" << "Good luck.\n";

    mainGameLoop mainLoop(*this);
    while (playerCurrentHP > 0)
    {
        mainLoop.mainLoop();
    }
}