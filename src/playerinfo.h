#ifndef PLAYERINFO_H
#define PLAYERINFO_H
#include "algorithm"
#include "utils/optional.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

///Default health of the player
const int DEFAULT_PLAYER_HEALTH = 10;

///Path constants for the different path types
const int BATTLE_PATH = 0;
const int CAMPFIRE_PATH = 1;
const int SHOP_PATH = 2;
const int EVENT_PATH = 3;
const int WIN_PATH = 4;

/**
 * Represents a Card which a player holds
 * This struct is a data class which holds
 * the necessary information for the corresponding
 * <code>CardTemplate</code> to perform its actions.
 * @see utils/cardtemplate.h
 * @see utils/database.h
 */
struct Card {

    private:
    // Global card id used for data storage
    int id;
    // Id of the card template
    int templateId;

    //The data needed by the CardTemplate
    std::map<const std::string, int> intAttributes;

    /**
     * Construct a card template with a given global card id and template id
     * @param cardId the unique storage id of the card
     * @param cardTemplateId the id of the corresponding <code>CardTemplate</code>
     */
    public:
    Card(const int &cardId, const int &cardTemplateId) {
        id = cardId;
        templateId = cardTemplateId;
    }

    /**
     * Construct a card template with a given global card id and template id
     * @param cardId the unique storage id of the card
     * @param cardTemplateId the id of the corresponding <code>CardTemplate</code>
     * @param ints the data stored on this card
     */
    Card(const int &cardId, const int &cardTemplateId,
         const std::map<const std::string, int> &ints) : Card(cardId, cardTemplateId) {
        intAttributes = ints;
    }

    ~Card() = default;

    [[nodiscard]] int getTemplateId() const {
        return templateId;
    }

    [[nodiscard]] int getId() const {
        return id;
    }

    /**
     * Get the data (int) stored for a given attribute key (std::string)
     * @param key The string key of the data
     * @return Return an optional wrapping the value stored at the given key or an empty
     * optional if no data was found for the given key
     */
    [[nodiscard]] Optional<int> getIntAttribute(const std::string &key) const {
        auto iter = intAttributes.find(key);
        if (iter != intAttributes.end()) {
            int i = iter->second;
            return Optional<int>(i);
        }
        return nullopt<int>();
    }

    /**
     * Set the value for a given attribute at a given key
     * @param key The key of the attribute
     * @param value The value to store
     */
    void setIntAttribute(const std::string &key, const int &value) {
        intAttributes[key] = value;
    }

    /**
     * Get a copy of the attributes stored on this card
     * @return Return the <code>std::map</code> which has string keys and int values
     */
    [[nodiscard]] std::map<const std::string, int> getIntAttributes() const {
        return std::map<const std::string, int>{intAttributes};
    }
};

/**
 * Represents the attributes which an <code>Entity</code> can have
 */
enum Attribute {

    // How much damage can the target sustain before dying
    HEALTH,
    // How much damage does the target do by default
    ATTACK,
    // How much damage can the target withstand before taking health damage
    DEFENCE

};

/**
 * Represents an entity capable of participating in a <code>Battle</code>
 * @see utils/battle.h
 */
struct Entity {

    /**
     * Helper method for getting the string name for an <code>Attribute</code>
     * @param attr The attribute
     * @return Returns the string name of the attribute
     * @throws std::invalid_argument if the passed attributed is not one of
     * <br>
     * <code>
     * Attribute::HEALTH <br>
     * Attribute::ATTACK <br>
     * Attribute::DEFENCE <br>
     * </code?
     */
    static std::string getAttributeName(const Attribute &attr) {
        switch (attr) {
            case HEALTH:
                return "HP";
            case ATTACK:
                return "Attack";
            case DEFENCE:
                return "Defence";
            default:
                throw std::invalid_argument("Unknown attribute!");
        }
    }

