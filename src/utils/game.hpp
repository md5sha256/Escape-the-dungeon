#ifndef GAME_HPP
#define GAME_HPP

#include "../playerinfo.hpp"
#include "string"

class GameClient {

    public:
    virtual ~GameClient() = default;

    virtual bool isRunning() noexcept(true) = 0;

    virtual void init() noexcept(false) = 0;

    virtual void start() noexcept(false) = 0;

    virtual void stop() noexcept(true) = 0;

    virtual void shutdown() noexcept(false) = 0;

    [[nodiscard]] virtual Player *getPlayer() const noexcept(true) = 0;

    //[[nodiscard]] virtual BattleHandler *getBattleHandler() const noexcept(true) = 0;

};

GameClient *newGameClient(const std::string &rootDir) noexcept(true);

#endif//GAME_HPP
