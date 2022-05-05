#include "game.hpp"

#include "command_executor.hpp"
#include "command_parser.hpp"
#include "commands.hpp"
#include "database.hpp"
#include "registry.hpp"

class SimpleGameClient : public GameClient {

    CommandExecutor *commandExecutor = nullptr;
    CommandParser *commandParser = nullptr;
    Database *database = nullptr;
    Player *player = nullptr;
    BattleHandler *battleHandler = nullptr;

    bool running = false;

    std::string dataDir;

    void validateState() const noexcept(false) {
        if (!running) {
            throw std::runtime_error("Client is not running!");
        }
    }

    void loadData() noexcept(false) {
        validateState();
        player = initPlayer();
        std::cout << "Loading data..." << std::endl;
        database->load();
        std::cout << "Data loading complete!" << std::endl;
    }

    void registerCommands() noexcept(false) {
        validateState();
        Command *echoCommand = new EchoCommand;
        Command *exitCommand = new ExitCommand(this);
        Command *skillsCommand = new SkillCommand;
        commandExecutor->registerCommand(echoCommand);
        commandExecutor->registerCommand(exitCommand);
        commandExecutor->registerCommand(skillsCommand);
    }

    bool awaitUserInput() {
        validateState();
        optional<CommandData> input = commandParser->processCommand();
        if (input.is_present()) {
            CommandData commandData = *input.value();
            if (commandData.getCommandName().empty()) {
                printf("%s: %s\n", "Unknown command", toString(commandData.getArgs()).data());
                return true;
            }
            try {
                commandExecutor->executeCommand(*player, commandData);
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
                case 0: {
                    std::cout << "battle placeholder" << std::endl;
                    break;
                }
                case 1: {
                    //when campfire is encountered
                    campfire(*player);
                    break;
                }
                case 2: {
                    // when shop is encountered
                    std::cout << "shop placeholder" << std::endl;
                    break;
                }
                case 3: {
                    // when random event is encountered
                    std::cout << "random event placeholder" << std::endl;
                    break;
                }
                case 4: {
                    std::cout << "boss placeholder" << std::endl;
                    // when boss is encountered
                    break;
                }
            }
            player->incrementPosition();
        }
    }

    public:
    explicit SimpleGameClient(const std::string &root) {
        dataDir = root;
    }

    ~SimpleGameClient() override {
        delete commandExecutor;
        delete commandParser;
        //delete battleHandler;
        delete player;
    }

    bool isRunning() noexcept(true) override {
        return running;
    }

    void init() noexcept(false) override {
        if (running) {
            return;
        }
        commandExecutor = newCmdExecutor();
        commandParser = newCmdParser();
        battleHandler = new BattleHandler;
        database = newDatabase(dataDir);
    }

    void start() noexcept(false) override {
        if (running) {
            return;
        }
        running = true;

        registerCommands();
        loadData();
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
    }

    [[nodiscard]] Player *getPlayer() const noexcept(true) override {
        return player;
    }


    [[nodiscard]] BattleHandler *getBattleHandler() const noexcept(true) override {
        return battleHandler;
    }

};

GameClient *newGameClient(const std::string &rootDir) noexcept(true) {
    return new SimpleGameClient{rootDir};
}
