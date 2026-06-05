#include <unordered_map>
#include <string>
#include <functional>
#include "consoleGame.hpp"

void basicCombat() 
{

}

void shopEntry()
{

}

gameDataCreation::gameDataCreation()
{
    locationActions = {
        {"basicCombat", basicCombat},
        { "shopEntry", shopEntry }
    };
}
