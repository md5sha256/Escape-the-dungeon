#include "utils/game.hpp"

int main() {
    GameClient* client = newGameClient();
    client->init();
    client->start();
    return 0;
}