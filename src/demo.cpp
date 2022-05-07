#include "demo.h"
#include "utils/game.h"

int main() {
    GameClient* client = newGameClient(getWorkDir() + "/");
    client->init();
    client->start();
    delete client;
    return 0;
}