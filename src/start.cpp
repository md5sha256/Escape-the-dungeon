#include "playerinfo.h"

#include <iostream>
#include <string>

/**
 * Initialize a player on the heap
 * This method will ask for input from cin directly and will
 * block until setup is complete
 * @return Returns a pointer to the player object
 */
Player* initPlayer() {
    // Read the name
    std::string name;
    std::cout << "Please enter your name: ";
    std::cin >> name;
    std::string input;
    // Create a new player on the heap
    auto *player = new Player(name);
    // Init default variables
    player->modifyAttribute(Attribute::HEALTH, DEFAULT_PLAYER_HEALTH);
    player->modifyAttribute(Attribute::ATTACK, 1);
    player->modifyAttribute(Attribute::DEFENCE, 0);
    player->addSkillPoints(5);
    // Prompt player to pick their skill points
    std::cout << "Please allocate your skill points:" << std::endl;
    allocateSkillPoints(player);
    Path g;
    // Generate 3 paths for the player to choose from
    g.generate();
    std::cout << "Please select your path to the entrance." << std::endl;
    std::cout << "Path 1:" << std::endl;
    for (int path : g.path1) {
        if (path == BATTLE_PATH)
            std::cout << "Battle >> ";
        if (path == CAMPFIRE_PATH)
            std::cout << "Campfire >> ";
        if (path == SHOP_PATH)
            std::cout << "Shop >> ";
        if (path == EVENT_PATH)
            std::cout << "Event >> ";
        if (path == WIN_PATH)
            std::cout << "Escape" << std::endl;
    }
    std::cout << "Path 2:" << std::endl;
    for (int path : g.path2) {
        if (path == BATTLE_PATH)
            std::cout << "Battle >> ";
        if (path == CAMPFIRE_PATH)
            std::cout << "Campfire >> ";
        if (path == SHOP_PATH)
            std::cout << "Shop >> ";
        if (path == EVENT_PATH)
            std::cout << "Event >> ";
        if (path == WIN_PATH)
            std::cout << "Escape" << std::endl;
    }
    std::cout << "Path 3:" << std::endl;
    for (int path : g.path3) {
        if (path == BATTLE_PATH)
            std::cout << "Battle >> ";
        if (path == CAMPFIRE_PATH)
            std::cout << "Campfire >> ";
        if (path == SHOP_PATH)
            std::cout << "Shop >> ";
        if (path == EVENT_PATH)
            std::cout << "Event >> ";
        if (path == WIN_PATH)
            std::cout << "Escape" << std::endl;
    }
    // Prompt the player for path selection
    std::cout << "Enter a path number: ";
    std::vector<int> playerPath;
    while (true) {
        std::cin >> input;
        if (input == "1")
            playerPath = g.path1;
        else if (input == "2")
            playerPath = g.path2;
        else if (input == "3")
            playerPath = g.path3;
        else {
            // Invalid number, ask the player to enter a new number
            std::cout << "Please enter a valid number!" << std::endl;
            continue;
        }
        // Output the chosen path
        std::cout << "You have chosen path " << input << std::endl;
        break;
    }
    // Update the player object
    player->setPath(playerPath);
    return player;
}

/**
 * Emit a friendly greeting message to a player with existing data
 * @param player A pointer to the player object
 */
void performGreetBack(Player *player) {
    std::cout << "Welcome back " << player->getName() << "!" << std::endl;
    // Print out the current state
    player->printMap();
    player->printStatus();
    if (player->getSkillPoints() > 0) {
        // Prompt the player to perform skill allocation because they have outstanding points
        std::cout << "You have some unallocated skill points. Run /skills allocate to allocate them!" << std::endl;
    }
}
