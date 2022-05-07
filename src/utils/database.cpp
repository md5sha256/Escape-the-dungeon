#include "database.h"
#include <sstream>

#include "../../include/rapidjson/document.h"
#include "optional.h"

#include "../../include/rapidjson/istreamwrapper.h"
#include "../../include/rapidjson/ostreamwrapper.h"
#include "../../include/rapidjson/prettywriter.h"

class JsonDatabase : public Database {

    private:
    //Name constants
    const char *GLOBAL_CARD_ID = "global-card-id";
    const char *TEMPLATE_ID = "templateId";
    const char *CARD_ID = "id";
    const char *INT_ATTRIBUTES = "int-attributes";
    const char *NAME = "name";
    const char *ATTRIBUTES = "attributes";
    const char *SKILL_POINTS = "skill-points";
    const char *GOLD = "gold";
    const char *PATH = "path";
    const char *PATH_POSITION = "path-pos";
    const char *INVENTORY = "inventory";

    // Default counter is set to 0
    int lastCardId = 0;

    /**
     * Init the global identifiers
     * @param document The Json document to read from
     */
    void initGlobalIds(rapidjson::Document &document) {
        auto cardId = document.FindMember(GLOBAL_CARD_ID);
        // An old value for the counter exists, so use that
        if (cardId != document.MemberEnd()) {
            lastCardId = cardId->value.GetInt();
        }
    }

    /**
     * Save the global identifiers
     * @param document The Json document to save to
     */
    void saveGlobalIds(rapidjson::Document &document) {
        rapidjson::Value value(lastCardId);
        document.AddMember(rapidjson::StringRef(GLOBAL_CARD_ID), value, document.GetAllocator());
    }

    protected:
    /**
     * Serialize a given card to a json document
     * @param card The card to serialize
     * @param alloc The document allocator used to expand the document
     * @return Return the rapidjson::Value which represents the data of the card
     */
    rapidjson::Value serialize_card(Card *card, rapidjson::Document::AllocatorType &alloc) {
        // Create child nodes for each variable
        rapidjson::Value templateId(card->getTemplateId());
        rapidjson::Value cardId(card->getId());
        rapidjson::Value intAttributes;
        // the int attributes node is an object node, so we need to explicitly set that
        intAttributes.SetObject();
        // Populate the attribute node with the card's attributes
        for (auto &pair : card->getIntAttributes()) {
            rapidjson::Value value(pair.second);
            intAttributes.AddMember(rapidjson::StringRef(pair.first.c_str()), value, alloc);
        }
        // The parent object which holds all the nodes for this data
        rapidjson::Value member;
        //we need to explicitly set it as a json object
        member.SetObject();
        // Add the child nodes to the parent
        member.AddMember(rapidjson::StringRef(TEMPLATE_ID), templateId, alloc);
        member.AddMember(rapidjson::StringRef(CARD_ID), cardId, alloc);
        member.AddMember(rapidjson::StringRef(INT_ATTRIBUTES), intAttributes, alloc);
        return member;
    }

    /**
     * Save a vector of cards to a given json document at a given key
     * @param document The document to save to
     * @param cards The cards to save
     * @param key The key to save to
     */
    void saveCards(rapidjson::Document &document, const std::vector<Card *> &cards, const char *key) {
        // Init the node and set is an object
        rapidjson::Value value;
        value.SetObject();
        rapidjson::Document::AllocatorType alloc = document.GetAllocator();
        // Save each card with the key as the index in the vector
        for (int i = 0; i < (int) cards.size(); i++) {
            rapidjson::Value member = serialize_card(cards[i], alloc);
            auto index = rapidjson::StringRef(std::to_string(i).data());
            value.AddMember(index, member, alloc);
        }
        // Save the node to the doccument at the given key
        document.AddMember(rapidjson::StringRef(key), value, document.GetAllocator());
    }

    /**
     * Deserialize a card from a given node
     * @param value The value to deserialize from
     * @return A pointer to a new Card object with values deserialized from the node
     */
    Card *deserialize_card(rapidjson::Value &value) {
        // Load the non-complex variables first
        int templateId = value.FindMember(TEMPLATE_ID)->value.GetInt();
        int cardId = value.FindMember(CARD_ID)->value.GetInt();
        // Load the attributes
        std::map<const std::string, int> intAttributes;
        auto ints = value.FindMember(INT_ATTRIBUTES);
        if (ints != value.MemberEnd()) {
            auto intValue = &ints->value;
            auto iter = intValue->MemberBegin();
            while (iter != intValue->MemberEnd()) {
                intAttributes[iter->name.GetString()] = iter->value.GetInt();
            }
        }
        return new Card{cardId, templateId, intAttributes};
    }

