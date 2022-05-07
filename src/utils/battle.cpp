#include "battle.hpp"

std::vector<Entity*> generateEnemies() {
    srand(time(nullptr));
    int num_enemy = (rand() % 5) + 1;
    std::vector<Entity*> enemies;//the list of enemy generated
    enemies.reserve(num_enemy);
    for (int i = 0; i < num_enemy; i++) {
        srand(time(nullptr) + i);
        //call for the helper function to generate one enemy
        Entity *temp = generateRandomEnemy();
        enemies.push_back(temp);//append this enemy to the enemy generated list
    }
    return enemies;
}

Entity* generateRandomEnemy() {
    typedef Entity::Attribute Attribute;
    int attack = (rand() % 5) + 1;
    int defence = (rand() % 5) + 1;
    int hp = (rand() % 4) + 1;
    std::string name;
    //naming of enemy
    if ((attack + defence) >= 8)//powerful enemy
        name = "Elite ";
    else if ((attack + defence) >= 5)//strong enemy
        name = "Adult ";
    else//weak enemy
        name = "Baby ";
    if (defence * 2 <= attack)//if attack is twice as great as defence
        name += "Zombie";
    else if (attack * 2 <= defence)//if defence is twice as great as attack
        name += "Skeleton";
    else//if somewhere balance
        name += "Shrem";
    std::map<Attribute, int> attrs = {{Attribute::HEALTH, hp}, {Attribute::ATTACK, attack}, {Attribute ::DEFENCE, defence}};
    return new Entity(name, attrs);
}

bool startBattle(Player *player, BattleHandler *handler) {
    if (handler->hasActiveBattle()) {
        return false;
    }
    std::vector<Entity*> enemies = generateEnemies();
    std::cout << "Alas, the wild creatures have caught up to " << player->getName() << std::endl;
    std::cout << enemies.size() << " enemies are now lined up to do battle..." << std::endl;
    std::cout << "Foul cries fill the air as the beasts prepare themselves for the carnage that lies ahead." << std::endl;
    std::cout << "Your first opponent " << enemies[0]->getName() << " awaits your challenge." << std::endl;
    std::cout << "Type /battle attack to attack the enemy or /cards use [index] to use a card" << std::endl;
    handler->startBattle(player, enemies);
    return true;
}