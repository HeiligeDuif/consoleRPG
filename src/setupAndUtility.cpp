#include "consoleGame.hpp"


void setupAndUtility::setConsoleOutputUTF8()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void setupAndUtility::printAscii(const std::string& fileName)
{
    std::string path = "graphics/" + fileName;
    std::ifstream file(path);

    if (!file) {
        mvprintw(0, 0, "ERROR: Can't find %s!", path.c_str());
        refresh();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        printw("%s\n", line.c_str());
    }

    refresh();
}

void setupAndUtility::vectorCreation(size_t amountOfChoices)
{
    gm.charPossibilities.resize(amountOfChoices);
    for (int i = 0; i < amountOfChoices; i++)
    {
        gm.charPossibilities[i] = static_cast<char>('A' + i);;
    }
}

char  setupAndUtility::correctInput(int menuStartY)
{
    bool succesfulInput = false;
    while (true)
    {
        int input = getch();
        if (input == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) != OK)
                continue;

            if (!(event.bstate & BUTTON1_CLICKED))
                continue;

            int row = event.y - menuStartY;

            if (row >= 0 && row < gm.charPossibilities.size())
            {
                clear();
                refresh();
                return gm.charPossibilities[row];
            }
        }

        char inputUpper = std::toupper(static_cast<unsigned char>(input));

        for (int i = 0; i < gm.charPossibilities.size(); i++)
        {
            if (inputUpper != gm.charPossibilities[i])
            {
                if (i == gm.charPossibilities.size() - 1)
                {
                    attron(COLOR_PAIR(1));
                    printw("Please enter an available value.\n");
                    attroff(COLOR_PAIR(1));
                    succesfulInput = false;
                }
            }
            else
            {
                succesfulInput = true;
                clear();
                refresh();
                return inputUpper;
            }
        }
    }
}

void setupAndUtility::yesOrNoFunction()
{
    setupAndUtility util(gm);
    util.vectorCreation(yesOrNo.size());

    int menuStartY, dummyX;
    getyx(stdscr, menuStartY, dummyX);

    for (int i = 0; i < yesOrNo.size(); i++)
    {
        printw("%c. %s \n", gm.charPossibilities[i], yesOrNo[i].c_str());
        refresh();
    }
}

int setupAndUtility::seedIteration(int divisionAmount)
{
    gm.seedValue= (gm.seedValue * 1664525) + 1013904223; //Definitely didn't steal this
    double seedDivision;
    seedDivision = gm.seedValue / 4294967296.0; //divide by max uint_32 value
    int randomizedOutput;
    randomizedOutput = seedDivision * divisionAmount;
    return randomizedOutput;
}

void setupAndUtility::unlockAbility(ability newAbility)
{
    gm.equippedAbilities.push_back(newAbility);
}