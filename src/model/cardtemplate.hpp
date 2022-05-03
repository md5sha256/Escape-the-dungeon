#ifndef CARDTEMPLATE_HPP
#define CARDTEMPLATE_HPP

#include "../playerinfo.hpp"
#include "card.hpp"

class CardTemplate {

    int id;

    protected:
    explicit CardTemplate(int cardId) {
        id = cardId;
    }

    public:
    [[nodiscard]] int getId() const noexcept(true) {
        return id;
    }

    virtual void onCardUse(const player_info &player, const Card &cardData) = 0;

};

#endif//CARDTEMPLATE_HPP
