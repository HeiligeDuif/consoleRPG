#include "consoleGame.hpp"
int seedValue = 0;
int amountOfChoices = 0;
int playerCurrentHP = 1;
int currentCombatEnemyCurrentHP = 1;

std::vector<char> charPossibilities;
std::vector<character> characters;
std::vector<enemy> enemies;
std::vector<location> locations;
std::vector<action> actions;
character player;

void setupAndUtility::startGame()
{
    setConsoleOutputUTF8();
    loadEnemies();
    loadCharacters();
    loadLocations();

    setseed();

    setClass();

    std::cout << "You are a traveller from another world...\n" << "Your only purpose is to survive...\n" << "Good luck.\n";

    mainGameLoop.mainLoop();
}

void setupAndUtility::setConsoleOutputUTF8()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void setupAndUtility::printAscii(std::string fileName) {
    std::string path = "graphics/" + fileName;
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "ERROR: Can't find " << path << " !" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }

    file.close();
}

void setupAndUtility::vectorCreation(size_t amountOfChoices)
{
    charPossibilities.resize(amountOfChoices);
    for (int i = 0; i < amountOfChoices; i++)
    {
        charPossibilities[i] = static_cast<char>('A' + i);;
    }
}

char  setupAndUtility::correctInput()
{
    bool succesfulInput = false;
    do
    {
        char input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        char inputUpper = std::toupper(input);

        for (int i = 0; i < charPossibilities.size(); i++)
        {
            if (inputUpper != charPossibilities[i])
            {
                if (i == charPossibilities.size() - 1)
                {
                    std::cout << RED << "Please enter an available value.\n" << RESET;
                    succesfulInput = false;
                }
            }
            else
            {
                succesfulInput = true;
                EMPTYSCREEN();
                return inputUpper;
            }
        }
    } while (succesfulInput != true);
}

void setupAndUtility::loadEnemies()
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

void setupAndUtility::loadCharacters()
{
    std::ifstream file("characters.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }

    json j;
    file >> j;

    characters = j.get<std::vector<character>>();
    size_t amountOfCharacters = characters.size();

    vectorCreation(amountOfCharacters);

    for (const auto& c : characters) {
        std::cout << "Succesfully loaded characters: " << c.name << " (max HP: " << c.hpMax << ")" << "\n";
        wait(20);
    }
    EMPTYSCREEN();
}

void setupAndUtility::loadLocations()
{
    std::ifstream file("locations.json");

    if (!file.is_open()) {
        throw std::runtime_error("Can't find JSON file!");;
    }

    json j;
    file >> j;

    locations = j.get<std::vector<location>>();
    size_t amountOfLocations = locations.size();

    vectorCreation(amountOfLocations);

    for (const auto& c : locations) {
        std::cout << "Succesfully loaded locations: " << c.name << "\n";
        wait(20);
    }
    EMPTYSCREEN();
}

void setupAndUtility::setseed()
{
    std::cout << "custom seed?\n";
    std::vector<std::string> yesOrNo =
    {
        "Yes",
        "No"
    };

    vectorCreation(yesOrNo.size());

    for (int i = 0; i < yesOrNo.size(); i++)
    {
        std::cout << charPossibilities[i] << ". " << yesOrNo[i] << "\n";
    }

    printAscii("warrior.txt");

    if (correctInput() == 'A')
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
    EMPTYSCREEN();
}

void setupAndUtility::setClass()
{
    vectorCreation(characters.size());
    std::cout << "Choose a character:\n";
    for (int i = 0; i < charPossibilities.size(); i++)
    {
        std::cout << charPossibilities[i]
            << ". "
            << characters[i].name << RED << " [ max HP: "
            << characters[i].hpMax << BLUE << " | ATK: "
            << characters[i].attack << RESET << "]\n";;
    }

    char playerChoice = correctInput();

    int characterChoiceInt = static_cast<int>(playerChoice) - 'A';
    //std::cout << characterChoiceInt;

    std::cout << "You chose: " << characters[characterChoiceInt].name << "\n";

    player = characters[characterChoiceInt];

    playerCurrentHP = player.hpMax;
}