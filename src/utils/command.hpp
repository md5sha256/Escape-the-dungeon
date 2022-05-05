#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../playerinfo.hpp"
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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

    ~CommandData() = default;

    std::string getCommandName() const {
        return std::string{commandName};
    }

    std::vector<std::string> getArgs() const {
        return std::vector<std::string>{args};
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "cmd name: " << commandName << std::endl;
        for (const std::string &s : args) {
            ss << s << std::endl;
        }
        return ss.str();
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

    protected:
    explicit Command(const std::string &_name) {
        name = _name;
    }

    public:
    virtual ~Command() = default;

    /**
     * Called when the command is executed by the user
     * @param args The arguments in the order they were passed
     * @return Return true if the command was executed successfully, false otherwise
     */
    virtual bool onCommand(Player &player, std::vector<std::string> &args) noexcept(false) {
        return true;
    }

    virtual void printUsages() const noexcept(true) {

    }

    std::string getName() const noexcept(true) {
        return std::string{name};
    }

    static void printLines(const std::vector<std::string> &messages) {
        for (const std::string& msg : messages) {
            if (!msg.empty()) {
                std::cout << msg << std::endl;
            }
        }
    }
};

#endif//COMMAND_HPP