#include "demo.h"
#include "utils/game.h"

int main() {
    // Instantiate the client
    GameClient* client = newGameClient(getWorkDir() + "/");
    // Init and start the client
    client->init();
    client->start();
    // Free the memory of the client at the end
    delete client;
    return 0;
}