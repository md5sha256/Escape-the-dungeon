#include "playerinfo.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

int bad_end = 0;

void event1(Player &p) {
    std::cout << p.getName() << " saw a pond of acidic mucus, inside it are some human skulls and left behind jewellery" << std::endl
         << "You can choose to: 1. go into the pond and pick up those jewllery || 2. Leave" << std::endl;
    std::cout << "Your choice(Enter the number): ";
    std::string input;

    while (true) {
        std::cin >> input;
        if (input == "1") {               //if player choose to go into the pond
            int damage = (rand() % 5) + 1;//deal 1-5 damage to player
            p.take_damage(damage);
            if (p.check_if_dead()) {
                std::cout << p.getName() << " endures the severe pain brought by strong acid, picked up the jewelry in the pond, but never got the chance to go back to the shore again" << std::endl;
                bad_end = 1;
            } else {
                int gold = (rand() % 70 + 30);
                std::cout << p.getName() << " brought back " << gold << " gold from the pond, but suffered " << damage << " damage" << std::endl;
                p.modifyGold(gold);//give reward
            }
            break;
        } else if (input == "2") {
            p.incrementPosition();//move to the next
            break;
        } else
            std::cout << "Input a valid number!" << std::endl;
    }
}

void event2(Player &p) {
    typedef Entity::Attribute Attribute;
    std::cout << p.getName() << " found a Trevi fountain " << std::endl
         << "You can choose to: 1. Throw 10 gold into the Fountain || 2. Leave" << std::endl;
    std::cout << "Your choice(Enter the number): ";
    std::string input;

    while (true) {
        std::cin >> input;
        if (input == "1") {         //if player choose spend gold
            int luck = (rand() % 2);//50/50 chance of getting reward
            if (luck == 1) {
                std::cout << p.getName() << " feels power filling the body" << std::endl;
                int value = (rand() % 3) + 1;
                int type = (rand() % 3);
                Attribute toModify;
                if (type == 0) {//boost HP
                    toModify = Attribute::HEALTH;
                } else if (type == 1)//boost attack
                    toModify = Attribute::ATTACK;
                else//boost spirit
                    toModify = Attribute::DEFENCE;
            } else {
                std::cout << p.getName() << " waits for minitus, nothing happens." << std::endl;
                std::cout << "Enter /next to move to the next point on the map." << std::endl;
            }
            break;
        } else if (input == "2") {
            p.incrementPosition();//move to the next
            break;
        } else
            std::cout << "Input a valid number!" << std::endl;
    }
}

void event3(Player &p) {
    std::cout << p.getName() << " saw a golden statue, seems to worth quite a lot of money" << std::endl
         << "You can choose to: 1. take the statue || 2. Leave" << std::endl;
    std::cout << "Your choice(Enter the number): ";
    std::string input;

    while (true) {
        std::cin >> input;
        if (input == "1") {         //if player choose take the statue
            int trap = (rand() % 3);//1/3 chance of initiate a trap
            if (trap == 1) {
                std::cout << p.getName() << " took the status and triggered a device, the last thing he/she remember, is a giant rock falling from the roof..." << std::endl;
                bad_end = 1;
            } else {
                std::cout << p.getName() << " took the status and wondered why such a precious would be placed here " << std::endl;
                std::cout << "It worth 100 gold!" << std::endl;
                p.modifyGold(100);//give reward
            }
            break;
        } else if (input == "2") {
            std::cout << p.getName() << " decided not to take the statue." << std::endl;
            p.incrementPosition();//move to the next
            break;
        } else
            std::cout << "Input a valid number!" << std::endl;
    }
}

void event4(Player &p) {
    //teleport the player,can be forward or backward;
    std::cout << p.getName() << " came to the folk of this road. " << std::endl
         << "You can choose to: 1. Go for the right || 2. Go for the left" << std::endl;
    std::cout << "Your choice(Enter the number): ";
    std::string input;
    int left = (rand() % 5) - 2; //either jump 2 points on the map or back 2 points (for the left)
    int right = (rand() % 5) - 2;//same as above, but for the right
    while (true) {
        std::cin >> input;
        if (input == "1") {//if player choose to go right
            std::cout << p.getName() << " decided to go right." << std::endl;
            std::cout << "You are transferred " << right << " from the current position(" << p.getPosition() << ")." << std::endl;
            p.modifyPosition(right);
            std::cout << "Your position now: " << std::endl;
            p.print_map();
            break;
        } else if (input == "2") {
            std::cout << "You are transferred " << left << " from the current position(" << p.getPosition() << ")." << std::endl;
            p.modifyPosition(-left);
            std::cout << "Your position now: " << std::endl;
            p.print_map();
            break;
        } else
            std::cout << "Input a valid number!" << std::endl;
    }
}

void random_event(Player &p) {
    srand(time(nullptr));
    int choice = rand() % 4;
    switch (choice) {
        case 0:
            event1(p);
            break;
        case 1:
            event2(p);
            break;
        case 2:
            event3(p);
            break;
        case 3:
            event4(p);
        default:
            throw std::invalid_argument("unexpected random choice: " + std::to_string(choice));
            return;
    }
}
