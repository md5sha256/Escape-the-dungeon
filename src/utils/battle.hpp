#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "../playerinfo.hpp"
#include "optional.hpp"
#include <cstdlib>
#include <ctime>
#include <string>

struct Battle {

    Player *player;
    std::vector<Entity*> enemies;

    public:
    Battle(Player *_player, const std::vector<Entity*> &opponents) {
        player = _player;
        enemies = opponents;
    }

    ~Battle() {
        for (const auto ptr : enemies) {
            delete ptr;
        }
        enemies.clear();
    }

    public:
    [[nodiscard]] Player *getPlayer() const {
        return player;
    }

    [[nodiscard]] std::vector<Entity*> getEnemies() const {
        return enemies;
    }

    [[nodiscard]] int getNumEnemies() const {
        return enemies.size();
    }

    [[nodiscard]] Optional<Entity> getCurrentOpponent() const {
        if (enemies.empty()) {
            return nullopt<Entity>();
        }
        return Optional<Entity>{enemies[0]};
    }

    Optional<Entity> update() {
        Optional<Entity> current = getCurrentOpponent();
        if (current.isPresent()) {
            Entity *enemy = current.value();
            if (enemy->isDead()) {
                enemies.erase(enemies.begin());
            }
            // Free up the enemy as they are no longer needed
            delete enemy;
        }
        return getCurrentOpponent();
    }

    void printOpponent() {
        Optional<Entity> optional = getCurrentOpponent();
        if (optional.isEmpty()) {
            return;
        }
        Entity* entity = optional.value();
        std::cout << "A " << entity->getName() << " steps up to challenge " << player->getName() << std::endl;
        std::cout << entity->getName() << "'s stats:" << std::endl;
        entity->printAttribute(Entity::Attribute::HEALTH);
        entity->printAttribute(Entity::Attribute::ATTACK);
    }

    [[nodiscard]] bool isValid() const {
        return !player->isDead() && !enemies.empty();
    }
};

class BattleHandler {

    private:
    Battle *currentBattle;

    public:
    BattleHandler() = default;

    ~BattleHandler() {
        endBattle();
    }

    public:
    bool startBattle(Player *player, const std::vector<Entity*> &opponents) {
        if (currentBattle != nullptr) {
            return false;
        }
        currentBattle = new Battle{player, opponents};
        return true;
    }

    bool hasActiveBattle() {
        return currentBattle != nullptr;
    }

    [[nodiscard]] Optional<Battle> getcurrentBattle() const noexcept(true) {
        return Optional<Battle>{currentBattle};
    }

    void endBattle() {
        delete currentBattle;
        currentBattle = nullptr;
    }
};

Entity *generateRandomEnemy();

std::vector<Entity*> generateEnemies();

bool startBattle(Player *player, BattleHandler *battleHandler);

#endif//BATTLE_HPP
