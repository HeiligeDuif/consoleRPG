#include <iostream>
#include <json/single_include/nlohmann/json.hpp>
#include <fstream>
#include <array>
#include <vector>
#include <cctype>
#include <string>
#include <limits>
#include <stdlib.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iomanip>
#include <chrono>
#include <thread>

//text colours
const char* RED = "\033[31m";
const char* BLUE = "\033[34m";
const char* RESET = "\033[0m";

void EMPTYSCREEN() 
{
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::cout << "\033[2J\033[H" << std::flush;
}

void wait(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

using json = nlohmann::json;

struct character{
  std::string name = "name";
  int hpMax = 0;
  int attack = 0;
};

struct enemy 
{
  std::string name = "name";
  int hpMax = 0;
  int attack = 0;

};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(character, name, hpMax, attack)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(enemy, name, hpMax, attack)

class choiceManager
{
public:
    uint32_t seedValue = 0;
    int amountOfChoices = 0;
    std::vector<char> charPossibilities;

    std::vector<character> characters;
    std::vector<enemy> enemies;

    enemy currentCombatEnemy;

    character player;

    int playerCurrentHP = 1;
	int currentCombatEnemyCurrentHP = 1;

    void startGame()
    {
        loadEnemies();
        loadCharacters();

        setseed();

        setClass();

        std::cout << "You are a traveller from another world...\n" << "Your only purpose is to survive...\n" << "Good luck.\n";
        
        mainLoop();
    }

private:
    void setseed() 
    {
        std::cout << "custom seed?\n";
        std::vector<std::string> yesOrNo =
        {
            "Yes",
            "No"
        };

        amountOfChoices = yesOrNo.size();
        vectorCreation();

        for (int i = 0; i < yesOrNo.size(); i++)
        {
            std::cout << charPossibilities[i] << ". " << yesOrNo[i] << "\n";
        }

        if (correctInput() == 'A')
        {
            std::cout << "Enter seed (8 numbers)\n";
            std::cin >> seedValue;
        }else{
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 99999999);

            seedValue = dist6(rng);
        }
        std::cout << "Seed:" << std::setfill('0') << std::setw(8) << seedValue << "\n";
        EMPTYSCREEN();
    }

    void vectorCreation()
    {
        charPossibilities.resize(amountOfChoices);
        for (int i = 0; i < amountOfChoices; i++)
        {
            charPossibilities[i] = static_cast<char>('A' + i);;
        }
    }

    void loadCharacters()
    {
        std::ifstream file("characters.json");

        if (!file.is_open()) {
            throw std::runtime_error("Can't find JSON file!");;
        }

        json j;
        file >> j;

        characters = j.get<std::vector<character>>();
        size_t amountOfCharacters = characters.size();
        amountOfChoices = amountOfCharacters;
        vectorCreation();

        for (const auto& c : characters) {
            std::cout << "Succesfully loaded characters: " << c.name << " (max HP: " << c.hpMax << ")" << "\n";
            wait(20);
        }
        EMPTYSCREEN();
    }

    void setClass()
    {
        std::cout << "Choose a character:\n";
        for (int i = 0; i < charPossibilities.size(); i++)
        {
            std::cout << charPossibilities[i]
                << ". "
                << characters[i].name << RED << " [ max HP: "
                << characters[i].hpMax << BLUE <<" | ATK: "
                << characters[i].attack << RESET << "]\n";;
        }

        char playerChoice = correctInput();

        int characterChoiceInt = static_cast<int>(playerChoice) - 'A';
        //std::cout << characterChoiceInt;

        std::cout << "You chose: " << characters[characterChoiceInt].name << "\n";

        player.name = characters[characterChoiceInt].name;
        player.hpMax = characters[characterChoiceInt].hpMax;
        player.attack = characters[characterChoiceInt].attack;

		playerCurrentHP = player.hpMax;
    }

    void loadEnemies()
    {
        std::ifstream file("enemies.json");

        if (!file.is_open()) {
            throw std::runtime_error("Can't find JSON file!");;
        }
        json j;
        file >> j;

        enemies = j.get<std::vector<enemy>>();

        for (const auto& c : enemies) {
            std::cout << "Succesfully loaded enemies: " << c.name << " (max HP: " << c.hpMax << ")" << "\n";
            wait(20);
        }
    }

    char  correctInput()
    {
        bool succesfulInput = false;
        do
        {
            char input;
            std::cin >> input;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            char inputUpper = std::toupper(input);

            for (int i = 0; i < charPossibilities.size(); i++)
            {
                if (inputUpper != charPossibilities[i])
                {
                    if (i == charPossibilities.size() - 1)
                    {
                        std::cout << RED << "Please enter an available value.\n" << RESET;
                        succesfulInput = false;
                    }
                }
                else
                {
                    succesfulInput = true;
                    EMPTYSCREEN();
                    return inputUpper;
                }
            }
        } while (succesfulInput != true);
    }

    void mainLoop()
    {
        multipleWaySplit(3);
		basicCombat();
    }

    void multipleWaySplit(int AmountOfRoadChoices)
    {
        amountOfChoices = AmountOfRoadChoices;
        vectorCreation();

        std::cout << "You arrive at a split in the road. There are " << amountOfChoices << " Possible ways. Which one will you take?\n";

        for (int i = 0; i < amountOfChoices; i++)
        {
            std::cout << "Road " << charPossibilities[i] << ".\n";
        }

        char roadChoiceMain = correctInput();
        std::cout << "You chose road: " << roadChoiceMain << "\n";
        return;
    }

    void basicCombat()
    {
        std::cout << "You see an enemy. Naturally you want to kill it. \n";
		selectEnemy();
        while (true)
        {
            combatTurn();
            if (playerCurrentHP <= 0 || currentCombatEnemyCurrentHP <= 0)
            {
                if (playerCurrentHP >= 1)
                {
                    std::cout << "You have succesfully defeated the enemy!\n";
                }
                else {
                    std::cout << "You have been defeated by: " << currentCombatEnemy.name << "!\n";
                }
                break;
            }
        }

    }

    void selectEnemy()
    {
		currentCombatEnemy = enemies[0];
		currentCombatEnemyCurrentHP = currentCombatEnemy.hpMax;
        
    }

    void combatTurn()
    {
        bool playerBlocking = false;
        playerCombatTurn(playerBlocking);

        if (currentCombatEnemyCurrentHP <= 0)
        {
            currentCombatEnemyCurrentHP = 0;
            std::cout << "The enemy is dead!\n";
            return;
        }

        std::cout << "The enemy has " << currentCombatEnemyCurrentHP << " HP left.\n";
        if (playerBlocking == true) 
        {
            std::cout << "The " << currentCombatEnemy.name << " attacks you for " << currentCombatEnemy.attack/2 << " damage.\n";

            playerCurrentHP -= currentCombatEnemy.attack/2;
        }
        else {
            std::cout << "The " << currentCombatEnemy.name << " attacks you for " << currentCombatEnemy.attack << " damage.\n";

            playerCurrentHP -= currentCombatEnemy.attack;
        }

        std::cout << "you have " << playerCurrentHP << " HP left.\n";
	}

    void playerCombatTurn(bool& playerBlocking)
    {
        std::vector<std::string> playerCombatOptions =
        {
            "Attack",
            "Defend",
            "Ability"
        };

        amountOfChoices = playerCombatOptions.size();
        vectorCreation();

        std::cout << "It is your turn, choose your action:\n";
        for (int i = 0; i < playerCombatOptions.size(); i++)
        {
            std::cout << charPossibilities[i] << ". " << playerCombatOptions[i] << "\n";
        }

        switch (correctInput())
        {
        case 'A':
            std::cout << "You attack the enemy for " << player.attack << " damage.\n";
            currentCombatEnemyCurrentHP -= player.attack;
            break;

        case 'B':
            std::cout << "You brace yourself...\n";
            playerBlocking = true;
            break;

        case 'C':
            break;
        }

        return;
    }

    void enemyIntent()
    {
        std::vector<std::string> enemyCombatOptions =
        {
            "Attack",
            "Defend",
            "Ability"
        };
    }

    void enemyCombatTurn()
    {

    }
};

int main()
{
    choiceManager manager;
    manager.startGame();
}