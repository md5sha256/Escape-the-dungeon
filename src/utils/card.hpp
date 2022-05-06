//
// Created by Andy on 5/5/2022.
//

#ifndef CARD_HPP
#define CARD_HPP

#include "../playerinfo.hpp"

class CardTemplate {

    int id;

    protected:
    explicit CardTemplate(int cardId) {
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

    virtual void displayCard(Card *card) {

    }

    virtual void initCard(Card *card) {

    }

    static bool checkDead(Player *player) {
        if (player->isDead()) {
            std::cout << "You may not use this card whilst dead!" << std::endl;
            return true;
        }
        return false;
    }
};

#endif//CARD_HPP
