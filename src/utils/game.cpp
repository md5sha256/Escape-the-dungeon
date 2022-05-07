#include "game.h"

#include "cardtemplates.h"
#include "command_executor.h"
#include "command_parser.h"
#include "commands.h"
#include "database.h"

/**
 * Implementation of the GameClient
 */
class SimpleGameClient : public GameClient {

    // Variables needed
    CommandExecutor *commandExecutor = nullptr;
    CommandParser *commandParser = nullptr;
    Database *database = nullptr;
    Player *player = nullptr;
    BattleHandler *battleHandler = nullptr;
    Registry<int, CardTemplate *> *templateRegistry = nullptr;

    bool running = false;

    std::string dataDir;

    /**
     * Ensure that the client is running
     * This method is used internally for sanity checking
     * @throws std::runteim_error if the client is not running
     */
    void validateState() const noexcept(false) {
        if (!running) {
            throw std::runtime_error("Client is not running!");
        }
    }

    /**
     * Load the data from the database
     */
    void loadData() noexcept(false) {
        validateState();
        std::cout << "Loading data..." << std::endl;
        Optional<Player> optionalPlayer = database->load();
        std::cout << "Data loading complete!" << std::endl;
        // Perform initial setup if the database is empty/corrupted
        if (optionalPlayer.isEmpty()) {
            player = initPlayer();
        } else {
            // Use the player object created by the database
            player = optionalPlayer.value();
            // Print a welcome message
            performGreetBack(player);
        }
    }

    /**
     * Persist the data in memory to disk through the database
     */
    void saveData() noexcept(false) {
        if (player != nullptr) {
            std::cout << "Saving data..." << std::endl;
            // Save player data
            database->save(*player);
            std::cout << "Data save complete!" << std::endl;
        }
    }

    /**
     * Register the commands which the player can use
     * all commands are prefixed with the forward slash ('/') character
     */
    void registerCommands() noexcept(false) {
        validateState();
        // Instantiate the commands
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
        // Register the commands to the command executor
        commandExecutor->registerCommands({
            restartCommand, exitCommand, skillsCommand,
            skipCommand, shopCommand, statusCommand,
            mapCommand, statsCommand, battleCommand,
            cardCommand, helpCommand
        });

    }

    /**
     * Register the card templates which can be generated
     */
    void registerTemplates() noexcept(false) {
        validateState();
        // Instantiate the templates
        CardTemplate *skillPointTemplate = new SkillPointCardTemplate;
        CardTemplate *damageTemplate = new DamageCardTemplate(this);
        CardTemplate *teleportTemplate = new TeleportCardTemplate(this);
        CardTemplate *winBattleTemplate = new WinBattleCardTemplate(this);
        CardTemplate *reviveTemplate = new ReviveCardTemplate;
        // Register the templates to the registry
        templateRegistry->add(skillPointTemplate->getId(), skillPointTemplate);
        templateRegistry->add(damageTemplate->getId(), damageTemplate);
        templateRegistry->add(teleportTemplate->getId(), teleportTemplate);
        templateRegistry->add(winBattleTemplate->getId(), winBattleTemplate);
        templateRegistry->add(reviveTemplate->getId(), reviveTemplate);
    }

    /**
     * Await for user input from std::cin
     * This function will block for user input. If the
     * input string is determined to start with a forward-slash ('/'),
     * the entire string will be parsed and the corresponding command
     * will be executed
     * @return <code>true</code> if input from cin was used to process commands <code>false</code otherwise
     */
    bool awaitUserInput() {
        validateState();
        // Try to parse a command from cin
        Optional<CommandData> input = commandParser->processCommand();
        if (input.isPresent()) {
            // Try to evaluate the command
            CommandData commandData = *input.value();
            if (commandData.getCommandName().empty()) {
                // no command was found
                printf("%s: %s\n", "Unknown command", toString(commandData.getArgs()).data());
                return true;
            }
            try {
                // Execute the command through the executor
                commandExecutor->executeCommand(player, commandData);
            } catch (CommandExecutionError &ex) {
                // Catch any unexpected errors
                printf("%s\n", "An unexpected error occurred when handling the previous command");
                std::cerr << ex.what() << std::endl;
            }
            // Delete the CommandData once it's been used
            delete input.value();
            return true;
        }
        return false;
    }

    /**
     * Check the player's current position and execute
     * the necessary code
     */
    void checkPosition() {
        if (player->getPosition() < 10) {
            switch (player->getPathAtPosition()) {
                case BATTLE_PATH: {
                    // Start a battle
                    startBattle(player, battleHandler);
                    // The battle command will increment the player's path
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
                    // /skip will be used to increment the player's path
                    std::cout << player->getName() << " arrived at a shop!" << std::endl;
                    std::cout << "run /shop to continue" << std::endl;
                    std::cout << "run /skip to skip this shop" << std::endl;
                    break;
                }
                case EVENT_PATH: {
                    // when random event is encountered
                    randomEvent(player);
                    // Move the player along
                    player->incrementPosition();
                    break;
                }
                case WIN_PATH: {
                    // The game has been won
                    std::cout << player->getName() << " has finally escaped the dungeon!" << std::endl;
                    std::cout << "type /restart " << " to restart the game" << std::endl;
                    break;
                }
            }
        }
    }

    public:
    explicit SimpleGameClient(const std::string &root) {
        dataDir = root;
    }

    ~SimpleGameClient() override {
        // Delete the points
        delete commandExecutor;
        delete commandParser;
        delete database;
        delete battleHandler;
        delete player;
        delete templateRegistry;
    }

    bool isRunning() noexcept(true) override {
        return running;
    }

    void init() noexcept(false) override {
        if (running) {
            return;
        }
        // seed rand
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
        // keep reading from cin and reacting until a command (/exit) is called
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
        // Save data
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
        // delete the existing player object
        delete player;
        // prompt the player to setup again
        player = initPlayer();
        // reset the database
        database->reset();
        // save the new data
        saveData();
        // print the player's current status
        player->printStatus();
    }

};

GameClient *newGameClient(const std::string &rootDir) noexcept(true) {
    return new SimpleGameClient{rootDir};
}
