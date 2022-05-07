#ifndef PLAYERINFO_HPP
#define PLAYERINFO_HPP
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include "utils/optional.hpp"
#include "algorithm"

const int DEFAULT_PLAYER_HEALTH = 10;

const int BATTLE_PATH = 0;
const int CAMPFIRE_PATH = 1;
const int SHOP_PATH = 2;
const int EVENT_PATH = 3;
const int WIN_PATH = 4;

struct Card {

    private:
    int id;

    int templateId;

    std::map<const std::string, int> intAttributes;

    public:
    Card(const int &cardId, const int &cardTemplateId) {
        id = cardId;
        templateId = cardTemplateId;
    }

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

    [[nodiscard]] Optional<int> getIntAttribute(const std::string &key) const {
        auto iter = intAttributes.find(key);
        if (iter != intAttributes.end()) {
            int i = iter->second;
            return Optional<int>(i);
        }
        return nullopt<int>();
    }
    void setIntAttribute(const std::string &key, const int &value) {
        intAttributes[key] = value;
    }

    [[nodiscard]] std::map<const std::string, int> getIntAttributes() const {
        return std::map<const std::string, int>{intAttributes};
    }

};

enum Attribute {

    HEALTH,
    ATTACK,
    DEFENCE,

};

struct Entity {

    static std::string getAttributeName(const Attribute &attr) {
        switch(attr) {
            case HEALTH: return "HP";
            case ATTACK: return "Attack";
            case DEFENCE: return "Defence";
            default:
                throw std::invalid_argument("Unknown attribute!");
        }
    }

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
    std::map<Attribute, int> attributes = {{Attribute::ATTACK, 0}, {Attribute::HEALTH, 0}, {Attribute::DEFENCE, 0}};

    public:
    explicit Entity(const std::string &_name) {
        name = _name;
    }

    Entity(const std::string &_name, const std::map<Attribute, int> &attrs) : Entity(_name){
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

    int modifyAttribute(Attribute attribute, const int &amt) {
        int ref = attributes[attribute];
        ref += amt;
        if (ref < 0) {
            int diff = amt - ref;
            // Min value for all attributes is 0
            attributes[attribute] = 0;
            return diff;
        }
        attributes[attribute] = ref;
        return amt;
    }

    [[nodiscard]] std::map<Attribute, int> getAttributes() const noexcept(true) {
        return attributes;
    }

    void setAttributes(std::map<Attribute, int> &_attributes) noexcept(true) {
        for (auto pair : _attributes) {
            attributes[pair.first] = pair.second;
        }
    }

    void printAHD() {
        std::cout << "HP: " << getAttribute(HEALTH) << std::endl;
        std::cout << "Attack: " << getAttribute(ATTACK) << std::endl;
        std::cout << "Defence: " << getAttribute(DEFENCE) << std::endl;
    }

    void printAttribute(Attribute attr) {
        printf("%s: %d\n", getAttributeName(attr).data(), getAttribute(attr));
    }

    void kill() {
        attributes[Attribute::HEALTH] = 0;
    }

    bool isDead() {
        return getAttribute(HEALTH) <= 0;
    }

    void takeDamage(int damage){
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
        const char* _name = name.c_str();
        printf("%s %s %d %s\n", _name, "suffered", -actual, "health damage");
    }

};

struct Player : public Entity {

    private:
    int skill_points = 0;
    int gold = 0;
    std::vector<int> path;
    int position = 0;
    std::vector<Card*> inventory;

    public:
    explicit Player(const std::string &name) : Entity(name) {
    }

    Player(const std::string &name, std::map<Attribute, int> &attrs) : Entity(name, attrs) {

    }

    std::vector<Card*> getInventory() {
        return inventory;
    }

    [[nodiscard]] int getInventorySize() const {
        return (int) inventory.size();
    }

    Card* getCard(int index) {
        if (index < 0 || index > getInventorySize() - 1) {
            throw std::invalid_argument("Invalid index: " + std::to_string(index));
        }
        return inventory[index];
    }

    void addCardToInventory(Card *card) {
        inventory.push_back(card);
    }

    bool removeCardFromInventory(Card* card) {
        auto iter = std::find(inventory.begin(), inventory.end(), card);
        if (iter != inventory.end()) {
            inventory.erase(iter);
            delete card;
            return true;
        }
        return false;
    }

    void setInventory(const std::vector<Card*> &inv) {
        inventory = inv;
    }


    std::vector<int> getPath() {
        return path;
    }

    void setPath(const std::vector<int> &_path) noexcept(true) {
        path = _path;
    }

