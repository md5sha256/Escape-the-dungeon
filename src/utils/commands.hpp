#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "../shop.hpp"
#include "command.hpp"
#include "game.hpp"
#include "iostream"
#include "utils.hpp"

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

class RestartCommand : public Command {

    private:
    GameClient *client;

    public:
    explicit RestartCommand(GameClient *_client) : Command("restart") {
        client = _client;
    }

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(false) override {
        std::cout << "Are you sure you want to reset your progress? (y/n)" << std::endl;
        std::string input;
        std::cin >> input;
        for (char &c : input) {
            c = (char) tolower(c);
        }
        if (input == "y" || input == "yes") {
            client->resetProgress();
            return true;
        } else if (input != "n" && input != "no") {
            std::cout << "Aborted shutdown, unknown input " << input << std::endl;
            return false;
        }
        return true;
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
        if (subCommand == "show" && args.size() == 1) {
            for (int i = 0; i < currentShop->getSize(); i++) {
                currentShop->printItem(client, player, i);
            }
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
            if (itemIndex < 1 || itemIndex > currentShop->getSize()) {
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
                currentShop->buyItem(player, itemIndex - 1);
            } else if (subCommand == "show") {
                currentShop->printItem(client, player, itemIndex - 1);
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

class StatusCommand : public Command {

    public:
    explicit StatusCommand() : Command("status") {
    }

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(false) override {
        player->printStatus();
        player->printMap();
        std::cout << std::endl;
        return true;
    }
};

class MapCommand : public Command {
    public:
    MapCommand() : Command("whereami") {
    }

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(false) override {
        player->printMap();
        std::cout << std::endl;
        return true;
    }
};

class StatsCommand : public Command {

    public:
    StatsCommand() : Command("stats") {
    }

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(false) override {
        player->printAHD();
        std::cout << std::endl;
        return true;
    }
};

class BattleCommand : public Command {

    private:
    BattleHandler *battleHandler;

    static void printBattleInfo(Battle *battle) {
        int num = battle->getNumEnemies();
        if (num == 0) {
            std::cout << "No enemies remain." << std::endl;
        } else if (num == 1) {
            std::cout << num << " enemy remains." << std::endl;
        } else {
            std::cout << num << " enemies remain." << std::endl;
        }
        Optional<Entity> opponent = battle->getCurrentOpponent();
        if (opponent.isPresent()) {
            battle->printOpponent(*opponent.value());
        }
    }

    void endBattle(Player *player) {
        battleHandler->endBattle();
        int gold = randInt(10, 50);
        std::cout << player->getName() << " has defeated those foul beasts. " << std::endl;
        std::cout << player->getName() << " trudges on to meet his next fate." << std::endl;
        std::cout << player->getName() << " looted " << gold << " gold." << std::endl;
        player->modifyGold(gold);
        player->printGold();
        player->incrementPosition();
        player->printMap();
    }

    bool performAttack(Player *player) {
        Optional<Battle> optionalBattle = battleHandler->getcurrentBattle();
        if (optionalBattle.isEmpty()) {
            std::cout << "You can only perform an attack whilst in battle!" << std::endl;
            return false;
        }
        if (player->isDead()) {
            std::cout << "You can't attack if you're dead!" << std::endl;
            return false;
        }
        Battle *battle = optionalBattle.value();
        Optional<Entity> optionalEnemy = battle->getCurrentOpponent();
        if (optionalEnemy.isEmpty()) {
            optionalEnemy = battle->update()->getCurrentOpponent();
        }
        if (optionalEnemy.isEmpty()) {
            endBattle(player);
            return true;
        }
        Entity *opponent = optionalEnemy.value();
        opponent->takeDamage(player->getAttribute(Attribute::ATTACK));
        if (opponent->isDead()) {
            std::cout << player->getName() << " has killed the " << opponent->getName() << std::endl << std::endl;
        } else {
            std::cout << std::endl << opponent->getName() << " strikes back!" << std::endl;
            player->takeDamage(opponent->getAttribute(Attribute::ATTACK));
            if (player->isDead()) {
                std::cout << player->getName() << " was mortally wounded " << std::endl;
                return true;
            }
        }
        battle->update();
        if (battle->getNumEnemies() == 0) {
            endBattle(player);
        } else {
            printBattleInfo(battle);
        }
        return true;
    }

    public:
    explicit BattleCommand(BattleHandler *handler) : Command("battle") {
        battleHandler = handler;
    }

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(false) override {
        if (args.empty()) {
            Optional<Battle> optional = battleHandler->getcurrentBattle();
            if (optional.isEmpty()) {
                std::cout << "You are not in a battle right now." << std::endl;
            } else {
                Battle *battle = optional.value();
                std::cout << "You are currently engaged in a battle." << std::endl;
                printBattleInfo(battle);
            }
            return true;
        }
        if (toLowercase(args[0]) == "attack") {
            return performAttack(player);
        }
        std::cout << "Unknown subcommand: " + args[0] << std::endl;
        return false;
    }


    void printUsages() const noexcept(true) override {
        std::cout << "Battle command: check if you are in a battle and attack" << std::endl << std::endl;
    }

};

class CardsCommand : public Command {

    GameClient* client;

    public:
    explicit CardsCommand(GameClient *_client) : Command("cards") {
        client = _client;
    }

    bool onCommand(Player *player, std::vector<std::string> &args) noexcept(false) override {
        if (args.empty()) {
            std::cout << "You currently have " << player->getInventorySize() << " cards in your inventory.";
            return true;
        }
        if (toLowercase(args[0]) == "use") {
            if (args.size() < 2) {
                return false;
            }
            Optional<int> index = fromString(args[1]);
            if (index.isEmpty() || player->getInventorySize()==0) {
                std::cout << "Invalid index: " << args[1] << std::endl;
                return false;
            }
            Registry<int, CardTemplate*> *registry = client->getCardTemplates();
            int cardIndex = *index.value();
            Card *card = player->getCard(cardIndex);
            CardTemplate* cardTemplate = *registry->get(cardIndex).value();
            if (cardTemplate->onCardUse(player, card)) {
                player->removeCardFromInventory(card);
            }
            return true;
        }
        return false;
    }

};

#endif//COMMANDS_HPP
