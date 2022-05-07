#include "playerinfo.hpp"

#include <iostream>
#include <string>

Player* initPlayer() {
    std::string name;
    std::cout << "Please enter your name: ";
    std::cin >> name;
    std::string input;
    std::cout << "Please allocate your skill points:" << std::endl;
    auto *player = new Player(name);
    player->modifyAttribute(Attribute::HEALTH, DEFAULT_PLAYER_HEALTH);
    player->modifyAttribute(Attribute::ATTACK, 1);
    player->modifyAttribute(Attribute::DEFENCE, 0);
    player->addSkillPoints(5);
    allocateSkillPoints(player);
    Path g;
    g.generate();
    std::cout << "Please select your path to the entrance." << std::endl;
    std::cout << "Path 1:" << std::endl;
    for (int i = 0; i < 10; i++) {
        if (g.path1[i] == 0)
            std::cout << "Battle >> ";
        if (g.path1[i] == 1)
            std::cout << "Campfire >> ";
        if (g.path1[i] == 2)
            std::cout << "Shop >> ";
        if (g.path1[i] == 3)
            std::cout << "Event >> ";
        if (g.path1[i] == 4)
            std::cout << "Boss >> Escape" << std::endl;
    }
    std::cout << "Path 2:" << std::endl;
    for (int i = 0; i < 10; i++) {
        if (g.path2[i] == 0)
            std::cout << "Battle >> ";
        if (g.path2[i] == 1)
            std::cout << "Campfire >> ";
        if (g.path2[i] == 2)
            std::cout << "Shop >> ";
        if (g.path2[i] == 3)
            std::cout << "Event >> ";
        if (g.path2[i] == 4)
            std::cout << "Boss >> Escape" << std::endl;
    }
    std::cout << "Path 3:" << std::endl;
    for (int i = 0; i < 10; i++) {
        if (g.path3[i] == 0)
            std::cout << "Battle >> ";
        if (g.path3[i] == 1)
            std::cout << "Campfire >> ";
        if (g.path3[i] == 2)
            std::cout << "Shop >> ";
        if (g.path3[i] == 3)
            std::cout << "Event >> ";
        if (g.path3[i] == 4)
            std::cout << "Boss >> Escape" << std::endl;
    }
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
            std::cout << "Please enter a valid number!" << std::endl;
            continue;
        }
        std::cout << "You have chosen path " << input << std::endl;
        break;
    }
    player->setPath(playerPath);
    return player;
}

void performGreetBack(Player *player) {
    std::cout << "Welcome back " << player->getName() << "!" << std::endl;
    player->printMap();
    player->printStatus();
    if (player->getSkillPoints() > 0) {
        std::cout << "You have some unallocated skill points. Run /skills allocate to allocate them!" << std::endl;
    }
}
