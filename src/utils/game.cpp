#include "game.hpp"

#include "../model/cardtemplate.hpp"
#include "command_executor.hpp"
#include "command_parser.hpp"
#include "commands.hpp"
#include "database.hpp"
#include "registry.hpp"

class SimpleGameClient : public GameClient {

    CommandExecutor *commandExecutor = nullptr;
    CommandParser *commandParser = nullptr;
    Database *database = nullptr;
    PlayerInfo* player = nullptr;

    bool running = false;

    std::string dataDir;

    void validateState() const noexcept(false) {
        if (!running) {
            throw std::runtime_error("Client is not running!");
        }
    }

    void loadData() noexcept(false) {
        validateState();
        player = new PlayerInfo;
        std::cout << "Loading data..." << std::endl;
        if (database != nullptr) {
            database->load();
        }
        std::cout << "Data loading complete!" << std::endl;
    }

    void registerCommands() noexcept(false) {
        validateState();
        Command *echoCommand = new EchoCommand;
        Command *exitCommand = new ExitCommand(this);
        commandExecutor->registerCommand(echoCommand);
        commandExecutor->registerCommand(exitCommand);
    }

    bool awaitUserInput() {
        validateState();
        optional<CommandData> input = commandParser->processCommand();
        if (input.is_present()) {
            try {
                commandExecutor->executeCommand(*player, *input.value());
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
        PlayerInfo p = *player;
        while(p.position!=10){
            if(p.path[p.position]==0){//when battle is encountered
                vector<enemy> enemy_generated;
                enemy_generated=generate_enemies();
            }
            else if(p.path[p.position]==1){//when campfire is encountered
                campfire(p);
            }
            else if(p.path[p.position]==2){//when shop is encountered

            }
            else if(p.path[p.position]==3){//when random event is encountered

            }
            else if(p.path[p.position]==4){//when boss is encountered

            }

            p.position+=1;
        }
    }

    public:
    explicit SimpleGameClient(const std::string &root) {
        dataDir = root;
    }

    ~SimpleGameClient() override {
        delete commandExecutor;
        delete commandParser;
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
        database = newDatabase(dataDir);
    }

    void start() noexcept(false) override {
        if (running) {
            return;
        }
        running = true;

        registerCommands();
        loadData();
        initPlayer(*player);
        while (running) {
            if (!awaitUserInput()) {
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

    [[nodiscard]] PlayerInfo *getPlayer() const noexcept(true) override {
        return player;
    }

};

GameClient *newGameClient(const std::string &rootDir) noexcept(true) {
    return new SimpleGameClient{rootDir};
}
