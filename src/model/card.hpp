#ifndef CARD_HPP
#define CARD_HPP

#include "../playerinfo.hpp"
#include "../utils/optional.hpp"
#include "map"
#include "type_traits"

struct Card {

    private:
    int id;

    int templateId;

    std::map<const std::string, int> intAttributes;
    std::map<const std::string, double> doubleAttributes;
    std::map<const std::string, std::string> stringAttributes;

    template<typename T>
    [[nodiscard]] static optional<T> getAttribute(const std::string &key, const std::map<const std::string, T> map) {
        auto iter = map.find(key);
        if (iter != map.end()) {
            auto value = iter->second;
            return optional<T>(&value);
        }
        return nullopt<T>();
    }

    public:
    Card(const int cardId, const int cardTemplateId) {
        id = cardId;
        templateId = cardTemplateId;
    }

    Card(int cardId, int cardTemplateId,
         const std::map<const std::string, int> &ints,
         const std::map<const std::string, double> &doubles,
         const std::map<const std::string, std::string> &strings) : Card(cardId, cardTemplateId) {
        intAttributes = ints;
        doubleAttributes = doubles;
        stringAttributes = strings;
    }

    [[nodiscard]] int getTemplateId() const {
        return templateId;
    }

    [[nodiscard]] int getId() const {
        return id;
    }

    [[nodiscard]] optional<double> getIntAttribute(const std::string &key) const {
        return getAttribute<double>(key, doubleAttributes);
    }

    [[nodiscard]] optional<int> getDoubleAttribute(const std::string &key) const {
        return getAttribute<int>(key, intAttributes);
    }

    [[nodiscard]] optional<std::string> getStringAttribute(const std::string &key) const {
        return getAttribute<std::string>(key, stringAttributes);
    }

    void setIntAttribute(const std::string &key, const int &value) {
        intAttributes[key] = value;
    }

    void setDoubleAttribute(const std::string &key, const double &value) {
        doubleAttributes[key] = value;
    }

    void setStringAttribute(const std::string &key, const std::string &value) {
        stringAttributes[key] = value;
    }

    std::map<const std::string, int> getIntAttributes() const {
        return std::map<const std::string, int>{intAttributes};
    }

    std::map<const std::string, double> getDoubleAttributes() const {
        return std::map<const std::string, double>{doubleAttributes};
    }

    std::map<const std::string, std::string> getStringAttributes() const {
        return std::map<const std::string, std::string>{stringAttributes};
    }



};

class CardTemplate {

    int id;

    protected:
    explicit CardTemplate(int cardId) {
        id = cardId;
    }

    public:
    [[nodiscard]] int getId() const noexcept(true) {
        return id;
    }

    virtual void onCardUse(const player_info &player, const Card &cardData) = 0;
};

#endif//CARD_HPP
