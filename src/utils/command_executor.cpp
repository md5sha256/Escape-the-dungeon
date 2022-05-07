#include "command_executor.hpp"
#include <iostream>

class SimpleExecutor : public CommandExecutor {

    private:
    typedef std::string string;

    std::map<string, Command*> registeredCommands;

    Optional<Command> findCommand(const string &name) {
        auto iter = registeredCommands.find(name);
        if (iter == registeredCommands.end()) {
            return nullopt<Command>();
        }
        return Optional<Command>(iter->second);
    }

    public:
    SimpleExecutor() : CommandExecutor() {

    }

    ~SimpleExecutor() override {
        for (std::pair<string, Command*> p : registeredCommands) {
            delete p.second;
        }
        registeredCommands.clear();
    }

    std::vector<Command *> getRegisteredCommands() noexcept(true) override {
        std::vector<Command *> ret;
        ret.reserve(registeredCommands.size());
        for (std::pair<string, Command*> p : registeredCommands) {
            ret.push_back(p.second);
        }
        return ret;
    }

    bool isCommandRegistered(const std::string &command) noexcept(true) override {
        return registeredCommands.find(command) != registeredCommands.end();
    }

    bool registerCommand(Command *command) noexcept(false) override {
        if (isCommandRegistered(command->getName())) {
            return false;
        }
        registeredCommands[command->getName()] = command;
        return true;
    }

    bool executeCommand(Player *player, const CommandData &data) noexcept(false) override {
        Optional<Command> optionalCmd = findCommand(data.getCommandName());
        if (optionalCmd.isEmpty()) {
            std::cout << "Unknown command: " << data.getCommandName() << std::endl;
            return false;
        }
        Command *command = optionalCmd.value();
        std::vector<string> args = data.getArgs();
        return command->onCommand( player, args);
    }
};

CommandExecutor* newCmdExecutor() {
    return new SimpleExecutor();
}