    /**
     * Helper method for getting the <code>Attribute</code> enum constant from
     * a string.
     * <strong>NOTE</note> This method is case sensitive </strong>
     * @param raw The raw string to be parsed
     * @return The attribute enum constant
     * @throws std::invalid_argument If the string is not exactly one of the following:
     * <p>
     * - HP <br>
     * - Attack <br>
     * - Defence <br>
     * </p>
     */
    static Attribute getAttribute(const std::string &raw) {
        if (raw == "HP") {
            return Attribute::HEALTH;
        } else if (raw == "Attack") {
            return Attribute::ATTACK;
        } else if (raw == "Defence") {
            return Attribute::DEFENCE;
        }
        throw std::invalid_argument("Unknown attribute: " + raw);
    }

    private:
    std::string name;
    // Default values are 0
    std::map<Attribute, int> attributes = {{Attribute::ATTACK, 0}, {Attribute::HEALTH, 0}, {Attribute::DEFENCE, 0}};

    public:
    explicit Entity(const std::string &_name) {
        name = _name;
    }

    Entity(const std::string &_name, const std::map<Attribute, int> &attrs) : Entity(_name) {
        for (const auto &pair : attrs) {
            attributes[pair.first] = pair.second;
        }
    }

    [[nodiscard]] std::string getName() const noexcept {
        return name;
    }

    [[nodiscard]] int getAttribute(Attribute attribute) {
        return attributes[attribute];
    }

    /**
     * Modify an attribute by a provided amount
     * Since values are positive clamped (min value = 0),
     * it is useful to know the amount the attribute actually changed by
     * @param attribute The attribute to change
     * @param amt The amount to change, can be positive or negative
     * @return Return the difference in the original value of the attribute and the new value
     */
    int modifyAttribute(Attribute attribute, const int &amt) {
        int ref = attributes[attribute];
        // Sum the original value with the amount to change by
        ref += amt;
        // Check negative case
        if (ref < 0) {
            int diff = amt - ref;
            // Min value for all attributes is 0
            attributes[attribute] = 0;
            return diff;
        }
        attributes[attribute] = ref;
        // Return then amt param as the actual attribute value did not get clamped
        return amt;
    }

    /**
     * Get a <code>std::map</code> copy of the <code>Entity</code>'s attributes
     * @return Returns a map with Attribute keys and int values
     */
    [[nodiscard]] std::map<Attribute, int> getAttributes() const noexcept(true) {
        return attributes;
    }

    /**
     * Set the attributes for this entity. This method will copy over the
     * values from the provided map.
     * @param _attributes The values to set
     */
    void setAttributes(std::map<Attribute, int> &_attributes) noexcept(true) {
        attributes = {{Attribute::ATTACK, 0}, {Attribute::DEFENCE, 0}, {Attribute::HEALTH, 0}};
        for (auto pair : _attributes) {
            attributes[pair.first] = pair.second;
        }
    }

    /**
     * Print attributes of this entity to std::cout
     * This method is effectively constant
     */
    void printAttributes() {
        std::cout << "HP: " << getAttribute(HEALTH) << std::endl;
        std::cout << "Attack: " << getAttribute(ATTACK) << std::endl;
        std::cout << "Defence: " << getAttribute(DEFENCE) << std::endl;
    }

    /**
     * Print the value of a given attribute to std::cout
     * @param attr The attribute to print
     */
    void printAttribute(Attribute attr) {
        printf("%s: %d\n", getAttributeName(attr).data(), getAttribute(attr));
    }

    /**
     * Kill this <code>Entity</code>, setting it's <code>Attribute::HEALTH</code> to 0
     */
    void kill() {
        attributes[Attribute::HEALTH] = 0;
    }

    /**
     * Check if this <code>Entity</code> is dead
     * @return <code>true</code> if the player has less than or 0 health <code>false</code> otherwise
     */
    bool isDead() {
        return getAttribute(HEALTH) <= 0;
    }

