#include "consoleGame.hpp"

int shopChoiceInt;

void activities::shopEntry()
{
    setupAndUtility util;
    std::cout << "Welcome to the shop!\n";
    std::cout << "We have these items for sale:" << "\n";
    util.vectorCreation(items.size());
    for (int i = 0; i < items.size(); i++)
    {
        std::cout << items[i].name << ", gives " << items[i].value << " " << items[i].bonus << "." << "\n";
    }

    std::cout << "Do you want to buy something?" << "\n";

    util.yesOrNoFunction();
    if (util.correctInput() == 'A')
    {
        std::cout << "Which item?" << "\n";
        util.vectorCreation(items.size() + 1); //reset after yesOrNo changed it
        for (int i = 0; i < items.size(); i++)
        {
            std::cout << charPossibilities[i] << ". " << items[i].name << " for " << items[i].price << "\n";
        }

        std::cout <<  charPossibilities.size();
        std::cout << charPossibilities[static_cast<int>(items.size())] << ". " << "Nevermind";

        char shopChoice = util.correctInput();
        shopChoiceInt = static_cast<int>(shopChoice);
    }
    else
    {
        //return to the village
    }
}

void activities::canPlayerBuy(int price) 
{

}