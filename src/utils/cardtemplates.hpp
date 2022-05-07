#ifndef CARDTEMPLATES_HPP
#define CARDTEMPLATES_HPP

#include "../playerinfo.hpp"
#include "card.hpp"
#include "game.hpp"
#include "optional.hpp"
#include "utils.hpp"

const int SKILL_POINT_CARD = 0;
const int DAMAGE_CARD = 1;
const int TELEPORT_CARD = 2;
const int WIN_BATTLE_CARD = 3;
const int REVIVE_CARD = 4;

class SkillPointCardTemplate : public CardTemplate {

    private:
    const std::string DATA_KEY = "point-amt";

    public:
    explicit SkillPointCardTemplate() : CardTemplate(SKILL_POINT_CARD) {
    }

    bool onCardUse(Player *player, Card *cardData) override {
        if (cardData->getTemplateId() != getId() || checkDead(player)) {
            return false;
        }
        Optional<int> amount = cardData->getIntAttribute(DATA_KEY);
        if (amount.isPresent()) {
            int value = *amount.value();
            if (value > 0) {
                player->addSkillPoints(value);
            } else {
                player->removeSkillPoints(value);
            }
            player->printUnallocatedSkillPoints();
            return true;
        }
        return false;
    }

    void initCard(Card *card, const int &amt) {
        card->setIntAttribute(DATA_KEY, amt);
    }

    void initCard(Card *card) override {
        int randomValue = rand() % 5 + 1;
        initCard(card, randomValue);
    }

    void displayCard(Card *card) override {
        if (card->getTemplateId() != getId()) {
            return;
        }
        Optional<int> amount = card->getIntAttribute(DATA_KEY);
        if (amount.isEmpty()) {
            return;
        }
        std::cout << "Skill Point Card" << std::endl
                  << std::endl;
        printf("%s %d \n", "Increase the number of skill points you can allocate by ", *amount.value());
    }
};

class DamageCardTemplate : public CardTemplate {

    const std::string DATA_KEY = "damage-amt";
    GameClient *client;

    public:
    explicit DamageCardTemplate(GameClient *_client) : CardTemplate(DAMAGE_CARD) {
        client = _client;
    }

    bool onCardUse(Player *player, Card *cardData) override {
        if (cardData->getTemplateId() != getId() || checkDead(player)) {
            return false;
        }
        Optional<int> amount = cardData->getIntAttribute(DATA_KEY);
        if (amount.isPresent()) {
            int value = *amount.value();
            Optional<Battle> battleOpt = client->getBattleHandler()->getcurrentBattle();
            if (battleOpt.isPresent()) {
                Battle* battle = battleOpt.value();
                Optional<Entity> opponent = battle->getCurrentOpponent();
                if (opponent.isPresent()) {
                    opponent.value()->takeDamage(value);
                }
                return true;
            } else {
                std::cout << "This card can only be used during a battle!" << std::endl;
            }
        }
        return false;
    }

    void initCard(Card *card, const int &amt) {
        card->setIntAttribute(DATA_KEY, clampPositive(amt));
    }

    void initCard(Card *card) override {
        int random = rand() % 10 + 1;
        initCard(card, random);
    }
};

class TeleportCardTemplate : public CardTemplate {

    const std::string DATA_KEY = "teleport-amt";

    GameClient *client;

    public:
    explicit TeleportCardTemplate(GameClient *_client) : CardTemplate(TELEPORT_CARD) {
        client = _client;
    }

    bool onCardUse(Player *player, Card *cardData) override {
        if (cardData->getTemplateId() != getId() || checkDead(player)) {
            return false;
        }
        Optional<int> distance = cardData->getIntAttribute(DATA_KEY);
        if (distance.isPresent()) {
            int amt = *distance.value();
            Optional<Battle> battleOpt = client->getBattleHandler()->getcurrentBattle();
            if (battleOpt.isPresent()) {
                std::cout << "You cannot teleport whilst you are fighting!" << std::endl;
                return false;
            } else {
                player->removeCardFromInventory(cardData);
                player->modifyPosition(amt);
                std::cout << "You have been teleported!" << std::endl;
                player->printMap();
                return true;
            }
        }
        return false;
    }

