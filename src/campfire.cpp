#include "playerinfo.h"
#include<iostream>
#include<string>

/**
 * Prompt the campfire event on the player
 * @param p  The player to prompt the event
 */
void campfire(Player *p) {
    std::string input;
    // Print storyline
    std::cout << p->getName() << " found a place to rest" << std::endl
              << "He/she started a campfire";
    std::cout << "Note: You obtained 2 additional skill points, you may want to allocate them." << std::endl;
    p->addSkillPoints(2);
    // Prompt the user to allocate their skill points
    allocateSkillPoints(p);
    std::cout << "With the yelling of monster closing in, " << p->getName() << " knows this won't be a safe place to stay." << std::endl;
    std::cout << "\"Better keep going before they reach me.\" " << p->getName() << " thinks." << std::endl;
}
