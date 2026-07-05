#include "consoleGame.hpp"
enemy currentCombatEnemy;
char playerAction;

    void combat::basicCombat()
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

    void combat::selectEnemy()
    {
        setupAndUtility util;
        int randomEnemy = util.seedIteration(enemies.size());
        currentCombatEnemy = enemies[randomEnemy];
        if (regionAssigner.contains(enemies[randomEnemy].region))
        {

        }
        else
        {
            std::cout << RED << "ERROR: This is not supposed to happen, blame the dev" << RESET << "\n";
        }
        currentCombatEnemyCurrentHP = currentCombatEnemy.hpMax;
        std::cout << "your current enemy is: " << enemies[randomEnemy].name << "." << "\n";
    }

    void combat::combatTurn()
    {
        bool playerBlocking = false;
        playerCombatTurn(playerBlocking);

        if (currentCombatEnemyCurrentHP <= 0)
        {
            currentCombatEnemyCurrentHP = 0;
            std::cout << "The enemy is dead!\n";

            std::cout << "You received " << currentCombatEnemy.goldReward << " gold!" << "\n";
            playerCurrentGold = playerCurrentGold + currentCombatEnemy.goldReward;
            std::cout << "You now have " << playerCurrentGold << " gold!" << "\n";
            return;
        }

        std::cout << "The enemy has " << currentCombatEnemyCurrentHP << " HP left.\n";
        if (playerBlocking == true)
        {
            std::cout << "The " << currentCombatEnemy.name << " attacks you for " << currentCombatEnemy.attack / 2 << " damage.\n";

            playerCurrentHP -= currentCombatEnemy.attack / 2;
        }
        else 
        {
            std::cout << "The " << currentCombatEnemy.name << " attacks you for " << currentCombatEnemy.attack << " damage.\n";

            playerCurrentHP -= currentCombatEnemy.attack;
        }

        std::cout << "you have " << playerCurrentHP << " HP left.\n";
    }

    void combat::playerCombatTurn(bool& playerBlocking)
    {
        setupAndUtility util;
        std::vector<std::string> playerCombatOptions =
        {
            "Attack",
            "Defend",
            "Ability"
        };

        util.vectorCreation(playerCombatOptions.size());

        std::cout << "It is your turn, choose your action:\n";
        for (int i = 0; i < playerCombatOptions.size(); i++)
        {
            std::cout << charPossibilities[i] << ". " << playerCombatOptions[i] << "\n";
        }

        switch (util.correctInput())
        {
        case 'A':
            std::cout << "You attack the enemy for " << player.attack << " damage.\n";
            currentCombatEnemyCurrentHP -= player.attack;
            playerAction = 'A';
            break;

        case 'B':
            std::cout << "You brace yourself...\n";
            playerBlocking = true;
            playerAction = 'B';
            break;

        case 'C':
            playerAction = 'C';
            break;
        }

        return;
    }

    void combat::enemyIntent()
    {
        std::vector<std::string> enemyCombatOptions =
        {
            "Attack",
            "Defend",
            "Ability"
        };
    }

    void combat::enemyCombatTurn()
    {
        switch (playerAction)
        {
        case 'A':       //player attacks
            
            break;
        case 'B':       //player blocks

            break;
        case 'C':       //player uses ability

            break;
        }
    }