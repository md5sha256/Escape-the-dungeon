#ifndef PLAYERINFO_HPP
#define PLAYERINFO_HPP
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "utils/optional.hpp"

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

    [[nodiscard]] optional<int> getIntAttribute(const std::string &key) const {
        return getAttribute<int>(key, intAttributes);
    }

    [[nodiscard]] optional<double> getDoubleAttribute(const std::string &key) const {
        return getAttribute<double>(key, doubleAttributes);
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

    [[nodiscard]] std::map<const std::string, int> getIntAttributes() const {
        return std::map<const std::string, int>{intAttributes};
    }

    [[nodiscard]] std::map<const std::string, double> getDoubleAttributes() const {
        return std::map<const std::string, double>{doubleAttributes};
    }

    [[nodiscard]] std::map<const std::string, std::string> getStringAttributes() const {
        return std::map<const std::string, std::string>{stringAttributes};
    }

};

struct Entity {

    public:
    enum Attribute {

        HEALTH,
        ATTACK,
        DEFENCE,

    };

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
    std::map<Attribute, int> defAttrs = {{Attribute::ATTACK, 0}, {Attribute::HEALTH, 0}, {Attribute::DEFENCE, 0}};
    std::map<Attribute, int> attributes = std::map<Attribute, int>(defAttrs);

    public:
    explicit Entity(const std::string &_name) {
        name = _name;
    }

    Entity(const std::string &_name, const std::map<Attribute, int> &attrs) : Entity(_name){
        for (auto pair : attrs) {
            attributes[pair.first] = pair.second;
        }
    }

    std::string getName() noexcept {
        return name;
    }

    [[nodiscard]] int getAttribute(const Attribute attribute) {
        return attributes[attribute];
    }

    void modifyAttribute(const Attribute attribute, const int &amt) {
        attributes[attribute] += amt;
        if (attributes[attribute] < 0) {
            attributes[attribute] = 0;
        }
    }

    [[nodiscard]] std::map<Attribute, int> getAttributes() const noexcept(true) {
        return attributes;
    }

    void setAttributes(std::map<Attribute, int> &_attributes) noexcept(true) {
        attributes.clear();
        attributes = std::map<Attribute, int>(defAttrs);
        for (auto pair : attributes) {
            attributes[pair.first] = pair.second;
        }
    }

    void printAHD() {
        std::cout << "HP: " << getAttribute(HEALTH) << std::endl;
        std::cout << "Attack: " << getAttribute(ATTACK) << std::endl;
        std::cout << "Defence: " << getAttribute(DEFENCE) << std::endl;
    }

    bool check_if_dead() {
        return getAttribute(HEALTH) <= 0;
    }

    void take_damage(int damage){
        int defence = getAttribute(DEFENCE);
        // Use up the defence points first
        modifyAttribute(DEFENCE, -damage);
        // Min value for all attributes is 0
        if (defence < damage) {
            // We deal the overflow to the health
            int healthDmg = damage - defence;
            modifyAttribute(HEALTH, -healthDmg);
        }
    }

};

struct Player : public Entity {

    private:
    int skill_points = 5;
    int gold = 0;
    std::vector<int> path;
    int position = 0;
    std::vector<Card*> currentDeck;
    std::vector<Card*> inventory;

    public:
    explicit Player(const std::string &name) : Entity(name) {
    }

    Player(const std::string &name, std::map<Attribute, int> &attrs) : Entity(name, attrs) {

    }

    std::vector<Card*> getCurrentDeck() {
        return currentDeck;
    }

    void setDeck(const std::vector<Card*> &deck) {
        currentDeck = deck;
    }

    std::vector<Card*> getInventory() {
        return inventory;
    }

    void setInventory(const std::vector<Card*> &_inventory) {
        inventory = _inventory;
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
        if (position < path.size()) {
            position++;
        }
    }

    int modifyPosition(const int &amt) noexcept(true) {
        position += amt;
        if (position > path.size() - 1) {
            position = (int) path.size() - 1;
        } else if (position < 0) {
            position = 0;
        }
        return position;
    }


    void printStatus() {
        printAHD();
        std::cout << "Skill Points: " << skill_points << std::endl;
    }

    void print_map() {
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
                else
                    std::cout << "Boss>>";
            } else {//highlight the current postion of player on map
                if (path[i] == 0)
                    std::cout << "**Battle** >> ";
                else if (path[i] == 1)
                    std::cout << "**Campfire** >> ";
                else if (path[i] == 2)
                    std::cout << "**Shop** >> ";
                else if (path[i] == 3)
                    std::cout << "**Event** >> ";
                else
                    std::cout << "**Boss** >> ";
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
        std::cout << "You have " << skill_points << " skill unallocated points at the moment" << std::endl;
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
        srand(time(nullptr) + 1);
        path1.push_back(0);          //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        for (int i = 0; i < 8; i++) {//generate the first path
            int j = rand() % 5;
            if (j == 3) {//limit the appearance of shop,campfire and random event to 1/4 probability
                int j = (rand() % 3) + 1;
                path1.push_back(j);
            } else
                path1.push_back(0);
        }
        srand(time(nullptr) + 2);
        path2.push_back(0);          //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        for (int i = 0; i < 8; i++) {//generate the second path
            int j = rand() % 5;
            if (j == 3) {//limit the appearance of shop,campfire and random event to 1/4 probability
                int j = (rand() % 3) + 1;
                path2.push_back(j);
            } else
                path2.push_back(0);
        }
        srand(time(NULL) + 3);//generate the third path
        path3.push_back(0);   //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        for (int i = 0; i < 8; i++) {
            int j = rand() % 5;
            if (j == 4) {//limit the appearance of shop,campfire and random event to 1/5 probability
                int j = (rand() % 3) + 1;
                path3.push_back(j);
            } else
                path3.push_back(0);
        }
        path1.push_back(4);//boss generation at the end of each path, with ID of 5
        path2.push_back(4);
        path3.push_back(4);
    }
};


inline Entity generateRandomEnemy() {
    typedef Entity::Attribute Attribute;
    int attack = (rand() % 5) + 1;
    int defence = (rand() % 5) + 1;
    int hp = (rand() % 4) + 1;
    std::string name;
    //naming of enemy
    if ((attack + defence) >= 8)//powerful enemy
        name = "Elite ";
    else if ((attack + defence) >= 5)//strong enemy
        name = "Adult ";
    else//weak enemy
        name = "Baby";
    if (defence * 2 <= attack)//if attack is twice as great as defence
        name += "Zombie";
    else if (attack * 2 <= defence)//if defence is twice as great as attack
        name += "Skeleton";
    else//if somewhere balance
        name += "Shrem";
    std::map<Attribute, int> attrs = {{Attribute::HEALTH, hp}, {Attribute::ATTACK, attack}, {Attribute ::DEFENCE, defence}};
    return Entity{name, attrs};
}

inline void allocateSkillPoints(Player &player) {
    typedef Entity::Attribute Attribute;
    player.printUnallocatedSkillPoints();
    std::string input;
    while (player.getSkillPoints() > 0) {
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
        player.modifyAttribute(toModify, 1);
        player.modifySkillPoints(-1);
    }
}

Player *initPlayer();

void performGreetBack(Player *player);

void random_event(Player &p);
void boss(Player p);
void campfire(Player &p);

#endif//PLAYERINFO_HPP
