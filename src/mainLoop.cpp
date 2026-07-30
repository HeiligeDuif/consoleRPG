#include "consoleGame.hpp"
int proximityCounter = 0;

void mainGameLoop::mainLoop()
{
    splitOptions(multipleWaySplit(2));
}

int mainGameLoop::multipleWaySplit(int AmountOfRoadChoices)
{
    setupAndUtility util(gm);

    util.vectorCreation(AmountOfRoadChoices);

    printw("You arrive at a split in the road. There are %i Possible ways. Which one will you take?\n", AmountOfRoadChoices);

    for (int i = 0; i < AmountOfRoadChoices; i++)
    {
        printw("%c. to the %s.\n",gm.charPossibilities[i], gm.locations[i].name.c_str());
    }

    char roadChoiceMain = util.correctInput();
    int roadChoiceInt = roadChoiceMain - 65;
    printw("You chose the road to the %s \n", gm.locations[roadChoiceInt].name.c_str());
    return roadChoiceMain - 'A';
}

void mainGameLoop::splitOptions(int chosenRoad)
{
    do
    {
        currentRegion = gm.locations[chosenRoad].name;

        if (gm.locations[chosenRoad].beenHere == false)
        {
            gm.locations[chosenRoad].beenHere = true;
            proximityCounter++;
        }

        setupAndUtility util(gm);

        util.vectorCreation(gm.locations[chosenRoad].possibleActions.size());
        printw("What do you want to do?\n");
        for (int i = 0; i < gm.locations[chosenRoad].possibleActions.size(); i++)
        {
            printw("%c. %s \n", gm.charPossibilities[i], gm.locations[chosenRoad].possibleActions[i].nameOfAction.c_str());
        }
        char chosenOption = util.correctInput();
        printw("You chose option: %c \n", chosenOption);
        
        if (locationActions.contains(gm.locations[chosenRoad].possibleActions[chosenOption - 'A'].resultOfAction))
        {
            locationActions[gm.locations[chosenRoad].possibleActions[chosenOption - 'A'].resultOfAction](); //execute function tied to string in JSON file
        }
        else
        {
            printw("No action found [error]");
        }
    } while (gm.leaving != true);
    gm.leaving = false;
}

void mainGameLoop::locationSampler()
{
    int randomAmountOfRoads;
    setupAndUtility util(gm);
    randomAmountOfRoads = util.seedIteration(4);
}

std::vector<location*> mainGameLoop::createLocationSample()
{
    setupAndUtility util(gm);
    std::vector<location*> filteredLocations = util.filterGameData<location>(gamedataBase, [](const location* e)
        {
            return e->proximity >= proximityCounter;
        });
    return filteredLocations;
}
