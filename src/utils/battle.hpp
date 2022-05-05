#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "../playerinfo.hpp"
#include "optional.hpp"
#include <cstdlib>
#include <ctime>
#include <string>

struct Battle {

    Player *player;
    Entity *entity;

    public:
    Battle(Player *_player, Entity *_entity) {
        player = _player;
        entity = _entity;
    }

    ~Battle() = default;

    public:
    [[nodiscard]] Player *getPlayer() const {
        return player;
    }

    [[nodiscard]] Entity *getEntity() const {
        return entity;
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
    bool startBattle(Player *player, Entity *entity) {
        if (currentBattle != nullptr) {
            return false;
        }
        currentBattle = new Battle{player, entity};
        return true;
    }

    bool hasActiveBattle() {
        return currentBattle != nullptr;
    }

    [[nodiscard]] optional<Battle> getcurrentBattle() const noexcept(true) {
        return optional<Battle>{currentBattle};
    }

    void endBattle() {
        delete currentBattle;
        currentBattle = nullptr;
    }
};

#endif//BATTLE_HPP