    /**
     * Damage this <code>Entity</code> by a given amount
     *
     * This function will use up the <code>Attribute::DEFENCE</code>
     * before deducting from <code>Attribute::HEALTH</code>
     *
     * This function will print a friendly message to std::cout
     * to inform the user of the amount of health damage this
     * <code>Entity</code> has suffered.
     *
     *
     * @param damage The amount of damage to deal
     * @throws std::invalid_argument Thrown if the damage value is negative
     */
    void takeDamage(const int &damage) {
        if (damage < 0) {
            throw std::invalid_argument("Invalid damage value! Damage must be >= 0");
        }
        int defence = getAttribute(DEFENCE);
        // Use up the defence points first
        modifyAttribute(DEFENCE, -damage);
        // Min value for all attributes is 0
        int actual(0);
        if (defence < damage) {
            // We deal the overflow to the health
            int healthDmg = damage - defence;
            actual = -modifyAttribute(HEALTH, -healthDmg);
        }
        // We only print damage done to health and not defence
        const char *_name = name.c_str();
        printf("%s %s %d %s\n", _name, "suffered", -actual, "health damage");
    }
};

/**
 * Represents the data for a Player
 */
struct Player : public Entity {

    private:
    int skillPoints = 0;
    int gold = 0;
    std::vector<int> path;
    int position = 0;
    std::vector<Card *> inventory;

    public:
    explicit Player(const std::string &name) : Entity(name) {
    }

    Player(const std::string &name, std::map<Attribute, int> &attrs) : Entity(name, attrs) {
    }

    /**
     * Get a copy of a <code>Card</code> <code>std::vector</code> which represents
     * this <code>Player</code>'s inventory
     * @return Return a card vector
     */
    std::vector<Card *> getInventory() {
        return inventory;
    }

    /**
     * Get the number of cards / the size of this <code>Player</code>'s inventory
     * @return
     */
    [[nodiscard]] int getInventorySize() const {
        return (int) inventory.size();
    }

    /**
     * Get the pointer to the card stored at a given index in the inventory
     * @param index The index
     * @return A pointer to a <code>Card</code>
     * @throws std::invalid_argument Thrown if the index is out of bounds
     */
    Card *getCard(const int &index) {
        if (index < 0 || index > getInventorySize() - 1) {
            throw std::invalid_argument("Invalid index: " + std::to_string(index));
        }
        return inventory[index];
    }

    /**
     * Append a card to the inventory
     * @param card
     */
    void addCardToInventory(Card *card) {
        inventory.push_back(card);
    }

    /**
     * Remove a card from the inventory. This
     * function will also delete the pointer if
     * the card was successfully removed.
     *
     * @param card The pointer the card
     * @return Return <code>true</code> if the card was removed, <code>false</code> otherwise
     */
    bool removeCardFromInventory(Card *card) {
        auto iter = std::find(inventory.begin(), inventory.end(), card);
        if (iter != inventory.end()) {
            inventory.erase(iter);
            delete card;
            return true;
        }
        return false;
    }

    /**
     * Set this <code>Player</code>'s inventory
     * @param inv The <code>std::vector</code> of <code>Card</code> pointers
     */
    void setInventory(const std::vector<Card *> &inv) {
        inventory = inv;
    }

    /**
     * Get a copy of the player's path. The int values
     * are the path constants defined at the top of playerinfo.h
     *
     * @return Return a <code>std::vector</code> of ints
     */
    std::vector<int> getPath() {
        return path;
    }

    /**
     * Set the path of this player
     * The values of the path should only be those of the path constants
     * defined at the top of playerinfo.h
     * <br>
     * <strong>This function will do no validation for the values of the path.</strong>
     * @param _path
     */
    void setPath(const std::vector<int> &_path) noexcept(true) {
        path = _path;
    }

    /**
     * Get the number of skill points the player has
     * @return
     */
    [[nodiscard]] int getSkillPoints() const {
        return skillPoints;
    }