    /**
     * Load the cards from a given json node
     * @param deckValue The json node to parse from
     * @return Returns a vector of cards
     */
    std::vector<Card *> loadCards(rapidjson::Value &deckValue) {
        // We can't guarantee that the ordered was preserved when reading the values,
        // so we save the values to a map with the key being the vector index and the value
        // being the card
        std::map<int, Card *> cards;
        int max = 0;
        auto iter = deckValue.MemberBegin();
        while (iter != deckValue.MemberEnd()) {
            // Deserialize each card and place it on the map
            int index = iter->name.GetInt();
            cards[index] = deserialize_card(iter->value);
            if (index > max) {
                max = index;
            }
        }
        // Copy the cards from the map onto a vector
        auto ret = std::vector<Card *>(max);
        for (auto pair : cards) {
            ret.at(pair.first) = pair.second;
        }
        return ret;
    }

    /**
     * Serialize a vector of ints to a json document
     * @param ints The vector to serialize
     * @param allocator The document allocator
     * @return The rapidjson::Value which represents the vector
     */
    static rapidjson::Value serializeInts(const std::vector<int> &ints, rapidjson::Document::AllocatorType &allocator) {
        rapidjson::Value ret;
        // Explicitly set the node as an array
        ret.SetArray();
        // Get the array accessor
        auto array = ret.GetArray();
        for (const int &i : ints) {
            // Add the ints to the array
            ret.PushBack(i, allocator);
        }
        return ret;
    }

    /**
     * Deserialize a vector of ints from a given json node
     * @param value The json node
     * @return A vector of ints
     */
    static std::vector<int> deserializeInts(const rapidjson::Value &value) {
        std::vector<int> ret;
        // Check if the node is an array first
        if (!value.IsArray()) {
            return ret;
        }
        // Get the array accessor object
        auto array = value.GetArray();
        // Reserve n-spaces on the vector
        ret.reserve(array.Size());
        auto iter = array.Begin();
        while (iter != array.End()) {
            // Append each element to the vector
            ret.push_back(iter->GetInt());
            iter++;
        }
        return ret;
    }

    /**
     * Serialize a map of attributes and ints to a json node
     * @param attributes The map of attributes
     * @param allocator The document allocator
     * @return The rapidjson::Value which represents the map of attributes
     */
    static rapidjson::Value serializeAttributes(const std::map<Attribute, int> &attributes, rapidjson::Document::AllocatorType &allocator) {
        rapidjson::Value ret;
        // Explicitly set the node as an object
        ret.SetObject();
        for (auto pair : attributes) {
            std::string name = Entity::getAttributeName(pair.first);
            rapidjson::Value value(pair.second);
            rapidjson::Value key(name.data(), allocator);
            // Save the child node to the parent node using the key and allocator
            ret.AddMember(key, value, allocator);
        }
        return ret;
    }

    /**
     * Deserialize a map of attributes from a json node
     * @param value The json node
     * @return Returns a map of attributes and ints
     */
    static std::map<Attribute, int> deserializeAttributes(rapidjson::Value &value) {
        std::map<Attribute, int> ret;
        auto iter = value.MemberBegin();
        // Iterate over all the child nodes
        while (iter != value.MemberEnd()) {
            auto key = &iter->name;
            auto val = &iter->value;
            std::string rawAttr = key->GetString();
            Attribute attribute;
            try {
                // try to parse the attribute enum from the key
                attribute = Entity::getAttribute(rawAttr);
            } catch (std::invalid_argument &ex) {
                // Handle potential data corruption/tampering
                std::cout << "[DATABASE] invalid attribute name " << attribute << " found when loading player data";
                continue;
            }
            // Load the integer value
            int attrValue = val->GetInt();
            // Place the key-value pair into the map
            ret[attribute] = attrValue;
            // move to the next child node
            iter++;
        }
        return ret;
    }

    /**
     * Save a <code>Player</code> to a json document
     * @param document
     * @param player
     */
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
        auto pathPos = document.FindMember(PATH_POSITION);
        if (pathPos != document.MemberEnd()) {
            player->modifyPosition(pathPos->value.GetInt());
        }
        auto path = document.FindMember(PATH);
        if (path != document.MemberEnd()) {
            player->setPath(deserializeInts(path->value));
        }
        auto attributes = document.FindMember(ATTRIBUTES);
        if (attributes != document.MemberEnd()) {
            auto attrs = deserializeAttributes(attributes->value);
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
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper, rapidjson::Document::EncodingType, rapidjson::UTF8<>> writer(databaseWrapper);
        writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
        data.Accept(writer);
        databaseFile.close();
    }

    Card *createCard(const int &templateId) noexcept(true) override {
        return new Card{lastCardId++, templateId};
    }

    void reset() noexcept(false) override {
        lastCardId = 0;
    }
};

Database *newDatabase(const std::string &path) noexcept(true) {
    return new JsonDatabase{path};
}
