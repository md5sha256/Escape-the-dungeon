#include "game.hpp"

#include "../model/cardtemplate.hpp"
#include "command_executor.hpp"
#include "command_parser.hpp"
#include "database.hpp"
#include "registry.hpp"
#include "commands.cpp"

class SimpleGameClient : public GameClient {

    CommandExecutor *commandExecutor = nullptr;
    CommandParser *commandParser = nullptr;
    Database *database = nullptr;

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
        database->load();
        std::cout << "Data loading complete!" << std::endl;
    }

    void registerCommands() noexcept(false) {
        validateState();
        Command *echoCommand = new EchoCommand;
        Command *exitCommand = new ExitCommand(this);
        commandExecutor->registerCommand(echoCommand);
        commandExecutor->registerCommand(exitCommand);
    }

    void awaitUserInput() {
        validateState();
        optional<CommandData> input = commandParser->processCommand();
        if (input.is_present()) {
            try {
                commandExecutor->executeCommand(*input.value());
            } catch (CommandExecutionError &ex) {
                printf("%s\n", "An unexpected error occurred when handling the previous command");
                std::cerr << ex.what() << std::endl;
            }
        }
        // Delete the CommandData once it's been used
        delete input.value();
    }

    public:
    SimpleGameClient(const std::string &root) {
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
        while (running) {
            awaitUserInput();
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
};

GameClient *newGameClient(const std::string &rootDir) noexcept(true) {
    return new SimpleGameClient{rootDir};
}