    /**
     * Change the number of skill points the player has
     * The value of the player's skill points is clamped positive (min value = 0)
     * @param amt The amount to change by, can be positive or negative
     */
    void modifySkillPoints(const int &amt) {
        skillPoints += amt;
        // Ensure that the skill points cannot be negative
        if (skillPoints < 0) {
            skillPoints = 0;
        }
    }

    /**
     * Get the amount of gold the player has
     * @return
     */
    [[nodiscard]] int getGold() const noexcept(true) {
        return gold;
    }

    /**
     * Change the amount of gold the player has
     * The value of the player's gold is clamped positive (min value = 0)
     * @param amount The amount to change by, can be positive or negative
     */
    void modifyGold(const int &amount) noexcept(true) {
        gold += amount;
        // Ensure that the amount of gold cannot be negative
        if (gold < 0) {
            gold = 0;
        }
    }

    /**
     * Get the current position the player is along the path
     * @return
     */
    [[nodiscard]] int getPosition() const noexcept(true) {
        return position;
    }

    /**
     * Get the path constant (see the top of playerinfo.h) at the player's
     * current position
     * @return
     * @see <code>Player#getPosition</code>
     */
    [[nodiscard]] int getPathAtPosition() const noexcept(true) {
        return path[position];
    }

    /**
     * Increment the position the player is in
     * This function will increase the player's position
     * by 1 as long as the position is less than the total length of the path
     */
    void incrementPosition() noexcept(true) {
        if (position < (int) path.size()) {
            position++;
        }
    }

    /**
     * Modify the position the player is in by a given amount
     * This function will change the player's position by the given amount
     * but clamp it to within 0 and the size of the path - 1
     * @param amt The amount to change by, can be positive or negative
     * @return Returns the position the player is in after the change was applied
     */
    int modifyPosition(const int &amt) noexcept(true) {
        position += amt;
        if (position > (int) path.size() - 1) {
            position = (int) path.size() - 1;
        } else if (position < 0) {
            position = 0;
        }
        return position;
    }

    /**
     * Emit the amount of gold a player has in a friendly message to std::cout
     */
    void printGold() const {
        std::cout << "Gold: " << gold << std::endl;
    }

    /**
     * Print the player's attributes, skill points and gold to std::cout
     * as a friendly message
     */
    void printStatus() {
        printAttributes();
        printUnallocatedSkillPoints();
        printGold();
    }

    /**
     * Print the player's path, highlighting the current position,
     * as a friendly message to std::cout
     */
    void printMap() {
        for (int i = 0; i < 10; i++) {
            if (i != position) {
                if (path[i] == BATTLE_PATH)
                    std::cout << "Battle >> ";
                else if (path[i] == CAMPFIRE_PATH)
                    std::cout << "Campfire >> ";
                else if (path[i] == SHOP_PATH)
                    std::cout << "Shop >> ";
                else if (path[i] == EVENT_PATH)
                    std::cout << "Event >> ";
            } else {
                //highlight the current position of player on map
                if (path[i] == BATTLE_PATH)
                    std::cout << "**Battle** >> ";
                else if (path[i] == CAMPFIRE_PATH)
                    std::cout << "**Campfire** >> ";
                else if (path[i] == SHOP_PATH)
                    std::cout << "**Shop** >> ";
                else if (path[i] == EVENT_PATH)
                    std::cout << "**Event** >> ";
            }
        }
        //print the final station on the map
        std::cout << "Escape" << std::endl;
    }

    /**
     * Add a given amount of skill points
     * @param amt The amount to add, cannot be negative
     * @throws std::invalid_argument if the amount is negative
     */
    void addSkillPoints(const int &amt) {
        if (amt < 0) {
            throw std::invalid_argument("amount cannot be negative!");
        }
        skillPoints += amt;
    }

    /**
     * Remove a given amount of skill points
     * The number of skill points is clamped positive (min value = 0)
     * @param amt The amount to remove, cannot be negative
     * @throws std::invalid_argument if the amount is negative
     */
    void removeSkillPoints(const int &amt) {
        if (amt < 0) {
            throw std::invalid_argument("amount cannot be negative!");
        }
        skillPoints += amt;
        if (skillPoints < 0) {
            skillPoints = 0;
        }
    }

