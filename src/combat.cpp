#include "consoleGame.hpp"
char playerAction;
int chosenAbility;
enemy currentActor;
std::vector<structSearcher> currentTurnCharacter;
int burnTimer = 0;

    void combat::basicCombat()
    {
        printw("You see an enemy. Naturally you want to kill it. \n");
        refresh();
        selectEnemy();
        int turnCounter = 0;
        while (true)
        {
            turnCounter++;
            printw("turn: %i \n", turnCounter);
            refresh();
            combatTurn();
            if (playerCurrentHP <= 0 || currentCombatEnemyCurrentHP <= 0)
            {
                if (playerCurrentHP >= 1)
                {
                    printw("You have succesfully defeated the enemy!\n");
                    refresh();
                }
                else 
                {
                    printw("You have been defeated by: %s! \n", gm.currentCombatEnemy.name.c_str());
                    printw("You lose! \n");
                    refresh();
                    exit(0);
                }
                break;
            }
        }
    }

    void combat::selectEnemy()
    {
        setupAndUtility util(gm);
        gm.availableEnemies = createEnemySample();

        if (gm.availableEnemies.empty()) {
            printw("Error: No enemies found in region: %s !\n", gm.currentRegion.c_str());
            refresh();
            return;
        }

        int randomEnemy = util.seedIteration(gm.availableEnemies.size());
        gm.currentCombatEnemy = *gm.availableEnemies[randomEnemy];

        currentCombatEnemyCurrentHP = gm.currentCombatEnemy.hpMax;
        printw("your current enemy is: %s. \n", gm.availableEnemies[randomEnemy]->name.c_str());
        refresh();
    }

    void combat::combatTurn()
    {
        bool playerBlocking = false;
        playerCombatTurn(playerBlocking);

        if (currentCombatEnemyCurrentHP <= 0)
        {
            currentCombatEnemyCurrentHP = 0;
            printw("The enemy is dead!\n");

            printw("You received %i gold! \n", gm.currentCombatEnemy.goldReward);
            playerCurrentGold = playerCurrentGold + gm.currentCombatEnemy.goldReward;
            printw("You now have %i gold! \n", playerCurrentGold);
            refresh();
            return;
        }

        printw("The enemy has %i HP left.\n", currentCombatEnemyCurrentHP);
        if (playerBlocking == true)
        {
            printw("The %s attacks you for %i damage.\n", gm.currentCombatEnemy.name.c_str(), gm.currentCombatEnemy.attack / 2);
            refresh();

            playerCurrentHP -= gm.currentCombatEnemy.attack / 2;
        }
        else 
        {
            printw("The %s attacks you for %i damage.\n", gm.currentCombatEnemy.name.c_str(), gm.currentCombatEnemy.attack);
            refresh();

            playerCurrentHP -= gm.currentCombatEnemy.attack;
        }

        printw("you have %i HP left.\n", playerCurrentHP);
        refresh();
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

        if (gm.equippedAbilities.size() > 0)
        {
            playerCombatOptions.push_back("Ability");
        }

        util.vectorCreation(playerCombatOptions.size());

        printw("It is your turn, choose your action:\n");
        refresh();
        int menuStartY, dummyX;
        getyx(stdscr, menuStartY, dummyX);

        for (int i = 0; i < playerCombatOptions.size(); i++)
        {
            printw("%c. %s \n", gm.charPossibilities[i], playerCombatOptions[i].c_str());
            refresh();
        }

        switch (util.correctInput(menuStartY))
        {
        case 'A':
            printw("You attack the enemy for %i damage.\n", player.attack);
            refresh();
            currentCombatEnemyCurrentHP -= player.attack;
            playerAction = 'A';
            break;

        case 'B':
            printw("You brace yourself...\n");
            refresh();
            playerBlocking = true;
            playerAction = 'B';
            break;

        case 'C':
            playerAction = 'C';
            printw("What ability do you want to use? \n");

            util.vectorCreation(gm.equippedAbilities.size());

            int menuStartY, dummyX;
            getyx(stdscr, menuStartY, dummyX);

            for (int i = 0; i < gm.equippedAbilities.size(); i++)
            {
                printw("%c. %s \n", gm.charPossibilities[i], gm.equippedAbilities[i].name.c_str());
                refresh();
            }
            chosenAbility = util.correctInput(menuStartY) - 'A';

            gm.currentAbilityAmount = gm.equippedAbilities[chosenAbility].amount;
            if (abilityAttributeAssigner.contains(gm.equippedAbilities[chosenAbility].effect))
            {
                abilityAttributeAssigner[gm.equippedAbilities[chosenAbility].effect]();
            }
            else
            {
                printw("You fucked up something, didn't you? \n");
                refresh();
            }

            gm.currentAbilitySpecialAmount = gm.equippedAbilities[chosenAbility].specialAmount;
            if (abilityAttributeAssigner.contains(gm.equippedAbilities[chosenAbility].special))
            {
                abilityAttributeAssigner[gm.equippedAbilities[chosenAbility].special]();
            }
            else
            {
                printw("You fucked up something, didn't you? \n");
                refresh();
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
        std::vector<enemy*> filteredEnemies = util.filterGameData<enemy>(gamedataBase, [this](const enemy* e)
            {
                return e->region == gm.currentRegion;
            });
        return filteredEnemies;
    }

    void combat::abilityDamage(int damageOfAbility)
    {
        printw("You casted: %s! \n", gm.equippedAbilities[chosenAbility].name.c_str());
        printw("the enemy took ");

        attron(COLOR_PAIR(1));
        printw("%i", gm.equippedAbilities[chosenAbility].amount);
        attroff(COLOR_PAIR(1));

        printw(" damage!\n");
        refresh();
        currentCombatEnemyCurrentHP -= gm.equippedAbilities[chosenAbility].amount;
    }

    void combat::abilityDoT(int duration)
    {
        burnTimer = duration;
    }

    void combat::abilitySoulScream(int soulScreamAmount)
    {
        printw("The enemy screams in agony, as you retrieve their soul from their body... \n");
        refresh();
        wait(100);
        gm.soulCounter++;
        printw("You now have %f soul", gm.soulCounter);
        if (gm.soulCounter != 1)
        {
            printw("s");
        }
        printw(". \n");
        wait(20);
        printw("The enemy has a dead look in his eyes, and seems less affected by his wounds... \n");
        refresh();
        currentCombatEnemyCurrentHP = currentCombatEnemyCurrentHP + soulScreamAmount;
    }

    void combat::endTurnEffects()
    {
        if (burnTimer > 0)
        {
            currentCombatEnemyCurrentHP--;
            burnTimer--;
            printw("The enemy %ss! he takes: ", gm.equippedAbilities[chosenAbility].special.c_str()); //i have my questions, but lets keep it this way until it becomes a problem

            attron(COLOR_PAIR(1));
            printw("1");
            attroff(COLOR_PAIR(1));

            printw(" damage!\n");
            printw("The enemy has: %i HP left! \n", currentCombatEnemyCurrentHP);
            refresh();
        }
    }