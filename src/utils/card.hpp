//
// Created by Andy on 5/5/2022.
//

#ifndef CARD_HPP
#define CARD_HPP

#include "../playerinfo.hpp"
#include "game.hpp"

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

    virtual void onCardUse(GameClient *client, Player &player, Card &cardData) = 0;
};

#endif//CARD_HPP
