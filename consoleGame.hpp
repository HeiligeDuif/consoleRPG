// functies.h
#ifndef consoleGame_HPP
#define consoleGame_HPP

#include <iostream>
#include <json/single_include/nlohmann/json.hpp>
#include <fstream>
#include <array>
#include <vector>
#include <cctype>
#include <string>
#include <limits>
#include <stdlib.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iomanip>
#include <chrono>
#include <thread>
#include <limits>
#include <unordered_map>
#include <functional>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif


//text colours
inline const char* RED = "\033[31m";
inline const char* BLUE = "\033[34m";
inline const char* RESET = "\033[0m";

inline void EMPTYSCREEN()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::cout << "\033[2J\033[H" << std::flush;
}

inline void wait(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

using json = nlohmann::json;

struct character {
    std::string name;
    int hpMax;
    int attack;
};

struct enemy
{
    std::string name;
    int hpMax;
    int attack;
};

struct action
{
    std::string nameOfAction;
    std::string resultOfAction;
};

struct location
{
    std::string name;
    std::string description;
    std::vector<action> possibleActions;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(enemy, name, hpMax, attack)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(character, name, hpMax, attack)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(action, nameOfAction, resultOfAction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(location, name, description, possibleActions)

extern uint32_t seedValue;
extern int amountOfChoices;
extern std::vector<char> charPossibilities;

extern std::vector<enemy> enemies;
extern std::vector<character> characters;
extern std::vector<location> locations;

extern enemy currentCombatEnemy;

extern character player;

extern int playerCurrentHP;
extern int currentCombatEnemyCurrentHP;

class setupAndUtility
{
public:
    void vectorCreation(size_t);
    char correctInput();
    void startGame();
private:
    void setConsoleOutputUTF8();
    void printAscii(std::string);
    void loadEnemies();
    void loadCharacters();
    void loadLocations();
    void setseed();
    void setClass();
};

class mainGameLoop
{
public:
    void mainLoop();
    int multipleWaySplit(int);
    void splitOptions(int);
private:

};

class gameDataCreation
{
public:
    gameDataCreation();

    std::unordered_map<std::string, std::function<void()>> locationActions;
};

class Combat 
{
public:
    void basicCombat();
private:
    void selectEnemy();
    void combatTurn();
    void playerCombatTurn(bool& playerBlocking);
    void enemyIntent();
    void enemyCombatTurn();
};
#endif