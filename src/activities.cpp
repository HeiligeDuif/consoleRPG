#include "consoleGame.hpp"

void activities::shopEntry()
{
    setupAndUtility util(gm);

    std::vector<item*> buyableItems = util.filterGameData<item>(gamedataBase, [](const item* currentItem)
        {
            return !currentItem->bought.value_or(false);
        });

    printw("Welcome to the shop!\n");
    printw("We have these items for sale:\n");
    printw("You have: %i gold. \n", playerCurrentGold);
    refresh();
    util.vectorCreation(buyableItems.size());

    for (int i = 0; i < buyableItems.size(); i++)
    {
        printw("%s, gives %d %s.\n",
            buyableItems[i]->name.c_str(),
            buyableItems[i]->value,
            buyableItems[i]->bonus.c_str());
        refresh();
    }

    printw("Do you want to buy something? \n");
    refresh();

    int menuStartY, dummyX;
    getyx(stdscr, menuStartY, dummyX);

    util.yesOrNoFunction();
    if (util.correctInput(menuStartY) == 'A')
    {
        printw("Which item? \n");
        refresh();
        int shopOptions = buyableItems.size()+1;
        util.vectorCreation(shopOptions); //reset after yesOrNo changed it

        int menuStartY, dummyX;
        getyx(stdscr, menuStartY, dummyX);

        for (int i = 0; i < buyableItems.size(); i++)

        {
            printw("%c. %s for %d gold.\n",
                gm.charPossibilities[i],
                buyableItems[i]->name.c_str(),
                buyableItems[i]->price);
            refresh();
        }

        printw("%c. Nevermind \n", gm.charPossibilities[static_cast<int>(buyableItems.size())]);
        refresh();

        int shopChoiceInt;

        char shopChoice = util.correctInput(menuStartY);
        shopChoiceInt = static_cast<int>(shopChoice-'A');
        if (shopChoiceInt == buyableItems.size())
        {
            return;
        }
        else {
            canPlayerBuy(shopChoiceInt, buyableItems);
        }
    }
    else 
    {
        //return to the village
    }
}

void activities::canPlayerBuy(int shopChoice, const std::vector<item*>& buyableItems)
{
    if (buyableItems[shopChoice]->price <= playerCurrentGold)
    {
        playerCurrentGold = playerCurrentGold - buyableItems[shopChoice]->price;
        if (valueAndStatConnector.contains(buyableItems[shopChoice]->bonus))
        {
            *valueAndStatConnector[buyableItems[shopChoice]->bonus] = *valueAndStatConnector[buyableItems[shopChoice]->bonus] + buyableItems[shopChoice]->value;
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

        if (buyableItems[shopChoice]->bought.has_value()) {
            buyableItems[shopChoice]->bought = true;
        }

        printw("You know have: %d %s.\n",
            *valueAndStatConnector[buyableItems[shopChoice]->bonus],
            buyableItems[shopChoice]->bonus.c_str());
        
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