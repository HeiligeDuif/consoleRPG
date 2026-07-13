#include "consoleGame.hpp"
bool leaving = false;

void activities::shopEntry()
{
    setupAndUtility util;
    std::cout << "Welcome to the shop!\n";
    std::cout << "We have these items for sale:" << "\n"; //verteld dat de dingen te koop zijn
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
            std::cout << charPossibilities[i] << ". " << items[i].name << " for " << items[i].price << " gold." << "\n";
        }

        std::cout << charPossibilities[static_cast<int>(items.size())] << ". " << "Nevermind" << "\n";

        int shopChoiceInt;

        char shopChoice = util.correctInput();
        shopChoiceInt = static_cast<int>(shopChoice-'A');
        canPlayerBuy(shopChoiceInt);
    }
    else 
    {
        //return to the village
    }
}

void activities::canPlayerBuy(int shopChoice) 
{
    if (items[shopChoice].price < playerCurrentGold)
    {
        playerCurrentGold = playerCurrentGold - items[shopChoice].price;
        if (valueAndStatConnector.contains(items[shopChoice].bonus))
        {
            *valueAndStatConnector[items[shopChoice].bonus] = *valueAndStatConnector[items[shopChoice].bonus] + items[shopChoice].value;
            //pointer to specific player struct
        }
        else
        {
            std::cout << RED << "ERROR: literally unplayable, devs pls fix" << "\n" << RESET;
        }
        std::cout << "You now have: " << playerCurrentGold << " gold." << "\n";
        std::cout << "You know have: " << *valueAndStatConnector[items[shopChoice].bonus] << " " << items[shopChoice].bonus << "." << "\n";
    }
    else
    {
        std::cout << "You don't have enough money, earn some by defeating some monsters." << "\n";
    }
}

void activities::leaveFunction()
{
    leaving = true;
}