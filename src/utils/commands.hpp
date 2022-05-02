#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "command_executor.hpp"
#include "iostream"
#include "sstream"

class EchoCommand : public Command {

    public:
    EchoCommand() : Command::Command("echo") {
    }

    bool onCommand(std::vector<std::string> args) noexcept(false) override {
        std::stringstream ss;
        for (const std::string &s : args) {
            ss << s << " ";
        }
        std::string ret = ss.str();
        std::cout << ret << std::endl;
        return true;
    }
};

#endif//COMMANDS_HPP