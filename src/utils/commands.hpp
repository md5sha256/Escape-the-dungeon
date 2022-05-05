#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "command.hpp"
#include "game.hpp"
#include "iostream"
#include "utils.hpp"

class EchoCommand : public Command {

    public:
    EchoCommand() : Command("echo") {
    }

    bool onCommand(Player &player, std::vector<std::string> &args) noexcept(true) override {
        for (const std::string &s : args) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
        return true;
    }
};

class ExitCommand : public Command {

    private:
    GameClient *gameClient;

    public:
    explicit ExitCommand(GameClient *client) : Command("exit") {
        gameClient = client;
    }

    ~ExitCommand() override {
        gameClient = nullptr;
    }

    bool onCommand(Player &player, std::vector<std::string> &args) noexcept(true) override {
        std::cout << "Are you sure you want to exit the game? (y/n)" << std::endl;
        std::string input;
        std::cin >> input;
        for (char &c : input) {
            c = (char) tolower(c);
        }
        if (input == "y" || input == "yes") {
            // Don't call shutdown here, only call stop ,
            // we hint to the client that we WANT to shut down asap.
            gameClient->stop();
            return true;
        } else if (input != "n" && input != "no") {
            std::cout << "Aborted shutdown, unknown input " << input << std::endl;
            return false;
        }
        return true;
    }
};

class SkillCommand : public Command {

    static std::string printUsage(const std::string &cmd) {
        if (cmd == "show") {
            return "/skills show (show the current skills and available points)";
        } else if (cmd == "allocate") {
            return "/skills allocate (allocate any available skill points)";
        }
        return "";
    }

    public:
    explicit SkillCommand() : Command("skills") {
    }


    bool onCommand(Player &player, std::vector<std::string> &args) noexcept(true) override {
        if (args.empty()) {
            printUsages();
            return false;
        } else if (toLowercase(args[0]) == "show") {
            player.printStatus();
            return true;
        } else if (toLowercase(args[0]) == "allocate") {
            allocateSkillPoints(player);
            return true;
        }
        printUsages();
        return false;
    }

    void printUsages() const noexcept(true) override {
        printLines({"Skill Command: view and manipulate your skills",
                    " ",
                    "/skills show | allocate | help",
                    printUsage("show"),
                    printUsage("help"),
                    "/skill help (show this help menu)"});
    }
};

#endif//COMMANDS_HPP