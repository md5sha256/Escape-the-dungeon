#include "campfire.hpp"

void campfire(Player *p) {
    std::string input;
    std::cout << p->getName() << " found a place to rest" << std::endl
              << "He/she started a campfire";
    std::cout << "Note: You obtained 2 additional skill points, you may want to allocate them." << std::endl;
    p->addSkillPoints(2);
    allocateSkillPoints(p);
    std::cout << "With the yelling of monster closing in, " << p->getName() << " knows this won't be a safe place to stay." << std::endl;
    std::cout << "\"Better keep going before they reach me.\" " << p->getName() << " thinks." << std::endl;
}
