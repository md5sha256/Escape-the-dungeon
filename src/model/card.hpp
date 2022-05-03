#ifndef CARD_HPP
#define CARD_HPP

#include "../utils/optional.hpp"
#include "map"
#include "type_traits"

struct Dummy {
    virtual void dummy() = 0;
};

template<typename T>
struct Attribute : Dummy {
    T value;
    void dummy() override {
    }
};

struct Card {

    int id;

    int templateId;

    std::map<int, Dummy *> attributes;

    public:
    Card(int cardId, int cardTemplateId) {
        id = cardId;
        templateId = cardTemplateId;
    }

    [[nodiscard]] int getTemplateId() const {
        return templateId;
    }

    [[nodiscard]] int getId() const {
        return id;
    }

    template<typename T>
    optional<Attribute<T>> getAttribute(int key) const {
        auto iter = attributes.find(key);
        if (iter == attributes.end()) {
            return nullopt<Attribute<T>>();
        }
        Dummy *dummy = iter->second;
        if (dynamic_cast<Attribute<T>*>(dummy) != NULL) {
            return optional<Attribute<T>>{(Attribute<T>) &dummy};
        }
        return nullopt<Attribute<T>>();
    }

    template<typename T>
    void setAttribute(int key, Attribute<T> value) {
        attributes[key] = *value;
    }
};

#endif//CARD_HPP
