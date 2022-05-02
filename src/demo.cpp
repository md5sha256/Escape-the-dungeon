#include "demo.hpp"

using namespace std;
int main() {
    CommandExecutor *executor = newCmdExecutor();
    auto *echoCmd = new EchoCommand;
    executor->registerCommand(echoCmd);
    CommandData sample("echo", vector<string>{"sample hello world!"});
    executor->executeCommand(sample);
    CommandParser*parser = newCmdParser();
    optional<CommandData> nextCmd = parser->processCommand();
    while (nextCmd.is_present()) {
        executor->executeCommand(*nextCmd.value());
        nextCmd = parser->processCommand();
    }
    cout << "Goodbye!";

    return 0;
}
