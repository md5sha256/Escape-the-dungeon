#include "game.hpp"

#include "cardtemplates.hpp"
#include "command_executor.hpp"
#include "command_parser.hpp"
#include "commands.hpp"
#include "database.hpp"

class SimpleGameClient : public GameClient {

    CommandExecutor *commandExecutor = nullptr;
    CommandParser *commandParser = nullptr;
    Database *database = nullptr;
    Player *player = nullptr;
    BattleHandler *battleHandler = nullptr;
    Registry<int, CardTemplate *> *templateRegistry = nullptr;

    bool running = false;

    std::string dataDir;

    void validateState() const noexcept(false) {
        if (!running) {
            throw std::runtime_error("Client is not running!");
        }
    }

    void loadData() noexcept(false) {
        validateState();
        std::cout << "Loading data..." << std::endl;
        Optional<Player> optionalPlayer = database->load();
        std::cout << "Data loading complete!" << std::endl;
        if (optionalPlayer.isEmpty()) {
            player = initPlayer();
        } else {
            player = optionalPlayer.value();
            performGreetBack(player);
        }
    }

    void saveData() noexcept(false) {
        if (player != nullptr) {
            std::cout << "Saving data..." << std::endl;
            database->save(*player);
            std::cout << "Data save complete!" << std::endl;
        }
    }

    void registerCommands() noexcept(false) {
        validateState();
        Command *restartCommand = new RestartCommand(this);
        Command *exitCommand = new ExitCommand(this);
        Command *skillsCommand = new SkillCommand;
        Command *skipCommand = new SkipCommand;
        Command *shopCommand = new ShopCommand(this);
        Command *statusCommand = new StatusCommand;
        Command *mapCommand = new MapCommand;
        Command *statsCommand = new StatsCommand;
        Command *battleCommand = new BattleCommand(battleHandler);
        Command *cardCommand = new CardsCommand(this);
        Command *helpCommand = new HelpCommand(commandExecutor);
        commandExecutor->registerCommands({
            restartCommand, exitCommand, skillsCommand,
            skipCommand, shopCommand, statusCommand,
            mapCommand, statsCommand, battleCommand,
            cardCommand, helpCommand
        });

    }

    void registerTemplates() noexcept(false) {
        validateState();
        CardTemplate *skillPointTemplate = new SkillPointCardTemplate;
        CardTemplate *damageTemplate = new DamageCardTemplate(this);
        CardTemplate *teleportTemplate = new TeleportCardTemplate(this);
        CardTemplate *winBattleTemplate = new WinBattleCardTemplate(this);
        CardTemplate *reviveTemplate = new ReviveCardTemplate;
        templateRegistry->add(skillPointTemplate->getId(), skillPointTemplate);
        templateRegistry->add(damageTemplate->getId(), damageTemplate);
        templateRegistry->add(teleportTemplate->getId(), teleportTemplate);
        templateRegistry->add(winBattleTemplate->getId(), winBattleTemplate);
        templateRegistry->add(reviveTemplate->getId(), reviveTemplate);
    }

    bool awaitUserInput() {
        validateState();
        Optional<CommandData> input = commandParser->processCommand();
        if (input.isPresent()) {
            CommandData commandData = *input.value();
            if (commandData.getCommandName().empty()) {
                printf("%s: %s\n", "Unknown command", toString(commandData.getArgs()).data());
                return true;
            }
            try {
                commandExecutor->executeCommand(player, commandData);
            } catch (CommandExecutionError &ex) {
                printf("%s\n", "An unexpected error occurred when handling the previous command");
                std::cerr << ex.what() << std::endl;
            }
            // Delete the CommandData once it's been used
            delete input.value();
            return true;
        }
        return false;
    }

    void checkPosition() {
        if (player->getPosition() < 10) {
            switch (player->getPathAtPosition()) {
                case BATTLE_PATH: {
                    startBattle(player, battleHandler);
                    break;
                }
                case CAMPFIRE_PATH: {
                    //when campfire is encountered
                    campfire(player);
                    // Move the player along
                    player->incrementPosition();
                    break;
                }
                case SHOP_PATH: {
                    std::cout << player->getName() << " arrived at a shop!" << std::endl;
                    std::cout << "run /shop to continue" << std::endl;
                    std::cout << "run /skip to skip this shop" << std::endl;
                    break;
                }
                case EVENT_PATH: {
                    // when random event is encountered
                    randomEvent(player);
                    // MOve the player along
                    player->incrementPosition();
                    break;
                }
                case WIN_PATH: {
                    std::cout << player->getName() << " has finally escaped the dungeon!" << std::endl;
                    std::cout << "type /restart " << " to restart the game" << std::endl;
                    return;
                }
            }
        }
    }

    public:
    explicit SimpleGameClient(const std::string &root) {
        dataDir = root;
    }

    ~SimpleGameClient() override {
        delete commandExecutor;
        delete commandParser;
        delete battleHandler;
        delete player;
    }

    bool isRunning() noexcept(true) override {
        return running;
    }

    void init() noexcept(false) override {
        if (running) {
            return;
        }
        srand(time(0));
        commandExecutor = newCmdExecutor();
        commandParser = newCmdParser();
        templateRegistry = newRegistry<int, CardTemplate *>();
        battleHandler = new BattleHandler;
        database = newDatabase(dataDir);
    }

    void start() noexcept(false) override {
        if (running) {
            return;
        }
        running = true;

        registerTemplates();
        registerCommands();
        loadData();
        checkPosition();
        while (running) {
            if (!awaitUserInput() && running) {
                checkPosition();
            }
        }
        // Shutdown as we are no longer running
        shutdown();
    }

    void stop() noexcept(true) override {
        running = false;
    }

    void shutdown() noexcept(false) override {
        // Call stop just in case we aren't already stopped
        stop();
        saveData();
    }

    [[nodiscard]] Player *getPlayer() const noexcept(true) override {
        return player;
    }

    [[nodiscard]] BattleHandler *getBattleHandler() const noexcept(true) override {
        return battleHandler;
    }

    [[nodiscard]] Registry<int, CardTemplate *> *getCardTemplates() const noexcept(true) override {
        return templateRegistry;
    }

    [[nodiscard]] Database *getDatabase() const noexcept(true) override {
        return database;
    }

    void resetProgress() noexcept(false) override {
        delete player;
        player = initPlayer();
        database->reset();
        saveData();
        player->printStatus();
    }

};

GameClient *newGameClient(const std::string &rootDir) noexcept(true) {
    return new SimpleGameClient{rootDir};
}
