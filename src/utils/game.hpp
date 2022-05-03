#ifndef GAME_HPP
#define GAME_HPP

#include "string"
#include "../playerinfo.hpp"

class GameClient {

    protected:
    typedef player_info PlayerInfo;

    public:
    virtual ~GameClient() = default;

    virtual bool isRunning() noexcept(true) = 0;

    virtual void init() noexcept(false) = 0;

    virtual void start() noexcept(false) = 0;

    virtual void stop() noexcept(true) = 0;

    virtual void shutdown() noexcept(false) = 0;

    [[nodiscard]] virtual PlayerInfo *getPlayer() const noexcept(true) = 0;

};

GameClient *newGameClient(const std::string &rootDir) noexcept(true);

#endif//GAME_HPP
