#include "consoleGame.hpp"

int playerCurrentHP = 1;
int playerCurrentGold = 0;
int currentCombatEnemyCurrentHP = 1;

uint32_t seedStart;

std::vector<action> actions;
std::vector<item> items;
std::vector<quest> quests;
character player;

std::map<std::string, std::function<void()>> locationActions;
std::map<std::string, int*> valueAndStatConnector;
std::map < std::string, std::function<void()>> abilityAttributeAssigner;
std::map<std::string, int*> factionAssigner;
std::map<std::string, int*> regionAssigner;

std::string region;
std::string faction;

std::string currentRegion;
std::vector<std::unique_ptr<structSearcher>> gamedataBase;

void gameDataCreation::gameDataGenerator()
{
    setupAndUtility util(gm);
    gamedataBase.reserve(gamedataBase.size() + gm.characters.size()+gm.enemies.size()+gm.locations.size()+items.size());

    util.addToDataBase<character>(gm.characters);
    util.addToDataBase<enemy>(gm.enemies);
    util.addToDataBase<location>(gm.locations);
    util.addToDataBase<item>(items);

    std::cout << "succesfully loaded " << gamedataBase.size() << " items to database." << "\n";
}

void gameDataCreation::loadEnemies()
{
    std::ifstream file("JSON/enemies.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");
    }
    json j;
    file >> j;

    gm.enemies = j.get<std::vector<enemy>>();

    for (const auto& c : gm.enemies) {
        std::cout << "Succesfully loaded enemies: " << c.name << " (max HP: " << c.hpMax << ")" << "\n";
        wait(20);
    }
    EMPTYSCREEN();
}

void gameDataCreation::loadCharacters()
{
    setupAndUtility util(gm);

    std::ifstream file("JSON/characters.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }

    json j;
    file >> j;

    gm.characters = j.get<std::vector<character>>();
    size_t amountOfCharacters = gm.characters.size();

    util.vectorCreation(amountOfCharacters);

    for (const auto& c : gm.characters) {
        std::cout << "Succesfully loaded characters: " << c.name << " (max HP: " << c.hpMax << ")" << "\n";
        wait(20);
    }
    EMPTYSCREEN();
}

void gameDataCreation::loadLocations()
{
    setupAndUtility util(gm);

    std::ifstream file("JSON/locations.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }

    json j;
    file >> j;

    gm.locations = j.get<std::vector<location>>();
    size_t amountOfLocations = gm.locations.size();

    util.vectorCreation(amountOfLocations);

    for (const auto& c : gm.locations) {
        std::cout << "Succesfully loaded locations: " << c.name << "\n";
        wait(20);
    }
    EMPTYSCREEN();
}

void gameDataCreation::loadAbilities()
{
    setupAndUtility util(gm);

    std::ifstream file("JSON/abilities.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }

    json j;
    file >> j;

    gm.abilities = j.get<std::vector<ability>>();
    size_t amountOfAbilities = gm.abilities.size();

    util.vectorCreation(amountOfAbilities);

    for (const auto& c : gm.abilities) {
        std::cout << "Succesfully loaded abilities: " << c.name << "\n";
        wait(20);
    }
    EMPTYSCREEN();
}

void gameDataCreation::loadItems()
{
    std::ifstream file("JSON/items.json");

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

void gameDataCreation::loadQuests()
{
    std::ifstream file("JSON/quests.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }
    json j;
    file >> j;

    quests = j.get<std::vector<quest>>();

    for (const auto& c : quests) {
        std::cout << "Succesfully loaded quests: " << c.name << " (quest type: " << c.type << ")" << "\n";
        wait(40);
    }
    EMPTYSCREEN();
}


void gameDataCreation::setseed()
{
    setupAndUtility util(gm);

    std::cout << "custom seed?\n";

    util.yesOrNoFunction();

    util.printAscii("warrior.txt");

    if (util.correctInput() == 'A')
    {
        std::cout << "Enter seed (8 numbers)\n";
        std::cin >> gm.seedValue;
    }
    else {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 99999999);

        gm.seedValue = dist6(rng);
    }
    std::cout << "Seed:" << std::setfill('0') << std::setw(8) << gm.seedValue << "\n";
    if(gm.seedValue==11111111)
    {
        playerCurrentGold = 100000;
    }
    seedStart = gm.seedValue;
    EMPTYSCREEN();
}

void gameDataCreation::setClass()
{
    setupAndUtility util(gm);

    util.vectorCreation(gm.characters.size());
    std::cout << "Choose a character:\n";

    for (int i = 0; i < gm.charPossibilities.size(); i++)
    {
        std::cout << gm.charPossibilities[i]
            << ". "
            << gm.characters[i].name << RED << " [ max HP: "
            << gm.characters[i].hpMax << BLUE << " | ATK: "
            << gm.characters[i].attack << RESET << "]\n";;
    }

    char playerChoice = util.correctInput();

    int characterChoiceInt = static_cast<int>(playerChoice) - 'A';

    std::cout << "You chose: " << gm.characters[characterChoiceInt].name << "\n";

    player = gm.characters[characterChoiceInt];

    if (player.name == "Wizard")
    {
        util.unlockAbility(gm.abilities[0]);
    }

    playerCurrentHP = player.hpMax;
}

void gameDataCreation::locationAction()
{  
    locationActions = 
    {
        {"basicCombat", [=]() {combat currentFight(gm); currentFight.basicCombat(); }},
        { "shopEntry", [=]() {activities activity(gm); activity.shopEntry(); }},
        {"leave", [=]() {activities activity(gm); activity.leaveFunction(); }},
        {"getQuest", [=]() {activities activity(gm); activity.getQuest(); }}
    };
}

void gameDataCreation::unorderedMapMaker()
{
    valueAndStatConnector =
    {
        {"hp", &player.hpMax},
        {"attack", &player.attack}
    };

    abilityAttributeAssigner =
    {
        {"damage",  [=]() {combat currentFight(gm); currentFight.abilityDamage(currentAbilityAmount); }},
        {"burn", [=]() {combat currentFight(gm); currentFight.abilityDoT(currentAbilitySpecialAmount); }}
    };
    /*
    factionAssigner =
    {
        {"city", }
    };
    regionAssigner =
    {
        {"street", }
    };
    */
}

