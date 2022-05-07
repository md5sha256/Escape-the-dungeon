#ifndef DATABASE_H
#define DATABASE_H

#include "../playerinfo.h"
#include "card.h"
#include "fstream"
#include "registry.cpp"
#include "string"

class Database {

    protected:
    const std::string PLAYER_DATA = "player-data.json";

    std::string root;

    std::vector<Card *> *cardInventory;
    std::vector<Card *> *cardDeck;

    explicit Database(const std::string &rootPath) {
        cardInventory = new std::vector<Card *>;
        cardDeck = new std::vector<Card *>;
        root = rootPath;
    }

    public:
    virtual ~Database() {
        for (auto ptr : *cardDeck) {
            delete ptr;
        }
        for (auto ptr : *cardInventory) {
            delete ptr;
        }
        delete cardInventory;
        delete cardDeck;
    }

    Optional<Player> load() noexcept(false) {
        return load(root);
    }

    virtual Optional<Player> load(const std::string &path) noexcept(false) = 0;

    void save(Player &player) noexcept(false) {
        save(root, player);
    }

    virtual Card *createCard(const int &templateId) noexcept(true) = 0;

    virtual void save(const std::string &path, Player &player) noexcept(false) = 0;


    virtual void reset() noexcept(false) = 0;
};

Database *newDatabase(const std::string &path) noexcept(true);

#endif//DATABASE_H
