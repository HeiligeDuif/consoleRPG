#include "consoleGame.hpp"

void shopEntry()
{

}

gameDataCreation::gameDataCreation()
{
    locationActions = {
        {"basicCombat", []() {Combat currentFight; currentFight.basicCombat(); }},
        { "shopEntry", shopEntry }
    };
}
