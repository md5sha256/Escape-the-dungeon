#ifndef GAME_HPP
#define GAME_HPP

#include "command.hpp"
#include "command_executor.hpp"
#include "command_parser.hpp"

class GameClient {

    public:
    virtual bool isRunning() noexcept(true) = 0;

    virtual void init() noexcept(false) = 0;

    virtual void start() noexcept(false) = 0;

    virtual void stop() noexcept(true) = 0;

    virtual void shutdown() noexcept(false) = 0;

};

GameClient* newGameClient() noexcept(true);

#endif//GAME_HPP
