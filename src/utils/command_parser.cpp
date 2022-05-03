#include "command_parser.hpp"

class SimpleCommandParser : public CommandParser {

    char identifier;

    public:
    explicit SimpleCommandParser(const char &id) {
        identifier = id;
    }

    SimpleCommandParser() : SimpleCommandParser('/') {
    }

    optional<CommandData> processCommand() noexcept(true) override {
        char raw[1024];
        std::cin.getline(raw,  1024, '\n');
        std::stringstream ss(raw);
        std::string cmdName;
        if (ss >> cmdName && !cmdName.empty()) {
            if (cmdName[0] != identifier) {
                printf("%s: %s\n", "Unknown command: ", cmdName.data());
                return nullopt<CommandData>();
            }
        } else {
            // Not enough input/blank string
            //std::cout << "Blank command entered. Please enter a command." << std::endl;
            return nullopt<CommandData>();
        }
        std::vector<std::string> args;
        std::string arg;
        while (ss >> arg) {
            args.push_back(arg);
        }
        cmdName.erase(cmdName.begin());
        return optional<CommandData>(new CommandData(cmdName, args));
    }
};

CommandParser* newCmdParser() {
    return new SimpleCommandParser;
}

