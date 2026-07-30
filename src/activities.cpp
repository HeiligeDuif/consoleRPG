#include "consoleGame.hpp"

void activities::shopEntry()
{
    setupAndUtility util(gm);
    printw("Welcome to the shop!\n");
    printw("We have these items for sale:\n");
    refresh();
    util.vectorCreation(items.size());
    for (int i = 0; i < items.size(); i++)
    {
        printw("%s, gives %d %s.\n",
            items[i].name.c_str(),
            items[i].value,
            items[i].bonus.c_str());
        refresh();
    }

    printw("Do you want to buy something? \n");
    refresh();

    util.yesOrNoFunction();
    if (util.correctInput() == 'A')
    {
        printw("Which item? \n");
        refresh();
        int shopOptions = items.size()+1;
        util.vectorCreation(shopOptions); //reset after yesOrNo changed it
        for (int i = 0; i < items.size(); i++)
        {
            printw("%c. %s for %d gold.\n",
                gm.charPossibilities[i],
                items[i].name.c_str(),
                items[i].price);
            refresh();
        }

        printw("%c. Nevermind \n", gm.charPossibilities[static_cast<int>(items.size())]);
        refresh();

        int shopChoiceInt;

        char shopChoice = util.correctInput();
        shopChoiceInt = static_cast<int>(shopChoice-'A');
        if (shopChoiceInt == items.size()) 
        {
            return;
        }
        else {
            canPlayerBuy(shopChoiceInt);
        }
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
            attron(COLOR_PAIR(1));
            printw("ERROR: literally unplayable, devs pls fix\n");
            attroff(COLOR_PAIR(1));
            refresh();
        }
        printw("You now have: %i gold. \n", playerCurrentGold);
        printw("You know have: %d %s.\n",
            *valueAndStatConnector[items[shopChoice].bonus],
            items[shopChoice].bonus.c_str());
        refresh();
    }
    else
    {
        printw("You don't have enough money, earn some by defeating some monsters.\n");
        refresh();
    }
}

void activities::leaveFunction()
{
    gm.leaving = true;
}

void activities::getQuest()
{

}