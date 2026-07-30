#include "consoleGame.hpp"

std::vector<std::unique_ptr<structSearcher>> gamedataBase;

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
std::map<std::string, ability*> abilityAssigner;
std::map<std::string, quest*> questAssigner;
std::map<std::string, int*> factionAssigner;
std::map<std::string, int*> regionAssigner;

std::string region;
std::string faction;

std::string currentRegion;

void gameDataCreation::gameDataGenerator()
{
    setupAndUtility util(gm);
    gamedataBase.reserve(gamedataBase.size() + gm.characters.size()+gm.enemies.size()+gm.locations.size()+items.size());

    util.addToDataBase<character>(gm.characters);
    util.addToDataBase<enemy>(gm.enemies);
    util.addToDataBase<location>(gm.locations);
    util.addToDataBase<item>(items);

    printw("succesfully loaded %zu items to database. \n", gamedataBase.size());
    refresh();
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
        printw("Succesfully loaded enemies: %s (max HP: %d) \n",c.name.c_str(), c.hpMax);
        refresh();
        wait(20);
    }
    clear();
    refresh();
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
        printw("Succesfully loaded characters: %s", c.name.c_str());
        attron(COLOR_PAIR(1));
        printw("(max HP : % d) \n", c.hpMax);
        attroff(COLOR_PAIR(1));
        wait(20);
        refresh();
    }
    clear();
    refresh();
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
        printw("Succesfully loaded locations: %s\n",c.name.c_str());
        wait(20);
    }
    clear();
    refresh();
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
        printw("Succesfully loaded abilities: %s\n", c.name.c_str());
        wait(40);
        refresh();
    }
    for (auto& ab : gm.abilities) {
        abilityAssigner[ab.name] = &ab;
    }
    clear();
    refresh();
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
        printw("Succesfully loaded items : %s (bonus type : %i) \n", c.name.c_str(), c.bonus.c_str());
        wait(40);
        refresh();
    }
    clear();
    refresh();
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
        printw("Succesfully loaded quests: %s (quest type: %s) \n", c.name.c_str(), c.type.c_str());
        wait(40);
        refresh();
    }
    clear();
    refresh();

    for (auto& ab : quests) {
        questAssigner[ab.name] = &ab;
    }
}


void gameDataCreation::setseed()
{
    setupAndUtility util(gm);

    printw("custom seed?\n");
    refresh();

    util.yesOrNoFunction();

    util.printAscii("warrior.txt");

    if (util.correctInput() == 'A')
    {
        printw("Enter seed (8 numbers)\n");
        refresh();
        echo();
        scanw("%d", &gm.seedValue);
        noecho();
    }
    else {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 99999999);

        gm.seedValue = dist6(rng);
    }
    printw("Seed:%08d\n", gm.seedValue);
    refresh();

    if(gm.seedValue==11111111)
    {
        playerCurrentGold = 100000;
    }
    seedStart = gm.seedValue;
    clear();
    refresh();
}

void gameDataCreation::setClass()
{
    setupAndUtility util(gm);

    util.vectorCreation(gm.characters.size());
    printw("Choose a character:\n");

    for (int i = 0; i < gm.charPossibilities.size(); i++)
    {
        printw("%c. %s", gm.charPossibilities[i], gm.characters[i].name.c_str());

        attron(COLOR_PAIR(1));
        printw(" [ max HP: %d", gm.characters[i].hpMax);
        attroff(COLOR_PAIR(1));

        attron(COLOR_PAIR(2));
        printw(" | ATK: %d", gm.characters[i].attack);

        printw("]\n");
        attroff(COLOR_PAIR(2));
    }
    refresh();

    char playerChoice = util.correctInput();

    int characterChoiceInt = static_cast<int>(playerChoice) - 'A';

printw:("You chose: %s \n", gm.characters[characterChoiceInt].name.c_str());
    refresh();

    player = gm.characters[characterChoiceInt];

    if (player.startingAbility.has_value())
    {
        util.unlockAbility(*abilityAssigner[player.startingAbility.value()]);
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
        {"damage",  [=]() {combat currentFight(gm); currentFight.abilityDamage(gm.currentAbilityAmount); }},
        {"soulScream",  [=]() {combat currentFight(gm); currentFight.abilitySoulScream(gm.currentAbilitySpecialAmount); }},
        {"burn", [=]() {combat currentFight(gm); currentFight.abilityDoT(gm.currentAbilitySpecialAmount); }}
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

