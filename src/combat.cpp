#include "consoleGame.hpp"
enemy currentCombatEnemy;
char playerAction;
int currentAbilityAmount;
int currentAbilitySpecialAmount;
int chosenAbility;
enemy currentActor;
std::vector<structSearcher> currentTurnCharacter;
int burnTimer = 0;

std::vector<enemy*> availableEnemies;

    void combat::basicCombat()
    {
        std::cout << "You see an enemy. Naturally you want to kill it. \n";
        selectEnemy();
        int turnCounter = 0;
        while (true)
        {
            turnCounter++;
            std::cout << "turn: " << turnCounter << "\n";
            combatTurn();
            if (playerCurrentHP <= 0 || currentCombatEnemyCurrentHP <= 0)
            {
                if (playerCurrentHP >= 1)
                {
                    std::cout << "You have succesfully defeated the enemy!\n";
                }
                else 
                {
                    std::cout << "You have been defeated by: " << currentCombatEnemy.name << "!\n";
                    std::cout << "You lose!" << "\n";
                    exit(0);
                }
                break;
            }
        }
    }

    void combat::selectEnemy()
    {
        setupAndUtility util(gm);
        availableEnemies = createEnemySample();

        if (availableEnemies.empty()) {
            std::cout << "Error: No enemies found in region: " << currentRegion << "!\n";
            return;
        }

        int randomEnemy = util.seedIteration(availableEnemies.size());
        currentCombatEnemy = *availableEnemies[randomEnemy];

        currentCombatEnemyCurrentHP = currentCombatEnemy.hpMax;
        std::cout << "your current enemy is: " << availableEnemies[randomEnemy]->name << "." << "\n";
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
        endTurnEffects();
    }

    void combat::playerCombatTurn(bool& playerBlocking)
    {
        setupAndUtility util(gm);
        std::vector<std::string> playerCombatOptions =
        {
            "Attack",
            "Defend",
        };

        if (equippedAbilities.size() > 0)
        {
            playerCombatOptions.push_back("Ability");
        }

        util.vectorCreation(playerCombatOptions.size());

        std::cout << "It is your turn, choose your action:\n";
        for (int i = 0; i < playerCombatOptions.size(); i++)
        {
            std::cout << gm.charPossibilities[i] << ". " << playerCombatOptions[i] << "\n";
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
            std::cout << "What ability do you want to use?" << "\n";

            util.vectorCreation(equippedAbilities.size());

            for (int i = 0; i < equippedAbilities.size(); i++)
            {
                std::cout << gm.charPossibilities[i] << ". " << equippedAbilities[i].name << "\n";
            }
            chosenAbility = util.correctInput() - 'A';

            currentAbilityAmount = equippedAbilities[chosenAbility].amount;
            if (abilityAttributeAssigner.contains(equippedAbilities[chosenAbility].effect))
            {
                abilityAttributeAssigner[equippedAbilities[chosenAbility].effect]();
            }
            else
            {
                std::cout << "You fucked up something, didn't you?" << "\n";
            }

            currentAbilitySpecialAmount = equippedAbilities[chosenAbility].specialAmount;
            std::cout << equippedAbilities[chosenAbility].specialAmount << " whatevs" << "\n";
            if (abilityAttributeAssigner.contains(equippedAbilities[chosenAbility].special))
            {
                abilityAttributeAssigner[equippedAbilities[chosenAbility].special]();
            }
            else
            {
                std::cout << "You fucked up something, didn't you?" << "\n";
            }
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

    std::vector<enemy*> combat::createEnemySample()
    {
        setupAndUtility util(gm);
        std::vector<enemy*> filteredEnemies = util.filterGameData<enemy>(gamedataBase, [](const enemy* e)
            {
                return e->region == currentRegion;
            });
        return filteredEnemies;
    }

    void combat::abilityDamage(int damageOfAbility)
    {
        std::cout << "You casted: " << equippedAbilities[chosenAbility].name << "!" << "\n";
        std::cout << "the enemy took " << RED << equippedAbilities[chosenAbility].amount << RESET << " damage!" << "\n";
        currentCombatEnemyCurrentHP -= equippedAbilities[chosenAbility].amount;
    }

    void combat::abilityDoT(int duration)
    {
        burnTimer = duration;
        //std::cout << "DEBUG: burnTimer 1:" << burnTimer << "\n";
    }

    void combat::endTurnEffects()
    {
        if (burnTimer > 0)
        {
            currentCombatEnemyCurrentHP--;
            burnTimer--;
            std::cout << "The enemy " << equippedAbilities[chosenAbility].special <<"s! he takes: " << RED << "1" << RESET << " damage!" << "\n";
            std::cout << "The enemy has: " << currentCombatEnemyCurrentHP << " HP left!" << "\n";
        }
    }