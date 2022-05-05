#include "database.hpp"

#include "../../include/rapidjson/document.h"
#include "optional.hpp"

#include "../../include/rapidjson/istreamwrapper.h"
#include "../../include/rapidjson/ostreamwrapper.h"
#include "../../include/rapidjson/writer.h"

class JsonDatabase : public Database {

    private:
    const char *TEMPLATE_ID = "templateId";
    const char *CARD_ID = "id";
    const char *INT_ATTRIBUTES = "int-attributes";
    const char *DOUBLE_ATTRIBUTES = "double-attributes";
    const char *STRING_ATTRIBUTES = "string-attributes";
    const char *DECK = "deck";
    const char *INVENTORY = "inventory";

    protected:
    rapidjson::Value serialize_card(Card *card) {
        rapidjson::Value templateId;
        templateId.SetInt(card->getTemplateId());
        rapidjson::Value cardId;
        cardId.SetInt(card->getId());
        rapidjson::Value intAttributes;
        rapidjson::Value doubleAttributes;
        rapidjson::Value stringAttributes;
        for (auto &pair : card->getIntAttributes()) {
            rapidjson::Value value;
            value.SetInt(pair.second);
            intAttributes[pair.first.c_str()] = value;
        }
        for (auto &pair : card->getDoubleAttributes()) {
            rapidjson::Value value;
            value.SetDouble(pair.second);
            doubleAttributes[pair.first.c_str()] = value;
        }
        for (auto &pair : card->getStringAttributes()) {
            rapidjson::Value value;
            std::string data = pair.second;
            value.SetString(data.c_str(), data.size());
            stringAttributes[pair.first.c_str()] = value;
        }
        rapidjson::Value member;
        member[TEMPLATE_ID] = templateId;
        member[CARD_ID] = cardId;
        member[INT_ATTRIBUTES] = intAttributes;
        member[DOUBLE_ATTRIBUTES] = doubleAttributes;
        member[STRING_ATTRIBUTES] = stringAttributes;
        return member;
    }

    void saveCards(rapidjson::Document &document, const vector<Card *> &cards, const char *key) {
        rapidjson::Value value;
        for (int i = 0; i < cards.size(); i++) {
            rapidjson::Value member = serialize_card(cards[i]);
            const char *index = to_string(i).c_str();
            value[index] = member;
        }
        document[key] = value;
    }

    void saveCards(rapidjson::Document &document) {
        saveCards(document, *getCardDeck(), DECK);
        saveCards(document, *getCardInventory(), INVENTORY);
    }

    Card deserialize_card(rapidjson::Value &value) {
        int templateId = value.FindMember(TEMPLATE_ID)->value.GetInt();
        int cardId = value.FindMember(CARD_ID)->value.GetInt();
        std::map<const std::string, int> intAttributes;
        std::map<const std::string, double> doubleAttributes;
        std::map<const std::string, std::string> stringAttributes;
        auto ints = value.FindMember(INT_ATTRIBUTES);
        if (ints != value.MemberEnd()) {
            auto intValue = &ints->value;
            auto iter = intValue->MemberBegin();
            while (iter != intValue->MemberEnd()) {
                intAttributes[iter->name.GetString()] = iter->value.GetInt();
            }
        }
        auto doubles = value.FindMember(DOUBLE_ATTRIBUTES);
        if (doubles != value.MemberEnd()) {
            auto doubleValue = &doubles->value;
            auto iter = doubleValue->MemberBegin();
            while (iter != doubleValue->MemberEnd()) {
                doubleAttributes[iter->name.GetString()] = iter->value.GetDouble();
            }
        }
        auto strings = value.FindMember(STRING_ATTRIBUTES);
        if (doubles != value.MemberEnd()) {
            auto stringValue = &strings->value;
            auto iter = stringValue->MemberBegin();
            while (iter != stringValue->MemberEnd()) {
                stringAttributes[iter->name.GetString()] = iter->value.GetString();
            }
        }
        return Card{cardId, templateId, intAttributes, doubleAttributes, stringAttributes};
    }

    vector<Card *> loadCards(rapidjson::Document &document, const char *key) {
        auto deck = document.FindMember(key);
        std::map<int, Card *> cards;
        int max = 0;
        if (deck != document.MemberEnd()) {
            auto deckValue = &deck->value;
            auto iter = deckValue->MemberBegin();
            while (iter != deckValue->MemberEnd()) {
                int index = iter->name.GetInt();
                Card card = deserialize_card(iter->value);
                cards[index] = &card;
                if (index > max) {
                    max = index;
                }
            }
        }
        auto ret = vector<Card *>(max);
        for (auto pair : cards) {
            ret.at(pair.first) = pair.second;
        }
        return ret;
    }

    public:
    explicit JsonDatabase(const std::string &rootPath) : Database(rootPath) {
    }

    ~JsonDatabase() = default;

    void load(const std::string &path) noexcept(false) override {
        ifstream databaseFile;
        databaseFile.open(path + CARD_DATABASE, std::ios::in | std::ios::ate);
        rapidjson::IStreamWrapper databaseWrapper(databaseFile);
        rapidjson::Document cards;
        cards.ParseStream(databaseWrapper);
        databaseFile.close();
        auto deck = loadCards(cards, DECK);
        cardDeck = &deck;
        auto inventory = loadCards(cards, INVENTORY);
        cardInventory = &inventory;
    }

    void save(const std::string &path) noexcept(false) override {
        rapidjson::Document cards;
        saveCards(cards);
        ofstream databaseFile;
        databaseFile.open(path + CARD_DATABASE, std::ios::out | std::ios::trunc);
        rapidjson::OStreamWrapper databaseWrapper = (databaseFile);
        rapidjson::Writer<rapidjson::OStreamWrapper> writer(databaseWrapper);
        cards.Accept(writer);
        databaseFile.close();
    }
};

Database *newDatabase(const std::string &path) noexcept(true) {
    return new JsonDatabase{path};
}
