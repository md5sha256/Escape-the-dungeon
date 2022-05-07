#ifndef COMMAND_EXECUTOR_HPP
#define COMMAND_EXECUTOR_HPP

#include "command.hpp"
#include "map"
#include "optional.hpp"
#include "../playerinfo.hpp"

class CommandExecutor {

    public:
    virtual ~CommandExecutor() = default;

    virtual std::vector<Command *> getRegisteredCommands() noexcept(true) = 0;

    virtual bool isCommandRegistered(const std::string &command) noexcept(true) = 0;

    virtual bool registerCommand(Command *command) noexcept(false) = 0;

    void registerCommands(const std::vector<Command*> &commands) {
        for (const auto &cmd : commands) {
            registerCommand(cmd);
        }
    }

    virtual bool executeCommand(Player *player, const CommandData &command) noexcept(false) = 0;
};

CommandExecutor* newCmdExecutor();

#endif//COMMAND_EXECUTOR_HPP