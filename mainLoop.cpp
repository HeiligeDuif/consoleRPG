#include "consoleGame.hpp"
int proximityCounter = 0;

void mainGameLoop::mainLoop()
{
    splitOptions(multipleWaySplit(2));
}

int mainGameLoop::multipleWaySplit(int AmountOfRoadChoices)
{
    setupAndUtility util;

    util.vectorCreation(AmountOfRoadChoices);

    std::cout << "You arrive at a split in the road. There are " << AmountOfRoadChoices << " Possible ways. Which one will you take?\n";

    for (int i = 0; i < AmountOfRoadChoices; i++)
    {
        std::cout << charPossibilities[i] << ". " << "Road " << charPossibilities[i] << " to " << locations[i].name << ".\n";
    }

    char roadChoiceMain = util.correctInput();
    std::cout << "You chose road: " << roadChoiceMain << "\n";
    return roadChoiceMain - 'A';
}

void mainGameLoop::splitOptions(int chosenRoad)
{
    do
    {
        currentRegion = locations[chosenRoad].name;

        if (locations[chosenRoad].beenHere == false)
        {
            locations[chosenRoad].beenHere = true;
            proximityCounter++;
        }

        setupAndUtility util;

        util.vectorCreation(locations[chosenRoad].possibleActions.size());
        std::cout << "What do you want to do?\n";
        for (int i = 0; i < locations[chosenRoad].possibleActions.size(); i++)
        {
            std::cout << charPossibilities[i] << ". " << locations[chosenRoad].possibleActions[i].nameOfAction << "\n";
        }
        char chosenOption = util.correctInput();
        std::cout << "You chose option: " << chosenOption << "\n";
        //should be split up
        if (locationActions.contains(locations[chosenRoad].possibleActions[chosenOption - 'A'].resultOfAction))
        {
            locationActions[locations[chosenRoad].possibleActions[chosenOption - 'A'].resultOfAction](); //execute function tied to string in JSON file
        }
        else
        {
            std::cout << "No action found [error]";
        }
    } while (leaving != true);
    leaving = false;
}

void mainGameLoop::locationSampler()
{
    int randomAmountOfRoads;
    setupAndUtility util;
    randomAmountOfRoads = util.seedIteration(4);
}

std::vector<location*> mainGameLoop::createLocationSample()
{
    setupAndUtility util;
    std::vector<location*> filteredLocations = util.filterGameData<location>(gamedataBase, [](const location* e)
        {
            return e->proximity >= proximityCounter;
        });
    return filteredLocations;
}
