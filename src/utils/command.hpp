#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>
#include <stdexcept>


struct CommandData {

    private:
    std::string commandName;
    std::vector<std::string> args;

    public:
    CommandData(const std::string &name, const std::vector<std::string> &arguments) {
        commandName = name;
        args = arguments;
    }

    CommandData(const struct CommandData::CommandData &data) : CommandData(data.commandName, data.args) {
    }

    ~CommandData() {
        commandName = "";
        args.clear();
    }

    std::string getCommandName() const {
        return std::string{commandName};
    }

    std::vector<std::string> getArgs() const {
        return std::vector<std::string>{args};
    }
};


struct CommandExecutionError : std::runtime_error {

    public:
    std::string command;

    CommandExecutionError(const std::string &_command, const std::string &message) : std::runtime_error(message) {
        command = _command;
    }
};

class Command {

    private:
    /**
     * Name of the command
     */
    std::string name;

    public:
    explicit Command(const std::string &_name) {
        name = _name;
    }
    /**
     * Called when the command is executed by the user
     * @param args The arguments in the order they were passed
     * @return Return true if the command was executed successfully, false otherwise
     */
    virtual bool onCommand(std::vector<std::string> args) noexcept(false) = 0;

    std::string getName() const noexcept(true) {
        return std::string{name};
    }

};

#endif //COMMAND_HPP