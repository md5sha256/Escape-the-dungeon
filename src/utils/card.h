#ifndef CARD_H
#define CARD_H

#include "../playerinfo.h"

class CardTemplate {

    int id;

    protected:
    explicit CardTemplate(const int &cardId) {
        id = cardId;
    }

    public:
    virtual ~CardTemplate() = default;

    [[nodiscard]] int getId() const noexcept(true) {
        return id;
    }

    virtual bool onCardUse(Player *player, Card *cardData) {
        return true;
    }

    virtual void displayCard(Card *card) = 0;

    virtual void initCard(Card *card) = 0;

    static bool checkDead(Player *player) {
        if (player->isDead()) {
            std::cout << "You may not use this card whilst dead!" << std::endl;
            return true;
        }
        return false;
    }
};

#endif//CARD_H