    void initCard(Card *card, const int &amt) {
        card->setIntAttribute(DATA_KEY, amt);
    }

    void initCard(Card *card) override {
        int sign;
        if (rand() % 2) {
            sign = -1;
        } else {
            sign = 1;
        }
        int random = rand() % 3 + 1;
        initCard(card, sign * random);
    }

    void displayCard(Card *card) override {
        if (card->getTemplateId() != getId()) {
            return;
        }
        Optional<int> distance = card->getIntAttribute(DATA_KEY);
        if (distance.isEmpty()) {
            return;
        }
        int raw = *distance.value();
        bool positive = raw >= 0;
        std::cout << "Teleportation Card" << std::endl
                  << std::endl;
        const char *msg = positive ? "forwards." : "backwards.";
        printf("%s %d %s %s\n", "When used, this card will teleport you", std::abs(raw), "spaces", msg);
        std::cout << "However, this card will not allow you to escape the final boss!" << std::endl;
    }
};

class WinBattleCardTemplate : public CardTemplate {

    const std::string DATA_KEY = "win-probability";

    GameClient *client;

    public:
    explicit WinBattleCardTemplate(GameClient *_client) : CardTemplate(WIN_BATTLE_CARD) {
        client = _client;
    }

    bool onCardUse(Player *player, Card *cardData) override {
        if (cardData->getTemplateId() != getId() || checkDead(player)) {
            return false;
        }
        Optional<int> amt = cardData->getIntAttribute(DATA_KEY);
        if (amt.isEmpty()) {
            return false;
        }
        Optional<Battle> optionalBattle = client->getBattleHandler()->getcurrentBattle();
        if (optionalBattle.isEmpty()) {
            std::cout << "You can only use this card during a battle!" << std::endl;
            return false;
        }
        Battle *battle = optionalBattle.value();
        int value = *amt.value();
        int random = rand() % 101;
        if (random < value) {
            // The battle was automatically won.
            Optional<Entity> opponent = battle->getCurrentOpponent();
            if (opponent.isPresent()) {
                opponent.value()->kill();
                std::cout << opponent.value()->getName() << " has been killed!";
            }
        } else {
            std::cout << battle->player->getName() << " tried to win the fight with an ancient curse but the curse proved ineffective!" << std::endl;
        }
        return true;
    }

    void initCard(Card *card, const int &chance) {
        card->setIntAttribute(DATA_KEY, clampPercent(chance));
    }

    void initCard(Card *card) override {
        int random = rand() % 100 + 1;
        initCard(card, random);
    }

    void displayCard(Card *card) override {
        if (card->getTemplateId() != getId()) {
            return;
        }
        Optional<int> amount = card->getIntAttribute(DATA_KEY);
        if (amount.isEmpty()) {
            return;
        }
        int percentChance = *amount.value();
        std::cout << "Ancient Battle Winning Curse" << std::endl
                  << std::endl;
        printf("%s %d%s\n", "When used, this card has a", percentChance, "% chance of instantly killing the enemy!");
    }
};

class ReviveCardTemplate : public CardTemplate {

    public:
    explicit ReviveCardTemplate() : CardTemplate(REVIVE_CARD) {
    }

    bool onCardUse(Player *player, Card *cardData) override {
        if (cardData->getTemplateId() != getId()) {
            return false;
        }
        if (!player->isDead()) {
            std::cout << "You can only revive yourself if you are dead!" << std::endl;
            return false;
        }
        player->modifyAttribute(Entity::Attribute::HEALTH, DEFAULT_PLAYER_HEALTH);
        std::cout << player->getName() << " was revived! The fight goes on..." << std::endl;
        return true;
    }

    void initCard(Card *card) override {
    }

    void displayCard(Card *card) override {
        if (card->getTemplateId() != getId()) {
            return;
        }
        std::cout << "Revive card" << std::endl
                  << std::endl;
        std::cout << "When used, this card revives the player" << std::endl;
        std::cout << "Use this card only when in dire need." << std::endl;
    }
};

#endif//CARDTEMPLATES_HPP