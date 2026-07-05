#include "consoleGame.hpp"
void setupAndUtility::startGame()
{
    gameDataCreation gameData;
    setConsoleOutputUTF8();
    gameData.loadEnemies();
    gameData.loadCharacters();
    gameData.loadLocations();
    gameData.locationAction();
    
    gameData.loadItems();
    gameData.unorderedMapMaker();

    gameData.setseed();

    gameData.setClass();

    std::cout << "You are a traveller from another world...\n" << "Your only purpose is to survive...\n" << "Good luck.\n";

    mainGameLoop mainLoop;
    while (playerCurrentHP > 0)
    {
        mainLoop.mainLoop();
    }
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

void setupAndUtility::yesOrNoFunction()
{
    setupAndUtility util;
    util.vectorCreation(yesOrNo.size());

    for (int i = 0; i < yesOrNo.size(); i++)
    {
        std::cout << charPossibilities[i] << ". " << yesOrNo[i] << "\n";
    }
}

int setupAndUtility::seedIteration(int divisionAmount)
{
    seedValue= (seedValue * 1664525) + 1013904223; //Definitely didn't steal this
    double seedDivision;
    seedDivision = seedValue / 4294967296.0; //divide by max uint_32 value
    int randomizedOutput;
    randomizedOutput = seedDivision * divisionAmount;
    return randomizedOutput;
}

void setupAndUtility::sampleMaker(std::vector<structSearcher> list)
{
    
}