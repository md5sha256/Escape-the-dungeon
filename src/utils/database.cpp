#include "database.hpp"
#include <sstream>

#include "../../include/rapidjson/document.h"
#include "optional.hpp"

#include "../../include/rapidjson/istreamwrapper.h"
#include "../../include/rapidjson/ostreamwrapper.h"
#include "../../include/rapidjson/prettywriter.h"

class JsonDatabase : public Database {

    private:
    const char *GLOBAL_CARD_ID = "global-card-id";
    const char *TEMPLATE_ID = "templateId";
    const char *CARD_ID = "id";
    const char *INT_ATTRIBUTES = "int-attributes";
    const char *DOUBLE_ATTRIBUTES = "double-attributes";
    const char *STRING_ATTRIBUTES = "string-attributes";
    const char *NAME = "name";
    const char *ATTRIBUTES = "attributes";
    const char *SKILL_POINTS = "skill-points";
    const char *GOLD = "gold";
    const char *PATH = "path";
    const char *PATH_POSITION = "path-pos";
    const char *INVENTORY = "inventory";

    int lastCardId = 0;

    void initGlobalIds(rapidjson::Document &document) {
        auto cardId = document.FindMember(GLOBAL_CARD_ID);
        if (cardId != document.MemberEnd()) {
            lastCardId = cardId->value.GetInt();
        }
    }

    void saveGlobalIds(rapidjson::Document &document) {
        rapidjson::Value value(lastCardId);
        document.AddMember(rapidjson::StringRef(GLOBAL_CARD_ID), value, document.GetAllocator());
    }

    protected:
    rapidjson::Value serialize_card(Card *card, rapidjson::Document::AllocatorType &alloc) {
        rapidjson::Value templateId(card->getTemplateId());
        rapidjson::Value cardId(card->getId());
        rapidjson::Value intAttributes;
        intAttributes.SetObject();
        rapidjson::Value doubleAttributes;
        doubleAttributes.SetObject();
        rapidjson::Value stringAttributes;
        stringAttributes.SetObject();
        for (auto &pair : card->getIntAttributes()) {
            rapidjson::Value value(pair.second);
            intAttributes.AddMember(rapidjson::StringRef(pair.first.c_str()), value, alloc);
        }
        for (auto &pair : card->getDoubleAttributes()) {
            rapidjson::Value value(pair.second);
            intAttributes.AddMember(rapidjson::StringRef(pair.first.c_str()), value, alloc);
        }
        for (auto &pair : card->getStringAttributes()) {
            rapidjson::Value value(rapidjson::StringRef(pair.second.data()), alloc);
            intAttributes.AddMember(rapidjson::StringRef(pair.first.c_str()), value, alloc);
        }
        rapidjson::Value member;
        member.SetObject();
        member.AddMember(rapidjson::StringRef(TEMPLATE_ID), templateId, alloc);
        member.AddMember(rapidjson::StringRef(CARD_ID), cardId, alloc);
        member.AddMember(rapidjson::StringRef(INT_ATTRIBUTES), intAttributes, alloc);
        member.AddMember(rapidjson::StringRef(DOUBLE_ATTRIBUTES), doubleAttributes, alloc);
        member.AddMember(rapidjson::StringRef(STRING_ATTRIBUTES), stringAttributes, alloc);
        return member;
    }

