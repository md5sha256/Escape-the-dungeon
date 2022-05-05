#include "demo.hpp"
#include "utils/game.hpp"

int main() {
    GameClient* client = newGameClient(getWorkDir() + "/data");
    client->init();
    client->start();
    delete client;
    return 0;
}