#ifndef BATTLE_H
#define BATTLE_H

#include "../playerinfo.h"
#include "list"
#include "optional.h"
#include <cstdlib>
#include <ctime>
#include <string>

struct Battle {

    Player *player;
    std::list<Entity*> enemies;

    public:
    Battle(Player *_player, const std::vector<Entity*> &opponents) {
        player = _player;
        for (const auto &enemy : opponents) {
            enemies.push_back(enemy);
        }
    }

    ~Battle() = default;

    [[nodiscard]] int getNumEnemies() const {
        return (int) enemies.size();
    }

    [[nodiscard]] Optional<Entity> getCurrentOpponent() const {
        if (enemies.empty()) {
            return nullopt<Entity>();
        }
        Entity *entity = enemies.front();
        return Optional<Entity>{entity};
    }

    Battle* update() noexcept(true) {
        Optional<Entity> current = getCurrentOpponent();
        if (current.isPresent()) {
            Entity *enemy = current.value();
            if (enemy->isDead()) {
                enemies.pop_front();
                // Free up the enemy as they are no longer needed
                delete enemy;
            }
        }
        return this;
    }

    void printOpponent(Entity &entity) const {
        std::cout << "A " << entity.getName() << " steps up to challenge " << player->getName() << std::endl;
        std::cout << entity.getName() << "'s stats:" << std::endl;
        entity.printAttribute(Attribute::HEALTH);
        entity.printAttribute(Attribute::ATTACK);
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

#endif//BATTLE_H
