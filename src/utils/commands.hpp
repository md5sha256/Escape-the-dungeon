#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "command_executor.hpp"
#include "iostream"
#include "sstream"

class EchoCommand : public Command {

    public:
    EchoCommand() : Command::Command("echo") {
    }

    bool onCommand(std::vector<std::string> &args) noexcept(false) override {
        for (const std::string &s : args) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
        return true;
    }
};

#endif//COMMANDS_HPP