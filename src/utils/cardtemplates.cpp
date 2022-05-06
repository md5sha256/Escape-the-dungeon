#include "../playerinfo.hpp"
#include "card.hpp"
#include "optional.hpp"

const int SKILL_POINT_CARD = 0;
const int ATTRIBUTE_CARD = 1;



class SkillPointCardTemplate : CardTemplate {

    private:
    const std::string DATA_KEY = "point-amt";

    public:
    explicit SkillPointCardTemplate() : CardTemplate(SKILL_POINT_CARD) {
    }

    void onCardUse(GameClient *client, Player &player, Card &cardData) override {
        if (cardData.getTemplateId() != getId()) {
            return;
        }
        Optional<int> amount = cardData.getIntAttribute(DATA_KEY);
        if (amount.isPresent()) {
            int value = *amount.value();
            if (value > 0) {
                player.addSkillPoints(value);
            } else {
                player.removeSkillPoints(value);
            }
            player.printUnallocatedSkillPoints();
        }
    }

    void initCard(Card &card, int amt) {
        card.setIntAttribute(DATA_KEY, amt);
    }

    void displayCard(Card &card) {
        if (card.getTemplateId() != getId()) {
            return;
        }
        Optional<int> amount = card.getIntAttribute(DATA_KEY);
        if (amount.isEmpty()) {
            return;
        }
        std::cout << "Skill Point Card" << std::endl << std::endl;
        printf("%s %d \n", "Increase the number of skill points you can allocate by ", *amount.value());
    }

};



class DamageCardTemplate : CardTemplate {

    const std::string DATA_KEY = "damage-amt";

    public:
    explicit DamageCardTemplate() : CardTemplate(ATTRIBUTE_CARD) {
    }

    void onCardUse(GameClient* client, Player &player, Card &cardData) override {
        if (cardData.getTemplateId() != getId()) {
            return;
        }
        Optional<int> amount = cardData.getIntAttribute(DATA_KEY);
        if (amount.isPresent()) {
            int value = *amount.value();
            Optional<Battle> battleOpt = client->getBattleHandler()->getcurrentBattle();
            if (battleOpt.isPresent()) {
                Entity *target = battleOpt.value()->entity;
                target->takeDamage(value);
            }
        }
    }

};