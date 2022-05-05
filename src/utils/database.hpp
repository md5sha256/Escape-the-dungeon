#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "../playerinfo.hpp"
#include "card.hpp"
#include "fstream"
#include "registry.cpp"
#include "string"

class Database {

    protected:
    const std::string PLAYER_DATA = "player-data.json";

    std::string root;

    Registry<int, CardTemplate *> *templates;
    std::vector<Card *> *cardInventory;
    std::vector<Card *> *cardDeck;

    explicit Database(const std::string &rootPath) {
        templates = new SimpleRegistry<int, CardTemplate *>();
        cardInventory = new std::vector<Card *>;
        cardDeck = new std::vector<Card *>;
        root = rootPath;
    }

    public:
    ~Database() {
        for (auto pair : templates->toMap()) {
            delete pair.second;
        }
        for (auto ptr : *cardDeck) {
            delete ptr;
        }
        for (auto ptr : *cardInventory) {
            delete ptr;
        }
        delete templates;
        delete cardInventory;
        delete cardDeck;
    }

    Registry<int, CardTemplate *> *getTemplates() {
        return templates;
    }

    optional<Player> load() noexcept(false) {
        return load(root);
    }

    virtual optional<Player> load(const std::string &path) noexcept(false) = 0;

    void save(Player &player) noexcept(false) {
        save(root, player);
    }

    virtual void save(const std::string &path, Player &player) noexcept(false) = 0;
};

Database *newDatabase(const std::string &path) noexcept(true);

#endif//DATABASE_HPP
