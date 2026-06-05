// functies.h
#ifndef consoleGame_HPP
#define consoleGame_HPP

#include <unordered_map>
#include <string>
#include <functional>

class gameDataCreation
{
public:
    gameDataCreation();

    std::unordered_map<std::string, std::function<void()>> locationActions;
};

#endif