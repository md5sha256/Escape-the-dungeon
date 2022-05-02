#ifndef COMMAND_EXECUTOR_HPP
#define COMMAND_EXECUTOR_HPP

#include "command.hpp"
#include "map"
#include "optional.hpp"

class CommandExecutor {

    public:
    CommandExecutor() = default;

    virtual std::vector<Command *> getRegisteredCommands() noexcept(true) = 0;

    virtual bool isCommandRegistered(const std::string &command) noexcept(true) = 0;

    virtual bool registerCommand(Command *command) noexcept(false) = 0;

    virtual bool executeCommand(const CommandData &command) noexcept(false) = 0;
};

CommandExecutor* newCmdExecutor();

#endif//COMMAND_EXECUTOR_HPP