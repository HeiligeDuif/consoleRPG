#include "consoleGame.hpp"


int amountOfChoices = 0;
int playerCurrentHP = 1;
int playerCurrentGold = 0;
int currentCombatEnemyCurrentHP = 1;
uint32_t seedValue;
uint32_t seedStart;

std::vector<char> charPossibilities;
std::vector<character> characters;
std::vector<enemy> enemies;
std::vector<location> locations;
std::vector<action> actions;
std::vector<item> items;
character player;

std::map<std::string, std::function<void()>> locationActions;
std::map<std::string, int*> valueAndStatConnector;

void gameDataCreation::loadEnemies()
{
    std::ifstream file("enemies.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }
    json j;
    file >> j;

    enemies = j.get<std::vector<enemy>>();

    for (const auto& c : enemies) {
        std::cout << "Succesfully loaded enemies: " << c.name << " (max HP: " << c.hpMax << ")" << "\n";
        wait(20);
    }
    EMPTYSCREEN();
}

void gameDataCreation::loadCharacters()
{
    setupAndUtility util;

    std::ifstream file("characters.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }

    json j;
    file >> j;

    characters = j.get<std::vector<character>>();
    size_t amountOfCharacters = characters.size();

    util.vectorCreation(amountOfCharacters);

    for (const auto& c : characters) {
        std::cout << "Succesfully loaded characters: " << c.name << " (max HP: " << c.hpMax << ")" << "\n";
        wait(20);
    }
    EMPTYSCREEN();
}

void gameDataCreation::loadLocations()
{
    setupAndUtility util;

    std::ifstream file("locations.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }

    json j;
    file >> j;

    locations = j.get<std::vector<location>>();
    size_t amountOfItems = locations.size();

    util.vectorCreation(amountOfItems);

    for (const auto& c : locations) {
        std::cout << "Succesfully loaded locations: " << c.name << "\n";
        wait(20);
    }
    EMPTYSCREEN();
}

void gameDataCreation::loadItems()
{
    std::ifstream file("items.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }
    json j;
    file >> j;

    items = j.get<std::vector<item>>();

    for (const auto& c : items) {
        std::cout << "Succesfully loaded items: " << c.name << " (bonus type: " << c.bonus << ")" << "\n";
        wait(40);
    }
    EMPTYSCREEN();
}

void gameDataCreation::setseed()
{
    setupAndUtility util;

    std::cout << "custom seed?\n";

    util.yesOrNoFunction();

    util.printAscii("warrior.txt");

    if (util.correctInput() == 'A')
    {
        std::cout << "Enter seed (8 numbers)\n";
        std::cin >> seedValue;
    }
    else {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 99999999);

        seedValue = dist6(rng);
    }
    std::cout << "Seed:" << std::setfill('0') << std::setw(8) << seedValue << "\n";
    if(seedValue==11111111)
    {
        playerCurrentGold = 100000;
    }
    seedStart = seedValue;
    EMPTYSCREEN();
}

void gameDataCreation::setClass()
{
    setupAndUtility util;

    util.vectorCreation(characters.size());
    std::cout << "Choose a character:\n";
    for (int i = 0; i < charPossibilities.size(); i++)
    {
        std::cout << charPossibilities[i]
            << ". "
            << characters[i].name << RED << " [ max HP: "
            << characters[i].hpMax << BLUE << " | ATK: "
            << characters[i].attack << RESET << "]\n";;
    }

    char playerChoice = util.correctInput();

    int characterChoiceInt = static_cast<int>(playerChoice) - 'A';
    //std::cout << characterChoiceInt;

    std::cout << "You chose: " << characters[characterChoiceInt].name << "\n";

    player = characters[characterChoiceInt];

    playerCurrentHP = player.hpMax;
}

void gameDataCreation::locationAction()
{
    
    locationActions = 
    {
        {"basicCombat", []() {combat currentFight; currentFight.basicCombat(); }},
        { "shopEntry", []() {activities activity; activity.shopEntry(); }}
    };
}

void gameDataCreation::valueAndStatConnectorFunction()
{
    valueAndStatConnector =
    {
        {"hp", &player.hpMax},
        {"attack", &player.attack}
    };
}

