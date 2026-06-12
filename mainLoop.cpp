#include "consoleGame.hpp"

void mainGameLoop::mainLoop()
{
    splitOptions(multipleWaySplit(1));
}

int mainGameLoop::multipleWaySplit(int AmountOfRoadChoices)
{
    setupAndUtility util;

    util.vectorCreation(AmountOfRoadChoices);

    std::cout << "You arrive at a split in the road. There are " << AmountOfRoadChoices << " Possible ways. Which one will you take?\n";

    for (int i = 0; i < AmountOfRoadChoices; i++)
    {
        std::cout << "Road " << charPossibilities[i] << " to " << locations[i].name << ".\n";
    }

    char roadChoiceMain = util.correctInput();
    std::cout << "You chose road: " << roadChoiceMain << "\n";
    return roadChoiceMain - 'A';
}

void mainGameLoop::splitOptions(int chosenRoad)
{
    setupAndUtility util;

    util.vectorCreation(locations[chosenRoad].possibleActions.size());
    std::cout << "What do you want to do?\n";
    for (int i = 0; i < locations[chosenRoad].possibleActions.size(); i++)
    {
        std::cout << charPossibilities[i] << ". " << locations[chosenRoad].possibleActions[i].nameOfAction << "\n";
    }
    char chosenOption = util.correctInput();
    std::cout << "You chose option: " << chosenOption << "\n";
    if (locationActions.contains(locations[chosenRoad].possibleActions[chosenOption - 'A'].resultOfAction))
    {
        locationActions[locations[chosenRoad].possibleActions[chosenOption - 'A'].resultOfAction]();
    }
}