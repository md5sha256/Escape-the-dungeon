#ifndef ESCAPE_THE_DUNGEON_COMMAND_H
#define ESCAPE_THE_DUNGEON_COMMAND_H

#include <string>
#include <vector>

using namespace std;

struct CommandData {
    private:
    string commandName;
    vector<string> args;

    CommandData(const string &name, const vector<string> &arguments) {
        commandName = name;
        args = arguments;
    }

    public:
    string getCommandName() {
        return {commandName};
    }

    public:
    vector<string> getArgs() {
        return {args};
    }
};

#endif//ESCAPE_THE_DUNGEON_COMMAND_H
