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
        std::cin.getline(raw, 1024, '\n');
        std::stringstream ss(raw);
        std::string cmdName;
        if (ss >> cmdName && !cmdName.empty()) {
            std::vector<std::string> args;
            if (cmdName[0] == identifier) {
                cmdName.erase(cmdName.begin());
            } else {
                args.push_back(cmdName);
                cmdName = "";
            }
            std::string arg;
            while (ss >> arg) {
                args.push_back(arg);
            }
            return optional<CommandData>(new CommandData(cmdName, args));
        } else {
            // Not enough input/blank string
            //std::cout << "Blank command entered. Please enter a command." << std::endl;
            return nullopt<CommandData>();
        }
    }
};

CommandParser *newCmdParser() {
    return new SimpleCommandParser;
}
