#include "consoleGame.hpp"


    void Combat::basicCombat()
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

    void Combat::selectEnemy()
    {
        currentCombatEnemy = enemies[0];
        currentCombatEnemyCurrentHP = currentCombatEnemy.hpMax;

    }

    void Combat::combatTurn()
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
            std::cout << "The " << currentCombatEnemy.name << " attacks you for " << currentCombatEnemy.attack / 2 << " damage.\n";

            playerCurrentHP -= currentCombatEnemy.attack / 2;
        }
        else {
            std::cout << "The " << currentCombatEnemy.name << " attacks you for " << currentCombatEnemy.attack << " damage.\n";

            playerCurrentHP -= currentCombatEnemy.attack;
        }

        std::cout << "you have " << playerCurrentHP << " HP left.\n";
    }

    void Combat::playerCombatTurn(bool& playerBlocking)
    {
        std::vector<std::string> playerCombatOptions =
        {
            "Attack",
            "Defend",
            "Ability"
        };

        vectorCreation(playerCombatOptions.size());

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

    void Combat::enemyIntent()
    {
        std::vector<std::string> enemyCombatOptions =
        {
            "Attack",
            "Defend",
            "Ability"
        };
    }

    void Combat::enemyCombatTurn()
    {

    }