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
#include <memory>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

//text colours
inline const char* RED = "\033[31m";
inline const char* BLUE = "\033[34m";
inline const char* RESET = "\033[0m";

std::vector<std::unique_ptr<structSearcher>> gamedataBase;

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

struct structSearcher
{
    virtual ~structSearcher() = default;
    virtual bool matches(const std::string& searchItem) const = 0;  
};

struct character:structSearcher
{
    std::string name;
    int hpMax;
    int attack;

    bool matches(const std::string& searchItem) const override {
        return name == searchItem;
    }
};

struct enemy :structSearcher
{
    std::string name;
    int hpMax;
    int attack;
    int goldReward;
    int difficultyIndicator;
    std::string faction;
    std::string region;

    bool matches(const std::string& searchItem) const override {
        return name == searchItem;
    }
};

struct action
{
    std::string nameOfAction;
    std::string resultOfAction;
};

struct location:structSearcher
{
    std::string name;
    std::string description;
    std::vector<action> possibleActions;

    bool matches(const std::string& searchItem) const override {
        return name == searchItem;
    }
};

struct item :structSearcher
{
    std::string name;
    std::string bonus;
    int value;
    int price;

    bool matches(const std::string& searchItem) const override {
        return name == searchItem;
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(enemy, name, hpMax, attack, goldReward, difficultyIndicator, faction, region)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(character, name, hpMax, attack)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(action, nameOfAction, resultOfAction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(location, name, description, possibleActions)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(item, name, bonus, value, price)

extern uint32_t seedValue;
extern int amountOfChoices;
extern std::vector<char> charPossibilities;

extern std::vector<enemy> enemies;
extern std::vector<character> characters;
extern std::vector<location> locations;
extern std::map<std::string, std::function<void()>> locationActions;
extern std::vector<item> items;

extern enemy currentCombatEnemy;

extern character player;

extern int playerCurrentHP;
extern int playerCurrentGold;
extern int currentCombatEnemyCurrentHP;

extern int shopChoiceInt;

static std::vector<std::string> yesOrNo =
{
    "Yes",
    "No"
};

extern std::map<std::string, int*> valueAndStatConnector;
extern std::map<std::string, int*> factionAssigner;
extern std::map<std::string, int*> regionAssigner;

class setupAndUtility
{ 
public:
    void vectorCreation(size_t);
    char correctInput();
    void startGame();
    void printAscii(std::string);
    void yesOrNoFunction();
    int seedIteration(int divisionAmount);
    void sampleMaker(std::vector<structSearcher> list);
private:
    void setConsoleOutputUTF8();  
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
    void loadEnemies();
    void loadCharacters();
    void loadLocations();
    void loadItems();
    void setseed();
    void setClass();
    void locationAction();
    void unorderedMapMaker();
};

class combat 
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

class activities
{
public:
    void shopEntry();
    void canPlayerBuy(int shopChoice);
};

#endif