    [[nodiscard]] int getSkillPoints() const {
        return skill_points;
    }

    void modifySkillPoints(const int &amt) {
        skill_points += amt;
        if (skill_points < 0) {
            skill_points = 0;
        }
    }

    [[nodiscard]] int getGold() const noexcept(true) {
        return gold;
    }

    void modifyGold(int amount) noexcept(true) {
        gold += amount;
        if (gold < 0) {
            gold = 0;
        }
    }

    [[nodiscard]] int getPosition() const noexcept(true) {
        return position;
    }

    [[nodiscard]] int getPathAtPosition() const noexcept(true) {
        return path[position];
    }

    void incrementPosition() noexcept(true) {
        if (position < (int) path.size()) {
            position++;
        }
    }

    int modifyPosition(const int &amt) noexcept(true) {
        position += amt;
        if (position > (int) path.size() - 1) {
            position = (int) path.size() - 1;
        } else if (position < 0) {
            position = 0;
        }
        return position;
    }

    void printGold() const {
        std::cout << "Gold: " << gold << std::endl;
    }

    void printStatus() {
        printAHD();
        printUnallocatedSkillPoints();
        printGold();
    }


    void printMap() {
        for (int i = 0; i < 10; i++) {
            if (i != position) {
                if (path[i] == 0)
                    std::cout << "Battle >> ";
                else if (path[i] == 1)
                    std::cout << "Campfire >> ";
                else if (path[i] == 2)
                    std::cout << "Shop >> ";
                else if (path[i] == 3)
                    std::cout << "Event >> ";
            } else {//highlight the current position of player on map
                if (path[i] == 0)
                    std::cout << "**Battle** >> ";
                else if (path[i] == 1)
                    std::cout << "**Campfire** >> ";
                else if (path[i] == 2)
                    std::cout << "**Shop** >> ";
                else if (path[i] == 3)
                    std::cout << "**Event** >> ";
            }
        }
        std::cout << "Escape" << std::endl;//print the final station on the map
    }

    void addSkillPoints(const int &amt) {
        if (amt < 0) {
            throw std::invalid_argument("amount cannot be negative!");
        }
        skill_points += amt;
    }

    void removeSkillPoints(const int &amt) {
        if (amt < 0) {
            throw std::invalid_argument("amount cannot be negative!");
        }
        skill_points += amt;
        if (skill_points < 0) {
            skill_points = 0;
        }
    }

    void printUnallocatedSkillPoints() const {
        std::cout << "You have " << skill_points << " unallocated skill points at the moment" << std::endl;
        if (skill_points > 0) {
            std::cout << "You can boost either your 'Attack', 'HP' or 'Defence'." << std::endl;
        }
    }

};

struct Path {
    std::vector<int> path1;
    std::vector<int> path2;
    std::vector<int> path3;
    //path id: 0 for battle,1 for campfire, 2 for shop,3 for random event
    void generate() {
        path1.push_back(0);          //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        for (int i = 0; i < 8; i++) {//generate the first path
            int j = rand() % 5;
            if (j == 2 || j == 3) {//limit the appearance of shop,campfire and random event to 1/2 probability
                int j = (rand() % 3) + 1;
                path1.push_back(j);
            } else
                path1.push_back(0);
        }
        path2.push_back(0);          //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        for (int i = 0; i < 8; i++) {//generate the second path
            int j = rand() % 5;
            if (j == 3) {//limit the appearance of shop,campfire and random event to 1/4 probability
                int j = (rand() % 3) + 1;
                path2.push_back(j);
            } else
                path2.push_back(0);
        }
        //generate the third path
        //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        path3.push_back(0);
        for (int i = 0; i < 8; i++) {
            int j = rand() % 5;
            if (j == 4) {//limit the appearance of shop,campfire and random event to 1/5 probability
                int j = (rand() % 3) + 1;
                path3.push_back(j);
            } else
                path3.push_back(0);
        }
        path1.push_back(WIN_PATH);
        path2.push_back(WIN_PATH);
        path3.push_back(WIN_PATH);
    }
};

inline void allocateSkillPoints(Player *player) {
    player->printUnallocatedSkillPoints();
    std::string input;
    while (player->getSkillPoints() > 0) {
        std::cout << "Input a number(1: Attack, 2: HP, 3: Defence): ";
        std::cin >> input;
        Attribute toModify;
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
        player->modifyAttribute(toModify, 1);
        player->modifySkillPoints(-1);
    }
}

Player *initPlayer();

void performGreetBack(Player *player);
void randomEvent(Player *p);
void campfire(Player *p);

#endif//PLAYERINFO_HPP
