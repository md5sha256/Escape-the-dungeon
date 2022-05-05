#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "card.hpp"
#include "fstream"
#include "registry.cpp"
#include "string"

class Database {

    protected:
    const std::string CARD_DATABASE = "card-database.json";

    std::string root;

    Registry<int, CardTemplate *> *templates;
    vector<Card *> *cardInventory;
    vector<Card *> *cardDeck;

    explicit Database(const std::string &rootPath) {
        templates = new SimpleRegistry<int, CardTemplate *>();
        cardInventory = new vector<Card *>;
        cardDeck = new vector<Card *>;
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

    vector<Card *> *getCardInventory() {
        return cardInventory;
    }

    vector<Card *> *getCardDeck() {
        return cardDeck;
    }

    void load() noexcept(false) {
        load(root);
    }

    virtual void load(const std::string &path) noexcept(false) = 0;

    void save() noexcept(false) {
        save(root);
    }

    virtual void save(const std::string &path) noexcept(false) = 0;
};

Database *newDatabase(const std::string &path) noexcept(true);

#endif//DATABASE_HPP
