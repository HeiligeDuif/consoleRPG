// functies.h
#ifndef consoleGame_HPP
#define consoleGame_HPP
#define PDC_NCMOUSE

#include <iostream>

#include <nlohmann/json.hpp>
#include <curses.h>

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
#include <unordered_map>
#include <functional>
#include <memory>
#include <variant>
#include <map>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

//text colours
inline const char* RED = "\033[31m";
inline const char* BLUE = "\033[34m";
inline const char* RESET = "\033[0m";

struct structSearcher;
extern std::vector<std::unique_ptr<structSearcher>> gamedataBase;

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

struct character:public structSearcher
{
    std::string name;
    int hpMax;
    int attack;
    std::optional<std::string> startingAbility;

    bool matches(const std::string& searchItem) const override {
        return name == searchItem;
    }
};

struct enemy:public structSearcher
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

struct location:public structSearcher
{
    std::string name;
    std::string description;
    std::vector<action> possibleActions;
    int rarity;
    int proximity;
    bool beenHere;

    bool matches(const std::string& searchItem) const override {
        return name == searchItem;
    }
};
extern int proximityCounter;

struct ability :public structSearcher
{
    std::string name;
    std::string effect;
    int amount;
    std::string special;
    int specialAmount;

    bool matches(const std::string& searchItem) const override {
        return name == searchItem;
    }
};

struct item :public structSearcher
{
    std::string name;
    std::string bonus;
    int value;
    int price;
    bool rebuyable;
    std::optional<bool> bought;

    bool matches(const std::string& searchItem) const override {
        return name == searchItem;
    }
};

struct quest
{
    std::string name;
    std::string description;
    std::string type;
    int targetAmount;
    bool completed;
    std::string rewardType;
    int rewardAmount;
};

inline void from_json(const nlohmann::json& j, enemy& c) {
    c.name = j.at("name").get<std::string>();
    c.hpMax = j.at("hpMax").get<int>();
    c.attack = j.at("attack").get<int>();
    c.goldReward = j.at("goldReward").get<int>();
    c.difficultyIndicator = j.at("difficultyIndicator").get<int>();
    c.faction = j.at("faction").get<std::string>();
    c.region = j.at("region").get<std::string>();
}
//JSON custom loader
inline void from_json(const nlohmann::json& j, character& c) {
    c.name = j.at("name").get<std::string>();
    c.hpMax = j.at("hpMax").get<int>();
    c.attack = j.at("attack").get<int>();

    if (j.contains("startingAbility") && !j["startingAbility"].is_null()) {
        c.startingAbility = j.at("startingAbility").get<std::string>();
    }
    else {
        c.startingAbility = std::nullopt;
    }
}
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(action, nameOfAction, resultOfAction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(location, name, description, possibleActions, rarity, proximity, beenHere)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ability, name, effect, amount, special, specialAmount)
inline void from_json(const nlohmann::json& j, item& c) {
    c.name = j.at("name").get<std::string>();
    c.bonus = j.at("bonus").get<std::string>();
    c.value = j.at("value").get<int>();
    c.price = j.at("price").get<int>();
    c.rebuyable = j.at("rebuyable").get<bool>();

    if (j.contains("bought") && !j["bought"].is_null()) {
        c.bought = j.at("bought").get<bool>();
    }
    else {
        c.bought = std::nullopt;
    }
}
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(quest, name, description, type, targetAmount, completed, rewardType, rewardAmount)

extern std::map<std::string, std::function<void()>> locationActions;
extern std::vector<item> items;

extern std::vector<quest> quests;

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
extern std::map<std::string, std::function<void()>> abilityAttributeAssigner;
//extern std::map<std::string, int*> factionAssigner;
//extern std::map<std::string, int*> regionAssigner;

extern std::string currentRegion;
extern std::vector<enemy*> availableEnemies; 

class gameManager
{
public:
    void startGame();
    uint32_t seedValue;
    std::vector<char> charPossibilities;

    std::vector<enemy> enemies;
    std::vector<character> characters;
    std::vector<location> locations;

    std::vector<ability> abilities;
    std::vector<ability> knownAbilities;
    std::vector<ability> equippedAbilities;

    int currentAbilityAmount;
    int currentAbilitySpecialAmount;
    ability newAbility;

    float soulCounter = 0;

    bool leaving = false;
private:

};

class setupAndUtility
{ 
public:
    setupAndUtility(gameManager& manager) : gm(manager) {}

    void vectorCreation(size_t);
    char correctInput(int menuStartY);
    void printAscii(const std::string& fileName);
    void yesOrNoFunction();
    int seedIteration(int divisionAmount);
    void unlockAbility(ability ewAbility);
    void setConsoleOutputUTF8();
    int handleMenuClickOrKey(int totalOptions);

    template <typename T, typename storage>
    void addToDataBase(storage& source)
    {
        for (const auto& stuff : source)
        {
            gamedataBase.push_back(std::make_unique<T>(stuff));
        }
    }

    template <typename T>
    std::vector<T*> filterGameData(const std::vector<std::unique_ptr<structSearcher>>& source,
        std::function<bool(const T*)> predicate)
    {
        std::vector<T*> results;
        for (const auto& item : source) {
            // typecast to T
            if (T* derived = dynamic_cast<T*>(item.get())) {
                // matching type
                if (predicate(derived)) {
                    results.push_back(derived);
                }
            }
        }
        return results;
    }

private:
    gameManager& gm;
};

class mainGameLoop
{
public:
    mainGameLoop(gameManager& manager) : gm(manager) {}

    void mainLoop();
    int multipleWaySplit(int);
    void splitOptions(int);
    std::vector<location*> createLocationSample();
private:
    gameManager& gm;

    void locationSampler();
};

class gameDataCreation
{
public:
    gameDataCreation(gameManager& manager) : gm(manager) {}

    void gameDataGenerator();
    void loadEnemies();
    void loadCharacters();
    void loadLocations();
    void loadAbilities();
    void loadItems();
    void loadQuests();
    void setseed();
    void setClass();
    void locationAction();
    void unorderedMapMaker();

private:
    gameManager& gm;
};

class combat 
{
public:
    combat(gameManager& manager) : gm(manager) {}

    void basicCombat();
    std::vector<enemy*> createEnemySample();
    void abilityDamage(int damageOfAbility);
    void abilitySoulScream(int soulScreamAmount);
    void abilityDoT(int duration);
private:
    gameManager& gm;

    void selectEnemy();
    void combatTurn();
    void playerCombatTurn(bool& playerBlocking);
    void enemyIntent();
    void enemyCombatTurn();
    void endTurnEffects();
};

class activities
{
public:
    activities(gameManager& manager) : gm(manager) {}

    void shopEntry();
    void canPlayerBuy(int shopChoice);
    void leaveFunction();
    void getQuest();
private:
    gameManager& gm;
};

#endif