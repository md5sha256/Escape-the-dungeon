#include "utils/game.hpp"
#include "utils/utils.hpp"

int main() {
    GameClient* client = newGameClient(getWorkDir() + "/data");
    client->init();
    client->start();
    delete &client;
    return 0;
}