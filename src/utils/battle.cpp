#include "battle.hpp"

std::vector<Entity> generate_enemies() {
    srand(time(nullptr));
    int num_enemy = (rand() % 5) + 1;
    std::vector<Entity> enemies;//the list of enemy generated
    enemies.reserve(num_enemy);
    for (int i = 0; i < num_enemy; i++) {
        srand(time(nullptr) + i);
        //call for the helper function to generate one enemy
        Entity temp = generateRandomEnemy();
        enemies.push_back(temp);//append this enemy to the enemy generated list
    }
    return enemies;
}

void battle() {
}