    /**
     * Print the number of unallocated skill points in a friendly message to std::cout
     * If skill points > 0, an extra prompt regarding the attributes which can be boosted
     * is emitted.
     */
    void printUnallocatedSkillPoints() const {
        std::cout << "You have " << skillPoints << " unallocated skill points at the moment" << std::endl;
        if (skillPoints > 0) {
            std::cout << "You can boost either your 'Attack', 'HP' or 'Defence'." << std::endl;
        }
    }
};

/**
 * Represents the paths a player can choose to take
 */
struct Path {
    std::vector<int> path1;
    std::vector<int> path2;
    std::vector<int> path3;
    // path id: 0 for battle,1 for campfire, 2 for shop,3 for random event
    void generate() {
        // player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        path1.push_back(0);
        // generate the first path
        for (int i = 0; i < 9; i++) {
            int j = rand() % 5;
            // limit the appearance of shop,campfire and random event to 1/2 probability
            if (j == 2 || j == 3) {
                int j = (rand() % 3) + 1;
                path1.push_back(j);
            } else
                path1.push_back(0);
        }
        // player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        path2.push_back(0);
        //generate the second path
        for (int i = 0; i < 9; i++) {
            int j = rand() % 5;
            //limit the appearance of shop,campfire and random event to 1/4 probability
            if (j == 3) {
                int j = (rand() % 3) + 1;
                path2.push_back(j);
            } else
                path2.push_back(0);
        }
        //generate the third path
        //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        path3.push_back(0);
        for (int i = 0; i < 9; i++) {
            int j = rand() % 5;
            //limit the appearance of shop,campfire and random event to 1/5 probability
            if (j == 4) {
                int j = (rand() % 3) + 1;
                path3.push_back(j);
            } else
                path3.push_back(0);
        }
        // Win path is always at the end
        path1.push_back(WIN_PATH);
        path2.push_back(WIN_PATH);
        path3.push_back(WIN_PATH);
    }
};

/**
 * Begin a prompt for the player to allocate their skill points
 * and will block for user input from std::cin until all points
 * are allocated
 * @param player The pointer to the player
 */
inline void allocateSkillPoints(Player *player) {
    // Let the player know how many points they can allocate
    player->printUnallocatedSkillPoints();
    std::string input;
    // While there are still unallocated points
    while (player->getSkillPoints() > 0) {
        std::cout << "Input a number(1: Attack, 2: HP, 3: Defence): ";
        std::cin >> input;
        Attribute toModify;
        // Map the input to the Attribute enum constant
        if (input == "1") {
            toModify = Attribute::ATTACK;
        } else if (input == "2") {
            toModify = Attribute::HEALTH;
        } else if (input == "3") {
            toModify = Attribute ::DEFENCE;
        } else if (input == "quit") {
            break;
        } else {
            std::cout << "Please enter a valid number." << std::endl;
            continue;
        }
        // Increase the attribute
        player->modifyAttribute(toModify, 1);
        // Remove the skill point from the player
        player->modifySkillPoints(-1);
    }
}

/**
 * Initialize a player on the heap
 * This method will ask for input from cin directly and will
 * block until setup is complete
 * @return Returns a pointer to the player object
 */
Player *initPlayer();

/**
 * Emit a friendly greeting message to a player with existing data
 * @param player A pointer to the player object
 */
void performGreetBack(Player *player);

/**
 * Prompt a random event to occur
 * This function will perform some interactions
 * with the player through std::cin but will
 * not change the player's position unless it is
 * through teleportation. The caller should
 * manually increment the player's position after
 * the function has returned.
 * @param p The player to spawn the random event against
 */
void randomEvent(Player *p);

/**
 * Prompt the campfire event on the player
 * @param p  The player to prompt the event
 */
void campfire(Player *p);

#endif//PLAYERINFO_H
