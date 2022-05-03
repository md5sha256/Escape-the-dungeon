#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "../model/cardtemplate.hpp"
#include "fstream"
#include "registry.hpp"
#include "string"

class Database {

    protected:
    const std::string TEMPLATE_FILE = "templates.json";
    const std::string CARD_DATABASE = "card-database.json";

    std::string root;

    Registry<int, CardTemplate> *templates;
    Registry<int, Card> *cards;

    explicit Database(const std::string &rootPath) {
        templates = newRegistry<int, CardTemplate>();
        cards = newRegistry<int, Card>();
        root = rootPath;
    }

    public:
    ~Database() {
        // Delete the actual registries
        delete &templates;
        delete &cards;
        // Delete the pointers
        delete templates;
        delete cards;
    }

    Registry<int, CardTemplate> *getTemplates() {
        return templates;
    }

    Registry<int, Card> *getCards() {
        return cards;
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
