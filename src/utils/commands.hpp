#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "command.hpp"
#include "game.hpp"
#include "iostream"
#include "utils.hpp"
#include "../shop.hpp"

class EchoCommand : public Command {

    public:
    EchoCommand() : Command("echo") {
    }

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(true) override {
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

    ~ExitCommand() override = default;

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(true) override {
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


    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(true) override {
        if (args.empty()) {
            printUsages();
            return false;
        } else if (toLowercase(args[0]) == "show") {
            player->printStatus();
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

class SkipCommand : public Command {
    public:
    explicit SkipCommand() : Command("skip") {

    }

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(false) override {
        if (player->getPathAtPosition() != SHOP_PATH) {
            std::cout << "You can only skip the shop!" << std::endl;
            return false;
        }
        std::cout << player->getName() << " left the shop." << std::endl;
        player->incrementPosition();
        player->printMap();
        return true;
    }

    void printUsages() const noexcept(true) override {
        printLines({"Skip command: skip the shop"});
    }

};

class ShopCommand : public Command {

    Shop *currentShop = nullptr;
    GameClient *client = nullptr;

    public:
    explicit ShopCommand(GameClient *_client) : Command("shop") {
        client = _client;
    }

    ~ShopCommand() override {
        delete currentShop;
    }

    static std::string printUsage(const std::string &cmd) {
        if (cmd == "show") {
            return "/shop show [index] (display the item for sale at a given position)";
        } else if (cmd == "buy") {
            return "/shop buy [index] (purchase the item at a given position)";
        }
        return "";
    }

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(true) override {
        if (player->getPathAtPosition() != SHOP_PATH) {
            std::cout << "You are not at a shop right now!" << std::endl;
            // Ensure that the shop is deleted
            delete currentShop;
            return false;
        }
        if (currentShop == nullptr) {
            currentShop = newShop(6);
            currentShop->generateItems(client);
        }
        if (args.empty()) {
            currentShop->greetPlayer(player);
            return true;
        }
        std::string subCommand = toLowercase(args[0]);
        if (subCommand == "help") {
            printUsages();
            return true;
        }
        if (args.size() >= 2) {
            Optional<int> optional = fromString(args[1]);
            if (optional.isEmpty()) {
                std::cout << "Invalid index: " + args[1] << std::endl;
                if (subCommand == "buy" || subCommand == "show") {
                    printUsage(subCommand);
                } else {
                    std::cout << "Unknown subcommand: " << subCommand << std::endl;
                    printUsages();
                }
                return false;
            }
            int itemIndex = *optional.value();
            if (itemIndex < 0 || itemIndex > currentShop->getSize() - 1) {
                std::cout << "Invalid index: " + args[1] << std::endl;
                if (subCommand == "buy" || subCommand == "show") {
                    printUsage(subCommand);
                } else {
                    std::cout << "Unknown subcommand: " << subCommand << std::endl;
                    printUsages();
                }
                return false;
            }
            if (subCommand == "buy") {
                currentShop->buyItem(player, itemIndex);
            } else if (subCommand == "show") {
                currentShop->printItem(client, player, itemIndex);
            } else {
                std::cout << "Unknown subcommand: " << subCommand << std::endl;
                printUsages();
                return false;
            }
            return true;
        }
        printUsages();
        return false;
    }

    void printUsages() const noexcept(true) override {
        printLines({"Shop Command: access the shop and buy new cards",
                    " ",
                    "/shop [buy | show | help",
                    printUsage("buy"),
                    printUsage("show"),
                    "/shop help (show this help menu)"});
    }

};

#endif//COMMANDS_HPP