    void saveCards(rapidjson::Document &document, const std::vector<Card *> &cards, const char *key) {
        rapidjson::Value value;
        value.SetObject();
        rapidjson::Document::AllocatorType alloc = document.GetAllocator();
        for (int i = 0; i < (int) cards.size(); i++) {
            rapidjson::Value member = serialize_card(cards[i], alloc);
            auto index = rapidjson::StringRef(std::to_string(i).data());
            value.AddMember(index, member, alloc);
        }
        document.AddMember(rapidjson::StringRef(key), value, document.GetAllocator());
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

    std::vector<Card *> loadCards(rapidjson::Value &deckValue) {
        std::map<int, Card *> cards;
        int max = 0;
        auto iter = deckValue.MemberBegin();
        while (iter != deckValue.MemberEnd()) {
            int index = iter->name.GetInt();
            Card card = deserialize_card(iter->value);
            cards[index] = &card;
            if (index > max) {
                max = index;
            }
        }
        auto ret = std::vector<Card *>(max);
        for (auto pair : cards) {
            ret.at(pair.first) = pair.second;
        }
        return ret;
    }

    static rapidjson::Value serializeInts(const std::vector<int> &ints, rapidjson::Document::AllocatorType &allocator) {
        rapidjson::Value ret;
        ret.SetArray();
        auto array = ret.GetArray();
        for (const int &i : ints) {
            ret.PushBack(i, allocator);
        }
        return ret;
    }

    static std::vector<int> deserializeInts(const rapidjson::Value &value) {
        std::vector<int> ret;
        if (!value.IsArray()) {
            return ret;
        }
        auto array = value.GetArray();
        ret.reserve(array.Size());
        auto iter = array.Begin();
        while (iter != array.End()) {
            ret.push_back(iter->GetInt());
            iter++;
        }
        return ret;
    }

    static rapidjson::Value serializeAttributes(const std::map<Entity::Attribute, int> &attributes, rapidjson::Document::AllocatorType &allocator) {
        rapidjson::Value ret;
        ret.SetObject();
        for (auto pair : attributes) {
            std::string name = Entity::getAttributeName(pair.first);
            rapidjson::Value value(pair.second);
            rapidjson::Value key(name.data(), allocator);
            ret.AddMember(key, value, allocator);
        }
        return ret;
    }

    static std::map<Entity::Attribute, int> deserializeAttributes(rapidjson::Value &value) {
        std::map<Entity::Attribute, int> ret;
        auto iter = value.MemberBegin();
        while (iter != value.MemberEnd()) {
            auto key = &iter->name;
            auto val = &iter->value;
            std::string rawAttr = key->GetString();
            Entity::Attribute attribute;
            try {
                attribute = Entity::getAttribute(rawAttr);
            } catch (std::invalid_argument &ex) {
                std::cout << "[DATABASE] invalid attribute name " << attribute << " found when loading player data";
                continue;
            }
            int attrValue = val->GetInt();
            ret[attribute] = attrValue;
            iter++;
        }
        return ret;
    }

    void savePlayer(rapidjson::Document &document, Player &player) {
        rapidjson::Value name(player.getName().data(), document.GetAllocator());
        rapidjson::Value gold(player.getGold());
        rapidjson::Value path = serializeInts(player.getPath(), document.GetAllocator());
        rapidjson::Value pos(player.getPosition());
        rapidjson::Value attributes = serializeAttributes(player.getAttributes(), document.GetAllocator());
        rapidjson::Value skillPoints(player.getSkillPoints());
        rapidjson::Document::AllocatorType alloc = document.GetAllocator();
        document.AddMember(rapidjson::StringRef(NAME), name, alloc);
        document.AddMember(rapidjson::StringRef(GOLD), gold, alloc);
        document.AddMember(rapidjson::StringRef(PATH), path, alloc);
        document.AddMember(rapidjson::StringRef(PATH_POSITION), pos, alloc);
        document.AddMember(rapidjson::StringRef(ATTRIBUTES), attributes, alloc);
        document.AddMember(rapidjson::StringRef(SKILL_POINTS), skillPoints, alloc);
        saveCards(document, player.getInventory(), INVENTORY);
    }

    Optional<Player> loadPlayer(rapidjson::Document &document) {
        auto name = document.FindMember(NAME);
        if (name == document.MemberEnd()) {
            return nullopt<Player>();
        }
        std::string playerName = name->value.GetString();
        auto *player = new Player(playerName);
        auto gold = document.FindMember(GOLD);
        if (gold != document.MemberEnd()) {
            player->modifyGold(gold->value.GetInt());
        }
        auto skillPoints = document.FindMember(SKILL_POINTS);
        if (skillPoints != document.MemberEnd()) {
            player->modifySkillPoints(skillPoints->value.GetInt());
        }
        auto path = document.FindMember(PATH);
        if (path != document.MemberEnd()) {
            player->setPath(deserializeInts(path->value));
        }
        auto attributes = document.FindMember(ATTRIBUTES);
        if (attributes != document.MemberEnd()) {
            std::cout << "Loading attributes" << std::endl;
            auto attrs = deserializeAttributes(attributes->value);
            std::cout << "Size: " << attrs.size() << std::endl;
            player->setAttributes(attrs);
        }
        auto inventory = document.FindMember(INVENTORY);
        if (inventory != document.MemberEnd()) {
            auto cards = loadCards(inventory->value);
            player->setInventory(cards);
        }
        return Optional<Player>{player};
    }

    public:
    explicit JsonDatabase(const std::string &rootPath) : Database(rootPath) {
    }

    ~JsonDatabase() = default;

    Optional<Player> load(const std::string &path) noexcept(false) override {
        std::ifstream databaseFile;
        databaseFile.open(path + PLAYER_DATA);
        rapidjson::IStreamWrapper databaseWrapper(databaseFile);
        rapidjson::Document data;
        data.ParseStream(databaseWrapper);
        databaseFile.close();
        if (!data.IsObject()) {
            return nullopt<Player>();
        }
        initGlobalIds(data);
        return loadPlayer(data);
    }

    void save(const std::string &path, Player &player) noexcept(false) override {
        rapidjson::Document data;
        data.SetObject();
        saveGlobalIds(data);
        savePlayer(data, player);
        std::ofstream databaseFile;

        databaseFile.open(path + PLAYER_DATA, std::ios::out | std::ios::trunc);
        rapidjson::OStreamWrapper databaseWrapper(databaseFile);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(databaseWrapper);
        writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
        data.Accept(writer);
        databaseFile.close();
    }

    Card *createCard(const int &templateId) noexcept(true) override {
        return new Card{lastCardId++, templateId};
    }
};

Database *newDatabase(const std::string &path) noexcept(true) {
    return new JsonDatabase{path};
}
