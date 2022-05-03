#include "commands.hpp"

class EchoCommand : public Command {

    public:
    EchoCommand() : Command("echo") {
    }

    bool onCommand(std::vector<std::string> &args) noexcept(false) override {
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
        delete gameClient;
    }

    bool onCommand(std::vector<std::string> &args) noexcept(false) override {
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